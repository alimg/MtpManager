#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MTPhandler.h"
#include "devfilesview.h"
#include <QMainWindow>
#include <QtGui>


Q_DECLARE_METATYPE ( LIBMTP_folder_t );


const static int FOLDER_ID_ROLE = 33;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MTPhandler mhandler;
    void updatefolders(const LIBMTP_folder_t *folder,QAbstractItemModel *mdl,QModelIndex index);
    void updatefiles(const LIBMTP_file_t *file,QAbstractItemModel *mdl,QModelIndex index);
    void openfolder_flv( unsigned int id);
private:
    Ui::MainWindow *ui;
    QStandardItemModel *devinfomodel;
    //QStandardItemModel *devfilemodel;
    LIBMTP_mtpdevice_t *Device;



    //Indicates current folder parent that showing on FilesListView
    unsigned int current_folder_id;
    int current_folder_UNSET; // Tells nothing is shown or current_folder_flv corrupt
    QString current_folder_path;
    //History of directories opened
    QList <LIBMTP_folder_t>dir_stack;

    void startFileUpload();
    void endFileUpload();
    int cloned_drop(void *arg);
    void update_adressline();
    QAction *NewFolderAct;
private slots:

    void on_adress_refresh_clicked();
    void on_devfilesview_customContextMenuRequested(QPoint pos);
    void on_adress_back_clicked();
    void on_adress_parent_clicked();
    void on_devfilesview_doubleClicked(QModelIndex index);
    void on_devfilestree_doubleClicked(QModelIndex index);
    void on_connect_clicked();
    void on_DevicesList_activated(int index);
    void on_refresh_clicked();
    void on_devfilesview_droppedItem(QList<QUrl> urls);

    void reload_files_list();
    void newfolder();
};

#endif // MAINWINDOW_H
