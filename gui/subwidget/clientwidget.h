#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

class KMShareClient;

class ClientWidget : public QWidget
{
public:
	ClientWidget(QWidget* parent = nullptr);
	~ClientWidget();

private:
	void initUI();
	void onConnectButtonClicked();
	void onDisconnectButtonClicked();
	void onTcpSocketConnected();
	void onTcpSocketDisconnected();
	void onGroupBoxClicked(bool checked = false);

	QPushButton* m_connectButton = nullptr;
	QPushButton* m_disconnectButton = nullptr;
	QLineEdit* m_ipEdit = nullptr;
	QLineEdit* m_portEdit = nullptr;
	QLabel* m_ipLabel = nullptr;
	QLabel* m_portLabel = nullptr;
	QLabel* m_statusLabel = nullptr;
	QGroupBox* m_groupBox = nullptr;
	QHBoxLayout* m_editLayout = nullptr;
	QHBoxLayout* m_buttonLayout = nullptr;
	QVBoxLayout* m_vLayout = nullptr;
	QVBoxLayout* m_groupVLayout = nullptr;
	KMShareClient* m_kmshareClient = nullptr;
};
