//TODO: slotAddImage, slotAddFolders, slotClearImages
#pragma once

#include <QWidget>
#include "ui_ImageSelect.h"

class ImageSelect : public QWidget
{
	Q_OBJECT

public:
	ImageSelect(QWidget *parent = Q_NULLPTR);
	~ImageSelect();

private:
	void connects();

	void slotAddImages();
	void slotAddFolders();
	void slotClearImages();

private:
	Ui::ImageSelect ui;
};
