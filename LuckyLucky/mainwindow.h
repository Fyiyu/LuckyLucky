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
    int wx_included;

    QString name;
    QString name_font;
    int name_font_size;
    int name_R;
    int name_G;
    int name_B;
    int name_x;
    int name_y;

    int count_number;
    QString count;
    QString count_font;
    int count_font_size;
    int count_R;
    int count_G;
    int count_B;
    int count_x;
    int count_y;
} luckyItem_t;

typedef struct
{
    QString name;
    QString id;
} nameList_t;

typedef enum
{
    NORMAL = 0,
    ROLL,
    STOP,
} State_t;

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
    void show_roll(luckyItem_t item);
    void show_result(luckyItem_t item);
    bool import_config(void);
    bool import_name(void);

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
    QVector<nameList_t> nameList_tdtech;
    QVector<nameList_t> nameList_wx;
    QVector<nameList_t> nameList_lucky;
    int luckyStep;
    State_t state;

    QImage bgimage;
};
#endif // MAINWINDOW_H
