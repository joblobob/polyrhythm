#include "mainwindow.h"
#include <QApplication>
#include <QScreen> 

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MainWindow *w = new MainWindow();
	w->move(w->screen()->geometry().center() - w->frameGeometry().center());
	
    w->show();

    return a.exec();
}
