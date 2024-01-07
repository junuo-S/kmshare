#pragma once

#include <QTcpSocket>

class KMShareClient : public QObject
{
	Q_OBJECT

public:
	KMShareClient(QObject* parent = nullptr);
	~KMShareClient();
	void connectToHost(const QString& hostName, quint16 port);
	void disconnectFromHost();
	void close();

signals:
	void connected();
	void disconnected();

private:
	void onReadyRead();
	void onTcpSocketConnected();
	void onTcpSocketDisconnected();

	QTcpSocket* m_tcpSocket = nullptr;
};
