#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <QObject>
#include <QWidget>
#include <QDebug>

struct everyLine{
    int lineNum;
    QString everyCommand;
    everyLine(){}
    everyLine(int n, QString c = ""):lineNum(n), everyCommand(c){}
};
class evalstate : public QObject
{
    Q_OBJECT
private:

    QVector<everyLine> allLine;
public:
    explicit evalstate(QObject *parent = nullptr);
    QVector<everyLine> *getLine();
signals:

public slots:
    void keepStateLine(QString str);

};

#endif // EVALSTATE_H
