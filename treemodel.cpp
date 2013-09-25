#include <QtGui>

 #include "treeitem.h"
 #include "treemodel.h"

 TreeModel::TreeModel(const QStringList &headers,
                      QObject *parent)
     : QAbstractItemModel(parent)
 {
     QVector<QVariant> rootData;
     foreach (QString header, headers)
         rootData << header;

     rootItem = new TreeItem(rootData,0,Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
     //setupModelData(data.split(QString("\n")), rootItem);
 }

 TreeModel::~TreeModel()
 {
     delete rootItem;
 }

 int TreeModel::columnCount(const QModelIndex & /* parent */) const
 {
     return rootItem->columnCount();
 }

 QVariant TreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DecorationRole && role != Qt::DisplayRole && role != Qt::EditRole && role<32)
         return QVariant();

     TreeItem *item = getItem(index);

     return item->data(index.column(),role);
 }

 Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;
     TreeItem *item = getItem(index);
     return item->flags(index.column());
     //return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }



 TreeItem *TreeModel::getItem(const QModelIndex &index) const
 {
     if (index.isValid()) {
         TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
         if (item) return item;
     }
     return rootItem;
 }

 QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
 {
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();

     TreeItem *parentItem = getItem(parent);

     TreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
 {
     bool success;

     beginInsertColumns(parent, position, position + columns - 1);
     success = rootItem->insertColumns(position, columns);
     endInsertColumns();

     return success;
 }

 bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
 {
     TreeItem *parentItem = getItem(parent);
     bool success;

     beginInsertRows(parent, position, position + rows - 1);
     success = parentItem->insertChildren(position, rows, rootItem->columnCount());
     endInsertRows();

     return success;
 }

 QModelIndex TreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TreeItem *childItem = getItem(index);
     TreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->childNumber(), 0, parentItem);
 }

 bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
 {
     bool success;

     beginRemoveColumns(parent, position, position + columns - 1);
     success = rootItem->removeColumns(position, columns);
     endRemoveColumns();

     if (rootItem->columnCount() == 0)
         removeRows(0, rowCount());

     return success;
 }

 bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
 {
     TreeItem *parentItem = getItem(parent);
     bool success = true;

     beginRemoveRows(parent, position, position + rows - 1);
     success = parentItem->removeChildren(position, rows);
     endRemoveRows();

     return success;
 }

 int TreeModel::rowCount(const QModelIndex &parent) const
 {
     TreeItem *parentItem = getItem(parent);

     return parentItem->childCount();
 }
 bool TreeModel::setFlags(const QModelIndex &index,Qt::ItemFlags flag)
 {
    TreeItem *item = getItem(index);
    return item->setFlags(index.column(),flag);
 }

 bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                         int role)
 {
     //if (role != Qt::EditRole && role!=Qt::DecorationRole)
     //    return false;

     TreeItem *item = getItem(index);
     bool result = item->setData(index.column(), value, role);

     if (result)
         emit dataChanged(index, index);

     return result;
 }

 bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                               const QVariant &value, int role)
 {
     if (role != Qt::EditRole || orientation != Qt::Horizontal)
         return false;

     bool result = rootItem->setData(section, value);

     if (result)
         emit headerDataChanged(orientation, section, section);

     return result;
 }


 QStringList TreeModel::mimeTypes() const
 {
     QStringList types;
     types << "application/vnd.text.list";
     return types;
 }
 QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             QString text = data(index, Qt::DisplayRole).toString();
             stream << text;
         }
     }

     mimeData->setData("application/vnd.text.list", encodedData);
     return mimeData;
 }

 bool TreeModel::dropMimeData(const QMimeData *data,
     Qt::DropAction action, int row, int column, const QModelIndex &parent)
 {
     fprintf(stderr,"sdfsdf \n");
     if (action == Qt::IgnoreAction)
         return true;

     if (!data->hasFormat("application/vnd.text.list"))
         return false;

     if (column > 0)
         return false;

     int beginRow;

     if (row != -1)
         beginRow = row;
     else if (parent.isValid())
          beginRow = parent.row();
     else
     beginRow = rowCount(QModelIndex());
     QByteArray encodedData = data->data("application/vnd.text.list");
     QDataStream stream(&encodedData, QIODevice::ReadOnly);
     QStringList newItems;
     int rows = 0;

     while (!stream.atEnd()) {
         QString text;
         stream >> text;
         newItems << text;
         ++rows;
     }
     insertRows(beginRow, rows, QModelIndex());
     foreach (QString text, newItems) {
         QModelIndex idx = index(beginRow, 0, QModelIndex());
         setData(idx, text);
         beginRow++;
     }

     return true;
 }

 Qt::DropActions TreeModel::supportedDropActions() const
 {
     return Qt::CopyAction | Qt::MoveAction;
 }
 void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
 {
     QList<TreeItem*> parents;
     QList<int> indentations;
     parents << parent;
     indentations << 0;

     int number = 0;

     while (number < lines.count()) {
         int position = 0;
         while (position < lines[number].length()) {
             if (lines[number].mid(position, 1) != " ")
                 break;
             position++;
         }

         QString lineData = lines[number].mid(position).trimmed();

         if (!lineData.isEmpty()) {
             // Read the column data from the rest of the line.
             QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
             QVector<QVariant> columnData;
             for (int column = 0; column < columnStrings.count(); ++column)
                 columnData << columnStrings[column];

             if (position > indentations.last()) {
                 // The last child of the current parent is now the new parent
                 // unless the current parent has no children.

                 if (parents.last()->childCount() > 0) {
                     parents << parents.last()->child(parents.last()->childCount()-1);
                     indentations << position;
                 }
             } else {
                 while (position < indentations.last() && parents.count() > 0) {
                     parents.pop_back();
                     indentations.pop_back();
                 }
             }

             // Append a new item to the current parent's list of children.
             TreeItem *parent = parents.last();
             parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
             for (int column = 0; column < columnData.size(); ++column)
                 parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
         }

         number++;
     }
 }
