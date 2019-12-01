#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <QMainWindow>
#include "console.h"
class BasicWindow : public QMainWindow
{
    Q_OBJECT
private:
    Console *console;
public:
    BasicWindow(QWidget *parent = nullptr);
    ~BasicWindow();
};
#endif // BASICWINDOW_H
