#include "DirectTransfer.h"

DirectTransfer::DirectTransfer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	init();
}

DirectTransfer::~DirectTransfer()
{
}

void DirectTransfer::setSDInfo(QMap<QString, SDInfo> sdInfo)
{
	m_sdInfo = sdInfo;
	
	QObjectList	children = ui.sdWidget->children();
	Q_FOREACH(QObject *obj, children)
	{
		QWidget *sd = qobject_cast<QWidget*>(obj);
		m_directLayout->removeWidget(sd);
		delete sd;
		sd = nullptr;
	}

	//��Ӽܴ�
	m_sdCheck.clear();
	QMapIterator<QString, SDInfo> iter(m_sdInfo);
	while (iter.hasNext())
	{
		iter.next();
		m_sdCheck[iter.key()] = false;
		CustomSD *sd = new CustomSD(iter.value(), ui.sdWidget);
		connect(sd, &CustomSD::signalCheckChanged, this, &DirectTransfer::directSelectChanged);
		connect(sd, &CustomSD::signalFormat, this, &DirectTransfer::formatSingleSD);

		m_directLayout->addWidget(sd);
	}
	m_directLayout->addStretch();
}

void DirectTransfer::init()
{
	ui.startTransfer->setCheckable(true);

	m_transfer = new Transfer;
	connect(m_transfer, &Transfer::signalDirectProcess, this, &DirectTransfer::refreshProcess);
	connect(m_transfer, &Transfer::signalDirectTransFailed, this, &DirectTransfer::transferFailed);
	connect(m_transfer, &Transfer::signalDirectTransFinished, this, &DirectTransfer::transferFinished);

	m_directLayout = new QVBoxLayout;
	ui.sdWidget->setLayout(m_directLayout);
	connect(ui.startFormat, &QPushButton::clicked, this, &DirectTransfer::formatSelectSD);
	connect(ui.startTransfer, &QPushButton::clicked, this, &DirectTransfer::transfer);

	connect(ui.selectSavePath, &QPushButton::clicked, this, &DirectTransfer::setSavePath);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &DirectTransfer::changeSavePath);
}

void DirectTransfer::directSelectChanged(int state, QString id)
{
	m_sdCheck[id] = state;
}

void DirectTransfer::formatSelectSD()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this, QStringLiteral("Ӱ�����ݸ�ʽ��"), QStringLiteral("�Ƿ��ѽ�Ӱ������ת����ϣ�"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
	{
		return;
	}
	reply = QMessageBox::question(this, QStringLiteral("Ӱ�����ݸ�ʽ����ȷ��"), QStringLiteral("ȷ�϶�Ӱ�����ݽ��и�ʽ����"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
	{
		return;
	}

	QMapIterator<QString, bool> iter(m_sdCheck);
	while (iter.hasNext())
	{
		iter.next();
		if (m_sdInfo.contains(iter.key()))
		{
			formatSD(m_sdInfo[iter.key()].path);
		}
	}

	QMessageBox::information(this, QStringLiteral("֪ͨ"), QStringLiteral("��ʽ������ɡ�"));
	emit signalRefreshSDInfo();
}

void DirectTransfer::formatSingleSD(QString path)
{
	QMessageBox::StandardButton reply = QMessageBox::question(this, QStringLiteral("Ӱ�����ݸ�ʽ��"), QStringLiteral("�Ƿ��ѽ�Ӱ������ת����ϣ�"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
	{
		return;
	}
	reply = QMessageBox::question(this, QStringLiteral("Ӱ�����ݸ�ʽ����ȷ��"), QStringLiteral("ȷ�϶�Ӱ�����ݽ��и�ʽ����"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
	{
		return;
	}

	formatSD(path);

	QMessageBox::information(this, QStringLiteral("֪ͨ"), QStringLiteral("��ʽ������ɡ�"));
	emit signalRefreshSDInfo();
}

void DirectTransfer::formatSD(QString path)
{
	QString disk = path;
	disk.remove('/');
	QProcess *p = new QProcess;
	//QString disk = it.key().toStdString().c_str();
	//m_formatedSD.append(disk);
	QStringList param;
	param << "/c"
		<< "format"
		<< disk
		<< "/q"
		<< "/y"
		<< "/fs:exFAT";

	p->start("cmd", param);
	//QMessageBox::information(this, QStringLiteral(""), QStringLiteral("��ʽ��������..."));
	p->waitForFinished();

	QString exePath = QCoreApplication::applicationDirPath();
	QString sony = exePath + "/sonyfiles";
	QDir src(sony);
	QDir dst(path);
	CopySonyFile(src, dst, true);
}

bool DirectTransfer::CopySonyFile(QDir src, QDir dst, bool bCoverIfFileExists)
{
	if (!dst.exists())
	{
		if (!dst.mkdir(dst.absolutePath()))
			return false;
	}

	src.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs);
	QFileInfoList fileInfoList = src.entryInfoList();
	//QStringList list = src.entryList();

	foreach(QFileInfo fileInfo, fileInfoList)
	{
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		//������Ŀ¼
		if (fileInfo.isDir())
		{
			//�ݹ���ÿ���
			if (!CopySonyFile(fileInfo.filePath(), dst.filePath(fileInfo.fileName()), true))
				return false;
		}
		//�������ļ�
		else
		{
			if (bCoverIfFileExists && dst.exists(fileInfo.fileName()))
			{
				dst.remove(fileInfo.fileName());
			}
			if (!QFile::copy(fileInfo.filePath(), dst.filePath(fileInfo.fileName())))
			{
				return false;
			}
		}
	}
	return true;
}

void DirectTransfer::transfer()
{
	if (ui.startTransfer->isChecked())	//����
	{
		QStringList transferList;
		QMapIterator<QString, bool> iter(m_sdCheck);
		while (iter.hasNext())
		{
			iter.next();
			if (iter.value())
			{
				transferList.append(iter.key());
			}
		}

		if (transferList.isEmpty() || m_savePath.isEmpty())
		{
			QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("��ѡ��洢·����Ҫ����ļܴΡ�"));
			return;
		}

		if (m_transfer == nullptr)
		{
			m_transfer = new Transfer;
		}
		m_transfer->setDstPath(m_savePath);
		m_transfer->setTransType(DIRECT);
		m_transfer->setSDCards(transferList, m_sdInfo);
		m_transfer->start();
		
		ui.startTransfer->setText(QStringLiteral("ֹͣ����"));
		ui.startTransfer->setChecked(true);
		unableOperation();
	}
	else
	{
		ui.startTransfer->setText(QStringLiteral("��ʼ����"));
		ui.startTransfer->setChecked(false);
		enableOperation();
	}
}

void DirectTransfer::refreshProcess(int value)
{
	ui.progressBar->setValue(value);
}

void DirectTransfer::transferFailed()
{
}

void DirectTransfer::transferFinished()
{
	ui.progressBar->setValue(100);
	QMessageBox::information(this, QStringLiteral(""), QStringLiteral("�ܴ��ļ�������ɡ�"));
	Log::INFO(QStringLiteral("�ܴ��ļ������ļ���"));
	transfer();
}

void DirectTransfer::unableOperation()
{
	ui.lineEdit->setDisabled(true);
	ui.selectSavePath->setDisabled(true);
	ui.startFormat->setDisabled(true);
}

void DirectTransfer::enableOperation()
{
	ui.lineEdit->setEnabled(true);
	ui.selectSavePath->setEnabled(true);
	ui.startFormat->setEnabled(true);
}

void DirectTransfer::setSavePath()
{
	m_savePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	ui.lineEdit->setText(m_savePath);
}

void DirectTransfer::changeSavePath(QString path)
{
	m_savePath = path;
}

