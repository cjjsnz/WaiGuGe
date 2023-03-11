#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLineSeries>
#include<QtCharts/QChartView>
#include<QtCharts/QSplineSeries>
#include<QtCharts/QLineSeries>
#include<QValueAxis>
#include"socketclient.h"
QT_CHARTS_USE_NAMESPACE // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <QLineSeries>
#include <QWidget>
#include "widget.h"
#include"socketclient.h"
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"
#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include <time.h>
#include <string.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
#define MAXLINE 128
#define SERV_PORT 8001
#define buflen 48
#define MAX_X 500
#define MAX_Y 50
#define TRUE 1
#define FALSE 0
extern float emgdata[10];
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTimer *timerDrawLine;
    QChart *chart;

    QLineSeries * series[10];
    /* QLineSeries * series1;
    QLineSeries * series2;
    QLineSeries * series3;
    QLineSeries * series4;*/
   


    QTimer *timeremg;

    QTimer *timersend;
    int socketfd;
	int clientfd;
    
public:
    void initChart();
    void getdatainit();
public slots:
    void drawline();
    void getsocket();
   /*void addSeries(QList<QPointF> &data);
    void timerEvent(QTimerEvent *event);
    */ 
    //void timerEvent(QTimerEvent *event);

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
