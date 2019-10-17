#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMultimedia/QSound>
#include <QtMultimedia/QSoundEffect>

#include <math.h>
#include <QTimer>

#include <QtConcurrent/QtConcurrent>

#define PI 3.14159264

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_bigCircle(nullptr),
    m_smallCircle(nullptr),
    m_dotCircle(nullptr),
    m_speedTimer(nullptr),
    m_pct(0.0),m_play(true),
    m_spd(100),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

void MainWindow::init() {
    addMedia("C:/dev/Polyrhythm/1.wav");
    addMedia("C:/dev/Polyrhythm/2.wav");
    addMedia("C:/dev/Polyrhythm/3.wav");
    addMedia("C:/dev/Polyrhythm/1.wav");
    addMedia("C:/dev/Polyrhythm/4.wav");
    addMedia("C:/dev/Polyrhythm/2.wav");
    addMedia("C:/dev/Polyrhythm/1.wav");
    addMedia("C:/dev/Polyrhythm/2.wav");

    m_pens << QPen(Qt::black)
           << QPen(Qt::black)
           << QPen(Qt::black)
           << QPen(Qt::blue)
           << QPen(Qt::red)
           << QPen(Qt::green)
           << QPen(Qt::cyan)
           << QPen(Qt::gray)
           << QPen(Qt::blue)
           << QPen(Qt::darkRed)
           << QPen(Qt::darkBlue)
           << QPen(Qt::darkGray)
           << QPen(Qt::darkCyan)
           << QPen(Qt::darkGreen)
           << QPen(Qt::darkMagenta)
           << QPen(Qt::black)
           << QPen(Qt::black);

}

void MainWindow::addMedia(const QString& pathToMedia){
    auto player = m_listPlayers.insert(m_listPlayers.count()+1, new QMediaPlayer(this));
    m_listPlayers[m_listPlayers.count()]->setVolume(100);
    m_listPlayers[m_listPlayers.count()]->setMedia(QUrl::fromLocalFile(pathToMedia));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    QRect r = ui->graphicsView->rect();
    int	nbCotes = ui->lineEdit->text().toInt();

    if(ui->graphicsView->scene() == nullptr)
        ui->graphicsView->setScene(new QGraphicsScene());

    //cercle
    QGraphicsScene* scene = ui->graphicsView->scene();

    if(m_bigCircle == nullptr)
        m_bigCircle = scene->addEllipse(0, 0, r.width() - 2, r.height() - 2, QPen(Qt::red));

    if(m_smallCircle == nullptr)
        m_smallCircle = scene->addEllipse(r.width()/2, 0, 15, 15, QPen(Qt::black), QBrush(Qt::red));

    if(m_dotCircle == nullptr)
        m_dotCircle = scene->addEllipse(r.width()/2, 0, 1, 1, QPen(Qt::black), QBrush(Qt::blue));

    scene->addLine(r.width()/2, r.height()/2, r.width()/2, r.height()/2, QPen(Qt::red));


    //formes a ajouter
    QPointF curPoint;
    double halfWidth = (r.width()/2) + 1;
    double halfHeight = (r.height()/2) + 1;
    double smallPolyW = r.width() / 6.0;
    double smallPolyH = r.height() / 6.0;

    int Surplus = 0;

    QPointF prevPoint(halfWidth, 0);
    QPolygonF forme(nbCotes+1);

    double rad = 0.0;

    for(int i = 1; i <= nbCotes; i++)
    {
        rad = ((((360.0/nbCotes)*i)+Surplus) * PI)/180.0;
        curPoint.setX(halfWidth - (sin(rad) * halfWidth));
        curPoint.setY(halfHeight - (cos(rad) * halfHeight));

        forme[i] = curPoint;
        prevPoint = curPoint;
    }

    double oldrad = ((((360.0/nbCotes)/2.0)+Surplus) * PI)/180.0;
    prevPoint.setX(halfWidth - (sin(oldrad) * smallPolyW));
    prevPoint.setY(halfHeight - (cos(oldrad) * smallPolyH));

    for(int j = 1; j <= nbCotes; j++)
    {
        //première ligne qui part du milieu en haut
        m_listLines.push_back(scene->addLine(QLineF(prevPoint, forme.at(j)), m_pens[nbCotes]));

        rad = oldrad + ((((360.0 / nbCotes) * j)) * PI)/180.0;
        curPoint.setX(halfWidth - (sin(rad) * smallPolyW));
        curPoint.setY(halfHeight - (cos(rad) * smallPolyH));

        //2eme ligne qui part du coin et rejoin le prochain point
        m_listLines.push_back(scene->addLine(QLineF(curPoint, forme.at(j)), m_pens[nbCotes]));

        //petite ligne au milieu
        m_listLines.push_back(scene->addLine(QLineF(prevPoint, curPoint), m_pens[nbCotes]));

        prevPoint = curPoint;
    }
}


void MainWindow::on_btnClear_clicked()
{
    m_speedTimer->stop();
    ui->graphicsView->scene()->clear();

    m_listLines.clear();
}

void MainWindow::on_btnGo_clicked()
{
    if(m_smallCircle != nullptr)
    {
        QRect r = ui->graphicsView->rect();
        double rad = (m_pct * PI)/180.0;
        double x =  (sin(rad) * (r.width()/2));
        double y = r.height()/2.0 - (cos(rad) * (r.height()/2));
        m_smallCircle->setPos(x-7,y-7);
        m_dotCircle->setPos(x,y);
        m_pct += 1;

        for(const QGraphicsLineItem* item : m_listLines)
        {
            if(m_dotCircle->collidesWithItem(item)) {
                if(item->pen().color() == Qt::blue) {
                    m_listPlayers[3]->play();
                }
                if(item->pen().color() == Qt::red) {
                    m_listPlayers[4]->play();
                }
                if(item->pen().color() == Qt::green) {
                    m_listPlayers[5]->play();
                }
                if(item->pen().color() == Qt::cyan) {
                    m_listPlayers[6]->play();
                }
                if(item->pen().color() == Qt::gray) {
                    m_listPlayers[7]->play();
                }
                m_play = !m_play;
            }
        }
    }
}

void MainWindow::on_btnTimer_clicked()
{
    if(m_speedTimer != nullptr) {
        m_speedTimer->stop();
        delete m_speedTimer;
    }

    m_speedTimer = new QTimer(this);

    m_speedTimer->setInterval(m_spd);
    connect(m_speedTimer, SIGNAL(timeout()), this, SLOT(on_btnGo_clicked()));
    m_speedTimer->start();
}

void MainWindow::on_m_tstSpd_textChanged(const QString &arg1)
{
    m_spd = arg1.toInt();
}
