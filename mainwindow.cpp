#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MTPhandler.h"
#include "treemodel.h"
#include <QtGui>
#include <cstdio>
#include <sys/stat.h>
#include <signal.h>
#include <QInputDialog>

int selected_device=0;

QProgressBar *PBar;
int connected;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Device = NULL;
    QWidget *t5=this->findChild<QWidget *>("progresswidget");
    t5->hide();
    PBar=this->findChild<QProgressBar *>("progressBar");
    PBar->setMaximum(100);
    QPushButton *t3=this->findChild<QPushButton *>("refresh");
    t3->setIcon(QIcon::fromTheme("view-refresh"));

    t3=this->findChild<QPushButton *>("connect");
    t3->setEnabled(false);

    QToolButton *t6 ;
    t6=this->findChild<QToolButton *>("adress_parent");
    t6->setIcon(QIcon::fromTheme("go-up"));
    t6=this->findChild<QToolButton *>("adress_back");
    t6->setIcon(QIcon::fromTheme("go-previous"));
    t6=this->findChild<QToolButton *>("adress_refresh");
    t6->setIcon(QIcon::fromTheme("view-refresh"));


    QTreeView *t=this->findChild<QTreeView *>("devfilestree");

    QStringList headers;
    headers << tr("Folder") << tr("obj_id") << tr("data") ;
    TreeModel *mdl = new TreeModel(headers,parent);

    //mdl->setHeaderData(3,Qt::Vertical,"obj_id",Qt::UserRole);
    //proxyModel->setSourceModel(mdl);


    t->setRootIsDecorated(true);
    t->setAlternatingRowColors(true);
    t->setModel(mdl);
    t->setSortingEnabled(true);
    t->hideColumn(1);
    t->hideColumn(2);

    devinfomodel= new QStandardItemModel (0,2,parent );
    devinfomodel->setHeaderData(0,Qt::Horizontal, QObject::tr("Info") );
    devinfomodel->setHeaderData(1,Qt::Horizontal, QObject::tr("Value") );
    devinfomodel->insertRows(0,7);
    devinfomodel->setData(devinfomodel->index(0, 0), "Name");
    devinfomodel->setData(devinfomodel->index(1, 0), "Flags" );
    devinfomodel->setData(devinfomodel->index(2, 0), "Product" );
    devinfomodel->setData(devinfomodel->index(3, 0), "Product ID" );
    devinfomodel->setData(devinfomodel->index(4, 0), "Vendor" );
    devinfomodel->setData(devinfomodel->index(5, 0), "Vendor ID" );
    devinfomodel->setData(devinfomodel->index(6, 0), "Bus location" );
    QTreeView *t2=this->findChild<QTreeView *>("devinfo");
    t2->setModel(devinfomodel);
    t2->setRootIsDecorated(false);
    //t2->setEditTriggers( QAbstractItemView::NoEditTriggers);


    headers.clear();
    headers << tr("File") << tr("Size") << tr("Type")<< tr("obj_id") << tr("obj_parent_id") ;
    QTreeView *t4=this->findChild<QTreeView *>("devfilesview");
    //connect(t4,SIGNAL(droppedItem(QList<QUrl>)),this,SLOT(on_devfilesview_droppedItem(QList<QUrl>)));
    TreeModel *t4mdl= new TreeModel(headers,parent);
    t4->setModel(t4mdl);
    t4->setRootIsDecorated(false);
    t4->setAlternatingRowColors(true);
    t4->setSortingEnabled(true);
    t4->hideColumn(3);t4->hideColumn(4);t4->setColumnWidth(0,250);t4->setColumnWidth(1,70);t4->setColumnWidth(2,70);
    t4->acceptDrops();
    t4->setDropIndicatorShown(true);
    t4->setDragEnabled(true);
    t4->setAcceptDrops(true);

    setAcceptDrops(true);
    NewFolderAct=new QAction(tr("New folder"),this);
    connect(NewFolderAct, SIGNAL(triggered()), this, SLOT(newfolder()));
}

MainWindow::~MainWindow()
{


    if(Device)
        LIBMTP_Release_Device(Device);


    delete ui;
}


void MainWindow::on_refresh_clicked()
{
    mhandler.updateDevices();
    QComboBox* l=this->findChild<QComboBox *>("DevicesList");
    l->clear();
    int k=mhandler.numrawdevices;
    if(k<1)
    {
        l->addItem("No Devices");
        return;
    }
    else {

        QPushButton *t3=this->findChild<QPushButton *>("connect");
        t3->setEnabled(true);
    }

    int i;
    for (i=0;i<k;i++)
    {
        if(mhandler.rawdevices[i].device_entry.product)
            l->addItem(mhandler.rawdevices[i].device_entry.product);
        else if(mhandler.rawdevices[i].device_entry.vendor)
            l->addItem(mhandler.rawdevices[i].device_entry.vendor);
        else l->addItem("<unknown device>");
    }
    on_DevicesList_activated(0);
}


void MainWindow::on_DevicesList_activated(int index)
{
    if(mhandler.numrawdevices<1)
        return ;

    selected_device=index;

    devinfomodel->setData(devinfomodel->index(1, 1), mhandler.rawdevices[index].device_entry.device_flags);
    devinfomodel->setData(devinfomodel->index(2, 1), mhandler.rawdevices[index].device_entry.product);
    devinfomodel->setData(devinfomodel->index(3, 1), mhandler.rawdevices[index].device_entry.product_id);
    devinfomodel->setData(devinfomodel->index(4, 1), mhandler.rawdevices[index].device_entry.vendor);
    devinfomodel->setData(devinfomodel->index(5, 1), mhandler.rawdevices[index].device_entry.vendor_id);
    devinfomodel->setData(devinfomodel->index(6, 1), mhandler.rawdevices[index].bus_location);


}



#define FOLDER_PARENT(p,a) {\
         p->child=a;\
         p->folder_id=a->parent_id;\
                            p->sibling=NULL;}

void MainWindow::reload_files_list()
{
    LIBMTP_folder_t *Folder =LIBMTP_Get_Folder_List(Device);



    char *friendlyname;
    friendlyname = LIBMTP_Get_Friendlyname(Device);
    fprintf(stderr,"reload files %s\n",friendlyname);

    QTreeView *t2=this->findChild<QTreeView *>("devfilestree");
    QAbstractItemModel *mdl= t2->model();
    mdl->removeRows(0,mdl->rowCount());
    mdl->insertRows(0,1);
    mdl->setData(mdl->index(0,0),friendlyname);
    mdl->setData(mdl->index(0,0),QIcon::fromTheme("multimedia-player"),Qt::DecorationRole);



    updatefolders(Folder,mdl,mdl->index(0,0));
    LIBMTP_folder_t *root;


    if(current_folder_UNSET) //no root directory.
    {
        LIBMTP_folder_t a;
        FOLDER_PARENT((&a),Folder)
        dir_stack.append(a);
        current_folder_id=a.folder_id;
    }
    else {
        root=get_folder_by_name(current_folder_path.toStdString().data(),Folder);
        fprintf(stderr,"root %s\n", root->name);
    }

    LIBMTP_destroy_folder_t(Folder);

    openfolder_flv(current_folder_id);
    current_folder_UNSET=0;
    free(friendlyname);

}
void MainWindow::on_adress_refresh_clicked()
{
    if(!connected)
        return;
    reload_files_list();
}
void MainWindow::on_connect_clicked()
{
    QStatusBar *t=this->findChild<QStatusBar *>("statusbar");
    t->showMessage(QObject::tr("Please Wait"));
    t->repaint();
    QCoreApplication::processEvents();

    if(Device)
        LIBMTP_Release_Device(Device);

    current_folder_UNSET=1;
    Device = mhandler.connectdevice(selected_device) ;

    t->clearMessage();

    reload_files_list();
    connected=1;
}
void MainWindow::on_devfilestree_doubleClicked(QModelIndex index)
{
   QTreeView *t2=this->findChild<QTreeView *>("devfilestree");
   QAbstractItemModel *mdl= t2->model();
   QVariant v= mdl->data( index,FOLDER_ID_ROLE);
   if(v!=QVariant())
   {
       LIBMTP_folder_t *p;
       p=((LIBMTP_folder_t*) v.data());
       if(!p)return;
       fprintf(stderr,"open %u \n",p->folder_id);

       LIBMTP_folder_t * root=LIBMTP_Get_Folder_List(Device);

       p=get_folder(p->folder_id,root);
       dir_stack.append(*p);

       fprintf(stderr,"name %s \n",p->name);

       openfolder_flv( p->folder_id );
       LIBMTP_destroy_folder_t(root);
   }
}

void MainWindow::updatefolders(const LIBMTP_folder_t *folder, QAbstractItemModel *mdl,QModelIndex index)
{

    if ( !folder)
    {
        return;
    }

    mdl->insertRows(0,1,index);

    mdl->setData(index.child(0,0),folder->name);

    mdl->setData(index.child(0,0),QIcon::fromTheme("folder"),Qt::DecorationRole);
    mdl->setData(index.child(0,0),QVariant::fromValue(*folder),FOLDER_ID_ROLE);
    //mdl->setData(index.child(0,4),folder->storage_id,Qt::UserRole);
    mdl->setData(index.child(0,1),folder->folder_id);

    updatefolders(folder->child,mdl,index.child(0,0));
    updatefolders(folder->sibling,mdl,index);
}

void MainWindow::update_adressline()
{
    QLineEdit *t=findChild<QLineEdit *>("adressline");

    QString str;
    LIBMTP_folder_t * root=LIBMTP_Get_Folder_List(Device);

    LIBMTP_folder_t *p=get_folder(current_folder_id,root);


    str.push_back(tr("\\"));
    while(p) {
        str.push_front(tr(p->name));
        str.push_front('\\');
        p=get_folder(p->parent_id,root);
    }

    current_folder_path=str;

    //fprintf(stderr,"%s\n",str.toAscii().data());
    t->setText(str);
    t->update( );

    LIBMTP_destroy_folder_t(root);
}

void MainWindow::openfolder_flv( unsigned int id)
{
    current_folder_id=id;
    LIBMTP_folder_t * root=LIBMTP_Get_Folder_List(Device);
    LIBMTP_file_t *File =LIBMTP_Get_Filelisting_With_Callback(Device,0,0);

    QTreeView *t=this->findChild<QTreeView *>("devfilesview");
    QAbstractItemModel *mdl= t->model();
    mdl->removeRows(0,mdl->rowCount());

    //if(folder==current_folder_flv)
    //    fprintf(stderr,"Invalid pointer!\n");
    // List children of folder
    LIBMTP_folder_t *p=get_folder(current_folder_id,root);
    if(!p)p=root;
    else p=p->child;
    for(;p;p=p->sibling)
    {
       mdl->insertRows(0,1);
       mdl->setData(mdl->index(0,0),p->name);
       mdl->setData(mdl->index(0,2),"folder");
       mdl->setData(mdl->index(0,0),QVariant::fromValue(*p),FOLDER_ID_ROLE);
       mdl->setData(mdl->index(0,0),QIcon::fromTheme("folder"),Qt::DecorationRole);
    }
    LIBMTP_file_t *pt2=File;


    for(;pt2;)
    {
        if(pt2->parent_id==current_folder_id)
        {
            mdl->insertRows(0,1);
            mdl->setData(mdl->index(0,0),pt2->filename);
            mdl->setData(mdl->index(0,1),QVariant::fromValue(pt2->filesize));
            mdl->setData(mdl->index(0,2),pt2->filetype);
            mdl->setData(mdl->index(0,0),QIcon::fromTheme("text-x-generic"),Qt::DecorationRole);
        }
        File=pt2;
        pt2=pt2->next;
        LIBMTP_destroy_file_t(File);
    }


    update_adressline();
    LIBMTP_destroy_folder_t(root);
}

void MainWindow::on_devfilesview_doubleClicked(QModelIndex index)
{
    QTreeView *t=this->findChild<QTreeView *>("devfilesview");
    QAbstractItemModel *mdl= t->model();
    if(mdl->data(mdl->index(index.row(),2))==QString("folder")){
        LIBMTP_folder_t p=*(LIBMTP_folder_t *)mdl->data(mdl->index(index.row(),0),FOLDER_ID_ROLE).data();
        dir_stack.append(p);

        openfolder_flv(p.folder_id);

    }
}

void MainWindow::on_adress_parent_clicked()
{
    if (dir_stack.length()<=1) return;
    if (dir_stack.last().parent_id == 0) // to avoid doubles
    {
        openfolder_flv( dir_stack[0].folder_id );

        return ;
    }
    LIBMTP_folder_t * root=LIBMTP_Get_Folder_List(Device);
    LIBMTP_folder_t *f=get_folder(dir_stack.last().parent_id,root);
    if(!f)return;
    dir_stack.append(*f);
    openfolder_flv( dir_stack.last().folder_id );
    LIBMTP_destroy_folder_t(root);

}

void MainWindow::on_adress_back_clicked()
{
    if (dir_stack.length()<2) return;

    dir_stack.removeLast();
    openfolder_flv( dir_stack.last().folder_id );
}


void MainWindow::newfolder()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Create new folder"),
                                             tr("Folder name"), QLineEdit::Normal,
                                             tr("Untitled"), &ok);
    if (ok && !text.isEmpty() && connected )
    {
        fprintf(stderr,"new folder --- \n");
        mhandler.create_folder(Device,current_folder_id,text.toStdString().data());
    }

    reload_files_list();
}

void MainWindow::on_devfilesview_customContextMenuRequested(QPoint pos)
{
    QMenu menu(this);
    menu.addAction(NewFolderAct);
    QTreeView *t4=this->findChild<QTreeView *>("devfilesview");

    menu.exec(t4->mapToGlobal(pos));
    //fprintf(stderr,"asdgfa \n");
}




void MainWindow::startFileUpload()
{
    QWidget *t5=this->findChild<QWidget *>("progresswidget");
    t5->show();

}
void MainWindow::endFileUpload()
{
    QWidget *t5=this->findChild<QWidget *>("progresswidget");
    t5->hide();

}

int sendfile_callback(const uint64_t sent, const uint64_t total, void const * const data)
{
    int percent = (sent*100)/total;

    //fprintf(stderr,"Progress: %llu of %llu (%d%%)\r", sent, total, percent);
    //fflush(stdout);
    PBar->setValue(percent);
    return 0;
}




class FileSendThread : public QThread
{
    public:
        void run();
        QAbstractItemModel *mdl;
        QList<QUrl> urls;
        MTPhandler *mhandler;
        LIBMTP_mtpdevice_t *Device;
        LIBMTP_folder_t *current_folder_flv;
        uint32_t current_folder_id;
};
void FileSendThread::run()
{
    fprintf(stderr,"DropItem\n");

    QUrl a;
    foreach(a,urls)
    {
        int i;
        char *path= strdup( a.path().toStdString().data() );
        fprintf(stderr,"%s\n",path);
        struct stat sb;
        stat(path,&sb);

        if(S_ISREG(sb.st_mode))
        {
            // path is a regular file

            char *fileName;

            for(fileName=path,i=strlen(path);i>0;i--)
                if(path[i]=='/')
                {
                    fileName=path+i+1;

                    break;
                }
            if( mdl->match(mdl->index(0,0),Qt::EditRole,QString(fileName),-1,0).count() )
            {
                //
            }
            else{

                mhandler->sendFile(Device, path ,fileName, current_folder_id, &sendfile_callback );
            }

        }
        else if(S_ISDIR(sb.st_mode))
        {
            fprintf(stderr,"fixme: Directory sending is NOT YET implemented.\n");
        }
        free(path);
    }
}

void MainWindow::on_devfilesview_droppedItem(QList<QUrl> urls)
{
    if(!Device)
        return;
    FileSendThread t;
    t.urls=urls;
    t.mhandler=&mhandler;
    QTreeView *t2=this->findChild<QTreeView *>("devfilesview");
    t.mdl= t2->model();
    t.Device=Device;
    LIBMTP_folder_t * root=LIBMTP_Get_Folder_List(Device);
    LIBMTP_folder_t *folder =get_folder(current_folder_id,root);
    if(folder==NULL)
        folder==root;

    t.current_folder_id=folder->folder_id;

    LIBMTP_destroy_folder_t(root);
    startFileUpload();
    t.run();
    endFileUpload();
}



