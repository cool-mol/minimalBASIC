#include "parser.h"
#include <fstream>
#include <QDir>
#include <QThread>
parser::parser(QObject *parent) : QObject(parent)
{
    eva = new evalstate();
    connect(this,&parser::stateCommand,eva,&evalstate::keepStateLine);
    connect(this,&parser::runFunctionSignal,this,&parser::RunFunction);
}


void parser::parseCommand(QString msg){
    qDebug() << "abc";
    firstWord = msg.section(' ',0,0);
    bool flag = false;
    firstWord.toInt(&flag);
    if(flag){
        emit stateCommand(msg);
    }else if(firstWord == "RUN"){
//        emit runFunctionSignal();
        RunFunction();
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
    }else{
        emit printSignal("Wrong Command! Please type \"HELP\" to get help.");
    }


}

QString parser::RunFunction(){
    sta = new statement();
    QVector<everyLine> *l = eva->getLine();
    QString Cmd;
    int index = 0, line;
    if(!l->empty()) {
        line = (*l)[0].lineNum;
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
            }else if(Cmd.section(' ',0,0) == "WRONG"){
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
    delete sta;
    return "";
}


void parser::writeFromExp(QString msg){
    emit printSignal(msg);
}
