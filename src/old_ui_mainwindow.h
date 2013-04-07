/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat 28. Jan 01:09:18 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *errer;
    QVBoxLayout *verticalLayout_6;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTextEdit *input;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *startButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *startButton_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_11;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QTextEdit *output;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_8;
    QSplitter *splitter_2;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_4;
    QLineEdit *fname;
    QPushButton *pushButton_7;
    QPushButton *pushButton_6;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *image_t_test;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout_5;
    QTextEdit *transformScript;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer_5;
    QWidget *tab;
    QComboBox *comboBox_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *image_orig;
    QLabel *image_r;
    QLabel *image_y;
    QLabel *image_res;
    QLabel *image_g;
    QLabel *image_b;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QPushButton *pushButton_10;
    QPushButton *custom;
    QWidget *tab_3;
    QLabel *map;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QLabel *original;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1111, 647);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setStyleSheet(QString::fromUtf8("background: none;"));
        errer = new QWidget();
        errer->setObjectName(QString::fromUtf8("errer"));
        verticalLayout_6 = new QVBoxLayout(errer);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        splitter = new QSplitter(errer);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        input = new QTextEdit(layoutWidget);
        input->setObjectName(QString::fromUtf8("input"));
        input->setStyleSheet(QString::fromUtf8("background: url(\":images/binary.png\");"));

        verticalLayout->addWidget(input);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget1);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 6, -1, -1);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        startButton = new QPushButton(layoutWidget1);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        horizontalLayout_2->addWidget(startButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        startButton_2 = new QPushButton(layoutWidget1);
        startButton_2->setObjectName(QString::fromUtf8("startButton_2"));

        horizontalLayout_2->addWidget(startButton_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_11 = new QPushButton(layoutWidget1);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));

        horizontalLayout_2->addWidget(pushButton_11);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        output = new QTextEdit(layoutWidget1);
        output->setObjectName(QString::fromUtf8("output"));
        output->setStyleSheet(QString::fromUtf8("background: url(\":images/binary.png\");"));

        verticalLayout_2->addWidget(output);


        verticalLayout_3->addLayout(verticalLayout_2);

        splitter->addWidget(layoutWidget1);

        verticalLayout_6->addWidget(splitter);

        tabWidget->addTab(errer, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_8 = new QVBoxLayout(tab_2);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        splitter_2 = new QSplitter(tab_2);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        layoutWidget2 = new QWidget(splitter_2);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        verticalLayout_7 = new QVBoxLayout(layoutWidget2);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_4 = new QPushButton(layoutWidget2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(23, 0));
        pushButton_4->setMaximumSize(QSize(23, 16777215));

        horizontalLayout_3->addWidget(pushButton_4);

        fname = new QLineEdit(layoutWidget2);
        fname->setObjectName(QString::fromUtf8("fname"));

        horizontalLayout_3->addWidget(fname);

        pushButton_7 = new QPushButton(layoutWidget2);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

        horizontalLayout_3->addWidget(pushButton_7);

        pushButton_6 = new QPushButton(layoutWidget2);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        horizontalLayout_3->addWidget(pushButton_6);


        verticalLayout_7->addLayout(horizontalLayout_3);

        scrollArea_2 = new QScrollArea(layoutWidget2);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setStyleSheet(QString::fromUtf8("padding: 0;"));
        scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea_2->setWidgetResizable(true);
        scrollArea_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 1048, 512));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        image_t_test = new QLabel(scrollAreaWidgetContents_2);
        image_t_test->setObjectName(QString::fromUtf8("image_t_test"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(image_t_test->sizePolicy().hasHeightForWidth());
        image_t_test->setSizePolicy(sizePolicy1);
        image_t_test->setMinimumSize(QSize(512, 512));
        image_t_test->setStyleSheet(QString::fromUtf8("background: url(\":images/binary.png\");"));
        image_t_test->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(image_t_test);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_7->addWidget(scrollArea_2);

        splitter_2->addWidget(layoutWidget2);
        layoutWidget3 = new QWidget(splitter_2);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        transformScript = new QTextEdit(layoutWidget3);
        transformScript->setObjectName(QString::fromUtf8("transformScript"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(transformScript->sizePolicy().hasHeightForWidth());
        transformScript->setSizePolicy(sizePolicy2);

        verticalLayout_5->addWidget(transformScript);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        pushButton_5 = new QPushButton(layoutWidget3);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(pushButton_5);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout_5->addLayout(horizontalLayout_5);

        splitter_2->addWidget(layoutWidget3);

        verticalLayout_8->addWidget(splitter_2);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        comboBox_2 = new QComboBox(tab);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(240, 40, 201, 22));
        comboBox_2->setStyleSheet(QString::fromUtf8("background-color: #CCCCCC;"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 75, 23));
        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(240, 0, 75, 23));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 120, 551, 541));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 532, 3120));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        image_orig = new QLabel(scrollAreaWidgetContents);
        image_orig->setObjectName(QString::fromUtf8("image_orig"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(image_orig->sizePolicy().hasHeightForWidth());
        image_orig->setSizePolicy(sizePolicy3);
        image_orig->setMinimumSize(QSize(512, 512));
        image_orig->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255);\n"
"border: 2px solid;"));

        gridLayout->addWidget(image_orig, 0, 0, 1, 1);

        image_r = new QLabel(scrollAreaWidgetContents);
        image_r->setObjectName(QString::fromUtf8("image_r"));
        sizePolicy3.setHeightForWidth(image_r->sizePolicy().hasHeightForWidth());
        image_r->setSizePolicy(sizePolicy3);
        image_r->setMinimumSize(QSize(512, 512));
        image_r->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));

        gridLayout->addWidget(image_r, 3, 0, 1, 1);

        image_y = new QLabel(scrollAreaWidgetContents);
        image_y->setObjectName(QString::fromUtf8("image_y"));
        sizePolicy3.setHeightForWidth(image_y->sizePolicy().hasHeightForWidth());
        image_y->setSizePolicy(sizePolicy3);
        image_y->setMinimumSize(QSize(512, 512));
        image_y->setMaximumSize(QSize(512, 512));
        image_y->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));

        gridLayout->addWidget(image_y, 2, 0, 1, 1);

        image_res = new QLabel(scrollAreaWidgetContents);
        image_res->setObjectName(QString::fromUtf8("image_res"));
        sizePolicy3.setHeightForWidth(image_res->sizePolicy().hasHeightForWidth());
        image_res->setSizePolicy(sizePolicy3);
        image_res->setMinimumSize(QSize(512, 512));
        image_res->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));

        gridLayout->addWidget(image_res, 1, 0, 1, 1);

        image_g = new QLabel(scrollAreaWidgetContents);
        image_g->setObjectName(QString::fromUtf8("image_g"));
        sizePolicy3.setHeightForWidth(image_g->sizePolicy().hasHeightForWidth());
        image_g->setSizePolicy(sizePolicy3);
        image_g->setMinimumSize(QSize(512, 512));
        image_g->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));

        gridLayout->addWidget(image_g, 5, 0, 1, 1);

        image_b = new QLabel(scrollAreaWidgetContents);
        image_b->setObjectName(QString::fromUtf8("image_b"));
        sizePolicy3.setHeightForWidth(image_b->sizePolicy().hasHeightForWidth());
        image_b->setSizePolicy(sizePolicy3);
        image_b->setMinimumSize(QSize(512, 512));
        image_b->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));

        gridLayout->addWidget(image_b, 4, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);
        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 0, 75, 23));
        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(0, 40, 201, 22));
        pushButton_10 = new QPushButton(tab);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setGeometry(QRect(0, 80, 75, 23));
        custom = new QPushButton(tab);
        custom->setObjectName(QString::fromUtf8("custom"));
        custom->setGeometry(QRect(470, 80, 75, 23));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        map = new QLabel(tab_3);
        map->setObjectName(QString::fromUtf8("map"));
        map->setGeometry(QRect(70, 40, 512, 512));
        sizePolicy3.setHeightForWidth(map->sizePolicy().hasHeightForWidth());
        map->setSizePolicy(sizePolicy3);
        map->setMinimumSize(QSize(512, 512));
        map->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));
        pushButton_8 = new QPushButton(tab_3);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(70, 10, 75, 23));
        pushButton_8->setStyleSheet(QString::fromUtf8(""));
        pushButton_9 = new QPushButton(tab_3);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(170, 10, 75, 23));
        original = new QLabel(tab_3);
        original->setObjectName(QString::fromUtf8("original"));
        original->setGeometry(QRect(70, 40, 512, 512));
        sizePolicy3.setHeightForWidth(original->sizePolicy().hasHeightForWidth());
        original->setSizePolicy(sizePolicy3);
        original->setMinimumSize(QSize(512, 512));
        original->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255)"));
        tabWidget->addTab(tab_3, QString());

        horizontalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_4, SIGNAL(clicked()), MainWindow, SLOT(openFileDlg()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), MainWindow, SLOT(doTransform()));
        QObject::connect(pushButton_6, SIGNAL(clicked()), MainWindow, SLOT(openSaveDlg()));
        QObject::connect(pushButton_7, SIGNAL(clicked()), MainWindow, SLOT(loadImage()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), MainWindow, SLOT(doTest()));
        QObject::connect(pushButton, SIGNAL(clicked()), MainWindow, SLOT(hideInfo()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), MainWindow, SLOT(restoreInfo()));
        QObject::connect(comboBox_2, SIGNAL(currentIndexChanged(int)), MainWindow, SLOT(changeTest(int)));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), MainWindow, SLOT(changeAlgo(int)));
        QObject::connect(pushButton_8, SIGNAL(clicked()), MainWindow, SLOT(custom()));
        QObject::connect(pushButton_9, SIGNAL(clicked()), MainWindow, SLOT(toggle_view()));
        QObject::connect(pushButton_10, SIGNAL(clicked()), MainWindow, SLOT(distortions()));
        QObject::connect(custom, SIGNAL(clicked()), MainWindow, SLOT(custom_action()));
        QObject::connect(startButton, SIGNAL(clicked()), MainWindow, SLOT(perform_tests()));
        QObject::connect(startButton_2, SIGNAL(clicked()), MainWindow, SLOT(afterTest()));
        QObject::connect(pushButton_11, SIGNAL(clicked()), MainWindow, SLOT(changeContainer()));
        QObject::connect(pushButton_11, SIGNAL(pressed()), MainWindow, SLOT(showPicture()));
        QObject::connect(pushButton_11, SIGNAL(released()), MainWindow, SLOT(hidePicture()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\241\321\202\320\265\320\263\320\276\321\202\320\265\321\201\321\202 v 1.0", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\320\222\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        input->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("MainWindow", "\320\227\320\220\320\237\320\243\320\241\320\232", 0, QApplication::UnicodeUTF8));
        startButton_2->setText(QApplication::translate("MainWindow", "\320\241\320\242\320\236\320\237", 0, QApplication::UnicodeUTF8));
        pushButton_11->setText(QApplication::translate("MainWindow", "\320\232\320\260\321\200\321\202\320\270\320\275\320\272\320\260", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213", 0, QApplication::UnicodeUTF8));
        output->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(errer), QApplication::translate("MainWindow", "\320\242\320\265\321\201\321\202\321\213", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        fname->setText(QString());
        pushButton_7->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        image_t_test->setText(QString());
        pushButton_5->setText(QApplication::translate("MainWindow", "\320\236\320\232", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\320\242\321\200\320\260\320\275\321\201\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "JPEG", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Noise", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Median", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Sharpen", 0, QApplication::UnicodeUTF8)
        );
        pushButton->setText(QApplication::translate("MainWindow", "Hide", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8));
        image_orig->setText(QString());
        image_r->setText(QString());
        image_y->setText(QString());
        image_res->setText(QString());
        image_g->setText(QString());
        image_b->setText(QString());
        pushButton_2->setText(QApplication::translate("MainWindow", "Restore", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "LSB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "\320\237\321\201\320\265\320\262\320\264\320\276\321\201\320\273\321\203\321\207\320\260\320\271\320\275\321\213\320\271 \320\270\320\275\321\202\320\265\321\200\320\262\320\260\320\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Kutter", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Koh", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Bengam", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Fridrich", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Fridrich2", 0, QApplication::UnicodeUTF8)
        );
        pushButton_10->setText(QApplication::translate("MainWindow", "Changes", 0, QApplication::UnicodeUTF8));
        custom->setText(QApplication::translate("MainWindow", "Custom", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\242\320\265\321\201\321\202\321\2132", 0, QApplication::UnicodeUTF8));
        map->setText(QString());
        pushButton_8->setText(QApplication::translate("MainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("MainWindow", "\320\236\321\200\320\270\320\263\320\270\320\275\320\260\320\273", 0, QApplication::UnicodeUTF8));
        original->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\320\237\320\270\320\272\321\201\320\265\320\273\320\270", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
