
#include "widget.h"
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE // 引入命名空间，必须放在ui_widget.h前
#include "ui_widget.h"
#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include <time.h>
#include <string.h>
#include<iostream>
#include<client.h>
using namespace std;
float emgdata[10];
client c1;
Widget::Widget(QWidget *parent)
	: QWidget(
		  parent),
	  ui(new Ui::Widget)
{
	ui->setupUi(this);
	this->setWindowTitle(QString("QtCharts绘图-肌电信号示波器 - V%1").arg(QT_VERSION));
	initChart();
	getdatainit();
	/*定时器*/
	timerDrawLine=new QTimer();
	timerDrawLine->start(1);
	connect(timerDrawLine,SIGNAL(timeout()),this,SLOT(drawline()));
	/*定时器2*/
	timeremg=new QTimer();
	timeremg->start(1);
	connect(timeremg,SIGNAL(timeout()),this,SLOT(getsocket()));
}

Widget::~Widget()
{
	delete ui;
}

void Widget::initChart()
{
	//初始化QChart的实例
	chart=new QChart();
	//初始化QLineSeries的实例
	for(int i=0;i<10;i++)
	{
		series[i] = new QLineSeries();

	}
	//设置曲线名称
	series[0]->setName("肌电信号1");
	series[1]->setName("肌电信号2");
	series[2]->setName("0电压分割线");
	series[3]->setName("上阈值分割线");
	series[4]->setName("下阈值分割线");
	series[5]->setName("一号电机实际电流");
	series[6]->setName("二号电机实际电流");
	//把曲线添加到QChart的实例chart中
	for(int i=0;i<7;i++)
	{
		chart->addSeries(series[i]);
	}


	//声明并初始化X Y两个轴
	QValueAxis *axisX = new QValueAxis();
	QValueAxis *axisY = new QValueAxis();
	//设置坐标轴显示的范围
	axisX->setRange(0, MAX_X);		 //范围
	axisY->setRange(-MAX_Y, MAX_Y);		 //范围
	//设置坐标轴上的格点
	axisY->setTickCount(10);	 //轴上有多少个标记数目
	axisX->setTickCount(10);		 //主要刻度
	//设置坐标轴显示的名称
	QFont font("Microsoft YaHei",8,QFont::Normal);
	axisX->setTitleText("time/(s)"); //标题
	axisY->setTitleText("肌电信号幅值");

	//设置网格不显示
	axisX->setGridLineVisible(true); //网格
	axisY->setGridLineVisible(false); //网格
	//安放坐标轴位置
	chart->addAxis(axisX, Qt::AlignBottom); //将坐标轴加到chart上，居下
	chart->addAxis(axisY, Qt::AlignLeft);	  //居左
	//把曲线关联到坐标轴
		for(int i=0;i<7;i++)
	{
		series[i]->attachAxis(axisX);
		series[i]->attachAxis(axisY);
	}
	//把chart显示在窗口上
	ui->chartView->setChart(chart);
	ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
	ui->chartView->setRubberBand(QChartView::NoRubberBand);  //矩形缩放

	//设置画笔颜色
	series[0]->setPen(QPen(Qt::green,2,Qt::SolidLine));
	series[1]->setPen(QPen(Qt::blue,2,Qt::SolidLine));
	series[2]->setPen(QPen(Qt::red,2,Qt::SolidLine));
	series[3]->setPen(QPen(Qt::gray,2,Qt::SolidLine));
	series[4]->setPen(QPen(Qt::black,2,Qt::SolidLine));
	series[5]->setPen(QPen(Qt::darkGray,2,Qt::SolidLine));
	series[6]->setPen(QPen(Qt:: magenta,2,Qt::SolidLine));
}

void Widget::getdatainit()
{
	c1.get_connect();
}

void Widget::getsocket()
{
	c1.getsocket();
}

void Widget::drawline()
{
	static int count1 =0;
	static QList<QPointF>ps;
	static QList<QPointF>cu1;
	static QList<QPointF>cu2;
	ps.push_back(QPointF(count1,emgdata[0]));
	cu1.push_back(QPointF(count1,emgdata[6]/100000));
	cu2.push_back(QPointF(count1,emgdata[7]/100000));
	//series->clear();
	if(count1>MAX_X)
	{
		//当曲线上最早的点超出x轴的范围时，剔除最早的点
		series[1]->removePoints(0,series[1]->count()-MAX_X);
		//更新X轴的范围
		//chart->axisX()->setMin(count1-MAX_X);
		//chart->axisX()->setMax(count1);
		ps.removeAt(0);
		cu1.removeAt(0);
		cu2.removeAt(0);
		chart->axisX()->setMin(count1-MAX_X);
		chart->axisX()->setMax(count1);
		//std::cout<<ps<<std::endl;
	}	
	series[1]->append(count1,emgdata[1]);
	series[2]->append(count1,0);
	series[3]->append(count1,35);
	series[4]->append(count1,15);
	series[0]->replace(ps);
	series[5]->replace(cu1);
	series[6]->replace(cu2);
	count1 ++;
}
					
