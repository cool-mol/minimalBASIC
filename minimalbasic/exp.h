#ifndef EXP_H
#define EXP_H

#include <QObject>
#include <QWidget>
#include <QMap>

class EvaluationContext;

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

class Expression : public QObject
{
    Q_OBJECT
public:
    explicit Expression(QObject *parent = nullptr);
    virtual ~Expression();
    virtual int eval(EvaluationContext & context) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    /* Getter methods for convenience */

    virtual int getConstantValue(){return 0;}
    virtual QString getIdentifierName(){return "";}
    virtual QString getOperator(){return "";}
    virtual Expression *getLHS(){return nullptr;}
    virtual Expression *getRHS(){return nullptr;}

signals:
    void writeSignal(QString s);
public slots:
};

class ConstantExp: public Expression {

public:

   ConstantExp(int val);

   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();

   virtual int getConstantValue();

private:

   int value;

};

class IdentifierExp: public Expression {

public:

   IdentifierExp(QString n);

   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();

   virtual QString getIdentifierName();

private:

   QString name;

};


class CompoundExp: public Expression {

public:

   CompoundExp(QString op = " ", Expression *lhs = nullptr, Expression *rhs = nullptr);
   virtual ~CompoundExp();

   virtual int eval(EvaluationContext & context);
   virtual QString toString();
   virtual ExpressionType type();

   virtual QString getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();
   virtual int Pow(int x, int y);
private:

   QString op;
   Expression *lhs, *rhs;

};

class EvaluationContext {

public:

   void setValue(QString var, int value);
   int getValue(QString var);
   bool isDefined(QString var);
    QMap<QString,int> *getTable();
private:

   QMap<QString,int> symbolTable;

};

#endif // EXP_H
