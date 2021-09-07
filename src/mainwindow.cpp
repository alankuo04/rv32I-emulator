#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "memorymapmodel.h"
#include "registermapmodel.h"

#include<QDir>
#include<QFileDialog>
#include<QElapsedTimer>
#include<QMessageBox>

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

    scrollbar = ui->MemoryList->verticalScrollBar();
    connect(scrollbar, SIGNAL(actionTriggered(int)), this, SLOT(scrollBar_scrolling()));

    ui->tabWidget->setTabText(0, "Register");
    ui->tabWidget->setTabText(1, "Memory");

    QList<QString> registerList;
    for(int i=0;i<32;i++)
        registerList.append(QString(register_list[i])+" (x"+QString::number(i)+")");
    ui->gotoRegister->addItems(registerList);
    QList<QString> memoryList;
    memoryList.append("");
    memoryList.append(".text");
    memoryList.append(".rodata");
    memoryList.append(".data");
    memoryList.append(".sdata");
    memoryList.append(".sbss");
    memoryList.append(".bss");
    ui->gotoSection->addItems(memoryList);

    connect(ui->gotoRegister, SIGNAL(currentIndexChanged(int)), this, SLOT(gotoMemoryByRegister()));
    connect(ui->gotoSection, SIGNAL(currentIndexChanged(int)), this, SLOT(gotoMemoryBySection()));

    ui->RegisterList->verticalHeader()->hide();
    ui->MemoryList->verticalHeader()->hide();
    ui->textBrowser->setCenterOnScroll(true);

    connect(ui->Console, SIGNAL(blockCountChanged(int)), this, SLOT(test()));
    ui->Console->setReadOnly(true);
}

void MainWindow::test()
{
    ui->Console->setReadOnly(false);
    qDebug()<<ui->Console->blockCount();
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
    elfReader = new ElfReader(fileReader->getFilePath());

    if(elfReader->isElf())
    {
        emulator = new Emulator(fileReader->getFilePath());
        fileReader->setText(elfReader->getTextSection());
        sectionMap = elfReader->getSectionMap();
        ui->textBrowser->setPlainText(fileReader->getText());

        ui->RegisterList->setModel(emulator->getRegisterMapModel());
        ui->MemoryList->setModel(emulator->getMemoryMapModel());

        ui->RegisterList->setColumnWidth(0, 120);
        ui->RegisterList->setColumnWidth(1, 120);
        ui->RegisterList->setColumnWidth(2, 120);

        ui->MemoryList->setColumnWidth(0, 80);
        ui->MemoryList->setColumnWidth(1, 80);
        ui->MemoryList->setColumnWidth(2, 50);
        ui->MemoryList->setColumnWidth(3, 50);
        ui->MemoryList->setColumnWidth(4, 50);
        ui->MemoryList->setColumnWidth(5, 50);
        //qDebug()<<sectionMap->keys();
        //qDebug()<<sectionMap->values();
    }
    else
    {
        ui->textBrowser->setPlainText("");
        ui->Console->setFont(QFont("Consolas"));
        ui->Console->appendPlainText("=========================");
        ui->Console->appendPlainText(QString("%1").arg("Not an elf file.", 20));
        ui->Console->appendPlainText("=========================");
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
    if(emulator!=nullptr)
    {
        QMessageBox *message = new QMessageBox;
        QPushButton *elfHeaderButton = message->addButton("ELF Header", QMessageBox::ActionRole);
        QPushButton *programHeaderButton = message->addButton("Program Header", QMessageBox::ActionRole);
        QPushButton *sectionHeaderButton = message->addButton("Section Header", QMessageBox::ActionRole);
        message->setStandardButtons(QMessageBox::Cancel);
        message->setDefaultButton(QMessageBox::Cancel);
        elfHeaderButton->disconnect();
        programHeaderButton->disconnect();
        sectionHeaderButton->disconnect();

        connect(elfHeaderButton, &QPushButton::pressed, [=](){message->setText(elfReader->showElfHeader());});
        connect(programHeaderButton, &QPushButton::pressed, [=](){message->setText(elfReader->showProgramHeader());});
        connect(sectionHeaderButton, &QPushButton::pressed, [=](){message->setText(elfReader->showSectionHeader());});
        message->setFont(QFont("Consolas"));
        message->setStyleSheet("QLabel{min-width: 550px;}");
        message->exec();
    }
}

void MainWindow::on_actionVersion_triggered()
{
    QMessageBox *message = new QMessageBox;
    message->setIcon(QMessageBox::Information);
    message->setText("RV32I Emulator");
    message->setInformativeText("Version: 1.1");
    message->setStandardButtons(QMessageBox::Ok);
    message->setDefaultButton(QMessageBox::Ok);
    message->exec();
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
        updateMemoryList();
    }
}

void MainWindow::on_actionStep_triggered()
{
    if(emulator!=nullptr)
    {
        highlightCurrentLine();
        updateRegisterList();
        updateMemoryList();
        QString temp = emulator->nextInstruction();
        if(!temp.isEmpty())
            ui->Console->appendPlainText(temp);
    }
}

void MainWindow::on_actionEnd_triggered()
{
    if(emulator!=nullptr)
    {
        while (!emulator->isEnd()) {
            QString temp = emulator->nextInstruction();
            if(!temp.isEmpty())
                ui->Console->appendPlainText(temp);
        }
        highlightCurrentLine();
        updateRegisterList();
        updateMemoryList();
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
            updateMemoryList();

            QString temp = emulator->nextInstruction();

            if(!temp.isEmpty())
                ui->Console->appendPlainText(temp);
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

void MainWindow::scrollBar_scrolling()
{
    //qDebug()<<scrollbar->sliderPosition()<<" "<<scrollbar->maximum();
    if(scrollbar->sliderPosition()==0 && scrollbar->maximum()==0)
        return;
    else if(scrollbar->sliderPosition() == 0)
    {
        emulator->getMemoryMapModel()->addStartIndex();
    }
    else if(scrollbar->sliderPosition() == scrollbar->maximum())
    {
        emulator->getMemoryMapModel()->subStartIndex();
    }
    updateMemoryList();
}

void MainWindow::gotoMemoryByRegister()
{
    if(emulator!=nullptr)
    {
        int memoryPosition = emulator->getRegisterMapModel()->temp_register[ui->gotoRegister->currentIndex()];
        //qDebug()<<ui->gotoRegister->currentIndex()<<" "<<memoryPosition;
        emulator->getMemoryMapModel()->setStartIndex(memoryPosition);
        updateMemoryList();
    }
}

void MainWindow::gotoMemoryBySection()
{
    if(emulator!=nullptr)
    {
        int memoryPosition = sectionMap->value(ui->gotoSection->currentText());
        //qDebug()<<ui->gotoSection->currentIndex()<<" "<<memoryPosition;
        emulator->getMemoryMapModel()->setStartIndex(memoryPosition);
        updateMemoryList();
    }
}

void MainWindow::on_DeleteConsole_clicked()
{
    ui->Console->setPlainText("");
}
