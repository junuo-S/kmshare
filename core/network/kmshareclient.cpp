#include "kmshareclient.h"

#include "core/event/abstractevent.h"
#include "core/event/mouseevent.h"
#include "core/event/keyboardevent.h"

KMShareClient::KMShareClient(QObject* parent /*= nullptr*/)
	: QObject(parent)
	, m_tcpSocket(new QTcpSocket(this))
{
	connect(m_tcpSocket, &QTcpSocket::connected, this, &KMShareClient::onTcpSocketConnected);
	connect(m_tcpSocket, &QTcpSocket::disconnected, this, &KMShareClient::onTcpSocketDisconnected);
	connect(m_tcpSocket, &QTcpSocket::readyRead, this, &KMShareClient::onReadyRead);
}

KMShareClient::~KMShareClient()
{

}

void KMShareClient::connectToHost(const QString& hostName, quint16 port)
{
	m_tcpSocket->connectToHost(hostName, port);
}

void KMShareClient::disconnectFromHost()
{
	m_tcpSocket->disconnectFromHost();
}

void KMShareClient::close()
{
	m_tcpSocket->close();
}

void KMShareClient::onReadyRead()
{
	AbstractEvent* event = MouseEvent::fromJsonString(m_tcpSocket->readAll().toStdString());
	if (event)
		event->post();
}

void KMShareClient::onTcpSocketConnected()
{
	emit connected();
}

void KMShareClient::onTcpSocketDisconnected()
{
	emit disconnected();
}
