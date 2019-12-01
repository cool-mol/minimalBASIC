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
public:
    statement *sta;
    explicit parser(QObject *parent = nullptr);
signals:
    void stateCommand(QString s);
    void wrongCase(QString s);
    void printSignal(QString s);
    void runFunctionSignal();
public slots:
    void parseCommand(QString msg);
    void writeFromExp(QString msg);
    QString RunFunction();
};

#endif // PARSER_H
