/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lbBackground;
    QLabel *lbMid;
    QLabel *lbTop;
    QLabel *lbBot;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(960, 540);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(236, 49, 60);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lbBackground = new QLabel(centralwidget);
        lbBackground->setObjectName(QString::fromUtf8("lbBackground"));
        lbBackground->setGeometry(QRect(0, 0, 960, 540));
        lbBackground->setPixmap(QPixmap(QString::fromUtf8(":/pic/background_pic_1.jpg")));
        lbBackground->setScaledContents(true);
        lbMid = new QLabel(centralwidget);
        lbMid->setObjectName(QString::fromUtf8("lbMid"));
        lbMid->setEnabled(false);
        lbMid->setGeometry(QRect(190, 140, 580, 220));
        lbMid->setFrameShape(QFrame::NoFrame);
        lbMid->setAlignment(Qt::AlignCenter);
        lbTop = new QLabel(centralwidget);
        lbTop->setObjectName(QString::fromUtf8("lbTop"));
        lbTop->setEnabled(false);
        lbTop->setGeometry(QRect(280, 20, 400, 120));
        lbTop->setFrameShape(QFrame::NoFrame);
        lbTop->setAlignment(Qt::AlignCenter);
        lbBot = new QLabel(centralwidget);
        lbBot->setObjectName(QString::fromUtf8("lbBot"));
        lbBot->setEnabled(false);
        lbBot->setGeometry(QRect(300, 360, 360, 130));
        lbBot->setFrameShape(QFrame::NoFrame);
        lbBot->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);
        lbMid->raise();
        lbTop->raise();
        lbBot->raise();
        lbBackground->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lbBackground->setText(QString());
        lbMid->setText(QCoreApplication::translate("MainWindow", "\344\270\255", nullptr));
        lbTop->setText(QCoreApplication::translate("MainWindow", "\344\270\212", nullptr));
        lbBot->setText(QCoreApplication::translate("MainWindow", "\344\270\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
