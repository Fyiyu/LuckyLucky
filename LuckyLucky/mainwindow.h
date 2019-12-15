#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct
{
    int index;
    QString name;
    int cnt;
} luckyItem_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private slots:
    void timeUpdate(void);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    bool isWindowMax;
    bool isError;
    bool isWindowShow;
    int ShowDelayCnt;

    QString Title;
    QVector<luckyItem_t> luckyItems;

    QImage bgimage;
};
#endif // MAINWINDOW_H
