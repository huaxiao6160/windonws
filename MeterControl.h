#ifndef METERCONTROL_H
#define METERCONTROL_H
#include"ui_MeterControl.h"
#include<QUdpSocket>
#include<QTimer>
#include<string>
using namespace std;
class CMeterControl : public QWidget
{
    Q_OBJECT
public:
    CMeterControl(QWidget*  = 0);
private slots:
    void startBtnClick();
    void startSendMsg();
    void resetBtnClick();
    void timeset(int);
    void timechange(int);
    void ABBtnClick();
    void ClearClick();
    void leftTurnClick();
    void rightTurnClick();
    void menuBoxChange(int);
    void stallsBoxChange(int);
    void timeSlider(int);
    void oilSlider(int);
    void rollSlider(int);
    void waterSlider(int);
    void leftshow();
    void rightshow();
    void autoCheck();
    void menuBtnPush();
private:
    Ui::Form ui;
    QUdpSocket* udpSocket;
    int timeclass;
    void sendmsg(char,char,char);
    void showTime(int mon = 0,int da = 0,int hou = 0,int min = 0);
    void adjustTime(void);
    int month,day,hour,minute,menuValue;
    int timeMax,rollMax,oilMax,waterMax;
    bool ABBtnValue,startBtnValue,leftshowValue,rightshowValue,leftclick,rightclick,
    autoCheckValue;
    QTimer* timer,*lefttimer,*righttimer;
    string m_ipaddr,m_port;
};

#endif // METERCONTROL_H
