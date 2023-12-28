#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include <QTcpSocket>
#include <QFile>
#include <QtNetwork>
#include <QObject>
#include <QCoreApplication>

class FileUpload: public QObject
{
    Q_OBJECT
public:
    FileUpload();
    void send(QString ip,int port,QString FileName);
    bool isOk=false;
    QString text;

public slots:
    void openFile(QString filename);
    void startTransfer();
    void updateClientProgress(qint64);
    void displayError(QAbstractSocket::SocketError);
    //void ReadMessage();
    void sleep(unsigned int msec);

private:
    QTcpSocket *m_tcpClient;
    QFile *m_localFile;
    qint64 m_totalBytes;
    qint64 m_bytesWritten;
    qint64 m_bytesToWrite;
    qint64 m_payloadSize;
    QString m_fileName;
    QByteArray m_outBlock;
    quint16 blockSize;
};

#endif // FILEUPLOAD_H
