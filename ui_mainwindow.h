/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Dec 25 17:54:53 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QProgressBar>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>
#include "devfilesview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *progresswidget;
    QHBoxLayout *progressbar;
    QLabel *operation;
    QProgressBar *progressBar;
    QToolButton *cancel_operation;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QTreeView *devinfo;
    QLabel *label;
    QPushButton *connect;
    QPushButton *refresh;
    QComboBox *DevicesList;
    QTabWidget *devicetabs;
    QWidget *foldervt;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter;
    QTreeView *devfilestree;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *adress_back;
    QToolButton *adress_parent;
    QToolButton *adress_refresh;
    QLineEdit *adressline;
    DevFilesView *devfilesview;
    QWidget *mediavt;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(873, 468);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setSizeIncrement(QSize(2, 2));
        MainWindow->setBaseSize(QSize(2, 2));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        progresswidget = new QWidget(centralwidget);
        progresswidget->setObjectName(QString::fromUtf8("progresswidget"));
        progresswidget->setMinimumSize(QSize(855, 27));
        progressbar = new QHBoxLayout(progresswidget);
        progressbar->setObjectName(QString::fromUtf8("progressbar"));
        operation = new QLabel(progresswidget);
        operation->setObjectName(QString::fromUtf8("operation"));

        progressbar->addWidget(operation);

        progressBar = new QProgressBar(progresswidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        progressbar->addWidget(progressBar);

        cancel_operation = new QToolButton(progresswidget);
        cancel_operation->setObjectName(QString::fromUtf8("cancel_operation"));

        progressbar->addWidget(cancel_operation);


        verticalLayout_2->addWidget(progresswidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(230, 320));
        devinfo = new QTreeView(groupBox);
        devinfo->setObjectName(QString::fromUtf8("devinfo"));
        devinfo->setGeometry(QRect(17, 90, 191, 231));
        devinfo->setContextMenuPolicy(Qt::ActionsContextMenu);
        devinfo->setSelectionMode(QAbstractItemView::NoSelection);
        devinfo->header()->setVisible(false);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 70, 78, 17));
        connect = new QPushButton(groupBox);
        connect->setObjectName(QString::fromUtf8("connect"));
        connect->setGeometry(QRect(150, 60, 61, 16));
        refresh = new QPushButton(groupBox);
        refresh->setObjectName(QString::fromUtf8("refresh"));
        refresh->setGeometry(QRect(180, 30, 31, 27));
        DevicesList = new QComboBox(groupBox);
        DevicesList->setObjectName(QString::fromUtf8("DevicesList"));
        DevicesList->setGeometry(QRect(10, 30, 171, 27));

        horizontalLayout->addWidget(groupBox);

        devicetabs = new QTabWidget(centralwidget);
        devicetabs->setObjectName(QString::fromUtf8("devicetabs"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(devicetabs->sizePolicy().hasHeightForWidth());
        devicetabs->setSizePolicy(sizePolicy2);
        foldervt = new QWidget();
        foldervt->setObjectName(QString::fromUtf8("foldervt"));
        verticalLayout_3 = new QVBoxLayout(foldervt);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget = new QWidget(foldervt);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        splitter = new QSplitter(widget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setEnabled(true);
        sizePolicy2.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy2);
        splitter->setOrientation(Qt::Horizontal);
        devfilestree = new QTreeView(splitter);
        devfilestree->setObjectName(QString::fromUtf8("devfilestree"));
        sizePolicy1.setHeightForWidth(devfilestree->sizePolicy().hasHeightForWidth());
        devfilestree->setSizePolicy(sizePolicy1);
        devfilestree->setMinimumSize(QSize(100, 261));
        devfilestree->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        devfilestree->setDragEnabled(true);
        devfilestree->setDragDropOverwriteMode(true);
        devfilestree->setDragDropMode(QAbstractItemView::DragDrop);
        devfilestree->setIconSize(QSize(16, 16));
        devfilestree->setSortingEnabled(true);
        splitter->addWidget(devfilestree);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        sizePolicy2.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy2);
        widget_2->setMinimumSize(QSize(400, 262));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget1 = new QWidget(widget_2);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setMinimumSize(QSize(301, 41));
        horizontalLayout_3 = new QHBoxLayout(widget1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        adress_back = new QToolButton(widget1);
        adress_back->setObjectName(QString::fromUtf8("adress_back"));

        horizontalLayout_3->addWidget(adress_back);

        adress_parent = new QToolButton(widget1);
        adress_parent->setObjectName(QString::fromUtf8("adress_parent"));

        horizontalLayout_3->addWidget(adress_parent);

        adress_refresh = new QToolButton(widget1);
        adress_refresh->setObjectName(QString::fromUtf8("adress_refresh"));

        horizontalLayout_3->addWidget(adress_refresh);

        adressline = new QLineEdit(widget1);
        adressline->setObjectName(QString::fromUtf8("adressline"));

        horizontalLayout_3->addWidget(adressline);


        verticalLayout->addWidget(widget1);

        devfilesview = new DevFilesView(widget_2);
        devfilesview->setObjectName(QString::fromUtf8("devfilesview"));
        devfilesview->setMinimumSize(QSize(301, 211));
        devfilesview->setContextMenuPolicy(Qt::CustomContextMenu);
        devfilesview->setAcceptDrops(true);
        devfilesview->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        devfilesview->setDragDropOverwriteMode(true);
        devfilesview->setDragDropMode(QAbstractItemView::DragDrop);
        devfilesview->setDefaultDropAction(Qt::CopyAction);

        verticalLayout->addWidget(devfilesview);

        splitter->addWidget(widget_2);

        horizontalLayout_2->addWidget(splitter);


        verticalLayout_3->addWidget(widget);

        devicetabs->addTab(foldervt, QString());
        mediavt = new QWidget();
        mediavt->setObjectName(QString::fromUtf8("mediavt"));
        devicetabs->addTab(mediavt, QString());

        horizontalLayout->addWidget(devicetabs);


        verticalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 873, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        devicetabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MTP Manager", 0));
        operation->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        cancel_operation->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Search Devices", 0));
        label->setText(QApplication::translate("MainWindow", "Device Info:", 0));
        connect->setText(QApplication::translate("MainWindow", "connect", 0));
        adress_back->setText(QApplication::translate("MainWindow", "...", 0));
        adress_parent->setText(QApplication::translate("MainWindow", "...", 0));
        adress_refresh->setText(QApplication::translate("MainWindow", "...", 0));
        devicetabs->setTabText(devicetabs->indexOf(foldervt), QApplication::translate("MainWindow", "File Manager", 0));
        devicetabs->setTabText(devicetabs->indexOf(mediavt), QApplication::translate("MainWindow", "Media", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
