#include "clientwidget.h"

#include "core/network/kmshareclient.h"

ClientWidget::ClientWidget(QWidget* parent /* = nullptr */)
	: QWidget(parent)
	, m_connectButton(new QPushButton(this))
	, m_disconnectButton(new QPushButton(this))
	, m_ipEdit(new QLineEdit(this))
	, m_portEdit(new QLineEdit(this))
	, m_ipLabel(new QLabel(this))
	, m_portLabel(new QLabel(this))
	, m_statusLabel(new QLabel(this))
	, m_groupBox(new QGroupBox(this))
	, m_editLayout(new QHBoxLayout(nullptr))
	, m_buttonLayout(new QHBoxLayout(nullptr))
	, m_vLayout(new QVBoxLayout(this))
	, m_groupVLayout(new QVBoxLayout(m_groupBox))
	, m_kmshareClient(new KMShareClient(this))
{
	initUI();
	m_groupBox->setChecked(false);
	connect(m_kmshareClient, &KMShareClient::connected, this, &ClientWidget::onTcpSocketConnected);
	connect(m_kmshareClient, &KMShareClient::disconnected, this, &ClientWidget::onTcpSocketDisconnected);
	connect(m_connectButton, &QPushButton::clicked, this, &ClientWidget::onConnectButtonClicked);
	connect(m_disconnectButton, &QPushButton::clicked, this, &ClientWidget::onDisconnectButtonClicked);
	connect(m_groupBox, &QGroupBox::toggled, this, &ClientWidget::clientMode);
}

ClientWidget::~ClientWidget()
{
	delete m_editLayout;
	delete m_buttonLayout;
}

void ClientWidget::clientMode(bool enable)
{
	if (enable == false)
		m_kmshareClient->close();
	emit turnOnClientMode(enable);
}

void ClientWidget::setGroupBoxChecked(bool checked)
{
	m_groupBox->setChecked(checked);
}

void ClientWidget::initUI()
{
	m_groupBox->setTitle(tr("client mode"));
	m_groupBox->setCheckable(true);
	m_groupBox->setLayout(m_groupVLayout);
	m_vLayout->addWidget(m_groupBox);

	m_statusLabel->setText(QString("%1: %2").arg(tr("status")).arg(tr("disconnected")));
	m_ipLabel->setText(tr("server ip"));
	m_portLabel->setText(tr("port"));
	m_ipEdit->setText("192.168.0.");
	m_portEdit->setText("9521");
	m_editLayout->addWidget(m_ipLabel);
	m_editLayout->addWidget(m_ipEdit);
	m_editLayout->addWidget(m_portLabel);
	m_editLayout->addWidget(m_portEdit);
	m_editLayout->setStretch(0, 1);
	m_editLayout->setStretch(1, 10);
	m_editLayout->setStretch(2, 1);
	m_editLayout->setStretch(3, 3);

	m_connectButton->setText(tr("connect"));
	m_disconnectButton->setText(tr("disconnect"));
	m_buttonLayout->addStretch();
	m_buttonLayout->addWidget(m_connectButton);
	m_buttonLayout->addWidget(m_disconnectButton);
	m_buttonLayout->addStretch();

	m_groupVLayout->addWidget(m_statusLabel);
	m_groupVLayout->addLayout(m_editLayout);
	m_groupVLayout->addLayout(m_buttonLayout);

	this->setLayout(m_vLayout);
}

void ClientWidget::onConnectButtonClicked()
{
	m_kmshareClient->connectToHost(m_ipEdit->text(), m_portEdit->text().toInt());
}

void ClientWidget::onDisconnectButtonClicked()
{
	m_kmshareClient->disconnectFromHost();
}

void ClientWidget::onTcpSocketConnected()
{
	m_statusLabel->setText(QString("%1: %2").arg(tr("status")).arg(tr("connected")));
}

void ClientWidget::onTcpSocketDisconnected()
{
	m_statusLabel->setText(QString("%1: %2").arg(tr("status")).arg(tr("disconnected")));
}
