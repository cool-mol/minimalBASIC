#include "evalstate.h"

evalstate::evalstate(QObject *parent) : QObject(parent)
{

}

void evalstate::keepStateLine(QString str){
    QString N = str.section(' ',0,0);
    bool numFlag;
    int num = N.toInt(&numFlag);
    str = str.section(' ',1,-1);
    if(allLine.empty()) allLine.push_back(everyLine(num, str));
    else {
        int i;
        for(i = 0;i < allLine.size();i ++){
            if(num < allLine[i].lineNum){
                allLine.insert(i,everyLine(num, str));
                break;
            }
            if(num == allLine[i].lineNum){
                allLine[i].everyCommand = str;
                break;
            }
        }
        if(i == allLine.size()) allLine.push_back(everyLine(num, str));
    }
    for(int i = 0;i < allLine.size();i ++){
        qDebug() << allLine[i].lineNum;
        qDebug() << allLine[i].everyCommand;
    }
}

QVector<everyLine> *evalstate::getLine(){
    return &allLine;
}

