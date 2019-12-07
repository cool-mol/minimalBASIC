#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <QMainWindow>
#include "console.h"
#include <QLayout>
#include <QSizePolicy>
class BasicWindow : public QMainWindow
{
    Q_OBJECT
private:
    Console *console;
    parser *Parser;
protected:
    void resizeEvent(QResizeEvent *);

public:
    BasicWindow(QWidget *parent = nullptr);
    ~BasicWindow();
signals:
    void inputNumSignal(QString num);
public slots:
    void callFromParser(QString msg);
    void callFromConsole(QString msg);
};
#endif // BASICWINDOW_H
