#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>
#include <QTime>
#include "fileupload.h"

FileUpload fp;
QString ip="150.158.112.219";
int port=6666;

void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void getFileName(QString root,QString type){
    QDirIterator it(root,QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot); //遍历所有目录和文件
    while(it.hasNext()){
        QString name=it.next();
        QFileInfo info(name);
        if(info.isDir()){
            getFileName(info.absoluteFilePath(),type);
        }else{
            if(info.suffix()==type){
                qDebug()<<("The file path is:"+name);
                fp.send(ip,port,name);
                while(!fp.isOk){
                    sleep(500);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<("hello let's start!");
    getFileName("D:\\github\\FtpServer","jpg");
    getFileName("D:\\github\\FtpServer","png");
    return a.exec();
}
