#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir directory = QFileDialog::getExistingDirectory(this);
    QStringList models = directory.entryList(QStringList() /*<< "*.obj" << "*.OBJ"*/,QDir::Files);
    for (int i=0; i< models.size(); i++)
    {
        QString file = (QString) models.at(i).constData();
        ui->listWidget->addItem(file);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_loadButton_clicked()
{
    if(ui->listWidget->selectedItems().size() != 0){
        QString file = ui->listWidget->currentItem()->text();
    }else{
        QMessageBox::warning(this, "Erreur fichier", "Attention, vous n'avez pas sélectionné de fichier !");
    }
}


void MainWindow::on_compressButton_clicked()
{
    if(ui->listWidget->selectedItems().size() != 0){
        QString file = ui->listWidget->currentItem()->text();
    }else{
        QMessageBox::warning(this, "Erreur fichier", "Attention, vous n'avez pas sélectionné de fichier !");
    }
}


void MainWindow::on_cryptButton_clicked()
{
    if(ui->listWidget->selectedItems().size() != 0){
        QString file = ui->listWidget->currentItem()->text();
    }else{
        QMessageBox::warning(this, "Erreur fichier", "Attention, vous n'avez pas sélectionné de fichier !");
    }
}


void MainWindow::on_decryptButton_clicked()
{
    if(ui->listWidget->selectedItems().size() != 0){
        QString file = ui->listWidget->currentItem()->text();
    }else{
        QMessageBox::warning(this, "Erreur fichier", "Attention, vous n'avez pas sélectionné de fichier !");
    }
}


