#include"MeterControl.h"
#include<QDateTime>
#include<QTimer>
#include<QTextCodec>
#include<fstream>
#include<QMessageBox>
#define TR(x) QStringLiteral(x)
#define DEBUG 1
CMeterControl::CMeterControl(QWidget* parent):QWidget(parent)
{
    const int timevalue = 36,oilvalue = 36,rollvalue = 36,watervalue = 36;
    ifstream input("Config.txt");
    if(!input)
    {
        QMessageBox::warning(this,"warning","Create Config.txt by default",QMessageBox::Yes);
        ofstream outfile("Config.txt");
        m_ipaddr = "10.42.1.162";
        m_port = "9994";
        outfile<<m_ipaddr<<endl<<m_port<<endl;
    }
    else
        input >> m_ipaddr>>m_port;
    timeMax = 240;
    rollMax = 240;
    oilMax = 100;
    waterMax = 100;
    leftclick = rightclick = false;
    m_ipaddr = "10.42.1.162";
    m_port = "9998";
    ui.setupUi(this);
    ui.setTimeBox->setEditText("month");
    ui.setTimeBox->addItem("month");
    ui.setTimeBox->addItem("day");
    ui.setTimeBox->addItem("hour");
    ui.setTimeBox->addItem("minute");
    ui.menuBox->setEditText(TR("向前"));
    ui.menuBox->addItem(TR("向前"));
    ui.menuBox->addItem(TR("选中"));
    ui.menuBox->addItem(TR("向后"));
    ui.stallsBox->setEditText(TR("P档位"));
    ui.stallsBox->addItem(TR("P档位"));
    ui.stallsBox->addItem(TR("R档位"));
    ui.stallsBox->addItem(TR("N档位"));
    ui.stallsBox->addItem(TR("D1档位"));
    ui.stallsBox->addItem(TR("D2档位"));
    ui.stallsBox->addItem(TR("D3档位"));
    ui.stallsBox->addItem(TR("D4档位"));
    timeclass = 0;
    menuValue = 0;
    QDateTime current = QDateTime::currentDateTime();
    month = current.toString("M").toInt();
    day = current.toString("d").toInt();
    QTime currenttime = QTime::currentTime();
    hour = currenttime.toString("h").toInt();
    minute = currenttime.toString("m").toInt();
    showTime();
    ui.setTimeSpinBox->setValue(month);
    udpSocket = new QUdpSocket;
    ABBtnValue = true;
    startBtnValue = true;
    autoCheckValue = true;
    ui.autoBtn->setIcon(QIcon(":/new/4.bmp"));
    ui.handcheck->setEnabled(false);
    ui.timeSlider->setMinimum(0);
    ui.timeSlider->setMaximum(timeMax);
    ui.timeSlider->setValue(timevalue);
    ui.timeEdit->setText(QString("%1").arg(timevalue));

    ui.oilSlider->setMinimum(0);
    ui.oilSlider->setMaximum(oilMax);
    ui.oilSlider->setValue(oilvalue);
    ui.oilEdit->setText(QString("%1").arg(oilvalue));

    ui.rollSlider->setMinimum(0);
    ui.rollSlider->setMaximum(rollMax);
    ui.rollSlider->setValue(oilvalue);
    ui.rollEdit->setText(QString("%1").arg(rollvalue));

    ui.waterSlider->setMinimum(0);
    ui.waterSlider->setMaximum(waterMax);
    ui.waterSlider->setValue(oilvalue);
    ui.waterEdit->setText(QString("%1").arg(watervalue));

    timer = new QTimer(this);
    lefttimer = new QTimer(this);
    leftshowValue = true;
    righttimer = new QTimer(this);
    rightshowValue = true;
    connect(ui.startBtn,SIGNAL(clicked()),this,SLOT(startBtnClick()));
    //connect(ui.stopBtn,SIGNAL(clicked()),this,SLOT(stopBtnClick()));
    connect(ui.setTimeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(timeset(int)));
    connect(ui.setTimeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(timechange(int)));
    connect(ui.menuBox,SIGNAL(currentIndexChanged(int)),this,SLOT(menuBoxChange(int)));
    connect(ui.stallsBox,SIGNAL(currentIndexChanged(int)),this,SLOT(stallsBoxChange(int)));
    connect(ui.ABBtn,SIGNAL(clicked()),this,SLOT(ABBtnClick()));
    connect(ui.ABClearBtn,SIGNAL(clicked()),this,SLOT(ClearClick()));
    connect(ui.leftTurn,SIGNAL(clicked()),this,SLOT(leftTurnClick()));
    connect(ui.rightTurn,SIGNAL(clicked()),this,SLOT(rightTurnClick()));
    connect(ui.timeSlider,SIGNAL(valueChanged(int)),this,SLOT(timeSlider(int)));
    connect(ui.oilSlider,SIGNAL(valueChanged(int)),this,SLOT(oilSlider(int)));
    connect(ui.rollSlider,SIGNAL(valueChanged(int)),this,SLOT(rollSlider(int)));
    connect(ui.waterSlider,SIGNAL(valueChanged(int)),this,SLOT(waterSlider(int)));
    connect(timer,SIGNAL(timeout()),this,SLOT(startSendMsg()));
    connect(lefttimer,SIGNAL(timeout()),this,SLOT(leftshow()));
    connect(righttimer,SIGNAL(timeout()),this,SLOT(rightshow()));
    connect(ui.autoBtn,SIGNAL(clicked()),this,SLOT(autoCheck()));
    connect(ui.menuBtn,SIGNAL(clicked()),this,SLOT(menuBtnPush()));
}

void CMeterControl::sendmsg(char first,char second,char third)
{
    if(!autoCheckValue)
    {
        char send[3] = {first,second,third};
        udpSocket->writeDatagram(send,3,
                             QHostAddress(QString(m_ipaddr.c_str())),QString(m_port.c_str()).toInt());
#if DEBUG
#endif
    }
}

void CMeterControl::startBtnClick(void)
{
    startBtnValue = !startBtnValue;
    if(!startBtnValue)
    {
        ui.startBtn->setIcon(QIcon(":/new/tingzhi_move.bmp"));
        timer->start(1000);
    }
    else
    {
        ui.startBtn->setIcon(QIcon(":/new/kaishi_move.bmp"));
        timer->stop();
    }
}
void CMeterControl::startSendMsg()
{
    sendmsg(0,0,0);
}

void CMeterControl::resetBtnClick(void)
{

}
void CMeterControl::timeset(int index)
{
    timeclass = index;
    switch (timeclass) {
    case 0:
        ui.setTimeSpinBox->setValue(month);
        break;
    case 1:
        ui.setTimeSpinBox->setValue(day);
        break;
    case 2:
        ui.setTimeSpinBox->setValue(hour);
        break;
    case 3:
        ui.setTimeSpinBox->setValue(minute);
        break;
    default:
        break;
    }
}
void CMeterControl::showTime(int mon, int da, int hou, int min)
{
   if(mon || da || hou || min)
      ui.timeLineEdit->setText(tr("%1-%2 %3:%4 ").arg(mon).arg(da).arg(hou).arg(min));
   else
       ui.timeLineEdit->setText(tr("%1-%2 %3:%4 ").arg(month).arg(day).arg(hour).arg(minute));
}

void CMeterControl::timechange(int index)
{
    switch (timeclass) {
    case 0:
        if((index - month) == 1)
            sendmsg(12,4,0);
        else if((month - index ) == 1)
            sendmsg(12,8,0);
        else
            sendmsg(0,0,0);
        month = index;
        adjustTime();
        ui.setTimeSpinBox->setValue(month);
        showTime();

        break;
    case 1:
        if((index - day) == 1)
            sendmsg(12,3,0);
        else if((day - index ) == 1)
            sendmsg(12,7,0);
        else
            sendmsg(0,0,0);
        day = index;
        adjustTime();
        ui.setTimeSpinBox->setValue(day);
        showTime();

        break;
    case 2:
        if((index - hour) == 1)
            sendmsg(12,2,0);
        else if((hour - index ) == 1)
            sendmsg(12,6,0);
        else
            sendmsg(0,0,0);
        hour = index;
        adjustTime();
        ui.setTimeSpinBox->setValue(hour);
        showTime();

        break;
    case 3:
        if((index - minute) == 1)
            sendmsg(12,1,0);
        else if((minute - index ) == 1)
            sendmsg(12,5,0);
        else
            sendmsg(0,0,0);
        minute = index;
        adjustTime();
        ui.setTimeSpinBox->setValue(minute);
        showTime();

        break;
    default:
        break;
    }
}
void CMeterControl::adjustTime(void)
{
    QDate date(2014,month,day);
    if(minute/60)
    {
        minute = minute%60;
        hour++;
    }
    if(hour/24)
    {
        hour = hour % 24;
        date = date.addDays(1);
    }
    date.getDate(NULL,&month,&day);
}
void CMeterControl::ABBtnClick()
{
    ABBtnValue = !ABBtnValue;
    if(ABBtnValue)
    {
        ui.ABBtn->setText("A");
        sendmsg(13,0,1);
    }
    else
    {
        ui.ABBtn->setText("B");
        sendmsg(13,1,1);
    }
}
void CMeterControl::ClearClick()
{
    sendmsg(13,1,0);
    sendmsg(13,0,0);
}
void CMeterControl::leftTurnClick()
{
    leftclick = !leftclick;
    if(leftclick)
    {
        sendmsg(6,1,0);
        lefttimer->start(500);
        ui.leftTurn->setIcon(QIcon(":/new/empty.bmp"));
    }
    else
    {
        sendmsg(6,0,0);
        lefttimer->stop();
        ui.leftTurn->setIcon(QIcon(":/new/turnleft.bmp"));
    }
}
void CMeterControl::rightTurnClick()
{
    rightclick = !rightclick;
    if(rightclick)
    {
        sendmsg(7,1,0);
        ui.rightTurn->setIcon(QIcon(":/new/empty.bmp"));
        righttimer->start(500);
    }
    else
    {
        sendmsg(7,0,0);
        righttimer->stop();
        ui.rightTurn->setIcon(QIcon(":/new/turnright.bmp"));
    }
    //sendmsg("rightTurn clicked");
}
void CMeterControl::menuBoxChange(int index)
{
    menuValue = index;
}
void CMeterControl::menuBtnPush()
{
    switch (menuValue) {
    case 0:
        sendmsg(4,0,0);
        break;
    case 1:
        sendmsg(3,0,0);
        break;
    case 2:
        sendmsg(5,0,0);
        break;
    default:
        break;
    }
}

void CMeterControl::stallsBoxChange(int index)
{
    sendmsg(8,(char)index,0);
}
void CMeterControl::timeSlider(int index)
{
     ui.timeEdit->setText(QString("%1").arg(index));
     float msg = float(95)/float(timeMax)*float(index);
     sendmsg(2,0,msg);
}
void CMeterControl::oilSlider(int index)
{
     ui.oilEdit->setText(QString("%1").arg(index));
     sendmsg(10,(char)index,0);
}
void CMeterControl::rollSlider(int index)
{
     ui.rollEdit->setText(QString("%1").arg(index));
     float msg = float(95)/float(rollMax)*float(index);
     sendmsg(1,0,msg);
}
void CMeterControl::waterSlider(int index)
{
     ui.waterEdit->setText(QString("%1").arg(index));
     sendmsg(11,(char)index,0);
}
void CMeterControl::leftshow()
{
    leftshowValue = !leftshowValue;
    if(leftshowValue)
        ui.leftTurn->setIcon(QIcon(":/new/empty.bmp"));
    else
        ui.leftTurn->setIcon(QIcon(":/new/turnleft.bmp"));
}
void CMeterControl::rightshow()
{
    rightshowValue = !rightshowValue;
    if(rightshowValue)
        ui.rightTurn->setIcon(QIcon(":/new/empty.bmp"));
    else
        ui.rightTurn->setIcon(QIcon(":/new/turnright.bmp"));
}
void CMeterControl::autoCheck()
{
    if(!autoCheckValue)
        sendmsg(14,1,0);
    autoCheckValue = !autoCheckValue;
    if(autoCheckValue)
    {
        ui.autoBtn->setIcon(QIcon(":/new/4.bmp"));
        ui.handcheck->setEnabled(false);
        lefttimer->stop();
        ui.leftTurn->setIcon(QIcon(":/new/turnleft.bmp"));
        righttimer->stop();
        ui.rightTurn->setIcon(QIcon(":/new/turnright.bmp"));
    }
    else
    {
        ui.autoBtn->setIcon(QIcon(":/new/5.bmp"));
        ui.handcheck->setEnabled(true);
        sendmsg(14,0,0);
    }

}
