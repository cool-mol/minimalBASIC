#include "basicwindow.h"


BasicWindow::BasicWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(400,150,600,600);
    console = new Console(this);
    console->setGeometry(0,0,600,600);
    console->write("Minimal BASIC -- Type HELP for help.\n\n");
}

BasicWindow::~BasicWindow()
{
}

