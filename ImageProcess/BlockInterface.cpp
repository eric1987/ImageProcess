#include <QFileDialog>
#include <QTextStream>
#include <QStandardPaths>
#include <QMessageBox>
#include "BlockInterface.h"
#include "Util.h"
#include "GenBlockFile.h"
#include "Log/Log.h"

BlockInterface::BlockInterface(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

BlockInterface::~BlockInterface()
{
}

void BlockInterface::init()
{
	connect(ui.clearPos, &QPushButton::clicked, this, &BlockInterface::clearPos);
	connect(ui.selectPosFile, &QPushButton::clicked, this, &BlockInterface::selectPos);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &BlockInterface::blockPathChanged);
	connect(ui.selectPath, &QPushButton::clicked, this, &BlockInterface::blockPathSelected);
	connect(ui.genBlock, &QPushButton::clicked, this, &BlockInterface::genBlockFile);
	connect(ui.imageBasePath, &QLineEdit::textChanged, this, &BlockInterface::basePathChanged);
	connect(ui.selectBashPath, &QPushButton::clicked, this, &BlockInterface::basePathSelected);
	connect(ui.templateFile, &QPushButton::clicked, this, &BlockInterface::onTemplateFile);
	connect(ui.lineEditTemplate, &QLineEdit::textChanged, this, &BlockInterface::onTemplateChanged);
}

void BlockInterface::selectPos()
{
	ui.posWidget->clear();
	m_posFiles = Util::getPosFile();
	ui.posWidget->addItems(m_posFiles);
}

void BlockInterface::blockPathChanged()
{
	m_blockPath = ui.lineEdit->text();
}

void BlockInterface::blockPathSelected()
{
	m_blockPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	ui.lineEdit->setText(m_blockPath);
}

void BlockInterface::basePathChanged()
{
	m_basePath = ui.imageBasePath->text();
}

void BlockInterface::basePathSelected()
{
	m_basePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	ui.imageBasePath->setText(m_basePath);
}

void BlockInterface::clearPos()
{
	ui.posWidget->clear();
	m_posFiles.clear();
}

void BlockInterface::genBlockFile()
{
	readPosFile();
	GenBlockFile *genBlock = new GenBlockFile;
	genBlock->setBlocks(m_blocks);
	genBlock->setTemplateFile(m_templateFile);
	genBlock->setBasePath(m_basePath);
	genBlock->setFile(m_blockPath + "/block.xls");
	connect(genBlock, &GenBlockFile::signalFinished, this, &BlockInterface::slotGenBlockFinished);
	connect(genBlock, &GenBlockFile::finished, genBlock, &GenBlockFile::deleteLater);
	genBlock->start();
}

void BlockInterface::readPosFile()
{
	m_blocks.clear();
	Q_FOREACH(QString file, m_posFiles)
	{
		readSignalFile(file);
	}
}

void BlockInterface::readSignalFile(QString posFile)
{
	int sort;
	int groupName;
	QString nickName;
	decodeFileName(posFile, groupName, sort, nickName);
	QString strGroup = QString::number(groupName);
	QFile file(posFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		Log::INFO(QStringLiteral("打开repos文件失败。"));
		return;
	}
	QTextStream in(&file);
	QString line;
	QList<Block> blocks;
	while (in.readLineInto(&line))
	{
		QStringList list = line.split(" ");
		Block block;
		if (list.size() == 5)
		{
			block.imageName = list[0];
			block.groupName = strGroup;
			block.nickName = nickName;
			block.latitude = list[2];
			block.longitude = list[3];
			block.height = list[4];
		}
		blocks.append(block);
	}
	if (m_blocks.value(groupName).find(sort) == m_blocks.value(groupName).end())
	{
		m_blocks[groupName].insert(sort, blocks);
	}
	file.close();	
}

void BlockInterface::decodeFileName(QString file, int &groupName, int &sort, QString &nickname)
{
	QRegExp reg;
	QString pattern1 = QStringLiteral("(相机)(\\w)");
	QString pattern2 = QStringLiteral("(架次)(\\d+)");
	reg.setPattern(pattern1);
	int pos = reg.indexIn(file);
	if (pos > -1)
	{
		nickname = reg.cap(2);
	}
	if (nickname == "A")
	{
		groupName = 1;
	}
	else if (nickname == "D")
	{
		groupName = 2;
	}
	else if (nickname == "W")
	{
		groupName = 3;
	}
	else if (nickname == "X")
	{
		groupName = 4;
	}
	else if (nickname == "S")
	{
		groupName = 5;
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("选择的pos文件名称有误。"));
	}
	reg.setPattern(pattern2);
	pos = reg.indexIn(file);
	if (pos > -1)
	{
		QString value = reg.cap(2);
		sort = value.toInt();
	}
}

void BlockInterface::slotGenBlockFinished(bool b)
{
	if (b)
	{
		QMessageBox::information(this, QStringLiteral("通知"), QStringLiteral("Block文件生成完毕。"));
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("通知"), QStringLiteral("Block文件生成失败，请先进行架次分类并检查是否一致。"));
	}
}

void BlockInterface::onTemplateFile()
{
	m_templateFile = QFileDialog::getOpenFileName(this, QStringLiteral("选择block模板文件"), "/home", tr("Block(*.xls)"));
	ui.lineEditTemplate->setText(m_templateFile);
}

void BlockInterface::onTemplateChanged(QString text)
{
	m_templateFile = text;
}
