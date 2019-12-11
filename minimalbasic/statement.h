#ifndef STATEMENT_H
#define STATEMENT_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QMap>
#include "exp.h"
#include "evalstate.h"
#include <QMutex>

enum statementType { RemStmt, LetStmt, PrintStmt, InputStmt, GotoStmt, IfStmt, EndStmt };

class statement : public QObject
{
    Q_OBJECT
private:
    QMap<QString, statementType> staTable;
    Expression *recursionForExp(QStringList &exp, int x, int y);
    bool isNormal(QStringList &e, int index);
//    QMutex  _mutex;
public:
    Expression *expression;
    EvaluationContext *evaContext;
    explicit statement(QObject *parent = nullptr);
    ~statement(){}
    QString difStmtCommand(QString s);
    void convertToExp(QString s);
signals:
    void writeSignal(QString msg);
public slots:
};



#endif // STATEMENT_H
