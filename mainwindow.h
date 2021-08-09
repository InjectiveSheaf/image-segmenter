#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>
#include <QPixmap>
#include <QCheckBox>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushOpen_clicked();

    void on_pushFilter_clicked();

    void on_pushProcess_clicked();

    void on_pushSave_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
