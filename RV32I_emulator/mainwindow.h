#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filereader.h>
#include <elfreader.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLoad_File_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionVersion_triggered();

    void on_actionReset_triggered();

    void on_actionStep_triggered();

    void on_actionEnd_triggered();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;
    FileReader fileReader;
};
#endif // MAINWINDOW_H
