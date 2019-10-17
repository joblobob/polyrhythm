#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsEllipseItem>
#include <QtMultimedia/QMediaPlayer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_btnAdd_clicked();

    void on_btnClear_clicked();

    void on_btnGo_clicked();

    void on_btnTimer_clicked();

    void on_m_tstSpd_textChanged(const QString &arg1);

private:

    void init();
    void addMedia(const QString& pathToMedia);

    Ui::MainWindow *ui;

    QGraphicsEllipseItem * m_bigCircle;
    QGraphicsEllipseItem * m_smallCircle;
    QGraphicsEllipseItem * m_dotCircle;
    QVector<QGraphicsLineItem*> m_listLines;
    QVector<QPen> m_pens;
    double m_pct;
    bool m_play;
    int m_spd;
    QMap<int, QMediaPlayer*> m_listPlayers;
    QTimer* m_speedTimer;

};

#endif // MAINWINDOW_H
