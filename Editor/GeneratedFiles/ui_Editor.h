/********************************************************************************
** Form generated from reading UI file 'Editor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorClass)
    {
        if (EditorClass->objectName().isEmpty())
            EditorClass->setObjectName(QStringLiteral("EditorClass"));
        EditorClass->resize(600, 400);
        menuBar = new QMenuBar(EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EditorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(EditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        EditorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorClass->setStatusBar(statusBar);

        retranslateUi(EditorClass);

        QMetaObject::connectSlotsByName(EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorClass)
    {
        EditorClass->setWindowTitle(QApplication::translate("EditorClass", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class EditorClass: public Ui_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
