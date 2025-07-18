/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *Action_Save;
    QAction *Action_Load;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QLabel *ImageSharpeningLabel;
    QSlider *ImageSharpeningSlider;
    QSpacerItem *verticalSpacer_6;
    QVBoxLayout *verticalLayout_3;
    QLabel *GammaCorrectionLabel;
    QSlider *GammaCorrectionSlider;
    QSpacerItem *verticalSpacer_7;
    QRadioButton *IfGrayscale;
    QSpacerItem *verticalSpacer_8;
    QRadioButton *IfEdgeDetect;
    QSpacerItem *verticalSpacer_4;
    QPushButton *ImageSitchingButton;
    QSpacerItem *verticalSpacer;
    QPushButton *ImageFusionButton;
    QSpacerItem *verticalSpacer_2;
    QPushButton *ImageTextureTransferButton;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_7;
    QLabel *SkinSmoothingLabel;
    QSlider *SkinSmoothingSlider_6;
    QSpacerItem *verticalSpacer_5;
    QVBoxLayout *verticalLayout_8;
    QLabel *FaceSlimming;
    QSlider *FaceSlimmingSlider_7;
    QLabel *label_show;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(610, 396);
        Action_Save = new QAction(MainWindow);
        Action_Save->setObjectName(QString::fromUtf8("Action_Save"));
        Action_Save->setCheckable(true);
        Action_Load = new QAction(MainWindow);
        Action_Load->setObjectName(QString::fromUtf8("Action_Load"));
        Action_Load->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(430, 10, 151, 321));
        verticalLayout_9 = new QVBoxLayout(layoutWidget);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ImageSharpeningLabel = new QLabel(layoutWidget);
        ImageSharpeningLabel->setObjectName(QString::fromUtf8("ImageSharpeningLabel"));

        verticalLayout_2->addWidget(ImageSharpeningLabel);

        ImageSharpeningSlider = new QSlider(layoutWidget);
        ImageSharpeningSlider->setObjectName(QString::fromUtf8("ImageSharpeningSlider"));
        ImageSharpeningSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(ImageSharpeningSlider);


        verticalLayout_9->addLayout(verticalLayout_2);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_6);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        GammaCorrectionLabel = new QLabel(layoutWidget);
        GammaCorrectionLabel->setObjectName(QString::fromUtf8("GammaCorrectionLabel"));

        verticalLayout_3->addWidget(GammaCorrectionLabel);

        GammaCorrectionSlider = new QSlider(layoutWidget);
        GammaCorrectionSlider->setObjectName(QString::fromUtf8("GammaCorrectionSlider"));
        GammaCorrectionSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(GammaCorrectionSlider);


        verticalLayout_9->addLayout(verticalLayout_3);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_7);

        IfGrayscale = new QRadioButton(layoutWidget);
        IfGrayscale->setObjectName(QString::fromUtf8("IfGrayscale"));

        verticalLayout_9->addWidget(IfGrayscale);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_8);

        IfEdgeDetect = new QRadioButton(layoutWidget);
        IfEdgeDetect->setObjectName(QString::fromUtf8("IfEdgeDetect"));

        verticalLayout_9->addWidget(IfEdgeDetect);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_4);

        ImageSitchingButton = new QPushButton(layoutWidget);
        ImageSitchingButton->setObjectName(QString::fromUtf8("ImageSitchingButton"));

        verticalLayout_9->addWidget(ImageSitchingButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);

        ImageFusionButton = new QPushButton(layoutWidget);
        ImageFusionButton->setObjectName(QString::fromUtf8("ImageFusionButton"));

        verticalLayout_9->addWidget(ImageFusionButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_2);

        ImageTextureTransferButton = new QPushButton(layoutWidget);
        ImageTextureTransferButton->setObjectName(QString::fromUtf8("ImageTextureTransferButton"));

        verticalLayout_9->addWidget(ImageTextureTransferButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_3);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        SkinSmoothingLabel = new QLabel(layoutWidget);
        SkinSmoothingLabel->setObjectName(QString::fromUtf8("SkinSmoothingLabel"));

        verticalLayout_7->addWidget(SkinSmoothingLabel);

        SkinSmoothingSlider_6 = new QSlider(layoutWidget);
        SkinSmoothingSlider_6->setObjectName(QString::fromUtf8("SkinSmoothingSlider_6"));
        SkinSmoothingSlider_6->setOrientation(Qt::Horizontal);

        verticalLayout_7->addWidget(SkinSmoothingSlider_6);


        verticalLayout_9->addLayout(verticalLayout_7);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_5);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        FaceSlimming = new QLabel(layoutWidget);
        FaceSlimming->setObjectName(QString::fromUtf8("FaceSlimming"));

        verticalLayout_8->addWidget(FaceSlimming);

        FaceSlimmingSlider_7 = new QSlider(layoutWidget);
        FaceSlimmingSlider_7->setObjectName(QString::fromUtf8("FaceSlimmingSlider_7"));
        FaceSlimmingSlider_7->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(FaceSlimmingSlider_7);


        verticalLayout_9->addLayout(verticalLayout_8);

        label_show = new QLabel(centralWidget);
        label_show->setObjectName(QString::fromUtf8("label_show"));
        label_show->setGeometry(QRect(30, 10, 381, 321));
        label_show->setFrameShape(QFrame::WinPanel);
        label_show->setLineWidth(1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 610, 17));
        menuBar->setDefaultUp(false);
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(Action_Load);
        mainToolBar->addAction(Action_Save);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        Action_Save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\233\276\347\211\207", nullptr));
#ifndef QT_NO_SHORTCUT
        Action_Save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        Action_Load->setText(QApplication::translate("MainWindow", "\345\212\240\350\275\275\345\233\276\347\211\207", nullptr));
#ifndef QT_NO_SHORTCUT
        Action_Load->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        ImageSharpeningLabel->setText(QApplication::translate("MainWindow", "\345\233\276\347\211\207\351\224\220\345\214\226", nullptr));
        GammaCorrectionLabel->setText(QApplication::translate("MainWindow", "\344\274\275\351\251\254\347\237\253\346\255\243", nullptr));
        IfGrayscale->setText(QApplication::translate("MainWindow", "\347\201\260\345\272\246\345\214\226", nullptr));
        IfEdgeDetect->setText(QApplication::translate("MainWindow", "Canny\350\276\271\347\274\230\346\243\200\346\265\213", nullptr));
        ImageSitchingButton->setText(QApplication::translate("MainWindow", "\345\233\276\347\211\207\346\213\274\346\216\245", nullptr));
        ImageFusionButton->setText(QApplication::translate("MainWindow", "\345\233\276\347\211\207\350\236\215\345\220\210", nullptr));
        ImageTextureTransferButton->setText(QApplication::translate("MainWindow", "\347\272\271\347\220\206\350\277\201\347\247\273", nullptr));
        SkinSmoothingLabel->setText(QApplication::translate("MainWindow", "\347\243\250\347\232\256", nullptr));
        FaceSlimming->setText(QApplication::translate("MainWindow", "\347\230\246\350\204\270", nullptr));
        label_show->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
