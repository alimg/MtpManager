#include "MTPhandler.h"

#include "mtputils/pathutils.h"

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>


LIBMTP_folder_t *get_folder(int folder_id,LIBMTP_folder_t* root){
    LIBMTP_folder_t *p=root,*k;
    while(p)
    {
        if(p->folder_id==folder_id)
            return p;
        k=get_folder(folder_id,p->child);
        if(k)return k;
        p=p->sibling;
    }
    return NULL;
}

LIBMTP_folder_t *get_folder_by_name(const char *str,LIBMTP_folder_t* root){
    LIBMTP_folder_t *p=root;

    //fprintf(stderr," -- -- root %s\n",root->name);
    int l=0;
    char *cp,*i;
    i=(char *)str;
    while(i[0]=='\\')i++;
    for(;i[l] && i[l]!='\\';l++);
    if (l==0)   return root;
    cp=strndup(i,l);
    while(p)
    {
        if( strcmp(p->name,cp) == 0)
        {
            free (cp);
            if(p->child)
                return get_folder_by_name(i+l,p->child);
            else return p;
        }
        p=p->sibling;
    }
    return root;
}

MTPhandler::MTPhandler()
{
    rawdevices=NULL;
    LIBMTP_Init();
    updateDevices();
}

MTPhandler::~MTPhandler()
{
    free(rawdevices);

}
int MTPhandler::updateDevices(){

    fprintf(stdout, "Listing raw device(s)\n");
    if(rawdevices){

        free(rawdevices);

    }
    err = LIBMTP_Detect_Raw_Devices(&rawdevices, &numrawdevices);
    switch(err) {
    case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
      fprintf(stdout, "   No raw devices found.\n");
      return 0;
    case LIBMTP_ERROR_CONNECTING:
      fprintf(stderr, "Detect: There has been an error connecting. Exiting\n");
      return 1;
    case LIBMTP_ERROR_MEMORY_ALLOCATION:
      fprintf(stderr, "Detect: Encountered a Memory Allocation Error. Exiting\n");
      return 1;
    case LIBMTP_ERROR_NONE:
      {
        int i;

        fprintf(stdout, "   Found %d device(s):\n", numrawdevices);
        for (i = 0; i < numrawdevices; i++) {
          if (rawdevices[i].device_entry.vendor != NULL ||
              rawdevices[i].device_entry.product != NULL) {
            fprintf(stdout, "   %s: %s (%04x:%04x) @ bus %d, dev %d\n",
                    rawdevices[i].device_entry.vendor,
                    rawdevices[i].device_entry.product,
                    rawdevices[i].device_entry.vendor_id,
                    rawdevices[i].device_entry.product_id,
                    rawdevices[i].bus_location,
                    rawdevices[i].devnum);
          } else {
            fprintf(stdout, "   %04x:%04x @ bus %d, dev %d\n",
                    rawdevices[i].device_entry.vendor_id,
                    rawdevices[i].device_entry.product_id,
                    rawdevices[i].bus_location,
                    rawdevices[i].devnum);
          }
        }
      }
      break;
    case LIBMTP_ERROR_GENERAL:
    default:
      fprintf(stderr, "Unknown connection error.\n");
      return 1;
    }

    return 0;


}

LIBMTP_mtpdevice_t *  MTPhandler::connectdevice(int devno){


    LIBMTP_mtpdevice_t * device = LIBMTP_Open_Raw_Device ( &rawdevices[devno]);
    if (device == NULL)
    {
        fprintf(stderr, "Unable to open raw device %d\n", devno);
        return NULL;
    }
    //LIBMTP_Dump_Errorstack(device);
    //LIBMTP_Clear_Errorstack(device);
    //LIBMTP_Dump_Device_Info(device);
    return device;
}

bool MTPhandler::sendFile(LIBMTP_mtpdevice_t *device, const char *from,const char *filename, uint32_t to_folder, const LIBMTP_progressfunc_t cb)
{
    bool ret=false;
    if(!device || !from || !to_folder)
    {
        fprintf(stderr,"MTPhandler: Nothing to be done \n");
        return false;
    }
    fprintf(stderr,"Sending %s \n",from);

    LIBMTP_filetype_t f = find_filetype(from);
    TagLib::FileRef tfile(from);
    if( tfile.isNull())
        return send_as_file(device,from,filename,to_folder,cb);
    TagLib::Tag *tag=tfile.tag();
    TagLib::AudioProperties *inf=tfile.audioProperties();
    if ( LIBMTP_FILETYPE_IS_TRACK(f) )
    {

        return send_as_track(device,from,filename,to_folder,tag,inf,cb);
    }
    else {

        return send_as_file(device,from,filename,to_folder,cb);
    }

    return ret;
}


static int add_track_to_album(LIBMTP_mtpdevice_t *device,LIBMTP_album_t *albuminfo, LIBMTP_track_t *trackmeta)
{
  LIBMTP_album_t *album;
  LIBMTP_album_t *found_album = NULL;
  int ret;

  /* Look for the album */
  album = LIBMTP_Get_Album_List(device);
  while(album != NULL) {
    if ((album->name != NULL &&
        album->artist != NULL &&
        !strcmp(album->name, albuminfo->name) &&
        !strcmp(album->artist, albuminfo->artist)) ||
          (album->name != NULL &&
        album->composer != NULL &&
        !strcmp(album->name, albuminfo->name) &&
        !strcmp(album->composer, albuminfo->composer))) {
      /* Disconnect this album for later use */
      found_album = album;
      album = album->next;
      found_album->next = NULL;
    } else {
      LIBMTP_album_t *tmp;

      tmp = album;
      album = album->next;
      LIBMTP_destroy_album_t(tmp);
    }
  }

  if (found_album != NULL) {
    uint32_t *tracks;

    tracks = (uint32_t *)malloc((found_album->no_tracks+1) * sizeof(uint32_t));
    printf("Album \"%s\" found: updating...\n", found_album->name);
    if (!tracks) {
      printf("failed malloc in add_track_to_album()\n");
      return 1;
    }
    found_album->no_tracks++;
    if (found_album->tracks != NULL) {
      memcpy(tracks, found_album->tracks, found_album->no_tracks * sizeof(uint32_t));
      free(found_album->tracks);
    }
    tracks[found_album->no_tracks-1] = trackmeta->item_id;
    found_album->tracks = tracks;
    ret = LIBMTP_Update_Album(device, found_album);
    LIBMTP_destroy_album_t(found_album);
  } else {
    uint32_t *trackid;

    trackid = (uint32_t *)malloc(sizeof(uint32_t));
    *trackid = trackmeta->item_id;
    albuminfo->tracks = trackid;
    albuminfo->no_tracks = 1;
    albuminfo->storage_id = trackmeta->storage_id;
    printf("Album doesn't exist: creating...\n");
    ret = LIBMTP_Create_New_Album(device, albuminfo);
    /* albuminfo will be destroyed later by caller */
  }

  if (ret != 0) {
    printf("Error creating or updating album.\n");
    printf("(This could be due to that your device does not support albums.)\n");
    LIBMTP_Dump_Errorstack(device);
    LIBMTP_Clear_Errorstack(device);
  } else {
    printf("success!\n");
  }
  return ret;
}

bool MTPhandler::send_as_file(LIBMTP_mtpdevice_t *device,const char *from,const char *filename, uint32_t to_folder,const LIBMTP_progressfunc_t cb)
{
    fprintf(stderr," NORMAL FILE \n");
    LIBMTP_file_t *filedata=LIBMTP_new_file_t();

    filedata->filename=strdup (filename );
    filedata->parent_id=to_folder;
    int ret=LIBMTP_Send_File_From_File(device,from,filedata,cb,NULL );

    if (ret != 0) {
       printf("Error sending file.\n");
       LIBMTP_Dump_Errorstack(device);
       LIBMTP_Clear_Errorstack(device);
    } else {
        printf("New file ID: %d\n", filedata->item_id);
    }
    LIBMTP_destroy_file_t(filedata);
    return ret;
}

bool MTPhandler::send_as_track(LIBMTP_mtpdevice_t *device,const char *from,const char *filename,uint32_t to_folder,
                   TagLib::Tag *tag,TagLib::AudioProperties *prop,const LIBMTP_progressfunc_t cb)
{
    int storageid=0;
    fprintf(stderr," Send Track . . . \n");
    LIBMTP_track_t *trackmeta;
    LIBMTP_album_t *albuminfo;
    int ret=0;

    trackmeta = LIBMTP_new_track_t();
    albuminfo = LIBMTP_new_album_t();

    trackmeta->filetype=find_filetype(from);
    trackmeta->title= strdup ( tag->title().to8Bit().data() );

    trackmeta->album= strdup ( tag->album().to8Bit().data() );
    albuminfo->name = strdup ( tag->album().to8Bit().data() );

    trackmeta->artist= strdup ( tag->artist().to8Bit().data() );
    albuminfo->artist= strdup ( tag->artist().to8Bit().data() );

    trackmeta->genre= strdup ( tag->genre().to8Bit().data() );
    albuminfo->genre= strdup ( tag->genre().to8Bit().data() );
    fprintf(stderr,"SONG; %s %s %s\n",trackmeta->title,trackmeta->album,trackmeta->artist);
    char *year_=new char[10];
    trackmeta->date = year_;
    sprintf(year_,"%d",tag->year());

    trackmeta->duration= prop->length();
    struct stat sb;
    stat(from, &sb);
    trackmeta->filename = strdup(filename);
    trackmeta->filesize = sb.st_size;
    trackmeta->parent_id= to_folder;
    trackmeta->tracknumber=tag->track();

   {
   int rc;
   char *desc = NULL;
   LIBMTP_devicestorage_t *pds = NULL;

   if ( 0 != (rc=LIBMTP_Get_Storage(device, LIBMTP_STORAGE_SORTBY_NOTSORTED)) )
   {   perror("LIBMTP_Get_Storage()");
        exit(-1);
   }
   for (pds = device->storage; pds != NULL; pds = pds->next)
   {
        if (pds->id == storageid)
        {   desc = strdup(pds->StorageDescription);
            break;
        }
   }
   if (NULL != desc)
   {   printf("Storage ID: %s (%u)\n", desc, storageid);
        free(desc);
   }
   else
        printf("Storage ID: %u\n", storageid);
   trackmeta->storage_id = storageid;
   }

   fprintf(stderr,"Sending track...\n");
   ret = LIBMTP_Send_Track_From_File(device, from, trackmeta, cb, NULL);
   if (ret != 0) {
      printf("Error sending track.\n");
      LIBMTP_Dump_Errorstack(device);
      LIBMTP_Clear_Errorstack(device);
    } else {
       printf("New track ID: %d\n", trackmeta->item_id);
    }

    /* Add here add to album call */
    if (albuminfo->name)
       ret = add_track_to_album(device,albuminfo, trackmeta);

    LIBMTP_destroy_album_t(albuminfo);
    LIBMTP_destroy_track_t(trackmeta);



    return ret;
}

int MTPhandler::create_folder(LIBMTP_mtpdevice_t *device, int p_id, const char *name)
{
    return LIBMTP_Create_Folder(device,(char *)name,p_id,0);
}
