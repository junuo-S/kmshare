#include "serverwidget.h"

#include "core/hook/hook.h"
#include "core/network/kmshareserver.h"

ServerWidget::ServerWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
	, m_groupBox(new QGroupBox(this))
	, m_groupVLayout(new QVBoxLayout(m_groupBox))
	, m_ipLabel(new QLabel(this))
	, m_ipCombox(new QComboBox(this))
	, m_statusLabel(new QLabel(this))
	, m_sharingCheckBox(new QCheckBox(this))
	, m_statusHLayout(new QHBoxLayout(nullptr))
	, m_portLabel(new QLabel(this))
	, m_portEdit(new QLineEdit(this))
	, m_changePortButton(new QPushButton(this))
	, m_portHLayout(new QHBoxLayout(nullptr))
	, m_openDeviceListButton(new QPushButton(this))
	, m_vLayout(new QVBoxLayout(this))
	, m_stopShareShortcut(new QShortcut(Qt::CTRL + Qt::ALT + Qt::Key_S, this))
	, m_kmshareServer(new KMShareServer(this))
{
	initUI();
	m_groupBox->setChecked(false);
	connect(m_sharingCheckBox, &QCheckBox::stateChanged, this, &ServerWidget::onSharingCheckBoxStateChanged);
	connect(m_stopShareShortcut, &QShortcut::activated, this, &ServerWidget::onStopShortcutActivated);
	connect(m_groupBox, &QGroupBox::toggled, this, &ServerWidget::serverMode);
}

ServerWidget::~ServerWidget()
{
	delete m_portHLayout;
	delete m_statusHLayout;
}

void ServerWidget::setGroupBoxChecked(bool checked)
{
	if (!checked)
	{
		m_sharingCheckBox->setCheckState(Qt::Unchecked);
	}
	m_groupBox->setChecked(checked);
}

void ServerWidget::initUI()
{
	m_vLayout->addWidget(m_groupBox);

	m_groupBox->setCheckable(true);
	m_groupBox->setTitle(tr("server mode"));
	m_ipLabel->setText(tr("local ip"));
	m_ipCombox->clear();
	m_ipCombox->addItems(m_kmshareServer->getLocalIPv4Address());
	m_ipCombox->setToolTip(tr("Possible IP addresses for this machine. If the connection fails, please try changing to another IP address"));
	m_statusLabel->setText(tr("status"));
	m_sharingCheckBox->setText(tr("sharing"));
	m_statusHLayout->addWidget(m_ipLabel);
	m_statusHLayout->addWidget(m_ipCombox);
	m_statusHLayout->addStretch();
	m_statusHLayout->addWidget(m_statusLabel);
	m_statusHLayout->addWidget(m_sharingCheckBox);

	m_portLabel->setText(tr("port"));
	m_portEdit->setText("9521");
	m_changePortButton->setText(tr("change port"));
	m_portHLayout->addWidget(m_portLabel);
	m_portHLayout->addWidget(m_portEdit);
	m_portHLayout->addWidget(m_changePortButton);
	m_openDeviceListButton->setText(tr("open device list"));

	m_groupVLayout->addLayout(m_statusHLayout);
	m_groupVLayout->addLayout(m_portHLayout);
	m_groupVLayout->addWidget(m_openDeviceListButton);
}

void ServerWidget::setServerWidgetsEnable(bool enable)
{
	m_portEdit->setEnabled(enable);
	m_changePortButton->setEnabled(enable);
	m_openDeviceListButton->setEnabled(enable);
	m_ipCombox->setEnabled(enable);
}

void ServerWidget::onSharingCheckBoxStateChanged(int state)
{
	if (state == Qt::Checked)
	{
		Hook::installGlobalMouseHook();
		setServerWidgetsEnable(false);
	}
	else
	{
		Hook::uninstallGlobalMouseHook();
		setServerWidgetsEnable(true);
	}
}

void ServerWidget::onStopShortcutActivated()
{
	m_sharingCheckBox->setCheckState(Qt::Unchecked);
}

void ServerWidget::serverMode(bool enable)
{
	if (enable)
	{
		m_ipLabel->setText(tr("local ip"));
		m_ipCombox->clear();
		m_ipCombox->addItems(m_kmshareServer->getLocalIPv4Address());
		m_kmshareServer->setPort(m_portEdit->text().toInt());
		m_kmshareServer->listen();
	}
	else
	{
		m_kmshareServer->close();
	}
	emit turnOnServerMode(enable);
}

