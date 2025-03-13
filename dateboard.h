#ifndef DATEBOARD_H
#define DATEBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include<QLabel>
#include<QMouseEvent>
#include<QScroller>
#include<QScrollArea>

namespace Ui {
class dateBoard;
}

class dateBoard : public   QWidget
{
    Q_OBJECT

public:
    explicit dateBoard(QLineEdit *edit,QWidget *parent);
    ~dateBoard();

    void setLineEdit(QLineEdit *edit);

signals:
    void sig_exitcharboard();


private slots:
    void slotexitClick();
    void slotsureClick();
    void slotScrollFinished(QScroller::State);

private:
    QLineEdit       *m_edit;
    Ui::dateBoard   *ui;


    QVBoxLayout     *yearLayout,*monLayout,*dayLayout;
    QWidget         *yearWg,*monWg,*dayWg;
    void createScrollArea(QScrollArea *scrollArea, int start, int count, const QString &suffix, Qt::Alignment align, int repeat);
};

#endif // DATEBOARD_H
