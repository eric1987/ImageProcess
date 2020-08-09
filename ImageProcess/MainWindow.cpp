#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connects();
}

void MainWindow::connects()
{
	//切换功能界面
	connect(ui.camSort, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);
	connect(ui.localSort, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);
	connect(ui.transFormat, &QToolButton::clicked, this, &MainWindow::slotChangeWidget);
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
	else
	{
		ui.stackedWidget->setCurrentIndex(2);
	}
}
