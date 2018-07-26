# musicplayer-based-on-Qt
//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
     * 播放信息初始化
     */
    isplay=false;
    issilence=false;
    ui->PlayButton->setIcon(QIcon(":/icon_geu8z7ksnav/play.png"));
    ui->PlayButton->setToolTip("播放");
    player=new QMediaPlayer();
    connect(player,&QMediaPlayer::positionChanged,this,&MainWindow::updatePosition);
    player->setVolume(100);
    ui->SoundSlider->sliderMoved(50);

    /*
     * 播放列表右键菜单
     */
    songMenu=new QMenu();
    playSong=new QAction(tr("播放歌曲"),this);
    songMenu->addAction(playSong);
    connect(playSong,SIGNAL(triggered()),this,SLOT(playsong()));
    addSong=new QAction(tr("添加歌曲"), this);
    songMenu->addAction(addSong);
    connect(addSong, SIGNAL(triggered()), this, SLOT(addsong()));
    delSong=new QAction(tr("删除歌曲"), this);
    songMenu->addAction(delSong);
    connect(delSong, SIGNAL(triggered()), this, SLOT(delMusic()));
    clearList=new QAction(tr("清空播放列表"), this);
    songMenu->addAction(clearList);
    connect(clearList, SIGNAL(triggered()), this, SLOT(clearMusic()));
    ui->SongList->setContextMenuPolicy(Qt::CustomContextMenu);

    /*
     * 播放列表布局及初始化
     */
    ui->SongList->setColumnCount(1);
    header<<"播放列表";
    ui->SongList->setHorizontalHeaderLabels(header);
    ui->SongList->setColumnWidth(0,ui->SongList->width());//初始化播放列表的列宽
    ui->SongList->setRowHeight(0,60);//初始化播放列表的行高
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    playerlist=new QMediaPlaylist(this);
    player->setPlaylist(playerlist);//设置相应playerlist
    ui->SongList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->SongList->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*
     * 播放模式初始化
     */
    model=0;
    ui->OrderButton->setToolTip("列表循环");
    playerlist->setPlaybackMode(QMediaPlaylist::Sequential);
    if(playerlist->isEmpty())
    {
        ui->SongName->setText("MusicPlayer");
        ui->SongName->adjustSize();
    }
    else
    {
        ui->SongList->setItem(playerlist->currentIndex(),0,new QTableWidgetItem(Names[playerlist->currentIndex()]));
        ui->SongName->setText(Names[playerlist->currentIndex()]);
        ui->SongName->adjustSize();
    }

    /*
     * 各按键提示信息
     */
    ui->BackGround->setToolTip("背景设置");
    ui->CharButton->setToolTip("字体设置");
    ui->MinButton->setToolTip("最小化");
    ui->CloseButton->setToolTip("关闭");
    ui->lastsong->setToolTip("上一曲");
    ui->nextsong->setToolTip("下一曲");
    ui->SoundSection->setToolTip("音量");

    /*
     * 背景设置菜单及背景初始化
     */
    backgroundmenu=new QMenu();
    picture1=new QAction(tr("默认背景1"),this);
    backgroundmenu->addAction(picture1);
    connect(picture1, SIGNAL(triggered()), this, SLOT(setpic1()));
    picture2=new QAction(tr("默认背景2"),this);
    backgroundmenu->addAction(picture2);
    connect(picture2, SIGNAL(triggered()), this, SLOT(setpic2()));
    picture3=new QAction(tr("默认背景3"),this);
    backgroundmenu->addAction(picture3);
    connect(picture3, SIGNAL(triggered()), this, SLOT(setpic3()));
    custompic=new QAction(tr("自定义背景"),this);
    backgroundmenu->addAction(custompic);
    connect(custompic, SIGNAL(triggered()), this, SLOT(setcuspics()));
    ui->BackGround->setContextMenuPolicy(Qt::CustomContextMenu);
    setpic1();

    /*
     * 全局字号设置
     */
    font.setPointSize(18);
    qApp->setFont(font);

    /*
     * 全局字体设置菜单
     */
    charmenu=new QMenu();
    char1=new QAction(tr("华文行楷（默认）"),this);
    charmenu->addAction(char1);
    connect(char1, SIGNAL(triggered()), this, SLOT(setchar1()));
    char2=new QAction(tr("华文舒体"),this);
    charmenu->addAction(char2);
    connect(char2, SIGNAL(triggered()), this, SLOT(setchar2()));
    char3=new QAction(tr("华文宋体"),this);
    charmenu->addAction(char3);
    connect(char3, SIGNAL(triggered()), this, SLOT(setchar3()));
    char4=new QAction(tr("华文楷体"),this);
    charmenu->addAction(char4);
    connect(char4, SIGNAL(triggered()), this, SLOT(setchar4()));
    char5=new QAction(tr("华文新魏"),this);
    charmenu->addAction(char5);
    connect(char5, SIGNAL(triggered()), this, SLOT(setchar5()));
    char6=new QAction(tr("华文隶书"),this);
    charmenu->addAction(char6);
    connect(char6, SIGNAL(triggered()), this, SLOT(setchar6()));
    char7=new QAction(tr("华文彩云"),this);
    charmenu->addAction(char7);
    connect(char7, SIGNAL(triggered()), this, SLOT(setchar7()));
    char8=new QAction(tr("Vivaldi"),this);
    charmenu->addAction(char8);
    connect(char8, SIGNAL(triggered()), this, SLOT(setchar8()));
    char9=new QAction(tr("Script MT Bold"),this);
    charmenu->addAction(char9);
    connect(char9, SIGNAL(triggered()), this, SLOT(setchar9()));
    ui->CharButton->setContextMenuPolicy(Qt::CustomContextMenu);
    setchar1();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 播放函数（自动关联）
 * PlayButton按钮
 * 歌名显示
 * SongName标签
 */
void MainWindow::on_PlayButton_clicked()
{
    if(playerlist->isEmpty())
        QMessageBox::information(this,tr(" "),tr("列表无歌曲\n请添加歌曲"));
    else
    {
        if(isplay)
        {
            player->pause();
            ui->PlayButton->setIcon(QIcon(":/icon_geu8z7ksnav/play.png"));
            ui->PlayButton->setToolTip("播放");
        }
        else
        {
            player->play();
            ui->PlayButton->setIcon(QIcon(":/icon_geu8z7ksnav/pause.png"));
            ui->PlayButton->setToolTip("暂停");
        }
        isplay=!isplay;
        //歌名显示
        ui->SongList->setItem(playerlist->currentIndex(),0,new QTableWidgetItem(Names[playerlist->currentIndex()]));
        ui->SongName->setText(Names[playerlist->currentIndex()]);
        ui->SongName->adjustSize();
    }
}

/*
 * 播放进度函数（自动关联）
 * ProgressSlider拖条
 */
void MainWindow::on_ProgressSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

/*
 * 歌曲播放时间及总时间表示
 * Time、TotalTime标签
 */
void MainWindow::updatePosition(qint64 position)
{
    ui->ProgressSlider->setMaximum(player->duration());
    ui->ProgressSlider->setValue(position);
    //当前时间构造及输出
    Time=new QTime(0,(position/60000)%60,(position/1000)%60);
    ui->Time->setText(Time->toString("mm:ss"));
    //总时间构造及输出
    TotalTime=new QTime(0,(player->duration()/60000)%60,(player->duration()/1000)%60);
    ui->TotalTime->setText(TotalTime->toString("mm:ss"));
    if(Time->toString("mm:ss")==TotalTime->toString("mm:ss"))
    {
        if(playerlist->isEmpty())
        {
            ui->SongName->setText("MusicPlayer");
            ui->SongName->adjustSize();
        }
        else
        {
            ui->SongList->setItem(playerlist->currentIndex(),0,new QTableWidgetItem(Names[playerlist->currentIndex()]));
            ui->SongName->setText(Names[playerlist->currentIndex()]);
            ui->SongName->adjustSize();
        }
    }
}

/*
 * 声音进度函数（自动关联）
 * SoundSlider拖条
 */
void MainWindow::on_SoundSlider_sliderMoved(int getvolume)
{
    player->setVolume(getvolume);
    ui->SoundSlider->setValue(getvolume);
}

/*
 * 播放列表右键（自动关联）
 */
void MainWindow::on_SongList_customContextMenuRequested(const QPoint &pos)
{
    songMenu->exec(QCursor::pos());
    pos.isNull();
}

/*
 * 播放列表右键菜单之播放歌曲
 */
void MainWindow::playsong()
{
    int i=ui->SongList->currentRow();
    playerlist->setCurrentIndex(i);
    isplay=false;
    on_PlayButton_clicked();
}

/*
 * 播放列表右键菜单之添加歌曲
 */
void MainWindow::addsong()
{
    QString initialName=QDir::homePath();//在window下返回用户（即我的文档）目录
    //getOpenFileNames函数
    //(parent指定父组件,caption对话框标题,dir对话框显示时默认打开目录,filter对话框的后缀名过滤器,selectedFilter默认选择的过滤器,options对话框的一些参数设定)
    QStringList pathList=QFileDialog::getOpenFileNames(this, tr("选择文件"), initialName, tr("MP3 Files (*.mp3)"));
    //用QStringList存放音乐文件的地址
    for(int i=0; i<pathList.size(); ++i)
    {
        QString path=QDir::toNativeSeparators(pathList.at(i));//把斜杠转化成反斜杠，获取歌曲的地址
        if(!path.isEmpty())
        {
            playerlist->addMedia(QUrl::fromLocalFile(path));
            //一定要用QUrl::fromLocalFile()，这个可以打开文件名或路径有中文或空格的文件.
            //直接用QUrl(fileName)只能打开英文名称和没有空格路径的文件.
            QString fileName=path.split("\\").last();//split获取包含后缀文件名
            int rowNumber=ui->SongList->rowCount();
            ui->SongList->insertRow(rowNumber);
            QString s=QString::number(ui->SongList->rowCount(),10);
            QString Name=fileName.split(".").front();
            Names[rowNumber]=Name;
            ui->SongList->setItem(rowNumber,0,new QTableWidgetItem(Name));
            songInform=new QStringList();
            songInform->append(Name);
        }
    }
}

/*
 * 播放列表右键菜单之删除歌曲
 */
void MainWindow::delMusic()
{
    if(!playerlist->isEmpty())
    {
        if(QMessageBox::Yes==QMessageBox::question(this,tr(" "),tr("确认删除歌曲?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {
            player->pause();
            int i=ui->SongList->currentRow();
            if(i<playerlist->currentIndex())
            {
                int pos,k;
                pos=ui->ProgressSlider->value();
                k=playerlist->currentIndex();
                for(int j=i;Names[j+1]!='\0';j++)
                    Names[j]=Names[j+1];
                songInform->removeAt(i);
                playerlist->removeMedia(i);
                ui->SongList->removeRow(i);
                playerlist->setCurrentIndex(k-1);
                ui->ProgressSlider->sliderMoved(pos);
            }
            else
            {
                for(int j=i;Names[j+1]!='\0';j++)
                    Names[j]=Names[j+1];
                songInform->removeAt(i);
                playerlist->removeMedia(i);
                ui->SongList->removeRow(i);
            }
            if(playerlist->isEmpty())
            {
                ui->SongName->setText("MusicPlayer");
                ui->SongName->adjustSize();
            }
            else
            {
                ui->SongList->setItem(playerlist->currentIndex(),0,new QTableWidgetItem(Names[playerlist->currentIndex()]));
                ui->SongName->setText(Names[playerlist->currentIndex()]);
                ui->SongName->adjustSize();
            }
            player->play();
            QMessageBox::information(this, tr(" "),tr("删除成功"));
        }
        else
            QMessageBox::information(this,tr(" "),tr("取消删除"));
    }
    else
        QMessageBox::information(this,tr(" "),tr("列表无歌曲"));
}

/*
 * 播放列表右键菜单之清空播放列表
 */
void MainWindow::clearMusic()
{
    if(!playerlist->isEmpty())
    {
        playerlist->clear();
        for(int i=0,j=ui->SongList->rowCount();i<j;i++)
            ui->SongList->removeRow(0);
        ui->SongList->setHorizontalHeaderLabels(header);
        ui->SongName->setText("MusicPlayer");
        ui->SongName->adjustSize();
        ui->Time->setText("00:00");
    }
    else
        QMessageBox::information(this,tr(" "),tr("列表无歌曲"));
}

/*
 * 上一首歌函数（自动关联）
 * lastsong按钮
 */
void MainWindow::on_lastsong_clicked()
{
    if(playerlist->isEmpty())
    {
        QMessageBox::information(this,tr(" "),tr("列表无歌曲\n请添加歌曲"));
        return;
    }
    //表头跳转至表尾
    int currentIndex=playerlist->currentIndex();
    if(currentIndex==0)
        currentIndex=playerlist->mediaCount();
    playerlist->setCurrentIndex(currentIndex);
    //随播放模式变化
    if(model==2)
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int i,num;
        i=playerlist->mediaCount();
        num=qrand()%i;
        if(num==playerlist->currentIndex())
            num++;
        playerlist->setCurrentIndex(num);
    }
    else
        playerlist->previous();
    isplay=false;
    on_PlayButton_clicked();
}

/*
 * 下一首歌函数（自动关联）
 * nextsong按钮
 */
void MainWindow::on_nextsong_clicked()
{
    if(playerlist->isEmpty())
    {
        QMessageBox::information(this,tr(" "),tr("列表无歌曲\n请添加歌曲"));
        return;
    }
    //表尾跳转至表头
    int currentIndex=playerlist->currentIndex();
    if(++currentIndex==playerlist->mediaCount())
        currentIndex=0;
    playerlist->setCurrentIndex(currentIndex);
    //随播放模式变化
    if(model==2)
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int i,num;
        i=playerlist->mediaCount();
        num=qrand()%i;
        if(num==playerlist->currentIndex())
            num++;
        playerlist->setCurrentIndex(num);
    }
    isplay=false;
    on_PlayButton_clicked();
}

/*
 * 播放模式函数（自动关联）
 * OrderButton按钮
 */
void MainWindow::on_OrderButton_clicked()
{
    if(model==0)
    {
        ui->OrderButton->setIcon(QIcon(":/icon_geu8z7ksnav/单曲循环.png"));
        ui->OrderButton->setToolTip("单曲循环");
        playerlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        model=1;
    }
    else if(model==1)
    {
        ui->OrderButton->setIcon(QIcon(":/icon_geu8z7ksnav/line.png"));
        ui->OrderButton->setToolTip("随机播放");
        playerlist->setPlaybackMode(QMediaPlaylist::Random);
        model=2;
    }
    else if(model==2)
    {
        ui->OrderButton->setIcon(QIcon(":/icon_geu8z7ksnav/exchange.png"));
        ui->OrderButton->setToolTip("列表循环");
        playerlist->setPlaybackMode(QMediaPlaylist::Sequential);
        model=0;
    }
}

/*
 * 声音-静音函数（自动关联）
 * SoundSection按钮
 */
void MainWindow::on_SoundSection_clicked()
{
    if(issilence)
    {
        ui->SoundSection->setIcon(QIcon(":/icon_geu8z7ksnav/静音.png"));
        volumevalue=ui->SoundSlider->value();
        player->setVolume(0);
        ui->SoundSection->setToolTip("静音");
    }
    else
    {
        ui->SoundSection->setIcon(QIcon(":/icon_geu8z7ksnav/声音.png"));
        player->setVolume(volumevalue);
        ui->SoundSection->setToolTip("声音");
    }
    issilence=!issilence;
}

/*
 * 播放列表双击播放函数（自动关联）
 */
void MainWindow::on_SongList_cellDoubleClicked()
{
    int i=ui->SongList->currentRow();
    playerlist->setCurrentIndex(i);
    isplay=false;
    on_PlayButton_clicked();
}

/*
 * 背景设置菜单点击出现（自动关联）
 */
void MainWindow::on_BackGround_clicked()
{
    backgroundmenu->exec(QCursor::pos());
    backgroundmenu->show();
}

/*
 * 背景设置菜单之默认背景1
 */
void MainWindow::setpic1()
{
    QPixmap pixmap=QPixmap(":/icon_geu8z7ksnav/6464456456456456ng15gfd.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
    backgroundmenu->adjustSize();
    backgroundmenu->menuAction()->setVisible(false);
}

/*
 * 背景设置菜单之默认背景2
 */
void MainWindow::setpic2()
{
    QPixmap pixmap=QPixmap(":/icon_geu8z7ksnav/07492f030f3856f14309a0f8e084840f.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
    backgroundmenu->adjustSize();
    backgroundmenu->menuAction()->setVisible(false);
}

/*
 * 背景设置菜单之默认背景3
 */
void MainWindow::setpic3()
{
    QPixmap pixmap=QPixmap(":/icon_geu8z7ksnav/12346469_2017_20180717-183221.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);
    backgroundmenu->adjustSize();
    backgroundmenu->menuAction()->setVisible(false);
}

/*
 * 背景设置菜单之自定义背景
 */
void MainWindow::setcuspics()
{
    QString picpath=QDir::homePath();
    picpath=QFileDialog::getOpenFileName(this,tr("选择图片"),picpath,tr(" "));
    QPixmap pixmap=QPixmap(picpath).scaled(this->size());
    QPalette palette(this->palette());
    if(!pixmap.isNull())
    {
        palette.setBrush(QPalette::Background,QBrush(pixmap));
        this->setPalette(palette);
    }
    backgroundmenu->adjustSize();
    backgroundmenu->menuAction()->setVisible(false);
}

/*
 * 窗口拖动功能实现
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(ReleaseCapture())
        SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

/*
 * 全局字体设置菜单点击出现（自动关联）
 */
void MainWindow::on_CharButton_clicked()
{
    charmenu->exec(QCursor::pos());
    charmenu->show();
}

/*
 * 全局字体设置菜单之华文行楷
 */
void MainWindow::setchar1()
{
    font.setFamily("华文行楷");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文行楷");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之华文舒体
 */
void MainWindow::setchar2()
{
    font.setFamily("华文舒体");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文舒体");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之华文宋体
 */
void MainWindow::setchar3()
{
    font.setFamily("华文宋体");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文宋体");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之华文楷体
 */
void MainWindow::setchar4()
{
    font.setFamily("华文楷体");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文楷体");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之华文新魏
 */
void MainWindow::setchar5()
{
    font.setFamily("华文新魏");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文新魏");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之华文隶书
 */
void MainWindow::setchar6()
{
    font.setFamily("华文隶书");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文隶书");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
   charmenu->menuAction()->setVisible(false);
   ui->TitleButton->adjustSize();
   ui->SongName->adjustSize();
   charmenu->adjustSize();
   charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之华文彩云
 */
void MainWindow::setchar7()
{
    font.setFamily("华文彩云");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("华文彩云");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之Lucida Calligraphy
 */
void MainWindow::setchar8()
{
    font.setFamily("Vivaldi");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("Vivaldi");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}

/*
 * 全局字体设置菜单之Script MT Bold
 */
void MainWindow::setchar9()
{
    font.setFamily("Script MT Bold");
    qApp->setFont(font);
    ui->SongList->setStyleSheet("Script MT Bold");
    ui->SongList->setStyleSheet("background:rgba(255, 255, 255, 100);");
    charmenu->menuAction()->setVisible(false);
    ui->TitleButton->adjustSize();
    ui->SongName->adjustSize();
    charmenu->adjustSize();
    charmenu->menuAction()->setVisible(false);
}


//main.cpp
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);//去掉窗口标题栏
    w.move(((QApplication::desktop()->width()-w.width())*0.5),(QApplication::desktop()->height()-w.height())*0.5);
    w.show();

    return a.exec();
}


//mainwindow.h
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


//Resourses/imageicons.qrc/icon_geu8z7ksnav/..for all pics in cpp
