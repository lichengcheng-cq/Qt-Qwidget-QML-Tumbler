#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLineEdit>
#include"dateboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    dateBoard*db;
    QLineEdit*QLE;

    // QObject interface
public:
    bool eventFilter(QObject *, QEvent *);
};

#endif // MAINWINDOW_H
