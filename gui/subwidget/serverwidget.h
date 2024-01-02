#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QShortcut>

class KMShareServer;

class ServerWidget : public QWidget
{
public:
	ServerWidget(QWidget* parent = nullptr);
	~ServerWidget();

private:
	void initUI();
	void setServerWidgetsEnable(bool enable);
	void onSharingCheckBoxStateChanged(int state);
	void onStopShortcutActivated();
	void serverMode(bool enable);

	QGroupBox* m_groupBox = nullptr;
	QVBoxLayout* m_groupVLayout = nullptr;
	QLabel* m_ipLabel = nullptr;
	QLabel* m_statusLabel = nullptr;
	QCheckBox* m_sharingCheckBox = nullptr;
	QHBoxLayout* m_statusHLayout = nullptr;
	QLabel* m_portLabel = nullptr;
	QLineEdit* m_portEdit = nullptr;
	QPushButton* m_changePortButton = nullptr;
	QHBoxLayout* m_portHLayout = nullptr;
	QPushButton* m_openDeviceListButton = nullptr;
	QVBoxLayout* m_vLayout = nullptr;
	QShortcut* m_stopShareShortcut = nullptr;
	KMShareServer* m_kmshareServer = nullptr;
};
