#include "widget.h"
#include <QDebug>
#include <QtNetwork>

Widget::Widget()
{
    connect(&m_tcpServer,SIGNAL(newConnection()),
            this,SLOT(acceptConnection()));
}

void Widget::start()
{
    if(!m_tcpServer.listen(QHostAddress::Any,6666)){
        qDebug()<<m_tcpServer.errorString();
        m_tcpServer.close();
        return;
    }
    qDebug()<<"listen 6666 now...";
}

void Widget::acceptConnection()
{
    m_totalBytes=0;
    m_bytesReceived=0;
    m_fileNameSize=0;
    m_tcpServerConnection=m_tcpServer.nextPendingConnection();
    connect(m_tcpServerConnection,SIGNAL(readyRead()),this,SLOT(updateServerProgress()));
    connect(m_tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

    qDebug()<<"accept connect";

}

void Widget::updateServerProgress()
{
    QDataStream in(m_tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_6);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (m_bytesReceived<=sizeof(qint64)*2){
        if((m_tcpServerConnection->bytesAvailable()>=sizeof(qint64)*2)&&(m_fileNameSize==0)){
            // 接收数据总大小信息和文件名大小信息
            in>>m_totalBytes>>m_fileNameSize;
            m_bytesReceived +=sizeof(qint64)*2;
        }
        if((m_tcpServerConnection->bytesAvailable()>=m_fileNameSize)&&(m_fileNameSize!=0)){
            // 接收文件名，并建立文件
            in>>m_fileName;
            m_bytesReceived+=m_fileNameSize;
            m_localFile = new QFile(root+m_fileName);
            if (!m_localFile->open(QFile::WriteOnly)){
                qDebug() << "server: open file error!";
                m_localFile->close();
                return;
            }
        }
        else{
            return;
        }
    }

    // 如果接收的数据小于总数据，那么写入文件
    if(m_bytesReceived<m_totalBytes) {
        m_bytesReceived+=m_tcpServerConnection->bytesAvailable();
        m_inBlock = m_tcpServerConnection->readAll();
        m_localFile->write(m_inBlock);
        m_inBlock.resize(0);
    }

    // 接收数据完成时
    if (m_bytesReceived==m_totalBytes){
        m_localFile->close();
        m_tcpServerConnection->close();
        qDebug()<<"receive successful!";

    }
}

void Widget::displayError(QAbstractSocket::SocketError socketError)
{

    Q_UNUSED(socketError)
    qDebug()<<m_tcpServerConnection->errorString();
    m_tcpServerConnection->close();
}
