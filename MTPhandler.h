#ifndef MTPHANDLER_H
#define MTPHANDLER_H

#include <libmtp.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

LIBMTP_folder_t *get_folder(int folder_id,LIBMTP_folder_t* root);
LIBMTP_folder_t *get_folder_by_name(const char *str,LIBMTP_folder_t* root);


class MTPhandler{
public:
    MTPhandler();
    ~MTPhandler();
    int updateDevices();
    LIBMTP_raw_device_t * rawdevices;
    int numrawdevices;

    LIBMTP_mtpdevice_t *device_list, *iter;
    LIBMTP_file_t *files;
    LIBMTP_mtpdevice_t *  connectdevice(int devno);

    bool sendFile(LIBMTP_mtpdevice_t *device,const char *from,const char *filename,uint32_t to_folder,const LIBMTP_progressfunc_t cb);

    bool send_as_file(LIBMTP_mtpdevice_t *device,const char *from,const char *filename,uint32_t to_folder,const LIBMTP_progressfunc_t cb);
    bool send_as_track(LIBMTP_mtpdevice_t *device,const char *from,const char *filename,uint32_t to_folder,
                       TagLib::Tag *,TagLib::AudioProperties *,const LIBMTP_progressfunc_t cb);
    int create_folder(LIBMTP_mtpdevice_t *device,int p_id, const char *name);

private:

    LIBMTP_error_number_t err;
    int i;
};


#endif // MTPHANDLER_H
