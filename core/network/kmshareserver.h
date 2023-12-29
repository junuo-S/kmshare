#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

struct Device
{
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
	bool listen();
	void close();
	void clearDeviceList();
	void startSharing();
	void stopSharing();

private:
	class SharingThread : public QThread
	{
	public:
		SharingThread(QObject* parent = nullptr);
		void quit();

	protected:
		virtual void run() override;

	private:
		bool m_isExit = false;
	};

	void onNewConnection();

	static QList<Device*> s_deviceList;
	quint16 m_port = 9521;
	QTcpServer* m_tcpServer = nullptr;
	SharingThread* m_sharingThread = nullptr;
};
