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

    /*m_listPlayers[3]->setMedia(QUrl::fromLocalFile("C:/dev/Polyrhythm/1.wav"));
    m_listPlayers[4]->setMedia(QUrl::fromLocalFile("C:/dev/Polyrhythm/2.wav"));
    m_listPlayers[5]->setMedia(QUrl::fromLocalFile("C:/dev/Polyrhythm/3.wav"));
    m_listPlayers[6]->setMedia(QUrl::fromLocalFile("C:/dev/Polyrhythm/1.wav"));
    m_listPlayers[7]->setMedia(QUrl::fromLocalFile("C:/dev/Polyrhythm/4.wav"));
    m_listPlayers[8]->setMedia(QUrl::fromLocalFile("C:/dev/Polyrhythm/2.wav"));*/
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
        m_bigCircle = scene->addEllipse(0, 0, r.width() - 1, r.height() - 1, QPen(Qt::red));

    if(m_smallCircle == nullptr)
        m_smallCircle = scene->addEllipse(r.width()/2, 0, 15, 15, QPen(Qt::black), QBrush(Qt::red));

    if(m_dotCircle == nullptr)
        m_dotCircle = scene->addEllipse(r.width()/2, 0, 1, 1, QPen(Qt::black), QBrush(Qt::blue));

    scene->addLine(r.width()/2, r.height()/2, r.width()/2, r.height()/2, QPen(Qt::red));
    QPen pen;

    switch(nbCotes)
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

    pen.setWidth(1);

    //formes a ajouter
    double x = 0;
    double y = 0;
    double halfWidth = (r.width()/2);
    double halfHeight = (r.height()/2);
    double oldx = halfWidth;
    double oldy = 0;
    int Surplus = 0;

    int *iTabPremierPolyx = new int[nbCotes+1];
    int *iTabPremierPolyy = new int[nbCotes+1];

    int it = 1;

    double rad = 0;

    for(double i = 1.0f; i <= nbCotes; i++)
    {
        rad = ((((360.0f/nbCotes)*i)+Surplus) * PI)/180.0f;
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

    double oldrad = ((((360.0f/nbCotes)/2.0f)+Surplus) * PI)/180.0f;
    oldx = halfWidth - (sin(oldrad) * (r.width()/6));
    oldy = halfHeight - (cos(oldrad) * (r.height()/6));

    for(double j = 1.0f; j <= nbCotes; j++)
    {
        m_listLines.push_back(scene->addLine((int)oldx, (int)oldy, iTabPremierPolyx[(int)j], iTabPremierPolyy[(int)j], pen));

        rad = oldrad + ((((360.0 / nbCotes) * j)) * PI)/180.0;
        x = halfWidth - (sin(rad) * (r.width()/6));
        y = halfHeight - (cos(rad) * (r.height()/6));

        m_listLines.push_back(scene->addLine((int)oldx, (int)oldy, (int)x, (int)y, pen));
        m_listLines.push_back(scene->addLine((int)x, (int)y, iTabPremierPolyx[(int)j], iTabPremierPolyy[(int)j], pen));
        oldx = x;
        oldy = y;
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
