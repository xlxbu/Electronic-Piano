//Liangxiao's QT code//
#pragma once


#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QImage>
#include <QRect>
#include <QTextEdit>
#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <iostream>
#include <sstream>


class Brick
{

  public:
    Brick(int, int, char*);
    ~Brick();

  public:
//    void resetState();
//    bool isDestroyed();
//    void setDestroyed(bool);
    QRect getRect();
//    void setRect(QRect);
    QImage & getImage();
    

  private:
    QImage image;
    QRect rect;
//    int position;
//    bool destroyed;

};


class Communicate : public QWidget
{
  Q_OBJECT

  public:
    Communicate(QWidget *parent = 0);
    virtual ~Communicate() {};

  private slots:
    void OnSave();
    void OnMode();
    void SearchBluetooth();
    void SearchFile();
    void Play();
    void Pause();
    void Connect_dev();
    void Keyboard1();
    void Keyboard2();
    void Keyboard3();
    void Keyboard4();
    void Keyboard5();
    void Keyboard6();
    void Keyboard7();
    void Keyboard_black1();
    void Keyboard_black2();
    void Keyboard_black3();
    void Keyboard_black4();
    void Keyboard_black5();
    void Volume();
    void Nextsong();
    void Previoussong();



  private:
    QVBoxLayout *hbox;
    QPushButton *button_play;
    QPushButton *button_pause;
    QPushButton *button_mode;
    QSlider *slider; 
//    QLabel *label;    //PLUS & minus
    QLabel *label2;   //Time
    QLabel *label3;   //Slider
    QSpinBox *spinbox;
    Brick  *bricks;
    QListWidget *l_BluetoothDevice;
    QListWidget *l_File;
//    QListWidget *l_BluetoothDevice;
    QPushButton *Bluetooth;
    QPushButton *OpenFile;
    QPushButton *Connect;
    QTextEdit *Info;

  private slots:
    void setTitle(int);
    void clearItems();
    void clearFiles();

  protected:
    void timerEvent(QTimerEvent *e);
    void paintEvent(QPaintEvent *e);
    void moveEvent(QMoveEvent *e);
//    void keyPressEvent(QKeyEvent *e)

};



#endif

