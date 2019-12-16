#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"
#include <QFile>
#include <QMouseEvent>
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isWindowMax = false;
    isError = false;
    isShowCross = false;
    isShow = false;
    location = this->geometry();
    mousePressed = false;
    luckyStep = STARTUP;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    timer->start(50);

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
    QString configFile = configPath + "/config/config.txt";
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
        while ( floStream.atEnd()==0 )
        {
            line = floStream.readLine();
            QStringList list = line.split(" ");
            luckyItem_t luckyItem;
            luckyItem.index = list[0].toInt();
            luckyItem.name = list[1];
            luckyItem.cnt = list[2].toInt();
            luckyItem.font = list[3];
            luckyItem.font_size = list[4].toInt();
            luckyItem.R = list[5].toInt();
            luckyItem.G = list[6].toInt();
            luckyItem.B = list[7].toInt();
            luckyItem.x = list[8].toInt();
            luckyItem.y = list[9].toInt();

            luckyItems.append(luckyItem);

            qDebug()<<QString::number(luckyItem.index)<<": "<<luckyItem.name<<", "<<QString::number(luckyItem.cnt);
        }
        qDebug()<<"luckyItems count is "<<QString::number(luckyItems.count());
        fConfig.close();
    }

    // 加载名单文件

    isShow = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 非全屏模式下窗口跟随鼠标移动 */
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !(this->isFullScreen())) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

/* 键盘按键事件响应 */
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    int currentHeight, currentWidth;
    QString configPath = QCoreApplication::applicationDirPath();
    QString configFile = configPath + "/config/config.txt";
    QFile fConfig(configFile);
    QString line;
    QStringList list;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    QTextStream floStream(&fConfig);
    int i = 0;

    switch (ev->key())
    {
    case Qt::Key_Escape:
        //if(QMessageBox::question(this,"Exit","comfirm exit LuckyLucky?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
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
    case Qt::Key_0: luckyStep = (luckyStep_t)0; break;
    case Qt::Key_1: luckyStep = (luckyStep_t)1; break;
    case Qt::Key_2: luckyStep = (luckyStep_t)2; break;
    case Qt::Key_3: luckyStep = (luckyStep_t)3; break;
    case Qt::Key_4: luckyStep = (luckyStep_t)4; break;
    case Qt::Key_5: luckyStep = (luckyStep_t)5; break;
    case Qt::Key_6: luckyStep = (luckyStep_t)6; break;
    case Qt::Key_7: luckyStep = (luckyStep_t)7; break;
    case Qt::Key_8: luckyStep = (luckyStep_t)8; break;
    case Qt::Key_9: luckyStep = (luckyStep_t)9; break;
    case Qt::Key_F6:
        isShowCross = !isShowCross;
        break;
    case Qt::Key_F5:
        if( !fConfig.open(QFile::ReadOnly))
        {
            qDebug("Load config file failed.");
        }
        else
        {
            isShow = false;

            floStream.setCodec(codec);
            luckyItems.clear();
            while ( floStream.atEnd()==0 )
            {
                line = floStream.readLine();
                QStringList list = line.split(" ");
                luckyItem_t luckyItem;
                i = 0;
                luckyItem.index = list[i++].toInt();
                luckyItem.name = list[i++];
                luckyItem.cnt = list[i++].toInt();
                luckyItem.font = list[i++];
                luckyItem.font_size = list[i++].toInt();
                luckyItem.R = list[i++].toInt();
                luckyItem.G = list[i++].toInt();
                luckyItem.B = list[i++].toInt();
                luckyItem.x = list[i++].toInt();
                luckyItem.y = list[i++].toInt();

                luckyItems.append(luckyItem);

                qDebug()<<QString::number(luckyItem.index)<<": "<<luckyItem.name<<", "<<QString::number(luckyItem.cnt);
            }
            qDebug()<<"luckyItems count is "<<QString::number(luckyItems.count());
            fConfig.close();
            isShow = true;
        }
        break;
    default:
        break;
    }

    QWidget::keyPressEvent(ev);
}

void MainWindow::timeUpdate()
{
    if(isError) this->close();
    if (!isShow) return;

    QImage image = bgimage;
    painter.begin(&image);
    if (isShowCross)
    {
        painter.setPen(QColor(182,0,7));
        painter.drawLine(960,0, 960, 1080);
        painter.drawLine(0,540,1920,540);
    }

    switch (luckyStep)
    {
    default:
        show_item(luckyItems.at((int)luckyStep));
        break;
    }

    painter.end();
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->lbBackground->setPixmap(pixmap);
}

void MainWindow::show_item(luckyItem_t item)
{
    painter.setPen(QColor(item.R, item.G, item.B));
    painter.setFont(QFont(item.font, item.font_size));
    painter.drawText(item.x,item.y, item.name);
}
