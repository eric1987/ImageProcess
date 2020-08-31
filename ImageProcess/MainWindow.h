#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>

#include "ui_MainWindow.h"
#include "Log/Log.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
		
public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	
private:
	void connects();

	void slotChangeWidget();
	void slotShowStatus(QString str);
private:
    Ui::MainWindowClass ui;
	
};
