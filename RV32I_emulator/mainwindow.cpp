#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDir>
#include<QFileDialog>
#include<QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , elfReader()
{
    ui->setupUi(this);
    QSpinBox *spinbox = new QSpinBox(this);
    spinbox->setRange(10, 2000);
    spinbox->setSingleStep(10);
    spinbox->setValue(1000);
    spinbox->setSuffix(" ms");
    ui->toolBar->addWidget(spinbox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_File_triggered()
{
    QString currentPath = QDir::currentPath();
    QString title = "Open a elf file";
    QString filter = "All file(*.*);;Elf file(*.elf)";
    elfReader.setFilePath(QFileDialog::getOpenFileName(this, title, currentPath, filter));
    if(elfReader.getFilePath().isEmpty())
        return;
    QFile file(elfReader.getFilePath());
    if(!file.exists())
        return;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // add elf reaeder
    elfReader.setText(file.readAll());
    file.close();
    if(!elfReader.isElfFile()){
        elfReader.setText("");
        return;
    }
    ui->textBrowser->setPlainText(elfReader.getText());
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_actionVersion_triggered()
{

}

void MainWindow::on_actionReset_triggered()
{

}

void MainWindow::on_actionStep_triggered()
{

}

void MainWindow::on_actionEnd_triggered()
{

}

void MainWindow::on_actionStop_triggered()
{

}
