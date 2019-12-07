#include "basicwindow.h"


BasicWindow::BasicWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(400,150,600,600);
    console = new Console(this);
    console->setGeometry(0,0,600,600);
    console->write("Minimal BASIC -- Type HELP for help.\n\n");

    Parser = new parser();
    connect(console,&Console::newLineWritten,Parser,&parser::parseCommand);
    connect(Parser,&parser::printSignal,console,&Console::write);
    connect(Parser,&parser::inputSignal,this,&BasicWindow::callFromParser);
    connect(this,&BasicWindow::inputNumSignal,Parser,&parser::reserveNumber);
    this->setGeometry(400,150,600,601);
}

void BasicWindow::resizeEvent(QResizeEvent *){
    console->setGeometry(0,0,this->width(),this->height());

}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::callFromParser(QString msg){
    qDebug() << "BasicWindow::callFromParser " << msg;
    disconnect(console,&Console::newLineWritten,Parser,&parser::parseCommand);
    connect(console,&Console::newLineWritten,this,&BasicWindow::callFromConsole);
}

void BasicWindow::callFromConsole(QString msg){
    qDebug() << "BasicWindow::callFromConsole " << msg;
    connect(console,&Console::newLineWritten,Parser,&parser::parseCommand);
    disconnect(console,&Console::newLineWritten,this,&BasicWindow::callFromConsole);
    emit inputNumSignal(msg);
}
