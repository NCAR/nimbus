/********************************************************************************
** Form generated from reading UI file 'ProxyMainWindow.ui'
**
** Created: Mon Dec 31 17:07:56 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROXYMAINWINDOW_H
#define UI_PROXYMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProxyMainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *_connect;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *_msgsFromUser;
    QLabel *label;
    QLabel *_msgsFromSwitch;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ProxyMainWindow)
    {
        if (ProxyMainWindow->objectName().isEmpty())
            ProxyMainWindow->setObjectName(QString::fromUtf8("ProxyMainWindow"));
        ProxyMainWindow->resize(829, 176);
        centralwidget = new QWidget(ProxyMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        _connect = new QPushButton(centralwidget);
        _connect->setObjectName(QString::fromUtf8("_connect"));

        verticalLayout_2->addWidget(_connect);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setTitle(QString::fromUtf8("Messages"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        _msgsFromUser = new QLabel(groupBox);
        _msgsFromUser->setObjectName(QString::fromUtf8("_msgsFromUser"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(_msgsFromUser->sizePolicy().hasHeightForWidth());
        _msgsFromUser->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(_msgsFromUser, 0, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        _msgsFromSwitch = new QLabel(groupBox);
        _msgsFromSwitch->setObjectName(QString::fromUtf8("_msgsFromSwitch"));
        sizePolicy1.setHeightForWidth(_msgsFromSwitch->sizePolicy().hasHeightForWidth());
        _msgsFromSwitch->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(_msgsFromSwitch, 1, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        ProxyMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ProxyMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 829, 22));
        ProxyMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ProxyMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ProxyMainWindow->setStatusBar(statusbar);

        retranslateUi(ProxyMainWindow);

        QMetaObject::connectSlotsByName(ProxyMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ProxyMainWindow)
    {
        ProxyMainWindow->setWindowTitle(QApplication::translate("ProxyMainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        _connect->setText(QApplication::translate("ProxyMainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ProxyMainWindow", "From user:", 0, QApplication::UnicodeUTF8));
        _msgsFromUser->setText(QApplication::translate("ProxyMainWindow", "user", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProxyMainWindow", "from inst:", 0, QApplication::UnicodeUTF8));
        _msgsFromSwitch->setText(QApplication::translate("ProxyMainWindow", "instrument", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProxyMainWindow: public Ui_ProxyMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROXYMAINWINDOW_H
