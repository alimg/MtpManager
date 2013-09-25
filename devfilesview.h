#ifndef DEVFILESVIEW_H
#define DEVFILESVIEW_H

#include <QUrl>
#include <QTreeView>



class DevFilesView : public QTreeView
{
    Q_OBJECT
public:
    explicit DevFilesView(QWidget *parent = 0);

    QString droppedURLs;

protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
signals:
    void droppedItem(QList<QUrl> urls);

public slots:
};

#endif // DEVFILESVIEW_H
