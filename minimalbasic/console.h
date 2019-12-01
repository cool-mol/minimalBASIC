#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include "parser.h"

class Console : public QTextEdit
{
    Q_OBJECT
private:
    parser *Parser;
public:
    explicit Console(QWidget *parent = nullptr);

signals:
//    void computerRound();
    void newLineWritten(QString newline);
public slots:
//    void clear();
    void write(QString msg);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};
#endif // CONSOLE_H
