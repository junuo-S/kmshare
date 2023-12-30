#pragma once

#include <QTcpSocket>

class KMShareClient : public QObject
{
public:
	KMShareClient(QObject* parent = nullptr);
	~KMShareClient();
	void connectToHost(const QString& hostName, quint16 port);
	void disconnectFromHost();

private:
	void onReadyRead();

	QTcpSocket* m_tcpSocket = nullptr;
};
