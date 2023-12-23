#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

class ServerWidget : public QWidget
{
public:
	ServerWidget(QWidget* parent = nullptr);
	~ServerWidget();

private:
	void initUI();

	QGroupBox* m_groupBox = nullptr;
	QVBoxLayout* m_groupVLayout = nullptr;
	QLabel* m_ipLabel = nullptr;
	QLabel* m_portLabel = nullptr;
	QLineEdit* m_portEdit = nullptr;
	QPushButton* m_changePortButton = nullptr;
	QHBoxLayout* m_portHLayout = nullptr;
	QPushButton* m_openDeviceListButton = nullptr;
	QVBoxLayout* m_vLayout = nullptr;
};
