#include "kmsharewindow.h"

#include "subwidget/clientwidget.h"

KMShareWindow::KMShareWindow(QWidget* parent)
	: QWidget(parent)
	, m_clientWidget(new ClientWidget(this))
	, m_vLayout(new QVBoxLayout(this))
{
	this->setLayout(m_vLayout);
	m_vLayout->addWidget(m_clientWidget);
}
