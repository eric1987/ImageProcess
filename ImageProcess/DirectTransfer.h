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

//Ӱ��sd������Ϣչʾ
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
			boxName = QStringLiteral("Ӱ��-%1(%2) Ӱ������%3").arg(m_info.nickname).arg(m_info.path).arg(m_info.imageSize);
		}
		else
		{
			boxName = QStringLiteral("Ӱ��-%1(����) Ӱ������%2").arg(m_info.nickname).arg(m_info.imageSize);
		}
		
		box = new QCheckBox(boxName);
		btn = new QPushButton(QStringLiteral("��ʽ��"));
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
	//����ѡ��״̬
	void setState(int state)
	{
		box->setCheckState(Qt::CheckState(state));
	}

signals:
	//ѡ��״̬�仯��Ϣ
	void signalCheckChanged(int state, QString id);
	//��ʽ���ź�
	void signalFormat(QString path);

private:
	//ѡ��״̬�仯��Ӧ����
	void checkChanged(int state)
	{
		emit signalCheckChanged(state, m_info.deviceID);
	}
	//��ʽ����Ӧ����
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

	//����sd����Ϣ
	//key��sd�����ƣ�asdw�� value��sd����Ϣ
	void setSDInfo(QMap<QString, SDInfo> sdInfo);

signals:
	//���͸�ʽ������ź�
	void signalFormatFinish();
	//����ˢ��SD����Ϣ�ź�
	void signalRefreshSDInfo();

private:
	//��ʼ��
	void init();

	//ֱ�Ӵ���checkboxѡ��
	//state��ѡ��״̬��id��ѡ���boxID��asdw
	void directSelectChanged(int state, QString id);
	
	//��ѡ���sd����ʽ��
	void formatSelectSD();

	//��ʽ����һsd
	void formatSingleSD(QString path);

	//��ʽ��sd��
	void formatSD(QString path);

	//����sony�ļ�
	bool CopySonyFile(QDir src, QDir dst, bool bCoverIfFileExists);

	//ֱ�Ӵ����ļ�
	void transfer();

	//���ô������״̬
	void setTransStateFinished();

	//�����ļ����ȸ���
	void refreshProcess(int value);

	//�����ļ�ʧ��
	void transferFailed();

	//�����ļ��ɹ�
	void transferFinished();
	
	//���ò���
	void unableOperation();

	//���ò���
	void enableOperation();

	//����Ӱ��ܴδ洢·��
	void setSavePath();
	void changeSavePath(QString path);
	//����ȫѡ/ȡ��
	void setCheckAll(int state);

	Ui::DirectTransfer ui;

	QMap<QString, SDInfo> m_sdInfo;			//sd����Ϣ
	QVBoxLayout *m_directLayout = nullptr;	//����
	QMap<QString, bool> m_sdCheck;			//sd��ѡ��״̬
	QString m_savePath;						//����·��
	Transfer *m_transfer = nullptr;			//�������
};
