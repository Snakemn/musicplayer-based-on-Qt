#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QList>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMenu>
#include <QTime>
#include <QFrame>
#include <QSlider>
#include <QDir>
#include <QCloseEvent>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QFrame>
#include <QtGui>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_PlayButton_clicked();

    void on_ProgressSlider_sliderMoved(int position);

    void updatePosition(qint64 position);

    void on_SoundSlider_sliderMoved(int getvolume);

    void playsong();

    void addsong();

    void delMusic();

    void clearMusic();

    void on_SongList_customContextMenuRequested(const QPoint &pos);

    void on_lastsong_clicked();

    void on_nextsong_clicked();

    void on_OrderButton_clicked();

    void on_SoundSection_clicked();

    void on_SongList_cellDoubleClicked();

    void on_BackGround_clicked();

    void setpic1();

    void setpic2();

    void setpic3();

    void setcuspics();

    void mousePressEvent(QMouseEvent *event);

    void on_CharButton_clicked();

    void setchar1();

    void setchar2();

    void setchar3();

    void setchar4();

    void setchar5();

    void setchar6();

    void setchar7();

    void setchar8();

    void setchar9();

private:
    Ui::MainWindow *ui;
    int model;
    int volumevalue;
    bool isplay;
    bool issilence;
    QFont font;
    QMenu *songMenu;
    QMenu *backgroundmenu;
    QMenu *charmenu;
    QTime *Time;
    QTime *TotalTime;
    QAction *playSong;
    QAction *addSong;
    QAction *delSong;
    QAction *clearList;
    QAction *picture1;
    QAction *picture2;
    QAction *picture3;
    QAction *custompic;
    QAction *char1;
    QAction *char2;
    QAction *char3;
    QAction *char4;
    QAction *char5;
    QAction *char6;
    QAction *char7;
    QAction *char8;
    QAction *char9;
    QString Names[999];
    QStringList header;
    QStringList *songInform;
    QMediaPlayer *player;
    QMediaPlaylist *playerlist;
};
#endif // MAINWINDOW_H
