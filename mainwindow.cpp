#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLE=new QLineEdit(this);
    QLE->setFocusPolicy(Qt::NoFocus);

    QLE->move(20,50);

    QLE->installEventFilter(this);
    setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
       db=new dateBoard(QLE,this);
       db->move(20,100);
       db->raise();
       db->show();
    }
    return QObject::eventFilter(obj, event);
}
