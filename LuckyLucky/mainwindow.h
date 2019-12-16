#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFont>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct
{
    int index;
    QString name;
    int cnt;

    QString font;
    int font_size;
    int R;
    int G;
    int B;
    int x;
    int y;
} luckyItem_t;

typedef enum
{
    STARTUP = 0,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SEX,
    SEVEN,
    EIGHT,
    NINE,
} luckyStep_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *ev);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void timeUpdate(void);

private:
    void show_item(luckyItem_t item);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QPainter painter;

    QPoint mousePoint;
    QRect location;
    bool mousePressed;
    bool isWindowMax;
    bool isError;
    bool isShowCross;
    bool isShow;

    QVector<luckyItem_t> luckyItems;
    luckyStep_t luckyStep;

    QImage bgimage;
};
#endif // MAINWINDOW_H
