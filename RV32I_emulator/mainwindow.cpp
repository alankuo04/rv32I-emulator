#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memorymapmodel.h"
#include "registermapmodel.h"

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
    ui->RegisterList->verticalHeader()->hide();
    ui->textBrowser->setCenterOnScroll(true);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileReader;
    delete emulator;
}

void MainWindow::highlightCurrentLine()
{
    int cursor = (emulator->getPC()-emulator->getEntry())/4;

    QTextCursor highlight_cursor(ui->textBrowser->document());
    highlight_cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, cursor);
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(Qt::red).lighter(150);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = highlight_cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);
    ui->textBrowser->setExtraSelections(extraSelections);

    //qDebug()<<cursor<<" "<<highlight_cursor.blockNumber()<<ui->textBrowser->blockCount();
    ui->textBrowser->setTextCursor(highlight_cursor);
}

void MainWindow::updateRegisterList()
{
    emulator->updateRegisterMapModel();
}

void MainWindow::updateMemoryList()
{
    emulator->updateMemoryMapModel();
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

        ui->RegisterList->setModel(emulator->getRegisterMapModel());
        ui->MemoryList->setModel(emulator->getMemoryMapModel());
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
    fileReader->setStop(true);
    if(!fileReader->getFilePath().isEmpty())
    {
        emulator = new Emulator(fileReader->getFilePath());
        ui->RegisterList->setModel(emulator->getRegisterMapModel());
        ui->MemoryList->setModel(emulator->getMemoryMapModel());
        highlightCurrentLine();
        updateRegisterList();
    }
}

void MainWindow::on_actionStep_triggered()
{
    if(emulator!=nullptr)
    {
        highlightCurrentLine();
        updateRegisterList();
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
        highlightCurrentLine();
        updateRegisterList();
    }
}

void MainWindow::on_actionStop_triggered()
{
    fileReader->setStop(true);
}

void MainWindow::on_actionRun_triggered()
{
    if(emulator!=nullptr)
    {
        fileReader->setStop(false);
        while (!emulator->isEnd() && !fileReader->isStop()) {

            highlightCurrentLine();
            updateRegisterList();

            QString temp = emulator->nextInstruction();

            if(!temp.isEmpty())
                ui->Console->append(temp);
            QElapsedTimer timer;
            timer.start();
            while (timer.elapsed()<fileReader->getInterval()) {
                QCoreApplication::processEvents();
            }
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
