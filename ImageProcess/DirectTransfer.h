#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QFileDialog>

#include "ui_DirectTransfer.h"
#include "Common.h"
#include "Transfer.h"

class CustomSD : public QWidget
{
	Q_OBJECT
public:
	CustomSD(SDInfo info, QWidget *parent = Q_NULLPTR)
		: QWidget(parent)
	{
		m_info = info;
		QString boxName;
		if (info.connected)
		{
			boxName = QStringLiteral("影像-%1(%2) 影像数量%3").arg(m_info.nickname).arg(m_info.path).arg(m_info.imageSize);
		}
		else
		{
			boxName = QStringLiteral("影像-%1(离线) 影像数量%2").arg(m_info.nickname).arg(m_info.imageSize);
		}
		
		box = new QCheckBox(boxName);
		btn = new QPushButton(QStringLiteral("格式化"));
		btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		btn->setBackgroundRole(QPalette::Highlight);

		layout = new QHBoxLayout(this);
		layout->setSpacing(10);
		layout->setContentsMargins(0, 0, 0, 0);
		this->setLayout(layout);
		layout->addWidget(box);
		layout->addWidget(btn);

		connect(box, &QCheckBox::stateChanged, this, &CustomSD::checkChanged);
		connect(btn, &QPushButton::clicked, this, &CustomSD::format);
	}
	~CustomSD() {}

signals:
	void signalCheckChanged(int state, QString id);
	void signalFormat(QString path);

private:
	void checkChanged(int state)
	{
		emit signalCheckChanged(state, m_info.deviceID);
	}
	void format()
	{
		emit signalFormat(m_info.path);
	}

	SDInfo m_info;
	QCheckBox *box = nullptr;
	QPushButton *btn = nullptr;
	QHBoxLayout *layout = nullptr;
};

class DirectTransfer : public QWidget
{
	Q_OBJECT

public:
	DirectTransfer(QWidget *parent = Q_NULLPTR);
	~DirectTransfer();

	//设置sd卡信息
	void setSDInfo(QMap<QString, SDInfo> sdInfo);

signals:
	void signalFormatFinish();
	void signalRefreshSDInfo();

private:
	//初始化
	void init();

	//直接传输checkbox选择
	void directSelectChanged(int state, QString id);
	
	//将选择的sd卡格式化
	void formatSelectSD();

	//格式化单一sd
	void formatSingleSD(QString path);

	//格式化sd卡
	void formatSD(QString path);

	//拷贝sony文件
	bool CopySonyFile(QDir src, QDir dst, bool bCoverIfFileExists);

	//直接传输文件
	void transfer();

	//传输文件进度更新
	void refreshProcess(int value);

	//传输文件失败
	void transferFailed();

	//传输文件成功
	void transferFinished();
	
	//
	void unableOperation();

	//
	void enableOperation();

	//设置影像架次存储路径
	void setSavePath();
	void changeSavePath(QString path);

	Ui::DirectTransfer ui;

	QMap<QString, SDInfo> m_sdInfo;
	QVBoxLayout *m_directLayout = nullptr;

	QMap<QString, bool> m_sdCheck;
	QString m_savePath;

	Transfer *m_transfer = nullptr;
};
