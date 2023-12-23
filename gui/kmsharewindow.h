#pragma once

#include <QWidget>
#include <QVBoxLayout>

class ClientWidget;

class KMShareWindow : public QWidget
{
public:
	KMShareWindow(QWidget* parent = nullptr);

private:
	ClientWidget* m_clientWidget = nullptr;
	QVBoxLayout* m_vLayout = nullptr;
};
