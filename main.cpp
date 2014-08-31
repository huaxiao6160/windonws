#include<QApplication>
#include"MeterControl.h"
#include"customStyle.h"
#include<QStyleFactory>
#include<QTextCodec>
int main(int argc,char* argv[])
{
    QApplication app(argc,argv);   
    Q_INIT_RESOURCE(MeterControl);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));
    QTextCodec *codec = QTextCodec::codecForName("System");
        QTextCodec::setCodecForLocale(codec);
       // QTextCodec::setCodecForCStrings(codec);
      //  QTextCodec::setCodecForTr(codec);
    //QPixmap pixmap(":/new/background.jpg");
   // QPalette paletee;

    CMeterControl form;
   // paletee.setBrush(form.backgroundRole(),QBrush(pixmap));
    //form.setPalette(paletee);
   // form.setAutoFillBackground(false);
    form.show();
    return app.exec();
}
