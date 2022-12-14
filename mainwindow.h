#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMessageBox>
#include <QMainWindow>
#include "opration.h"
#include "analyse.h"
#include <QFont>
#include "ui_mainwindow.h"
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
    void exitbtn_clicked();
    void analysebtn_clicked();
    void loadbtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
