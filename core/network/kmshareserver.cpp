#include "kmshareserver.h"

#include <QNetworkInterface>

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

QStringList KMShareServer::getLocalIPv4Address() const
{
	QStringList ipAddrs;
	static QStringList virtualAdapters = { "VMware", "VirtualBox" };
	for (const auto& _interface : QNetworkInterface::allInterfaces())
	{
		bool isVirtual = false;
		for (const QString& adapter : virtualAdapters)
		{
			if (_interface.humanReadableName().contains(adapter, Qt::CaseInsensitive))
			{
				isVirtual = true;
				break;
			}
		}
		if (isVirtual) continue;

		auto type = _interface.type();
		if (type == QNetworkInterface::Ethernet
			|| type == QNetworkInterface::Wifi)
		{
			auto allEntries = _interface.addressEntries();
			for (const auto& entry : allEntries)
			{
				auto hostAddress = entry.ip();
				if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol
					&& !hostAddress.isLoopback()
					&& !hostAddress.isLinkLocal())
				{
					ipAddrs.append(hostAddress.toString());
				}
			}
		}
	}

	return ipAddrs;
}

bool KMShareServer::listen()
{
	return m_tcpServer->listen(QHostAddress::Any, m_port);
}

void KMShareServer::close()
{
	m_tcpServer->close();
	clearDeviceList();
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
