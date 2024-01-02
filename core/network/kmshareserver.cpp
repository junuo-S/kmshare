#include "kmshareserver.h"

#include "core/event/eventqueue.h"
#include "core/event/abstractevent.h"

QList<Device*> KMShareServer::s_deviceList;

KMShareServer::KMShareServer(QObject* parent /*= nullptr*/)
	: m_tcpServer(new STcpServer(this))
	, m_sharingThread(new SharingThread(this))
{
	m_sharingThread->start();
	connect(m_tcpServer, &STcpServer::newConnection, m_sharingThread, &SharingThread::addNewDevice, Qt::QueuedConnection);
}

KMShareServer::~KMShareServer()
{
	m_tcpServer->close();
	clearDeviceList();
	if (m_sharingThread)
	{
		m_sharingThread->quit();
		m_sharingThread->wait();
	}
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

void KMShareServer::startSharing()
{
	m_sharingThread = new SharingThread(this);
	m_sharingThread->start();
}

void KMShareServer::stopSharing()
{
	if (m_sharingThread)
	{
		m_sharingThread->quit();
		m_sharingThread->wait();
		delete m_sharingThread;
		m_sharingThread = nullptr;
	}
}

void KMShareServer::onNewConnection(qintptr socketDescriptor)
{
	
}

KMShareServer::SharingThread::SharingThread(QObject* parent /*= nullptr*/)
	: QThread(parent)
{

}

void KMShareServer::SharingThread::quit()
{
	m_isExit = true;
	EventQueue::instance()->notifyAll();
	QThread::quit();
}

void KMShareServer::SharingThread::addNewDevice(qintptr socketDescriptor)
{
	Device* device = new Device;
	device->m_tcpSocket = new QTcpSocket(this);
	device->m_tcpSocket->setSocketDescriptor(socketDescriptor);
	device->m_address = device->m_tcpSocket->peerAddress().toString();
	device->m_port = device->m_tcpSocket->peerPort();
	device->m_deviceName = device->m_tcpSocket->peerName();
	s_deviceList.append(device);
}

void KMShareServer::SharingThread::run()
{
	auto eventQueue = EventQueue::instance();
	while (!m_isExit)
	{
		if (eventQueue->empty())
		{
			eventQueue->waitForEvent();
		}
		else
		{
			auto event = eventQueue->front();
			for (Device* device : s_deviceList)
			{
				if (device->m_needShare && device->isConnected())
				{
					device->m_tcpSocket->write(event->toString().c_str());
					qDebug() << event->toString().c_str();
				}
			}
			eventQueue->pop();
		}
	}
}

STcpServer::STcpServer(QObject* parent /*= nullptr*/)
	: QTcpServer(parent)
{

}

void STcpServer::incomingConnection(qintptr socketDescriptor)
{
	emit newConnection(socketDescriptor);
}
