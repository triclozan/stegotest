#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QBitArray;
#include <QThread>
namespace Ui {
    class MainWindow;
}

class CAlgorithm;
class CTest;
class QString;
class CImageHistory;
class QTextEdit;
class QLabel;


class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QTextEdit* ptr, QString& str, QString container_name);
    void parse(QString& data, int depth=0);
signals:
    void aSignal(QString data);
    void done();
protected:
    void run();
private:
    QTextEdit* output;
    QString data;
    QString container_name;
};


/*class Thread2 : public QThread
{
    Q_OBJECT
public:
    Thread2(QTextEdit* ptr, QString& str);
    void parse(QString& data, int depth=0);
signals:
    void done();
protected:
    void run();
private:
    QLabel* output;
    QString data;
};*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void doTest();
    void hideInfo();
    void restoreInfo();
    void changeAlgo(int index);
    void changeTest(int index);
    void openFileDlg();
    void doTransform();
    void openSaveDlg();
    void loadImage();
    void custom();
    void custom_action();
    void distortions();
    void toggle_view();
    void keyPressEvent(QKeyEvent * event);
    void perform_tests();
    void log(QString data);
    void afterTest();
    void shadowComments(bool b);
    void showContextMenu(const QPoint &pt);
    void saveScript();
    void saveAsScript();
    void loadScript(QString fname = "");
    void outputContextMenu(const QPoint &pt);
    void setOutputClear(bool b);
    void changeContainer();

    void showPicture();
    void hidePicture();
private:
    void parse(QString& data, int depth=0);
    Ui::MainWindow *ui;
    int AlgId;
    int TestId;
    CAlgorithm* alg;
    CTest* test;
    CImageHistory* backup;
    QImage pmask;
    Thread* thread;
    QString log_buffer;
    QString script_fname;
    bool start_buffer;
    bool shadow_comment;
    bool clear_output;
    QMenu* menu_input;
    QMenu* menu_output;
    QLabel* lab;
    QString container;
    bool test_image_loaded;
    bool transform_image_loaded;
};


#endif // MAINWINDOW_H
