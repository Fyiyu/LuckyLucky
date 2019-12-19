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
    luckyStep = 0;
    state = NORMAL;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    timer->start(100);
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    bgimage = QImage(":/pic/background_pic_1.jpg");

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
    if (!import_config())
    {
        qDebug("Load config file failed.");
        QMessageBox::critical(this,"Error","can not load config file.",QMessageBox::Yes);
        isError = true;
    }
    else
    {
        show_item(luckyItems.at(0));
    }

    // 加载名单文件
    if (!import_name())
    {
        qDebug("Load name file failed.");
        QMessageBox::critical(this,"Error","can not load name file.",QMessageBox::Yes);
        isError = true;
    }

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

    switch (ev->key())
    {
    case Qt::Key_Escape:
        if(!(this->isFullScreen()))
        {
            if(QMessageBox::question(this,"退出","要退出吗?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)  this->close();
            break;
        }
    case Qt::Key_F11:
        if(this->isFullScreen()) this->showNormal();
        else this->showFullScreen();
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
    case Qt::Key_0: if (state != ROLL) { state = NORMAL; luckyStep = 0; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_1: if (state != ROLL) { state = NORMAL; luckyStep = 1; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_2: if (state != ROLL) { state = NORMAL; luckyStep = 2; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_3: if (state != ROLL) { state = NORMAL; luckyStep = 3; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_4: if (state != ROLL) { state = NORMAL; luckyStep = 4; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_5: if (state != ROLL) { state = NORMAL; luckyStep = 5; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_6: if (state != ROLL) { state = NORMAL; luckyStep = 6; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_7: if (state != ROLL) { state = NORMAL; luckyStep = 7; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_8: if (state != ROLL) { state = NORMAL; luckyStep = 8; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_9: if (state != ROLL) { state = NORMAL; luckyStep = 9; show_item(luckyItems.at((int)luckyStep)); timer->stop();} break;
    case Qt::Key_F6: isShowCross = !isShowCross; break;
    case Qt::Key_F5: isShow = false; import_config(); isShow = true; break;
    case Qt::Key_Enter: state = (luckyStep == 0) ? NORMAL : ((state == NORMAL) ? ROLL : STOP); if(state == STOP) timer->stop(); else timer->start(100); break;
    default: break;
    }

    QWidget::keyPressEvent(ev);
}

bool MainWindow::import_config()
{
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    QString configPath = QCoreApplication::applicationDirPath();
    QString configFile = configPath + "/config/config.txt";
    qDebug()<<"Load config file:"+configFile;
    QFile fConfig(configFile);

    int i = 0;

    if( !fConfig.open(QFile::ReadOnly))
    {
        return false;
    }
    else
    {
        luckyItems.clear();
        QTextStream floStream(&fConfig);
        floStream.setCodec(codec);
        while ( floStream.atEnd()==0 )
        {
            i = 0;
            line = floStream.readLine();
            QStringList list = line.split("\t");
            luckyItem_t luckyItem;

            luckyItem.index = list[i++].toInt();
            luckyItem.wx_included = list[i++].toInt();

            luckyItem.name = list[i++];
            luckyItem.name_font = list[i++];
            luckyItem.name_font_size = list[i++].toInt();
            luckyItem.name_R = list[i++].toInt();
            luckyItem.name_G = list[i++].toInt();
            luckyItem.name_B = list[i++].toInt();
            luckyItem.name_x = list[i++].toInt();
            luckyItem.name_y = list[i++].toInt();

            luckyItem.count_number = list[i++].toInt();
            luckyItem.count = list[i++];
            luckyItem.count_font = list[i++];
            luckyItem.count_font_size = list[i++].toInt();
            luckyItem.count_R = list[i++].toInt();
            luckyItem.count_G = list[i++].toInt();
            luckyItem.count_B = list[i++].toInt();
            luckyItem.count_x = list[i++].toInt();
            luckyItem.count_y = list[i++].toInt();

            luckyItems.append(luckyItem);

            qDebug()<<QString::number(luckyItem.index)<<": "<<luckyItem.name<<" "<<luckyItem.count;
        }
        qDebug()<<"luckyItems count is "<<QString::number(luckyItems.count());
        fConfig.close();
    }
    return true;
}

bool MainWindow::import_name()
{
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    QString namePath = QCoreApplication::applicationDirPath();
    QString nameFile = namePath + "/config/name.txt";
    qDebug()<<"Load name file:"+nameFile;
    QFile fName(nameFile);
    if( !fName.open(QFile::ReadOnly))
    {
        return false;
    }
    else
    {
        nameList_tdtech.clear();
        nameList_wx.clear();
        QTextStream floStream(&fName);
        floStream.setCodec(codec);
        while ( floStream.atEnd()==0 )
        {
            line = floStream.readLine();
            QStringList list = line.split("\t");
            nameList_t nameList;

            int type = list[0].toInt();
            nameList.name = list[1];
            nameList.id = list[2];
            nameList.flag = false;
            if (type == 0)
            {
                nameList_tdtech.append(nameList);
            }
            else
            {
                nameList_wx.append(nameList);
            }
        }
        qDebug()<<"tdtech cnt is "<<QString::number(nameList_tdtech.count())<<", wx cnt is "<<QString::number(nameList_wx.count());
        fName.close();
    }
    return true;
}

void MainWindow::timeUpdate()
{
    if(isError) this->close();
    if (!isShow) return;

    switch (state)
    {
    case NORMAL:
        break;
    case ROLL:
        show_roll(luckyItems.at((int)luckyStep));
        break;
    case STOP:
        show_result(luckyItems.at((int)luckyStep));
        break;
    default:
        break;
    }
}

void MainWindow::show_item(luckyItem_t item)
{
    QImage image = bgimage;
    painter.begin(&image);
    if (isShowCross)
    {
        painter.setPen(QColor(182,0,7));
        painter.drawLine(960,0, 960, 1080);
        painter.drawLine(0,540,1920,540);
    }

    painter.setPen(QColor(item.name_R, item.name_G, item.name_B));
    painter.setFont(QFont(item.name_font, item.name_font_size));
    painter.drawText(item.name_x,item.name_y, item.name);

    painter.setPen(QColor(item.count_R, item.count_G, item.count_B));
    painter.setFont(QFont(item.count_font, item.count_font_size));
    painter.drawText(item.count_x,item.count_y, item.count);

    painter.end();
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->lbBackground->setPixmap(pixmap);
}

const point_t points[30] =
{
          {470,200}, {820, 200}, {1170, 200},
          {470,300}, {820, 300}, {1170, 300},
    {370,400}, {720, 400}, {1020, 400}, {1320, 400},
    {370,500}, {720, 500}, {1020, 500}, {1320, 500},
    {370,600}, {720, 600}, {1020, 600}, {1320, 600},
    {370,700}, {720, 700}, {1020, 700}, {1320, 700},
          {470,800}, {820, 800}, {1170, 800},
          {470,900}, {820, 900}, {1170, 900},
               {720, 1000}, {1020, 1000}
};

void MainWindow::show_roll(luckyItem_t item)
{
    QImage image = bgimage;
    painter.begin(&image);

    int count = item.count_number;
    int total_count = (!item.wx_included) ? nameList_tdtech.count() : nameList_tdtech.count() + nameList_wx.count();
    int randnum;
    QSet<int> lucky_set, point_set;

    //清除lucky对象
    nameList_lucky.clear();

    //产生需要个数的随机数
    for (int i=0; i<count;)
    {
        randnum = qrand() % total_count;
        if(!lucky_set.contains(randnum))
        {
            lucky_set.insert(randnum);
            i++;
        }
    }
//    for (int i=0; i<count;)
//    {
//        randnum = qrand() % 30;
//        if(!point_set.contains(randnum))
//        {
//            point_set.insert(randnum);
//            i++;
//        }
//    }
    QList<int> lucky_list = lucky_set.values();
//    QList<int> point_list = point_set.values();

    //将随机数对应的名单复制到lucky对象中
    for (int i=0; i<count; i++)
    {
        if (lucky_list[i] >= nameList_tdtech.count())
        {
            nameList_lucky.append(nameList_wx.at(lucky_list[i] - nameList_tdtech.count()));
        }
        else
        {
            nameList_lucky.append(nameList_tdtech.at(lucky_list[i]));
        }
    }

    //绘制选中名单
    painter.setPen(QColor(item.count_R, item.count_G, item.count_B));
    painter.setFont(QFont("隶书", 50));
    for (int i=0; i<count; i++)
    {
        painter.drawText(points[i].x, points[i].y, nameList_lucky.at(i).name);
    }

    painter.end();
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->lbBackground->setPixmap(pixmap);
}

void MainWindow::show_result(luckyItem_t item)
{
    (void)item;
//    painter.setPen(QColor(item.name_R, item.name_G, item.name_B));
//    painter.setFont(QFont(item.name_font, item.name_font_size));
//    painter.drawText(item.name_x,item.name_y, "结果页面");
}
