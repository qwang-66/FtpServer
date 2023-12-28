#include "fileupload.h"
#include <QDebug>
#include <QTime>

FileUpload::FileUpload()
{ 
    m_payloadSize=64*1024;
    m_totalBytes=0;
    m_bytesWritten=0;
    m_bytesToWrite=0;
    m_tcpClient=new QTcpSocket();

    connect(m_tcpClient,SIGNAL(connected()),
            this,SLOT(startTransfer()));

    connect(m_tcpClient,SIGNAL(bytesWritten(qint64)),
            this,SLOT(updateClientProgress(qint64)));

    connect(m_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

//    connect(m_tcpClient,SIGNAL(readyRead()),
//            this,SLOT(ReadMessage()));
}

void FileUpload::openFile(QString filename)
{

    m_fileName=filename;
    if(!m_fileName.isEmpty()){
        qDebug()<<("Open file successfully!");
    }
}

void FileUpload::send(QString ip,int port,QString FileName)
{
    isOk=false;
    blockSize=0;
    openFile(FileName);
    m_bytesWritten=0;
    m_tcpClient->connectToHost(ip,port);
}


void FileUpload::startTransfer()
{
    m_localFile=new QFile(m_fileName);
    if(!m_localFile->open(QFile::ReadOnly)){
        qDebug()<<"client：open file error!";
        return;
    }

    m_totalBytes=m_localFile->size();
    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_6);
    QString currentFileName=m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);

    //文件总大小、文件名大小、文件名
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    m_totalBytes+=m_outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<m_totalBytes<<qint64(m_outBlock.size()-sizeof(qint64)*2);
    m_bytesToWrite=m_totalBytes-m_tcpClient->write(m_outBlock);
    m_outBlock.resize(0);
}

void FileUpload::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void FileUpload::updateClientProgress(qint64 numBytes)
{
    m_bytesWritten+=(int)numBytes;
    if(m_bytesToWrite>0){
        m_outBlock=m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
        m_bytesToWrite-=(int)m_tcpClient->write(m_outBlock);
        m_outBlock.resize(0);
    }
    else{
        m_localFile->close();
    }

    if(m_bytesWritten==m_totalBytes){
        qDebug()<<("send to server successfully!");
        m_localFile->close();
        sleep(500);
        m_tcpClient->close();
        isOk=true;
    }
}

void FileUpload::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<m_tcpClient->errorString();
    m_tcpClient->close();
}

//void FileUpload::ReadMessage()
//{
//    qDebug()<<"得到反馈";
//    QDataStream in(m_tcpClient);
//    in.setVersion(QDataStream::Qt_5_6);
//    if(blockSize==0)
//    {
//        if(m_tcpClient->bytesAvailable() < (int)sizeof(quint16)) return;
//        in >> blockSize;
//    }
//    if(m_tcpClient->bytesAvailable() < blockSize) return;

//    in >> text;

//    if(text=="finish")
//}
