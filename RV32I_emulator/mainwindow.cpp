#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDir>
#include<QFileDialog>
#include<QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    fileReader = new FileReader();
    ui->setupUi(this);
    spinbox = new QSpinBox(this);
    spinbox->setRange(10, 2000);
    spinbox->setSingleStep(10);
    spinbox->setValue(1000);
    spinbox->setSuffix(" ms");
    connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(spinBox_valueChanged()));
    ui->toolBar->addWidget(spinbox);
    ui->tabWidget->setTabText(0, "Register");
    ui->tabWidget->setTabText(1, "Memory");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileReader;
    delete emulator;
}

void MainWindow::on_actionLoad_File_triggered()
{
    QString currentPath = QDir::currentPath();
    QString title = "Open a elf file";
    QString filter = "All file(*.*);;Elf file(*.elf);;Sprite Elf file(*.se)";
    fileReader->setFilePath(QFileDialog::getOpenFileName(this, title, currentPath, filter));
    if(fileReader->getFilePath().isEmpty())
        return;
    ElfReader elfReader(fileReader->getFilePath());

    if(elfReader.isElf())
    {
        emulator = new Emulator(fileReader->getFilePath());
        fileReader->setText(elfReader.getTextSection());
        ui->textBrowser->setPlainText(fileReader->getText());

    }
    else
    {
        ui->textBrowser->setPlainText("");
    }

}

void MainWindow::on_actionExit_triggered()
{
    delete ui;
    delete fileReader;
    delete emulator;
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
    delete emulator;
    emulator = new Emulator(fileReader->getFilePath());
}

void MainWindow::on_actionStep_triggered()
{
    if(emulator!=nullptr)
    {
        QString temp = emulator->nextInstruction();
        if(!temp.isEmpty())
            ui->Console->append(temp);
    }
}

void MainWindow::on_actionEnd_triggered()
{
    if(emulator!=nullptr)
    {
        while (!emulator->isEnd()) {
            QString temp = emulator->nextInstruction();
            if(!temp.isEmpty())
                ui->Console->append(temp);
        }
    }
}

void MainWindow::on_actionStop_triggered()
{
    fileReader->setStop(true);
}

void MainWindow::on_actionRun_triggered()
{
    fileReader->setStop(false);
    while (!emulator->isEnd() && !fileReader->isStop()) {
        QString temp = emulator->nextInstruction();

        int cursor = (emulator->getPC()-emulator->getEntry())/4;

        QTextCursor highlight_cursor(ui->textBrowser->textCursor());
        highlight_cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, cursor);
        QTextCharFormat color;
        color.setBackground(Qt::red);
        highlight_cursor.setCharFormat(color);

        qDebug()<<cursor<<" "<<highlight_cursor.position();

        if(!temp.isEmpty())
            ui->Console->append(temp);
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed()<fileReader->getInterval()) {
            QCoreApplication::processEvents();
        }
    }
}

void MainWindow::spinBox_valueChanged()
{
    fileReader->setInterval(spinbox->value());
    fileReader->setStop(true);
}

void MainWindow::on_DeleteConsole_clicked()
{
    ui->Console->setPlainText("");
}
