#include <QStringList>
#include <cstdio>
#include "treeitem.h"
#include "mainwindow.h"


TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent, Qt::ItemFlags f)
 {
     parentItem = parent;
     itemData = data;
     flagData.insert(0,itemData.size(),f);
     decData.insert(0,itemData.size(),QVariant());

 }

 TreeItem::~TreeItem()
 {
     qDeleteAll(childItems);
 }

 TreeItem *TreeItem::child(int number)
 {
     return childItems.value(number);
 }

 int TreeItem::childCount() const
 {
     return childItems.count();
 }

 int TreeItem::childNumber() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

     return 0;
 }

 int TreeItem::columnCount() const
 {
     return itemData.count();
 }

 QVariant TreeItem::data(int column,int role) const
 {
     if(role==Qt::DecorationRole)
     {
         return decData.value(column);
     }
     if(role==FOLDER_ID_ROLE)
         return folderd;
     return itemData.value(column);
 }

 bool TreeItem::insertChildren(int position, int count, int columns)
 {
     if (position < 0 || position > childItems.size())
         return false;

     for (int row = 0; row < count; ++row) {
         QVector<QVariant> data(columns);
         TreeItem *item = new TreeItem(data, this);
         childItems.insert(position, item);
     }

     return true;
 }

 bool TreeItem::insertColumns(int position, int columns)
 {
     if (position < 0 || position > itemData.size())
         return false;

     for (int column = 0; column < columns; ++column)
         itemData.insert(position, QVariant());

     foreach (TreeItem *child, childItems)
         child->insertColumns(position, columns);

     return true;
 }

 TreeItem *TreeItem::parent()
 {
     return parentItem;
 }

 bool TreeItem::removeChildren(int position, int count)
 {
     if (position < 0 || position + count > childItems.size())
         return false;

     for (int row = 0; row < count; ++row)
         delete childItems.takeAt(position);

     return true;
 }

 bool TreeItem::removeColumns(int position, int columns)
 {
     if (position < 0 || position + columns > itemData.size())
         return false;

     for (int column = 0; column < columns; ++column)
         itemData.remove(position);

     foreach (TreeItem *child, childItems)
         child->removeColumns(position, columns);

     return true;
 }

 bool TreeItem::setData(int column, const QVariant &value,int role)
 {
     if (column < 0 || column >= itemData.size())
         return false;

     switch(role)
     {
     case Qt::EditRole:
         itemData[column] = value;
         break;
     case Qt::DecorationRole:
         decData[column] = value;
         break;
     case FOLDER_ID_ROLE:
        folderd=value;
        break;

     }

     return true;
 }


 bool TreeItem::setFlags(int column,Qt::ItemFlags flag){
    flagData[column]=flag;
    return 1;
 }

 Qt::ItemFlags TreeItem::flags(int column)
 {
     return flagData[column];

 }

