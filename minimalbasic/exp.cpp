#include "exp.h"
#include <QtMath>
/*
 * Functions in Expression are defined here.
*/
Expression::Expression(QObject *parent) : QObject(parent){}
Expression::~Expression(){}


/*
 * Functions in ConstantExp are defined here.
*/

ConstantExp::ConstantExp(int val) {value = val;}
int ConstantExp::eval(EvaluationContext & context) {return value;}
QString ConstantExp::toString() {return QString::number(value);}
ExpressionType ConstantExp::type() {return CONSTANT;}
int ConstantExp::getConstantValue() {return value;}

/*
 * Functions in IdentifierExp are defined here.
*/

IdentifierExp::IdentifierExp(QString n) {name = n;}

int IdentifierExp::eval(EvaluationContext & context) {
    if (!context.isDefined(name)) throw(name + QString(" is undefined"));
       return context.getValue(name);
}

QString IdentifierExp::toString(){return "";}
ExpressionType IdentifierExp::type(){return IDENTIFIER;}

QString IdentifierExp::getIdentifierName(){return name;}


/*
 * Functions in CompoundExp are defined here.
*/
CompoundExp::CompoundExp(QString o, Expression *l, Expression *r){op = o;lhs = l; rhs= r;}
CompoundExp::~CompoundExp(){}

int CompoundExp::eval(EvaluationContext & context){
    int right = rhs->eval(context);
       if (op == "=") {
          context.setValue(lhs->getIdentifierName(), right);
          return right;
       }
       int left = lhs->eval(context);
       if (op == "+") return left + right;
       if (op == "-") return left - right;
       if (op == "*") return left * right;
       if (op == "/") {
          if (right == 0) {
              throw(QString("Division by 0"));
          }
          return left / right;
       }
       if (op == "**") {
           return Pow(left, right);
       }
       throw (QString("Illegal operator in expression"));

}
QString CompoundExp::CompoundExp::toString(){return "";}
ExpressionType CompoundExp::type(){return COMPOUND;}
int CompoundExp::Pow(int x, int y){
    qreal X = x;
    qreal Y = y;
    qreal ret = qPow(X, Y);
    int r = ret;
    return r;
}
QString CompoundExp::getOperator(){return op;}
Expression *CompoundExp::getLHS(){return lhs;}
Expression *CompoundExp::getRHS(){return rhs;}

/*
 * Functions in EvaluationContext are defined here.
*/
void EvaluationContext::setValue(QString var, int value){symbolTable[var] = value;}
int EvaluationContext::getValue(QString var){return symbolTable[var];}
bool EvaluationContext::isDefined(QString var){return symbolTable.contains(var);}
QMap<QString,int>* EvaluationContext::getTable(){return &symbolTable;}
