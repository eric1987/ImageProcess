#pragma once

#include <QWidget>
#include "ui_BlockInterface.h"
#include "Common.h"

class BlockInterface : public QWidget
{
	Q_OBJECT

public:
	BlockInterface(QWidget *parent = Q_NULLPTR);
	~BlockInterface();

private:
	void init();
	void selectPos();
	void posTypeChanged(int index);
	void blockPathChanged();
	void blockPathSelected();
	void basePathChanged();
	void basePathSelected();
	void clearPos();
	void genBlockFile();
	void readPosFile();
	void readSignalFile(QString file);
	void decodeFileName(QString file, int &groupName, int &sort, QString &nickname);
	void slotGenBlockFinished(bool b);
	void onTemplateFile();
	void onTemplateChanged(QString text);

private:
	Ui::BlockInterface ui;
	QStringList m_posFiles;
	int m_posType;
	QString m_blockPath;
	QString m_basePath;
	QMap<int, QStringList> m_images;
	QMap<int, QMap<int, QList<Block>>> m_blocks;
	QString m_templateFile;
};
