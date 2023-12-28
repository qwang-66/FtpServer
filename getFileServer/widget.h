#ifndef WIDGET_H
#define WIDGET_H

#include <QAbstractSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

class Widget : public QObject
{
    Q_OBJECT
public:
    Widget();
    void start();
public slots:

    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);


private:
    QTcpServer m_tcpServer;
    QTcpSocket *m_tcpServerConnection;
    qint64 m_totalBytes;
    qint64 m_bytesReceived;
    qint64 m_fileNameSize;
    QString m_fileName;
    QFile *m_localFile;
    QByteArray m_inBlock;
    QString root="C://get_res//";
};

#endif // WIDGET_H
