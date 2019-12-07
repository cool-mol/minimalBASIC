#include "statement.h"
#include <QStringList>
#include <QMap>
//#                   _ooOoo_
//#                  o8888888o
//#                  88" . "88
//#                  (| -_- |)
//#                  O\  =  /O
//#               ____/`---'\____
//#             .'  \\|     |//  `.
//#            /  \\|||  :  |||//  \
//#           /  _||||| -:- |||||-  \
//#           |   | \\\  -  /// |   |
//#           | \_|  ''\---/''  |   |
//#           \  .-\__  `-`  ___/-. /
//#         ___`. .'  /--.--\  `. . __
//#      ."" '<  `.___\_<|>_/___.'  >'"".
//#     | | :  `- `.;`\ _ /`;.`/ - ` : | |
//#     \  \ `-.   \_ __\ /__ _/   .-` /  /
//#======`-.____`-.___\_____/___.-`____.-'======
//#                   `=---='
//#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//#  佛祖保佑 代码高性能 不宕机 无bug
statement::statement(QObject *parent) : QObject(parent)
{
    staTable["REM"] = RemStmt;
    staTable["LET"] = LetStmt;
    staTable["PRINT"] = PrintStmt;
    staTable["INPUT"] = InputStmt;
    staTable["GOTO"] = GotoStmt;
    staTable["IF"] = IfStmt;
    staTable["END"] = EndStmt;
    evaContext = new EvaluationContext();
}

QString statement::difStmtCommand(QString s)
{
    QString firstWord, otherWord;
    firstWord = s.section(' ', 0, 0);
    otherWord = s.section(' ', 1, -1);
    bool flag = false;
    otherWord.toInt(&flag);
    if (!staTable.contains(firstWord))
    {
        return "wrong";
    }
    else
    {
        try
        {
            switch (staTable[firstWord])
            {

            case RemStmt:
                break;

            case LetStmt:
                convertToExp(otherWord);
                expression->eval(*evaContext);
                delete expression;
                break;

            case PrintStmt:
            {
                convertToExp(otherWord);
                int num = expression->eval(*evaContext);
                delete expression;
                return "print " + QString::number(num);
            }

            case InputStmt:
                if(otherWord.section(' ',0,0) != otherWord.section(' ',-1,-1)){
                    return "wrong";
                }
                if(flag) throw QString(" can't be a number.");

                return QString("input ") + otherWord;

            case GotoStmt:
                if(!flag) throw QString(" no such number.");
                return s.section(' ', 1, 1);
            case IfStmt:
            {
                if (s.section(' ', -2, -2) != "THEN")
                {
                    return "wrong";
                }
                int exp1;
                int exp2;
                QString exp1_str;
                QString exp2_str;
                QStringList l = s.split(' ');
                int i;
                for (i = 0; i < l.size(); i++)
                    if (l[i] == "<" || l[i] == ">" || l[i] == "<=" || l[i] == ">=" || l[i] == "==" || l[i] == "!=")
                        break;
                exp1_str = s.section(' ', 1, i - 1);
                exp2_str = s.section(' ', i + 1, -3);
                convertToExp(exp1_str);
                exp1 = expression->eval(*evaContext);
                delete expression;
                convertToExp(exp2_str);
                exp2 = expression->eval(*evaContext);
                delete expression;
                if (l[i] == "<" && exp1 < exp2)
                    return s.section(' ', -1, -1);
                else if (l[i] == ">" && exp1 > exp2)
                    return s.section(' ', -1, -1);
                else if (l[i] == "<=" && exp1 <= exp2)
                    return s.section(' ', -1, -1);
                else if (l[i] == ">=" && exp1 >= exp2)
                    return s.section(' ', -1, -1);
                else if (l[i] == "==" && exp1 == exp2)
                    return s.section(' ', -1, -1);
                else if (l[i] == "!=" && exp1 != exp2)
                    return s.section(' ', -1, -1);
                else
                    break;
            }

            case EndStmt:
                return "end";
            }
        }
        catch (QString s)
        {
            return QString("WRONG : ") + s;
        }
    }
    return "";
}

void statement::convertToExp(QString s){
    QStringList holeExpression;
    QVector<QString> operators;
    QVector<QString> operands;

    // 将输入进来的字符串初始化，放在holeExpression里
    for(int i = 0;i < s.size();i ++){
        if(s[i] == '=' || s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '(' || s[i] == ')'){
            s.insert(i++, ' ');
            s.insert(i + 1, ' ');
        }
    }
    s = s.trimmed();
    s.replace(QRegExp("[\\s]+"), " ");
    holeExpression = s.split(' ');
    expression = recursionForExp(holeExpression, 0, holeExpression.size());
}

// 思前想后，还是用递归来完成吧，不然感觉套不上老师的ppt
Expression *statement::recursionForExp(QStringList &exp, int x, int y)
{
    if (y - x == 1)
    { // 只剩一个的时候一定是叶子节点
        Expression *e;
        bool flag = false;
        exp[x].toInt(&flag);
        if (flag)
        {
            e = new ConstantExp(exp[x].toInt());
        }
        else
        {
            e = new IdentifierExp(exp[x]);
        }
        return e;
    }
    /*
           local_r记录当前要转化的表达式生成二叉树的根节点操作符的位置
           bracketFlag记录是否当前搜索在括号里面
           m_m_p记录当前表达式中括号外面最右边的+、-位置
           a_s_p记录当前表达式中括号外面最右边的*、/位置
           e_p记录当前表达式中括号最右边的=位置
    */
    int local_r = 0;
    int m_m_p = 0, a_s_p = 0, e_p = 0;
    int bracketFlag = 0; // 是否在括号外面，如果在括号外面则为0
    for (int i = x; i < y; i++)
    {
        if (exp[i] == "(")
            bracketFlag++;
        else if (exp[i] == ")")
            bracketFlag--;
        if (bracketFlag == 0)
        {
            if (exp[i] == "*" || exp[i] == "/")
                a_s_p = i;
            else if (exp[i] == "+" || exp[i] == "-")
                m_m_p = i;
            else if (exp[i] == "=")
                e_p = i;
        }
    }
    if ((m_m_p == 0) && (a_s_p == 0) && (e_p == 0))
        //如果式子整个有括号如(5-2*3+7)，即括号外面没有操作符，则去掉括号找二叉树
        return recursionForExp(exp, x + 1, y - 1);
    else
    {
        //如果有+或者-，则根节点为最右边的+或-，否则是最右边的*或/
        if (e_p > 0)
            local_r = e_p;
        else if (m_m_p > 0)
            local_r = m_m_p;
        else if (a_s_p > 0)
            local_r = a_s_p;
        //确定根节点和根节点的左孩子和右孩子
        Expression *b = new CompoundExp(exp[local_r], recursionForExp(exp, x, local_r), recursionForExp(exp, local_r + 1, y));
        return b;
    }
}
