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
}
