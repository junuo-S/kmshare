#include "kmsharewindow.h"

#include "subwidget/clientwidget.h"
#include "subwidget/serverwidget.h"

KMShareWindow::KMShareWindow(QWidget* parent)
	: QWidget(parent)
	, m_clientWidget(new ClientWidget(this))
	, m_serverWidget(new ServerWidget(this))
	, m_vLayout(new QVBoxLayout(this))
{
	m_vLayout->addWidget(m_clientWidget);
	m_vLayout->addWidget(m_serverWidget);
	connect(m_clientWidget, &ClientWidget::turnOnClientMode, this, &KMShareWindow::onTurnOnClientMode);
	connect(m_serverWidget, &ServerWidget::turnOnServerMode, this, &KMShareWindow::onTurnOnServerMode);
}

KMShareWindow::~KMShareWindow()
{

}

void KMShareWindow::onTurnOnServerMode(bool enable)
{
	if (enable)
	{
		m_clientWidget->setGroupBoxChecked(false);
	}
}

void KMShareWindow::onTurnOnClientMode(bool enable)
{
	if (enable)
	{
		m_serverWidget->setGroupBoxChecked(false);
	}
}
