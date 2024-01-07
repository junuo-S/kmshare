#include "kmshareserver.h"

#include "core/event/eventqueue.h"
#include "core/event/abstractevent.h"

QList<Device*> KMShareServer::s_deviceList;

KMShareServer::KMShareServer(QObject* parent /*= nullptr*/)
	: m_tcpServer(new QTcpServer(this))
{
	connect(m_tcpServer, &QTcpServer::newConnection, this, &KMShareServer::onNewConnection);
}

KMShareServer::~KMShareServer()
{
	m_tcpServer->close();
	clearDeviceList();
}

const QList<Device*>& KMShareServer::deviceList()
{
	return s_deviceList;
}

quint16 KMShareServer::port() const
{
	return m_port;
}

void KMShareServer::setPort(qint16 port)
{
	m_port = port;
}

QString KMShareServer::serverAddress() const
{
	return m_tcpServer->serverAddress().toString();
}

bool KMShareServer::listen()
{
	return m_tcpServer->listen(QHostAddress::Any, m_port);
}

void KMShareServer::close()
{
	m_tcpServer->close();
}

void KMShareServer::clearDeviceList()
{
	for (Device* device : s_deviceList)
		delete device;
	s_deviceList.clear();
}

void KMShareServer::onNewConnection()
{
	Device* device = new Device;
	device->m_tcpSocket = m_tcpServer->nextPendingConnection();
	device->m_address = device->m_tcpSocket->peerAddress().toString();
	device->m_port = device->m_tcpSocket->peerPort();
	device->m_deviceName = device->m_tcpSocket->peerName();
	s_deviceList.append(device);
}
