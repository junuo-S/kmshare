#include "serverwidget.h"

ServerWidget::ServerWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
	, m_groupBox(new QGroupBox(this))
	, m_groupVLayout(new QVBoxLayout(m_groupBox))
	, m_ipLabel(new QLabel(this))
	, m_portLabel(new QLabel(this))
	, m_portEdit(new QLineEdit(this))
	, m_changePortButton(new QPushButton(this))
	, m_portHLayout(new QHBoxLayout(nullptr))
	, m_openDeviceListButton(new QPushButton(this))
	, m_vLayout(new QVBoxLayout(this))
{
	initUI();
}

ServerWidget::~ServerWidget()
{
	delete m_portHLayout;
}

void ServerWidget::initUI()
{
	m_vLayout->addWidget(m_groupBox);

	m_groupBox->setCheckable(true);
	m_groupBox->setTitle(tr("server mode"));
	m_ipLabel->setText(QString("%1: %2").arg(tr("local ip")).arg("127.0.0.1"));
	m_portLabel->setText(tr("port"));
	m_portEdit->setText("9521");
	m_changePortButton->setText(tr("change port"));
	m_portHLayout->addWidget(m_portLabel);
	m_portHLayout->addWidget(m_portEdit);
	m_portHLayout->addWidget(m_changePortButton);
	m_openDeviceListButton->setText(tr("open device list"));

	m_groupVLayout->addWidget(m_ipLabel);
	m_groupVLayout->addLayout(m_portHLayout);
	m_groupVLayout->addWidget(m_openDeviceListButton);
}

