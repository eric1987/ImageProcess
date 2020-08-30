#pragma once
#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>

class CustomSortie : public QWidget
{
	Q_OBJECT
public:
	CustomSortie(int sortie, int size, QWidget *parent = Q_NULLPTR)
		: QWidget(parent)
	{
		m_sortie = sortie;
		box = new QCheckBox(QStringLiteral("架次%1  影像%2张").arg(sortie).arg(size));
		btn = new QPushButton(QStringLiteral("查看详情"));
		btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		btn->setBackgroundRole(QPalette::Highlight);

		layout = new QHBoxLayout(this);
		layout->setSpacing(5);
		layout->setContentsMargins(0, 0, 0, 0);
		this->setLayout(layout);
		layout->addWidget(box);
		layout->addWidget(btn);

		connect(box, &QCheckBox::stateChanged, this, &CustomSortie::checkChanged);
		connect(btn, &QPushButton::clicked, this, &CustomSortie::showSortie);
	}
	~CustomSortie() {}

	void setState(int state)
	{
		box->setCheckState(Qt::CheckState(state));
	}

signals:
	void signalCheckChanged(int state, int sortie);
	void signalSortieDetail(int sortie);

private:
	void checkChanged(int state)
	{
		emit signalCheckChanged(state, m_sortie);
	}

	void showSortie()
	{
		emit signalSortieDetail(m_sortie);
	}

	int m_sortie = -1;
	QCheckBox *box = nullptr;
	QPushButton *btn = nullptr;
	QHBoxLayout *layout = nullptr;
};