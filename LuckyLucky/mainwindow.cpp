#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"
#include <QFile>
#include <QMouseEvent>
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <QPainter>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isWindowMax = false;
    isError = false;
    isWindowShow = false;
    ShowDelayCnt = 1;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    timer->start(1000);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    bgimage = QImage(":/pic/background_pic_1.jpg");

    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");

    // 加载CSS文件
    QString ccsPath = QCoreApplication::applicationDirPath();
    QString ccsFile = ccsPath + "/config/stylesheet.css";
    qDebug()<<"Load css file:"+ccsFile;
    QFile fCss(ccsFile);
    if( !fCss.open(QFile::ReadOnly))
    {
        qDebug("Load css file failed.");
    }
    else
    {
        QString strCssContent(fCss.readAll());
        setStyleSheet(strCssContent);
        fCss.close();
    }

    // 加载配置文件
    QString configPath = QCoreApplication::applicationDirPath();
    QString configFile = ccsPath + "/config/config.txt";
    qDebug()<<"Load config file:"+configFile;
    QFile fConfig(configFile);
    if( !fConfig.open(QFile::ReadOnly))
    {
        qDebug("Load config file failed.");
        QMessageBox::critical(this,"Error","can not load config file.",QMessageBox::Yes);
        isError = true;
    }
    else
    {
        QTextStream floStream(&fConfig);
        floStream.setCodec(codec);
        Title = floStream.readLine();
        while ( floStream.atEnd()==0 )
        {
            line = codec->fromUnicode(floStream.readLine());
            QStringList list = line.split(" ");
            luckyItem_t luckyItem;
            luckyItem.index = list[0].toInt();
            luckyItem.name = list[1];
            luckyItem.cnt = list[2].toInt();
        }
        fConfig.close();
    }

    isWindowShow = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeUpdate()
{
    if(isError)
    {
        this->close();
    }

    if(!isWindowShow)
    {
        return;
    }

    QImage image = bgimage;
    QPainter *painter = new QPainter(&image);
    painter->begin(&image);

    painter->end();
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->lbBackground->setPixmap(pixmap);
    delete painter;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    int currentHeight, currentWidth;
    switch (ev->key())
    {
    case Qt::Key_Escape:
        if(QMessageBox::question(this,"Exit","comfirm exit LuckyLucky?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
        {
            this->close();
        }
        break;
    case Qt::Key_F11:
        if(this->isFullScreen())
        {
            this->showNormal();
        }
        else
        {
            this->showFullScreen();
        }
        currentHeight = this->geometry().height();
        currentWidth = this->geometry().width();
        if ((currentWidth/currentHeight) > 1920/1080)
        {
            ui->lbBackground->setGeometry((currentWidth - currentHeight*1920/1080)/2,0, currentHeight*1920/1080, currentHeight);
        }
        else
        {
            ui->lbBackground->setGeometry(0, (currentHeight - currentWidth*1080/1920)/2, currentWidth, currentWidth*1080/1920);
        }
        break;
    default:
        break;
    }

    QWidget::keyPressEvent(ev);
}
