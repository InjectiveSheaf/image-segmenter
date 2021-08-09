#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "segmentation_interface.h"
#include "drawer.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushOpen_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open A File", "C://");
    QPixmap file(fileName);
    ui->label->setPixmap(file);
    ui->label->setScaledContents(true);
}

void MainWindow::on_pushSave_clicked(){
    if(ui->label_2->hasScaledContents() == 0){
        QMessageBox::information(NULL, QObject::tr("Ошибка"), tr("Вы не выбрали изображение"));
    }
    else{
        QPixmap file = *(ui->label_2->pixmap());
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), QString(), tr("Images (*.png)"));
        if (!fileName.isEmpty()){
            file.save(fileName);
        }
    }
}

void MainWindow::on_pushFilter_clicked()
{
    if(ui->label->hasScaledContents() == 0){
        QMessageBox::information(NULL, QObject::tr("Ошибка"), tr("Вы не выбрали изображение"));
    }
    else{
        QPixmap file = *(ui->label->pixmap());
        QImage original_image = file.toImage();
        QImage processed_image(original_image.width(), original_image.height(), original_image.format());
        Drawer MF;
        processed_image = MF.Filter(original_image);
        QPixmap result = QPixmap::fromImage(processed_image);
        ui->label->setPixmap(result);
        ui->label->setScaledContents(true);
    }
}

void MainWindow::on_pushProcess_clicked()
{
    uint color;

    if (ui->checkBox_2->isChecked() == false) {
        color = 0;
    }
    if (ui->checkBox_2->isChecked() == true) {
        color = 16777215;
    }

    if (ui->label->hasScaledContents() == 0){
        QMessageBox::information(NULL, QObject::tr("Ошибка"), tr("Вы не выбрали изображение"));
    }
    else{
        QPixmap file = *(ui->label->pixmap());

        QImage original_image = file.toImage();
        QImage processed_image(original_image.width(), original_image.height(), original_image.format());

        original_image = original_image.convertToFormat(QImage::Format_Grayscale8);

        float threshold = ui->Threshold_SB->value();
        int minimum_segment_size = ui->MinSize_SB->value();

        Merge *merge = new Merge(threshold);
        Metric *metric = new Metric();

        GraphSegmentation GS(metric, merge);
        GS.buildGraph(original_image);
        GS.GraphPrimarySegmentation();
        GS.MinimumSegmentSize(minimum_segment_size);
        processed_image = GS.GetLabel();

        Drawer drawer;

        if (ui->checkBox->isChecked() == false) {}

        else {
            processed_image = drawer.Draw_Contours(processed_image, file.toImage(), color);
        }

        QPixmap result = QPixmap::fromImage(processed_image);
        ui->label_2->setPixmap(result);
        ui->label_2->setScaledContents(true);
    }
}
