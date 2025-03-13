#include "dateboard.h"
#include "ui_dateboard.h"

#include <QScrollBar>
#include <QDebug>

#define constH   80
dateBoard::dateBoard(QLineEdit *edit,QWidget *parent) :
 m_edit(edit), ui(new Ui::dateBoard),
  QWidget(parent)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    // 禁用按钮焦点
    ui->canceBtn->setFocusPolicy(Qt::NoFocus);
    ui->sureBtn->setFocusPolicy(Qt::NoFocus);

    // 连接信号槽
    connect(ui->canceBtn, &QPushButton::clicked, this, &dateBoard::slotexitClick);
    connect(ui->sureBtn, &QPushButton::clicked, this, &dateBoard::slotsureClick);

    // 创建滚动选择器
    createScrollArea(ui->yearScrollArea, 2010, 30, "年", Qt::AlignRight,1);
    createScrollArea(ui->monScrollArea, 1, 12, "月", Qt::AlignHCenter, 2);
    createScrollArea(ui->dayScrollArea, 1, 31, "日", Qt::AlignLeft, 2);

    // 设定初始滚动位置
    ui->yearScrollArea->verticalScrollBar()->setValue(880);
    ui->monScrollArea->verticalScrollBar()->setValue(640);
    ui->dayScrollArea->verticalScrollBar()->setValue(2400);

    // 禁用滚动区域焦点
    ui->yearScrollArea->setFocusPolicy(Qt::NoFocus);
    ui->monScrollArea->setFocusPolicy(Qt::NoFocus);
    ui->dayScrollArea->setFocusPolicy(Qt::NoFocus);

    // 隐藏当前 widget
    ui->currWidget->setVisible(false);
    setLineEdit(edit);
}

dateBoard::~dateBoard()
{
    delete ui;
}

void dateBoard::setLineEdit(QLineEdit *edit)
{
    m_edit  = edit;
    QStringList time=edit->text().split("-");
    if(time.size()==3)
    {
    ui->yearScrollArea->verticalScrollBar()->setValue((time.at(0).toInt()-2011)*80);

    if(time.at(1).toInt()!=1)
        ui->monScrollArea->verticalScrollBar()->setValue((time.at(1).toInt()-2)*80);
    else
        ui->monScrollArea->verticalScrollBar()->setValue((time.at(1).toInt()-2)*80+80*12);

    if(time.at(2).toInt()!=1)
        ui->dayScrollArea->verticalScrollBar()->setValue((time.at(2).toInt()-2)*80);
    else
        ui->dayScrollArea->verticalScrollBar()->setValue((time.at(2).toInt()-2)*80+31*80);
    }
}

void dateBoard::slotexitClick()
{
    this->close();
}

void dateBoard::slotsureClick()
{
    int yearcy  = ui->yearScrollArea->verticalScrollBar()->value();
    int moncy   = ui->monScrollArea->verticalScrollBar()->value();
    int daycy   = ui->dayScrollArea->verticalScrollBar()->value();

    yearcy  = yearcy/constH;
    moncy   = moncy/constH;
    daycy   = daycy/constH;

    yearcy  = yearcy+2011;

    moncy   = (moncy+2)%12;
    if(moncy==0)
        moncy=12;

    daycy   = (daycy+2)%31;
    if(daycy==0)
        daycy=31;


    if( ( moncy==2)||( moncy==4)||( moncy==6)||( moncy==9)||( moncy==11) )
    {
        if(daycy>30)
            return;
    }

    if( moncy==2)
    {
        if( ( (yearcy%4==0)&&(yearcy%100!=0) )||(yearcy%400==0) )
        {
            if( daycy>29)
                return;
        }
        else
        {
            if(daycy>28)
                return;
        }
    }

    QString str="";

    QString yearStr = QString("%1").arg( yearcy,4,10,QChar('0') );
    QString monStr  = QString("%1").arg(moncy,2,10,QChar('0') );
    QString dayStr  = QString("%1").arg(daycy,2,10,QChar('0') );

    str=yearStr+"-"+monStr+"-"+dayStr;

    m_edit->setText(str);

    slotexitClick();
}
void dateBoard::slotScrollFinished(QScroller::State newState)
{
    if (newState == QScroller::Inactive) {
        int yearcy  = ui->yearScrollArea->verticalScrollBar()->value();
        int moncy   = ui->monScrollArea->verticalScrollBar()->value();
        int daycy   = ui->dayScrollArea->verticalScrollBar()->value();

        if(moncy<6*80)
        {
            moncy+=12*80;
        }
        else if(moncy>18*80)
        {
            moncy-=12*80;
        }

        if(daycy<15*80)
        {
            daycy+=31*80;
        }
        else if(daycy>46*80)
        {
            daycy-=31*80;
        }

        int yearAligned = (yearcy + 40) / 80 * 80;
        int monAligned  = (moncy + 40) / 80 * 80;
        int dayAligned  = (daycy + 40) / 80 * 80;

        ui->yearScrollArea->verticalScrollBar()->setValue(yearAligned);
        ui->monScrollArea->verticalScrollBar()->setValue(monAligned);
        ui->dayScrollArea->verticalScrollBar()->setValue(dayAligned);
    }
}

void dateBoard::createScrollArea(QScrollArea *scrollArea, int start, int count, const QString &suffix, Qt::Alignment align, int repeat)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *container = new QWidget;

    for (int r = 0; r < repeat; ++r) {  // 可选：是否重复填充
        for (int i = 0; i < count; ++i) {
            QLabel *label = new QLabel(scrollArea);
            label->setText(QString::number(start + i) + suffix);
            label->setStyleSheet("font-size:24px;");
            label->setAlignment(align | Qt::AlignVCenter);
            label->setFixedHeight(80);
            layout->addWidget(label);
        }
    }

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    container->setLayout(layout);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 启用平滑滚动
    QScroller *scroller = QScroller::scroller(scrollArea->viewport());
    scroller->grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    connect(scroller,SIGNAL(stateChanged(QScroller::State)),this,SLOT(slotScrollFinished(QScroller::State)));
}
