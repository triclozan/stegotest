﻿#include <math.h>
#include <vector>
#include <QImage>
#include <QFile>
#include <QBitArray>
#include <QFileDialog>
#include <QString>
#include <QKeyEvent>
#include <QPainter>
#include <QFile>
#include <QUrl>
#include <QThread>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTextCursor>
#include <QMessageBox>
#include "calglsb.h"
#include "calgcombine.h"
#include "calgpri.h"
#include "calgkoh.h"
#include "calgkohy.h"
#include "calgkutter.h"
#include "calglangelaar.h"
#include "calgbengam.h"
#include "calgfridrich.h"
#include "calgfridrich2.h"
#include "calgconvolution.h"
#include "calgwalsh.h"
#include "calgpolynom.h"
#include "calglegendre.h"
#include "cimagehistory.h"
#include "cintegrator.h"
#include "ctestjpeg.h"
#include "ctesttransform.h"
#include "ctestmedian.h"
#include "ctestnoise.h"
#include "ctestdistortion.h"
#include "ctestkoh.h"
#include "ctransform.h"
#include "dct.h"
#include "RNG/stocc.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

void Thread::parse(QString& data, int depth)
{
    QStringList commands = data.split("\n");    //Newline character is a command separator
    for (int i=0; i<commands.size(); i++) {     //Process commands one by one
        int j = 0;
        int count = 0;

        if (depth == 0) {                                               //Output the command if we haven't entered the recursion yet
            emit aSignal("-------<b>" + commands[i] + "</b>-------");
        }

        while ((j = commands[i].indexOf("$", j)) != -1)  {              //Read a cycle if presented
            ++j;
            ++count;
            if (count > 3) {
                break;
            }
        }

        if (j != -1) {                                                  //Expand the cycle into series of commands $start$bound$increment$
            QStringList test_params_list = commands[i].left(j).split("$");
            double start = test_params_list[1].toDouble();
            double increment = test_params_list[3].toDouble();
            double bound = test_params_list[2].toDouble();
            test_params_list.removeAt(2);
            test_params_list.removeAt(2);
            for (double m=start; (start < bound) ? (m <= bound) : (m >= bound); m+=increment) {
                test_params_list[1] = QString::number(m);
                QString str = test_params_list.join("") + commands[i].mid(j);
                //qDebug() << str;
                //return;
                parse(str, depth+1);
            }
            //emit aSignal("</table>");
            emit aSignal("--------------------------------");
            continue;
        }

        while (commands[i].indexOf("%") >= 0) {                         //Inline replacements of %name%val% form
            QStringList tmp_list = commands[i].split("%");
            QString name = tmp_list[1];
            QString value = tmp_list[2];
            commands[i].replace("%" + name + "%" + value + "%", value);
            commands[i].replace("%" + name + "%", value);
        }

        CAlgorithm* alg = 0;
        CTest* test = 0;
        int sep = commands[i].indexOf("->");                            //Separator between algorithm params and action params

        if (sep < 0) {                                                  //Commands with no separator are skipped
            continue;
        }

        QString alg_data = commands[i].left(sep).trimmed();
        QString test_data = commands[i].mid(sep+2).trimmed();
        QString alg_params, alg_name;
        QString test_name, test_params;

        if (alg_data.indexOf("?") < 0) {                                //Parse algorithm name and params
            alg_name = alg_data.trimmed();
            alg_params = "";
        }
        else {
            alg_name = alg_data.left(alg_data.indexOf("?")).trimmed();
            alg_params = alg_data.mid(alg_data.indexOf("?") + 1).trimmed();
        }

        if (test_data.indexOf("?") < 0) {                               //Parse action name and params
            test_name = test_data.trimmed();
            test_params = "";
        }
        else {
            test_name = test_data.left(test_data.indexOf("?")).trimmed();
            test_params = test_data.mid(test_data.indexOf("?") + 1).trimmed();
        }

        alg_name = alg_name.toLower();                                  //Names are case insensitive
        test_name = test_name.toLower();

        QStringList columns;
        QStringList col_value = alg_params.split("@");                  //Collect output values from algo part
        for (int i=0; 2*i+1<col_value.size(); i++) {
            columns.push_back(col_value[2*i+1]);
        }
        alg_params = alg_params.remove("@");

        col_value = test_params.split("@");                             //Collect output values from action part
        for (int i=0; 2*i+1<col_value.size(); i++) {
            columns.push_back(col_value[2*i+1]);
        }
        test_params = test_params.remove("@");

        QByteArray p, p2;
        QString out;
        QTextStream out_stream(&out);

        if (alg_name == "koh") {                                        //Instantiate an algorithm object depending on the name
            alg = new CAlgKoh;
        } else if (alg_name == "fridrich") {
            alg = new CAlgFridrich;
        } else if (alg_name == "fridrich2") {
            alg = new CAlgFridrich2;
        } else if (alg_name == "convolution") {
            alg = new CAlgConvolution;
        } else if (alg_name == "polynom") {
            alg = new CAlgPolynom;
        } else if (alg_name == "legendre") {
            alg = new CAlgLegendre;
        } else if (alg_name == "langelaar") {
            alg = new CAlgLangelaar;
        } else if (alg_name == "kutter") {
            alg = new CAlgKutter;
        } else if (alg_name == "walsh") {
            alg = new CAlgWalsh;
        } else if (alg_name == "combine") {
            alg = new CAlgCombine;
        }

        if (test_name == "jpeg") {                                      //Instantiate an action object depending on the name, run the action
            test = new CTestJpeg;
        } else if (test_name == "transform") {
            test = new CTestTransform;
        }

        if (test_name == "distortion") {                                //Measure distortion introduced by watermark
            test = new CTestDistortion;
            QByteArray ba;
            dynamic_cast<CTestDistortion*>(test)->Test(alg, test_params, alg_params, ba, container_name, "..\\stego\\test\\in.txt");
            CImgCompare::Result res = DESERIALIZE(ba,CImgCompare::Result);
            out_stream << "<tr><td>" + columns.join("</td><td>") + "</td><td>" << res.psnr << "</td></tr>";
            /*out_stream << "md: " << res.md << "\n";
            out_stream << "ad: " << res.ad << "\n";
            out_stream << "nad: " << res.nad << "\n";
            out_stream << "mse: " << res.mse << "\n";
            out_stream << "nmse: " << res.nmse << "\n";
            out_stream << "l2: " << res.l2 << "\n";
            out_stream << "snr: " << res.snr << "\n";
            out_stream << "psnr: " << res.psnr << "\n";
            out_stream << "image fidelity: " << res.ifid << "\n";
            out_stream << "hs: " << res.hs << "\n";
            out_stream << "sc: " << res.sc << "\n";
            out_stream << "cq: " << res.cq << "\n";
            out_stream << "nc: " << res.nc;*/
            emit aSignal(out.replace(".", ","));
        }
        else if (test_name == "view") {                                                             //Embed WM to file... same as save?
            test = new CTest;
            test->Visual(alg, test_params, alg_params, container_name, 0, 512*512/(64*8));
        }
        else if (test_name == "picture") {                                                          //Embed and extract WM as 1-bit picture
            test = new CTest;
            test->Picture(alg, test_params, alg_params, container_name, 0);
        }
        else if (test_name == "save") {                                                             //Save WM to file
            test = new CTest;
            test->Save(alg, test_params, alg_params, container_name);
        }
        else if (test_name == "check") {                                                            //Check presence of WM in file
            test = new CTest;
            double res = test->Check(alg, test_params, alg_params, container_name);
            emit aSignal(QString::number(res));
        }
        else {                                                                                      //For jpeg and transform actions
            QByteArray ba;
            test->Test(alg, test_params, alg_params, ba, container_name, "../stego/test/in.txt");
            CTest::sResults res = DESERIALIZE(ba,CTest::sResults);
            qDebug() << alg_params << " " << test_params << " " << res.BER;
            //out_stream << "<tr><td>" + columns.join("</td><td>") + "</td><td>" << res.BER << "</td></tr>";//alg_params << " " << test_params << " " << res.BER;
            out_stream << columns.join(" ") << " " << res.BER;
            emit aSignal(out.replace(".", ","));
        }

        if (depth == 0) {                                                   //If we have not entered the recursion yet, print command separator
            emit aSignal("--------------------------------");
        }

        delete alg;                                                         //Release the used memory
        delete test;
    }
}

void Thread::run()
{
    parse(data);
    emit done();
}

Thread::Thread(QTextEdit* ptr, QString& str, QString container_name)
{
    output = ptr;
    data = str;
    this->container_name = container_name;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thread(0),
    settings("./settings.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    lab = 0;
    alg = new CAlgLSB;
    test = new CTestJpeg;
    AlgId = TestId = 0;
    backup = new CImageHistory(10);                                                 //Preserve 10 last transforms

    //-------------------------------------Init menu---------------------------------------
    QString str = "Save";
    QAction* save = new QAction(str, this);
    save->setShortcuts(QKeySequence::Save);
    connect(save, SIGNAL(triggered()), this, SLOT(saveScript()));

    str = "Save As";
    QAction* save_as = new QAction(str, this);
    save_as->setShortcuts(QKeySequence::SaveAs);
    connect(save_as, SIGNAL(triggered()), this, SLOT(saveAsScript()));

    str = "Load";
    QAction* load = new QAction(str, this);
    load->setShortcuts(QKeySequence::Open);
    connect(load, SIGNAL(triggered()), this, SLOT(loadScript()));

    str = "Fade comments";
    QAction* comment = new QAction(str, 0);
    comment->setChecked(false);
    comment->setCheckable(true);
    connect(comment, SIGNAL(triggered(bool)), this, SLOT(shadowComments(bool)));

    menu_input = ui->input->createStandardContextMenu();
    menu_input->addSeparator();
    menu_input->addAction(load);
    menu_input->addAction(save);
    menu_input->addAction(save_as);
    menu_input->addAction(comment);

    str = "Clear on start";
    QAction* clear = new QAction(str, 0);
    clear->setChecked(false);
    clear->setCheckable(true);
    connect(clear, SIGNAL(triggered(bool)), this, SLOT(setOutputClear(bool)));

    menu_output = ui->output->createStandardContextMenu();
    menu_output->addSeparator();
    menu_output->addAction(clear);

    connect(ui->input, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    ui->input->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->output, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(outputContextMenu(const QPoint&)));
    ui->output->setContextMenuPolicy(Qt::CustomContextMenu);
    clear_output = shadow_comment = false;
    //------------------------------------/Init menu---------------------------------------

    loadScript("custom.ini");                                           //Load sample script
    ui->image_t_test->setGeometry(0, 0, 900, 600);                      //Set image transform area size

    StochasticLib1 rng(100500);                                         //Initialize random number generator

    container = settings.value("defaultContainer").toString();          //Take default test picture from settings

    ui->tabWidget->removeTab(2);                                        //FIXME: hides two unused tabs
    ui->tabWidget->removeTab(2);

    lab = new QLabel(0);                                                //Initialize the label displaying current test picture

    test_image_loaded = transform_image_loaded = false;

    QFile f(container);                                                 //Test if the test image was loaded
    if (f.open(QIODevice::ReadOnly)) {
        test_image_loaded = true;
    }
    f.close();
}

//Displays the current test picture
void MainWindow::showPicture()
{
    if (lab->isVisible()) {                                                         //Hide test picture if it's already shown
        lab->hide();
        return;
    }
    if (!test_image_loaded) {                                                       //Return if text picture is not loaded
        return;
    }
    QPixmap p = QPixmap(container).scaled(QSize(256,256), Qt::KeepAspectRatio);
    lab->setPixmap(p);
    lab->setGeometry(this->geometry().x(), this->geometry().y(), 256, 256);
    lab->setWindowFlags(Qt::FramelessWindowHint);
    lab->show();
    ui->pushButton_11->setDown(true);
    this->setFocus();
    ui->pushButton_11->setChecked(true);
}

void MainWindow::hidePicture()
{
    qDebug() << "HIDE";
    lab->hide();
}

//Main tab: show context menu for input textarea
void MainWindow::showContextMenu(const QPoint &pt)
{
    menu_input->exec(ui->input->mapToGlobal(pt));
}

//Main tab: show context menu for output textarea
void MainWindow::outputContextMenu(const QPoint &pt)
{
    menu_output->exec(ui->output->mapToGlobal(pt));
}

//Main tab: setter for "Clear on start" option
void MainWindow::setOutputClear(bool b)
{
    clear_output = b;
}

//Main tab: change test container using file dialog
void MainWindow::changeContainer()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (!dialog.exec()) {
        return;
    }
    container = dialog.selectedFiles().at(0);
    test_image_loaded = true;
    lab->hide();
}

//Save script to an arbitrary file
void MainWindow::saveAsScript()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec()) {
        script_fname = dialog.selectedFiles().at(0);
        QFile out(dialog.selectedFiles().at(0));
        out.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream t(&out);
        t << ui->input->toPlainText();
        out.close();
    }
}

//Save script to the file from which it was loaded
void MainWindow::saveScript()
{
    QFile out(script_fname);
    out.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream t(&out);
    t << ui->input->toPlainText();
    out.close();
}

//Loads script from file
void MainWindow::loadScript(QString fname)
{
    if (fname == "") {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (!dialog.exec()) {
            return;
        }
        fname = dialog.selectedFiles().at(0);
    }

    script_fname = fname;
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    ui->input->clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->input->append(line);
    }

    file.close();
}

//Destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete menu_input;
    delete menu_output;
}

//Hide test picture on window close
void MainWindow::closeEvent(QCloseEvent *event) {
    lab->hide();
}

//Transformation tab: Opens file choose dialog and outputs its chosen file path into textbo
void MainWindow::openFileDlg()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec()) {
        ui->fname->setText(dialog.selectedFiles().at(0));
    }
}

//Transformation tab: Saves current image into a file
void MainWindow::openSaveDlg()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec()) {
        ui->image_t_test->pixmap()->toImage().save(dialog.selectedFiles().at(0));
    }
}

//Performs image transform on Transformation tab
void MainWindow::doTransform()
{
    if (!transform_image_loaded) {
        QMessageBox::warning(0, "Предупреждение", "Сначала загрузите тестовое изображение!");
        return;
    }
    QString tmp = ui->transformScript->toPlainText();
    QTextStream in(&tmp);
    CTransform t;
    QImage orig = backup->Current();                                    //Take current state from the top of the stack
    QImage out = orig;
    while (!in.atEnd()) {                                               //Read and run input script line by line excluding comments and empty lines
        QString line = in.readLine();
        if (line.trimmed().size() == 0 || line.trimmed()[0] == '#') {
            continue;
        }
        t.performTransform(orig, out, line);
        orig = out;
    }
    ui->image_t_test->setPixmap(QPixmap::fromImage(out));
    backup->Add(out);                                                   //Save current state in the stack
}

//Loads new test image on Transform tab from file
void MainWindow::loadImage()
{
    QFile file(ui->fname->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    file.close();
    ui->image_t_test->setPixmap(QPixmap(ui->fname->text()));
    QImage in = ui->image_t_test->pixmap()->toImage();
    backup->Add(in);
    transform_image_loaded = true;
}

//Keyboard events handler
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case (Qt::Key_Z):                                                               //Undo and Redo for Transform tab
        if (event->modifiers() & Qt::ControlModifier) {
            if (event->modifiers() & Qt::ShiftModifier) {
                ui->image_t_test->setPixmap(QPixmap::fromImage(backup->Redo()));
            }
            else {
                ui->image_t_test->setPixmap(QPixmap::fromImage(backup->Undo()));
            }
        }
        break;
    case (Qt::Key_Return):                                                          //Hotkey for applying the changes on Transform tab
        if (event->modifiers() & Qt::ControlModifier) {
            doTransform();
        }
        break;
    case (Qt::Key_1):                                                               //??? some rotation probe
        QTransform transform, transform2;
        transform.scale(0.8, 0.8);
        ui->image_t_test->setPixmap(ui->image_t_test->pixmap()->transformed(transform, Qt::SmoothTransformation));
        transform2.scale(1/0.8, 1/0.8);
        ui->image_t_test->setPixmap(ui->image_t_test->pixmap()->transformed(transform2, Qt::SmoothTransformation));
        break;
    }
}

//Logging to the output field in html format
void MainWindow::log(QString data)
{
    if (data.left(6) == "<table") {             //Table is buffered to insert multipart html data
        log_buffer.append(data);
        start_buffer = true;
    }
    else if (data.left(7) == "</table") {
        log_buffer.append(data);
        start_buffer = false;
        ui->output->append(log_buffer);
        log_buffer = "";
    }
    else if (start_buffer) {
        log_buffer.append(data);
    }
    else {
        ui->output->append(data);
    }
}

//Start button click action: perform calculations in a separate thread
void MainWindow::perform_tests()
{
    if (!test_image_loaded) {                                               //Check it the test image is loaded
        QMessageBox::warning(0, "Предупреждение", "Сначала загрузите тестовое изображение!");
        return;
    }

    if (clear_output) {                                                     //Clear the output if the corresponding option is active
        ui->output->clear();
    }

    QString data;
    QString tmp = ui->input->toPlainText();                                 //Read input into temprorary variable
    QTextStream in(&tmp);
    while (!in.atEnd()) {                                                   //Skip empty and commented lines
        QString line = in.readLine();
        if (line.trimmed().size() == 0 || line.trimmed()[0] == '#') {
            continue;
        }
        data.append(line + "\n");
        qDebug() << line.size();
        //ui->input->appendPlainText(line);
    }
    data.remove(data.size()-1, 1);                                          //Remove newline character in the end of the input

    start_buffer = false;
    thread = new Thread(ui->output, data, container);                       //Create a new thread that performs the calculations
    QObject::connect(thread, SIGNAL(aSignal(QString)), this, SLOT(log(QString)));
    QObject::connect(thread, SIGNAL(done()), this, SLOT(afterTest()));      //Calculation end results in the same action as stop button click
    ui->startButton->setEnabled(false);                                     //Disable start button while the calculcation runs
    thread->start();                                                        //Start the calculation in the new thread
}

//Context menu action: Fade out commented lines of the script
void MainWindow::shadowComments(bool b)
{
    shadow_comment = b;
    QString tmp = ui->input->toPlainText();
    QTextStream in(&tmp);
    QString res;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (b && line.trimmed()[0] == '#') {
            res.append("<font color=\"#CCCCCC\">" + line + "</font><br/>");
        }
        else {
            res.append("<font color=\"#000000\">" + line + "</font><br/>");
        }
    }

    res.remove(res.size()-5, 5);
    QTextCursor cur = ui->input->textCursor();
    int pos = cur.position();
    ui->input->setHtml(res);
    cur.setPosition(pos);
    ui->input->setTextCursor(cur);
}

//Stop button click action: delete the thread and enable start button after finishing the script
void MainWindow::afterTest()
{
    if (thread) {
        thread->terminate();
        ui->startButton->setEnabled(true);
        delete thread;
        thread = 0;
    }
}

//*******************************************************************UNUSED**************************************************************
//UNUSED
void MainWindow::distortions()
{
    int i=1,j=25;
    QString res;
    CTestDistortion t;
    QByteArray p = QByteArray((char*)&i, sizeof(i));
    QByteArray p2 = QByteArray((char*)&j, sizeof(j));
}

//UNUSED
void MainWindow::custom_action()
{
    CAlgorithm* alg = new CAlgKoh;
    int tresh = 100;
    QByteArray result;
    CTest::sResults res;
    CTest* test = new CTestTransform;
    for (int i = 1; i <= 5; i+=1) {
                QString tset =  /*QString::number(tINoise) + ":" + QString::number(1) + ";" +
                QString::number(tAdMedFilter) + ":" + QString::number(1) + ";" +*/
                //QString::number(tSharpen) + ":" + QString::number(i * 0.01);
                /*QString::number(tINoise) + ":" + QString::number(i) + ";" +
                QString::number(tAdMedFilter) + ":" + QString::number(1);*/
                QString::number(tAMFilter) + ":" + QString::number(i);
                QString p = tset;
         //     QByteArray p = QByteArray((char*)&i, sizeof(i));
                QString p2 = "tresh=" + QString::number(tresh);
                test->Test(alg, p, p2, result, settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");
                res = DESERIALIZE(result,CTest::sResults);
                qDebug() /*<< i << " "*/ << res.BER;
    }




    CTestDistortion d;
    QByteArray p = QByteArray();
    QByteArray p2 = QByteArray((char*)&tresh, sizeof(tresh));
    QString str;
    //d.Test(alg, p, p2, str, settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");
}

//UNUSED
void MainWindow::changeTest(int index) {
    if (TestId != index) {
        TestId = index;
        //qDebug() << "AAA";
        delete test;
        //CTestTransform* ttest;
        //CTestTransform::sTransform t;
        //CTestTransform::shParams sh;
        switch(index) {
        case 0:
            test = new CTestJpeg;
            break;
        case 1:
            test = new CTestNoise;
            break;
        case 2:
            test = new CTestMedian;
            break;
        case 3:
            test = new CTestTransform;
            break;
        }
    }

}

//UNUSED
void MainWindow::changeAlgo(int index) {
    if (AlgId != index) {
        AlgId = index;
        delete alg;
        switch(index) {
        case 0:
            alg = new CAlgLSB;
            break;
        case 1:
            alg = new CAlgPRI;
            break;
        case 2:
            alg = new CAlgKutter;
            break;
        case 3:
            alg = new CAlgKohY;
            break;
        case 4:
            alg = new CAlgBengam;
            break;
        case 5:
            alg = new CAlgFridrich;
            break;
        case 6:
            alg = new CAlgFridrich2;
            break;
        }
    }

}

//UNUSED
void MainWindow::doTest()
{
    //qDebug() << TestId;
    //int params = 70;
    //CTest::sResults res;
    QString tset;
    switch(TestId) {
    case 0:
        /*int i = 100, j = 10;
        QByteArray p = QByteArray((char*)&i, sizeof(i));
        QByteArray p2 = QByteArray((char*)&j, sizeof(i));
        test.Test(alg, p, p2, res, settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");
        qDebug() << j << " " << i << " " << res.toAscii().constData();*/
        for (int j = 30; j <= 30; j+=10) {
            for (int i = 100; i >= 5; i-=5) {
                    QByteArray p = QByteArray((char*)&i, sizeof(i));
                    QByteArray p2 = QByteArray((char*)&j, sizeof(i));
                    //test->Test(alg, p, p2, res, settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");
                    //qDebug() << j << " " << i << " " << res.BER;
            }
        }
        break;
    case 1:
        for (int j = 100; j <= 100; j+=10) {
            for (int i = 1; i <= 100; i+=10) {
                    QByteArray p = QByteArray((char*)&i, sizeof(i));
                    QByteArray p2 = QByteArray((char*)&j, sizeof(i));
                    //test->Test(alg, p, p2, res, settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");
                    //qDebug() << j << " " << i << " " << res.BER;
            }
        }
        break;
    case 2:
    case 3:
        for (int j = 100; j <= 100; j+=10) {
            for (int i = 1; i <= 100; i+=10) {
                tset =  /*QString::number(tINoise) + ":" + QString::number(1) + ";" +
                        QString::number(tAdMedFilter) + ":" + QString::number(1) + ";" +
                        QString::number(tSharpen) + ":" + QString::number(0.01);*/
                        QString::number(tUNoise) + ":" + QString::number(0);/* + ";" +
                        QString::number(tAdMedFilter) + ":" + QString::number(1);*/
                QByteArray p = tset.toUtf8();
                //QByteArray p = QByteArray((char*)&i, sizeof(i));
                QByteArray p2 = QByteArray((char*)&j, sizeof(i));
                //test->Test(alg, p, p2, res, settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");
                //qDebug() << j << " " << i << " " << res.BER;
            }
        }
        break;
    }

}

//UNUSED
void MainWindow::hideInfo()
{
    int t = 5;
    QByteArray p = QByteArray((char*)&t, sizeof(t));
    alg->SetParams(p);
    alg->GenKey("..\\stego\\test\\in.txt");
        //qDebug() << "Wah!";
    alg->HideToFile(settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt", "..\\stego\\test\\2.bmp");

    QPixmap pm = QPixmap("..\\stego\\test\\2.bmp").scaled(QSize(512, 512), Qt::KeepAspectRatio);
    QImage img = pm.toImage();
    QImage img_b = img, img_r = img, img_g = img, img_y = img;

    for (int i=0; i<img.width(); i++) {
        for (int j=0; j<img.width(); j++) {
            QColor color(img.pixel(i, j));
            int y = (PIXELRANGE(ROUND(0.299*color.red() + 0.587*color.green() + 0.114*color.blue())));
            img_b.setPixel(i, j, QColor(0, 0, color.blue()).rgb());
            img_r.setPixel(i, j, QColor(color.red(), 0, 0).rgb());
            img_g.setPixel(i, j, QColor(0, color.green(), 0).rgb());
            img_y.setPixel(i, j, QColor(y, y, y).rgb());
        }
    }


    ui->image_b->setPixmap(QPixmap::fromImage(img_b));
    ui->image_g->setPixmap(QPixmap::fromImage(img_g));
    ui->image_r->setPixmap(QPixmap::fromImage(img_r));
    ui->image_y->setPixmap(QPixmap::fromImage(img_y));
    ui->image_orig->setPixmap(QPixmap(settings.value("defaultContainer").toString()).scaled(QSize(512, 512), Qt::KeepAspectRatio));
    ui->image_res->setPixmap(pm);
}

//UNUSED
void MainWindow::restoreInfo()
{
    alg->RestoreFromFile("..\\stego\\test\\2.bmp", "..\\stego\\test\\out.txt");
}

//UNUSED
void MainWindow::custom()
{
    QImage res = QImage(512, 512, QImage::Format_ARGB32);
    CTestKoh ctest;
    pmask = ctest.Test(settings.value("defaultContainer").toString(), "..\\stego\\test\\in.txt");

    for (int y = 0; y < pmask.height(); ++y) {
      QRgb *row = (QRgb*)pmask.scanLine(y);
      for (int x = 0; x < pmask.width(); ++x) {
        ((unsigned char*)&row[x])[3] = 255;
      }
    }
    ui->map->setPixmap(QPixmap::fromImage(pmask).scaled(QSize(512, 512), Qt::KeepAspectRatio));

    int alpha = 127;
    for (int y = 0; y < pmask.height(); ++y) {
      QRgb *row = (QRgb*)pmask.scanLine(y);
      for (int x = 0; x < pmask.width(); ++x) {
          if (((unsigned char*)&row[x])[0] > alpha)  {
            ((unsigned char*)&row[x])[3] = 255;
          }
          else {
              ((unsigned char*)&row[x])[3] = 0;
          }
      }
    }


    //pmask.fill(qRgba(0,0,0,0));
    QImage img;
    //i.fill(qRgba(0,0,0,0));
    img = QImage(settings.value("defaultContainer").toString());
    for (int i=0; i<img.width(); i++) {
        for (int j=0; j<img.height(); j++) {
            img.setPixel(i, j, 255 - (img.pixel(i, j) & 0xFF));
        }
    }
    QPainter::CompositionMode mode = QPainter::CompositionMode_SourceAtop;

    QPainter painter(&res);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(res.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, img);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, pmask);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(res.rect(), Qt::white);
    painter.end();

    //resultLabel->setPixmap(QPixmap::fromImage(resultImage));
    qDebug() << pmask.pixel(50, 50);
    ui->original->setPixmap(QPixmap::fromImage(res).scaled(QSize(512, 512), Qt::KeepAspectRatio));
    //background: rgba(170, 85, 255, 50%)
}

//UNUSED
void MainWindow::toggle_view()
{
    if (ui->original->isHidden()) {
        ui->original->show();
    }
    else {
        ui->original->hide();
    }
}
