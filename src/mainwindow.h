#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QScrollBar>
#include <QComboBox>
#include <filereader.h>
#include <elfreader.h>
#include <emulator.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void highlightCurrentLine();
    void updateRegisterList();
    void updateMemoryList();

private slots:
    void on_actionLoad_File_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionVersion_triggered();
    void on_actionReset_triggered();
    void on_actionStep_triggered();
    void on_actionEnd_triggered();
    void on_actionStop_triggered();
    void on_actionRun_triggered();
    void spinBox_valueChanged();
    void scrollBar_scrolling();
    void gotoMemoryByRegister();
    void gotoMemoryBySection();
    void on_DeleteConsole_clicked();
    void test();
    void test2();

signals:
    void setStdin(QString);

private:
    Ui::MainWindow *ui = nullptr;
    QSpinBox *spinbox = nullptr;
    QScrollBar *scrollbar = nullptr;

    FileReader *fileReader = nullptr;
    ElfReader *elfReader = nullptr;
    Emulator *emulator = nullptr;
    QMap<QString, int> *sectionMap = nullptr;

    QString currentConsoleText;
};
#endif // MAINWINDOW_H
