#ifndef PARSER_H
#define PARSER_H
#include <QObject>
#include <QWidget>
#include <QFile>
#include <QThread>
#include "evalstate.h"
#include "statement.h"
class parser : public QObject
{
    Q_OBJECT
private:
    evalstate *eva;
    QString firstWord;
    statement *sta;
public:
    int inputLineNum;
    explicit parser(QObject *parent = nullptr);
signals:
    void stateCommand(QString s);
    void printSignal(QString s);
    void runFunctionSignal();
    void inputSignal(QString s);
public slots:
    void parseCommand(QString msg);
    QString RunFunction(int LineNumber);
    void reserveNumber(QString num);
};

#endif // PARSER_H
