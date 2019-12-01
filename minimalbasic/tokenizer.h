#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QObject>
#include <QWidget>
#include <QString>
class tokenizer : public QObject
{
    Q_OBJECT
private:
    QString str;
public:
    explicit tokenizer(QObject *parent = nullptr);
    QString stringSection();
signals:

public slots:
};

#endif // TOKENIZER_H
