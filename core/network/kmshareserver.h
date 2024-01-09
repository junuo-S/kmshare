#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

class Device
{
public:
	~Device()
	{
		delete m_tcpSocket;
	}

	bool isConnected() const 
	{
		return m_tcpSocket->state() == QAbstractSocket::ConnectedState;
	}

	QString m_deviceName;
	QString m_address;
	quint16 m_port = 0;
	bool m_needShare = true;
	QTcpSocket* m_tcpSocket = nullptr;
};

class KMShareServer : public QObject
{
public:
	KMShareServer(QObject* parent = nullptr);
	~KMShareServer();
	static const QList<Device*>& deviceList();
	quint16 port() const;
	void setPort(qint16 port);
	QString serverAddress() const;
	QStringList getLocalIPv4Address() const;
	bool listen();
	void close();
	void clearDeviceList();

private:
	void onNewConnection();

	static QList<Device*> s_deviceList;
	quint16 m_port = 9521;
	QTcpServer* m_tcpServer = nullptr;
};
