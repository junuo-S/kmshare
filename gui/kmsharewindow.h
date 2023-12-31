﻿#pragma once

#include <QWidget>
#include <QVBoxLayout>

class ClientWidget;
class ServerWidget;

class KMShareWindow : public QWidget
{
public:
	KMShareWindow(QWidget* parent = nullptr);
	~KMShareWindow();

private:
	void onTurnOnServerMode(bool enable);
	void onTurnOnClientMode(bool enable);

	ClientWidget* m_clientWidget = nullptr;
	ServerWidget* m_serverWidget = nullptr;
	QVBoxLayout* m_vLayout = nullptr;
};
