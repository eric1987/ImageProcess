#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connects();

	Log::Start();
}

void MainWindow::connects()
{
	//切换功能界面
	connect(ui.camSort, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);
	connect(ui.localSort, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);
	connect(ui.transFormat, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);
	connect(ui.block, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);

	connect(ui.page_1, &CamSort::signalSDInfo, ui.page_3, &DirectTransfer::setSDInfo);
	connect(ui.page_1, &CamSort::signalStatus, this, &MainWindow::slotShowStatus);
}

void MainWindow::slotChangeWidget()
{
	QToolButton *btn = qobject_cast<QToolButton*>(sender());//使用Qt的类型转换，将指针恢复为QAction类型
	if (btn == ui.camSort)
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
	else if(btn == ui.localSort)
	{
		ui.stackedWidget->setCurrentIndex(1);
	}
	else if(btn == ui.transFormat)
	{
		ui.page_1->refreshSDInfo();
		ui.stackedWidget->setCurrentIndex(2);
	}
	else
	{
		ui.stackedWidget->setCurrentIndex(3);
	}
}

void MainWindow::slotShowStatus(QString mes)
{
	ui.statusBar->showMessage(mes);
}
