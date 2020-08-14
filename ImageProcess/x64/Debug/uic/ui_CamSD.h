/********************************************************************************
** Form generated from reading UI file 'CamSD.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMSD_H
#define UI_CAMSD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CamSD
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *icon;
    QVBoxLayout *verticalLayout;
    QLabel *sd;
    QProgressBar *progressBar;
    QLabel *usage;

    void setupUi(QWidget *CamSD)
    {
        if (CamSD->objectName().isEmpty())
            CamSD->setObjectName(QStringLiteral("CamSD"));
        CamSD->resize(280, 86);
        horizontalLayout = new QHBoxLayout(CamSD);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        icon = new QLabel(CamSD);
        icon->setObjectName(QStringLiteral("icon"));
        icon->setMinimumSize(QSize(78, 0));
        icon->setStyleSheet(QStringLiteral("border-image: url(\"D:/Download/sd.png\");"));
        icon->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        horizontalLayout->addWidget(icon);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        sd = new QLabel(CamSD);
        sd->setObjectName(QStringLiteral("sd"));

        verticalLayout->addWidget(sd);

        progressBar = new QProgressBar(CamSD);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximumSize(QSize(200, 16777215));
        progressBar->setStyleSheet(QLatin1String("QProgressBar {   border: 2px solid grey;   background-color: #FFFFFF;}\n"
"QProgressBar::chunk {   background-color: #26A0DA;   width: 30px;}\n"
""));
        progressBar->setValue(15);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        verticalLayout->addWidget(progressBar);

        usage = new QLabel(CamSD);
        usage->setObjectName(QStringLiteral("usage"));

        verticalLayout->addWidget(usage);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(CamSD);

        QMetaObject::connectSlotsByName(CamSD);
    } // setupUi

    void retranslateUi(QWidget *CamSD)
    {
        CamSD->setWindowTitle(QApplication::translate("CamSD", "CamSD", Q_NULLPTR));
        icon->setText(QString());
        sd->setText(QApplication::translate("CamSD", "\345\275\261\345\203\217", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        progressBar->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        usage->setText(QApplication::translate("CamSD", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CamSD: public Ui_CamSD {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMSD_H
