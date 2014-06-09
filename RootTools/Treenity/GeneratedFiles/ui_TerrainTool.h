/********************************************************************************
** Form generated from reading UI file 'TerrainTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINTOOL_H
#define UI_TERRAINTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainTool
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_strength;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBox_size;
    QLabel *label_6;
    QComboBox *comboBox_brushStyle;
    QPushButton *pushButton_smoothAll;
    QComboBox *comboBox_options;
    QLabel *label_4;
    QCheckBox *checkBox_autosmooth;

    void setupUi(QWidget *TerrainTool)
    {
        if (TerrainTool->objectName().isEmpty())
            TerrainTool->setObjectName(QStringLiteral("TerrainTool"));
        TerrainTool->resize(400, 301);
        gridLayout = new QGridLayout(TerrainTool);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(TerrainTool);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        doubleSpinBox_strength = new QDoubleSpinBox(TerrainTool);
        doubleSpinBox_strength->setObjectName(QStringLiteral("doubleSpinBox_strength"));
        doubleSpinBox_strength->setMinimum(-1000);
        doubleSpinBox_strength->setMaximum(1000);
        doubleSpinBox_strength->setSingleStep(0.5);
        doubleSpinBox_strength->setValue(2);

        gridLayout->addWidget(doubleSpinBox_strength, 1, 1, 1, 1);

        label = new QLabel(TerrainTool);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(TerrainTool);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(90, 16777215));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinBox_size = new QSpinBox(TerrainTool);
        spinBox_size->setObjectName(QStringLiteral("spinBox_size"));
        spinBox_size->setMinimum(-99);
        spinBox_size->setValue(2);

        gridLayout->addWidget(spinBox_size, 0, 1, 1, 1);

        label_6 = new QLabel(TerrainTool);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        comboBox_brushStyle = new QComboBox(TerrainTool);
        comboBox_brushStyle->setObjectName(QStringLiteral("comboBox_brushStyle"));

        gridLayout->addWidget(comboBox_brushStyle, 2, 1, 1, 1);

        pushButton_smoothAll = new QPushButton(TerrainTool);
        pushButton_smoothAll->setObjectName(QStringLiteral("pushButton_smoothAll"));

        gridLayout->addWidget(pushButton_smoothAll, 5, 0, 1, 2);

        comboBox_options = new QComboBox(TerrainTool);
        comboBox_options->setObjectName(QStringLiteral("comboBox_options"));

        gridLayout->addWidget(comboBox_options, 3, 1, 1, 1);

        label_4 = new QLabel(TerrainTool);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        checkBox_autosmooth = new QCheckBox(TerrainTool);
        checkBox_autosmooth->setObjectName(QStringLiteral("checkBox_autosmooth"));

        gridLayout->addWidget(checkBox_autosmooth, 4, 1, 1, 1);


        retranslateUi(TerrainTool);

        QMetaObject::connectSlotsByName(TerrainTool);
    } // setupUi

    void retranslateUi(QWidget *TerrainTool)
    {
        TerrainTool->setWindowTitle(QApplication::translate("TerrainTool", "Form", 0));
        label_3->setText(QApplication::translate("TerrainTool", "Brush options:", 0));
        label->setText(QApplication::translate("TerrainTool", "Brush size:", 0));
        label_2->setText(QApplication::translate("TerrainTool", "Brush strength:", 0));
        label_6->setText(QApplication::translate("TerrainTool", "Brush style:", 0));
        comboBox_brushStyle->clear();
        comboBox_brushStyle->insertItems(0, QStringList()
         << QApplication::translate("TerrainTool", "Ring", 0)
         << QApplication::translate("TerrainTool", "Wireframe", 0)
        );
        pushButton_smoothAll->setText(QApplication::translate("TerrainTool", "Smooth all", 0));
        comboBox_options->clear();
        comboBox_options->insertItems(0, QStringList()
         << QApplication::translate("TerrainTool", "Default", 0)
         << QApplication::translate("TerrainTool", "Flat", 0)
         << QApplication::translate("TerrainTool", "Plateau", 0)
         << QApplication::translate("TerrainTool", "Smooth only", 0)
        );
        label_4->setText(QApplication::translate("TerrainTool", "Smooth:", 0));
        checkBox_autosmooth->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TerrainTool: public Ui_TerrainTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINTOOL_H
