#include "devfilesview.h"
#include <cstdio>
#include <QMainWindow>
#include <QtGui>
#include <QTreeView>
DevFilesView::DevFilesView(QWidget *parent) :
    QTreeView(parent)
{

    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
}

void DevFilesView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}
void DevFilesView::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
     event->acceptProposedAction();
}
void DevFilesView::dropEvent(QDropEvent *event)
{
    if(!event->mimeData()->hasUrls())
        return;
    event->acceptProposedAction();

    emit droppedItem(event->mimeData()->urls());

}
void DevFilesView::contextMenuEvent(QContextMenuEvent *event)
{
    //fprintf(stderr,"asdfasdf\n");

}
