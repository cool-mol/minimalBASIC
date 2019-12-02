#include "console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>
#include <QPalette>
#include <QFont>
#include "statement.h"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    //connect(this,&Console::newLineWritten,this,&Console::write);

    this->setTextBackgroundColor(QColor(QString("black")));
    this->setTextColor(QColor(QString("white")));
    this->setFont(QFont(QString("Consolas")));
}

void Console::keyPressEvent(QKeyEvent *event)
{
    //退格和delete都是把这行清空
    this->setTextColor(QColor(QString("lightblue")));
    if (event->key() == Qt::Key_Backspace){
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        return;
    }
    if (event->key() == Qt::Key_Delete){
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        return;
    }
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        this->setTextColor(QColor(QString("white")));
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        //如果输入.的话 电脑跑一轮
//        if(lastLine == ".")
//            emit computerRound();
//        else
        // 将输入进来的内容都替换成标准格式
        lastLine = lastLine.trimmed();
        lastLine.replace(QRegExp("[\\s]+"), " ");
        emit newLineWritten(lastLine);
        //newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}

void Console::write(QString msg)
{
    this->append(msg);
}

void Console::inputWaitFunction(QString msg){
    qDebug() << "input " << msg;

}
