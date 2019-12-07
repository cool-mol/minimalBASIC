#include "parser.h"
#include <fstream>
#include <QDir>
#include <QThread>
#include <iterator>
parser::parser(QObject *parent) : QObject(parent)
{
    eva = new evalstate();
    connect(this,&parser::stateCommand,eva,&evalstate::keepStateLine);
    sta = nullptr;
    promptSta = new statement();
//    inputLineNum = INT_MAX;
}


void parser::parseCommand(QString msg){
    qDebug() << "abc";
    QVector<everyLine> *l = eva->getLine();
    firstWord = msg.section(' ',0,0);
    bool flag = false, flag2 = false;
    firstWord.toInt(&flag);
    msg.section(' ',1 ,1).toInt(&flag2);
    if(flag && firstWord.toInt() > 0){
        emit stateCommand(msg);
    }else if(firstWord == "RUN"){
//        emit runFunctionSignal();
        int num = -1;
        if(!l->empty()) num = (*l)[0].lineNum;
        delete promptSta;
        promptSta = new statement();
        RunFunction(num);
//        inputLineNum = -1;
    }else if(firstWord == "LIST"){
        QVector<everyLine> *l = eva->getLine();
        for(int i = 0;i < l->size();i ++){
            emit printSignal(QString::number((*l)[i].lineNum) + " " + (*l)[i].everyCommand);
        }
    }else if(firstWord == "CLEAR"){
        QVector<everyLine> *l = eva->getLine();
        QVector<everyLine> pNullVector;
        l->swap(pNullVector);
    }else if(firstWord == "HELP"){
        QDir d;
        QFile file(d.absoluteFilePath(":/help.txt"));
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            str = str.trimmed();
            str.replace(QRegExp("[\\s]+"), " ");
            qDebug()<< str;
            emit printSignal(str);
        }
    }else if(firstWord == "QUIT"){
        exit(0);
    }else if(firstWord == "PRINT"){
        QString Cmd = promptSta->difStmtCommand(msg);
        qDebug() << Cmd;
        if(Cmd.section(' ', 0, 0) == "WRONG"){
            emit printSignal(Cmd);
        }else   emit printSignal(Cmd.section(' ', 1, 1));
    }else if(firstWord == "LET"){
        if(!flag2){
            QString Cmd = promptSta->difStmtCommand(msg);
        }else {
            emit printSignal("Wrong Command! Can't be Number!");
        }
    }else if(firstWord == "INPUT"){
        promptSta->evaContext->setValue(msg.section(' ',1,1),-1);
        emit inputSignal(msg.section(' ',1,1) + " p");
    }
    else{
        emit printSignal("Wrong Command! Please type \"HELP\" to get help.");
    }


}

QString parser::RunFunction(int LineNumber){
//    if(inputLineNum == -1){
//        return "";
//    }
    if(sta == nullptr){
        sta = new statement();
    }
    QVector<everyLine> *l = eva->getLine();
    QString Cmd;
    int index = 0, line = LineNumber;
    for(index = 0;index < (*l).size();index ++){
        if(line == (*l)[index].lineNum) break;
    }
    if(!l->empty()) {
        Cmd = sta->difStmtCommand((*l)[index].everyCommand);
        while (Cmd != "end"){
            bool flag;
            flag = false;
            Cmd.toInt(&flag);
            if(Cmd == "wrong") {
                emit printSignal("WRONG:line " + QString::number(line) + " : Wrong Command!");
                break;
            }else if(flag){
                line = Cmd.toInt();
                int i;
                for(i = 0;i < l->size();i ++){
                    if(line == (*l)[i].lineNum){
                        index = i;
                        break;
                    }
                }
                if(i == l->size()){
                    emit printSignal("WRONG:line " + QString::number(line) + " : NO such LINE to GOTO!");
                    break;
                }
            }else if(Cmd.section(' ',0,0) == "print"){
                emit printSignal(Cmd.section(' ',1));
                index ++;
                if(index == l->size()) break;
                line = (*l)[index].lineNum;
            }else if(Cmd.section(' ', 0 , 0) == "input"){
                inputLineNum = (*l)[++index].lineNum;
                sta->evaContext->setValue(Cmd.section(' ',1,1),-1);
                emit inputSignal(Cmd.section(' ',1,1));
                return "input";
            }
            else if(Cmd.section(' ',0,0) == "WRONG"){
                emit printSignal("WRONG:line " + QString::number(line) + Cmd.section(' ',1));
                break;
            }
            else{
                index ++;
                if(index == l->size()) break;
                line = (*l)[index].lineNum;
            }
            for(int i = 0;i < l->size();i ++){
                if((*l)[i].lineNum == line) {
                    Cmd = sta->difStmtCommand((*l)[index].everyCommand);
                    qDebug() << "layle" << line;
                }
            }
        }
    }
    if(index == l->size()){
        delete sta;
        sta =  nullptr;
    }
    return "";
}

void parser::reserveNumber(QString num){
    bool flag = false;
    int number = num.toInt(&flag);
    if(!flag) {
        emit printSignal("Wrong Input : " + num + " !");
        return;
    }
    QMap<QString, int>::iterator i;
    statement *s;
    if(firstWord == "INPUT") s = promptSta;
    else s = sta;
    QMap<QString, int> *Table = s->evaContext->getTable();
    for(i = Table->begin(); i != Table->end();i ++){
        if(i.value() == -1){
            qDebug() << "heihei " << i.key() << num ;
            s->evaContext->setValue(i.key(),number);
        }
    }
    if(firstWord != "INPUT") {
        this->RunFunction(inputLineNum);
    }
}
