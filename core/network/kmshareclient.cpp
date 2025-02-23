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
	// 因为mousemove一般比较快，所以必须要处理TCP粘包，不然多个事件粘在一起无法响应
	QString bufferString = m_tcpSocket->readAll();
	QStringList eventStringList = bufferString.split("split");
	for (const QString& eventString: eventStringList)
	{
		if (eventString.isEmpty())
			continue;
		qDebug() << eventString;
		AbstractEvent* event = MouseEvent::fromJsonString(eventString.toStdString());
		if (event)
		{
			qDebug() << "posted";
			event->post();
		}
	}
}

void KMShareClient::onTcpSocketConnected()
{
	emit connected();
}

void KMShareClient::onTcpSocketDisconnected()
{
	emit disconnected();
}
