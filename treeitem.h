#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
 #include <QVariant>
 #include <QVector>

 class TreeItem
 {
 public:
     TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0,  Qt::ItemFlags f=Qt::ItemFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled));
     ~TreeItem();

     TreeItem *child(int number);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column , int role=Qt::EditRole) const;
     bool insertChildren(int position, int count, int columns);
     bool insertColumns(int position, int columns);
     TreeItem *parent();
     bool removeChildren(int position, int count);
     bool removeColumns(int position, int columns);
     int childNumber() const;
     bool setData(int column, const QVariant &value,int role=Qt::EditRole);
     bool setFlags(int column,Qt::ItemFlags flag);
     Qt::ItemFlags flags(int column);
 private:
     QList<TreeItem*> childItems;
     QVector<QVariant> itemData;
     QVector<QVariant> decData;
     QVector<Qt::ItemFlags> flagData;
     QVariant folderd;
     TreeItem *parentItem;
 };

#endif // TREEITEM_H
