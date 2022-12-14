#include "mainwindow.h"
#include <qsplashscreen.h>
#include <QApplication>
#include <QDateTime>
#include <QDesktopWidget>



int main(int argc, char *argv[])
{
    setbuf(stdout,NULL);
    QApplication a(argc, argv);

    //加载开机动画
    QDesktopWidget* desktop = QApplication::desktop();
    QPixmap pixmap(":/sun.jpg");
    QSplashScreen splash(pixmap);
    splash.setFixedSize(1158,660);
    splash.setWindowOpacity(0.8);
    splash.setFont(QFont("microsoft yahei", 12, QFont::Bold));
    splash.move((desktop->width() - 1158)/ 2, (desktop->height() - 660) /2);
    splash.show();
    splash.showMessage("程序正在加载......", Qt::AlignCenter, Qt::blue);
    QDateTime time = QDateTime::currentDateTime();
    QDateTime currentTime = QDateTime::currentDateTime(); //记录当前时间
    while (time.secsTo(currentTime) <= 4) // 5为需要延时的秒数
    {
        currentTime = QDateTime::currentDateTime();
        a.processEvents();
    };
    MainWindow w;
    w.show();
    //主程序启动 开机动画结束
    splash.finish((&w));
    return a.exec();
}
