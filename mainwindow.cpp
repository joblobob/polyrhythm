#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMultimedia/QSound>
#include <math.h>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_bigCircle(NULL),
    m_smallCircle(NULL),
    m_pct(0.0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QRect r = ui->graphicsView->rect();
    int		startAngle = ui->lineEdit->text().toInt();
    int		endAngle = 45;
    bool	filled = false;

    if(ui->graphicsView->scene() == NULL)
        ui->graphicsView->setScene(new QGraphicsScene());


    //cercle
    QGraphicsScene* scene = ui->graphicsView->scene();

    if(m_bigCircle == NULL)
        m_bigCircle = scene->addEllipse(0, 0, r.width() - 1, r.height() - 1, QPen(Qt::red));

    if(m_smallCircle == NULL)
        m_smallCircle = scene->addEllipse(r.width()/2, 0, 15, 15, QPen(Qt::black), QBrush(Qt::red));

    scene->addLine(r.width()/2, r.height()/2, r.width()/2, r.height()/2, QPen(Qt::red));
    QPen pen;

    switch(startAngle)
    {
        case 3:
            pen = QPen(Qt::blue);
            break;
        case 4:
            pen = QPen(Qt::red);
            break;
        case 5:
            pen = QPen(Qt::green);
            break;
        case 6:
            pen = QPen(Qt::cyan);
            break;
        case 7:
            pen = QPen(Qt::gray);
            break;
        case 8:
            pen = QPen(Qt::black);
            break;
        default:
            pen = QPen(Qt::blue);
            break;
    }
    pen.setWidth(5);
    //formes a ajouter
    double x = 0;
    double y = 0;
    double halfWidth = (r.width()/2);
    double halfHeight = (r.height()/2);
    int ioldx =  halfWidth;
    int ioldy = 0;
    int ix;
    int iy;
    double oldx = halfWidth;
    double oldy = 0;
    int Surplus = 0;

    int *iTabPremierPolyx;
    int *iTabPremierPolyy;

    iTabPremierPolyx = new int[startAngle+1];
    iTabPremierPolyy = new int[startAngle+1];

    int it = 1;

    double rad = 0;

    double PI = 3.14159264;


    for(double i = 1.0f; i <= startAngle; i++)
    {
        rad = ((((360.0f/startAngle)*i)+Surplus) * PI)/180.0f;
        x = halfWidth - (sin(rad) * halfWidth);
        y = halfHeight - (cos(rad) * halfHeight);

        iTabPremierPolyx[it] = (int)x;
        iTabPremierPolyy[it] = (int)y;
        it++;

    //	g2.drawLine((int)oldx, (int)oldy, (int)x, (int)y);		//Ligne cote du polygone

    //	g2.drawLine((r.width/2), (r.height/2), (int)x, (int)y); //du centre vers le pic du polygone
        oldx = x;
        oldy = y;
    }

    double oldrad = ((((360.0f/startAngle)/2.0f)+Surplus) * PI)/180.0f;
    oldx = halfWidth - (sin(oldrad) * (r.width()/6));
    oldy = halfHeight - (cos(oldrad) * (r.height()/6));

    int *iTabDeuxPolyx;
    int *iTabDeuxPolyy;
    iTabDeuxPolyx = new int[startAngle+1];
    iTabDeuxPolyy = new int[startAngle+1];
    qApp->processEvents();
    for(double j = 1.0f; j <= startAngle; j++)
    {
        m_listLines.push_back(scene->addLine((int)oldx, (int)oldy, iTabPremierPolyx[(int)j], iTabPremierPolyy[(int)j], pen));

        rad = oldrad + ((((360.0 / startAngle) * j)) * PI)/180.0;
        x = halfWidth - (sin(rad) * (r.width()/6));
        y = halfHeight - (cos(rad) * (r.height()/6));

        m_listLines.push_back(scene->addLine((int)oldx, (int)oldy, (int)x, (int)y, pen));
        m_listLines.push_back(scene->addLine((int)x, (int)y, iTabPremierPolyx[(int)j], iTabPremierPolyy[(int)j], pen));
        oldx = x;
        oldy = y;
    }


}


void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scene()->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(m_smallCircle != NULL)
    {
        double PI = 3.14159264;
        QRect r = ui->graphicsView->rect();
        double  rad = /*oldrad +*/ ((m_pct)  * PI)/180.0;
        double x = /*r.width()/2.0 -*/ (sin(rad) * (r.width()/2));
        double y = r.height()/2.0 - (cos(rad) * (r.height()/2));
        m_smallCircle->setPos(x-7,y-7);
        m_pct += 1;

        foreach(QGraphicsLineItem * item,m_listLines)
        {
            if(m_smallCircle->collidesWithItem(item)){
           // QSoundEffect effect;
           // effect.setSource(QUrl("C:/dev/Polyrhythm/snare1.wav"));
           // effect.setLoopCount(QSoundEffect::Infinite);
         //   effect.setVolume(1.0);
          //  effect.play();

            QSound::play("C:/dev/Polyrhythm/snare1.wav");
            }
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_3_clicked()));
    timer->start();
}
