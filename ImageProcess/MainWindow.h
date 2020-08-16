#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
		
public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	
private:
	void connects();

	void slotChangeWidget();
private:
    Ui::MainWindowClass ui;
	
};
