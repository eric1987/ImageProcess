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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CamSD
{
public:
    QLabel *label;

    void setupUi(QWidget *CamSD)
    {
        if (CamSD->objectName().isEmpty())
            CamSD->setObjectName(QStringLiteral("CamSD"));
        CamSD->resize(400, 213);
        label = new QLabel(CamSD);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 40, 54, 12));

        retranslateUi(CamSD);

        QMetaObject::connectSlotsByName(CamSD);
    } // setupUi

    void retranslateUi(QWidget *CamSD)
    {
        CamSD->setWindowTitle(QApplication::translate("CamSD", "CamSD", Q_NULLPTR));
        label->setText(QApplication::translate("CamSD", "\345\275\261\345\203\217SD", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CamSD: public Ui_CamSD {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMSD_H
