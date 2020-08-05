#include <iostream>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cctype>


//  Выводит ошибку и выходит из программы
void handle_error()
{
    std::cout << "error" << std::endl;
    exit(1);
}


//  Типы лексем
enum class TLexem
{
    START,  //  начало выражения
    NUM,    //  число
    ADD,    //  сложение
    SUB,    //  вычитание
    MUL,    //  умножение
    DIV,    //  деление
    MINUS,  //  унарный минус
    OPEN,   //  открывающая скобка
    CLOSE,  //  закрывающаяся скобка
    END     //  конец выражения
};


//  Класс для высчитывания выражения
class Calculator
{
    TLexem curlex;      //  Тип текущей лексемы
    int64_t curvalue;   //  Текущее значение
    int64_t cursign;    //  Текущий знак (для корректной работы с унарным минусом)
    std::string expr;   //  Выражение
    size_t exprPos;     //  Позиция в выражении
    
    void nextlexem();       //  Получение очередной лексемы
    int64_t add_sub_expr(); //  Разбор на слагаемые
    int64_t mul_div_expr(); //  Разбор на множители
    int64_t atom_expr();    //  Разбор на множителя: -множитель|(выражение)|число
    
public:
    Calculator(const char* expression);
    int64_t calculate();    //  Вычисление выражения
};


//  Конструктор
Calculator::Calculator(const char* expression)
{
    curlex = TLexem::START;
    curvalue = 0;
    cursign = 1;
    exprPos = 0;
    expr = std::string(expression);
}


//  Получение лексемы
void Calculator::nextlexem()
{
    //  Пропускаем пробелы
    while (exprPos < expr.length() && isspace(expr[exprPos]))
    {
        exprPos++;
    }

    //  Если конец строки -> конец разбора
    if (exprPos == expr.length())
    {
        curlex = TLexem::END;
    }
    //  Если '(' или ')'
    else if (expr[exprPos] == '(')
    {
        curlex = TLexem::OPEN;
        exprPos++;
    }
    else if (expr[exprPos] == ')')
    {
        curlex = TLexem::CLOSE;
        exprPos++;
    }
    //  Если одна из операций: +, -, *, /
    else if (expr[exprPos] == '+')
    {
        curlex = TLexem::ADD;
        exprPos++;
    }
    else if (expr[exprPos] == '-')
    {
        //  Если тек лексема число или закрывающая скобка, то вычитание
        if (curlex == TLexem::NUM || curlex == TLexem::CLOSE)
        {
            curlex = TLexem::SUB;
        }
        //  Иначе унарный минус
        else
        {
            curlex = TLexem::MINUS;
        }
        exprPos++;
    }
    else if (expr[exprPos] == '*')
    {
        curlex = TLexem::MUL;
        exprPos++;
    }
    else if (expr[exprPos] == '/')
    {
        curlex = TLexem::DIV;
        exprPos++;
    }
    //  Если число
    else if (isdigit(expr[exprPos]))
    {
        curlex = TLexem::NUM;
        curvalue = 0;
        while (exprPos < expr.length() && isdigit(expr[exprPos]))
        {
            curvalue = curvalue * 10 + expr[exprPos] - '0';
            exprPos++;
        }
    }
    //  Иначе неизвестный символ -> ошибка
    else
    {
        handle_error();
    }
}


//  Разбор на слагаемые
int64_t Calculator::add_sub_expr()
{
    //  Разбираем на слагаемые и получаем результат
    int64_t value = mul_div_expr();

    //  Пока сложение или вычитание
    while (curlex == TLexem::ADD || curlex == TLexem::SUB)
    {
        TLexem operation = curlex;
        //  Получаем следующею лексему
        nextlexem();
        //  И выполняем операцию
        if (operation == TLexem::ADD)
        {
            value += mul_div_expr();
        }
        else
        {
            value -= mul_div_expr();
        }
    }
    
    //  Возвращаем результат
    return value;
}


//  Разбор на множители
int64_t Calculator::mul_div_expr()
{
    //  Разбираем на множители и получаем результат
    int64_t value = atom_expr();

    //  Пока умножение или деление
    while (curlex == TLexem::MUL || curlex == TLexem::DIV)
    {
        TLexem operation = curlex;
        //  Получаем следующею лексему
        nextlexem();
        //  И выполняем операцию
        if (operation == TLexem::MUL)
        {
            value *= mul_div_expr();
        }
        else
        {
            int64_t tmp = mul_div_expr();
            if (tmp == 0)
            {
                handle_error();
            }
            value /= tmp;
        }
    }
    
    return value;
}


//  Разбор множителя
int64_t Calculator::atom_expr()
{
    int64_t value;

    switch (curlex)
    {
    //  Если унарный минус, то учитываем это, продолжаем разбор множителя
    case TLexem::MINUS:
        cursign = -cursign;
        nextlexem();
        value = atom_expr();
        break;
    //  Если число, вычисляем выражение
    case TLexem::NUM:
        value = curvalue * cursign;
        cursign = 1;    //  сброс знака
        nextlexem();
        break;
    //  Если открывающаяся скобка, то подвыражение
    case TLexem::OPEN:
        nextlexem();
        //  разбираем бодвыражение
        value = add_sub_expr();
        //  если вконце закрывающая скобка, то ОК
        if (curlex == TLexem::CLOSE)
        {
            nextlexem();
        }
        //  Иначе ошибка
        else
        {
            handle_error();
        }
        break;
    //  Неверная лексема
    default:
        handle_error();
    }

    return value;
}


//  Вычисление выражения
int64_t Calculator::calculate()
{
    //  Получаем первую лексему
    nextlexem();

    //  Начинаем разбор
    int64_t result = add_sub_expr();

    //  Если мы не в конце выражения, то ошибка
    if (curlex != TLexem::END)
    {
        handle_error();
    }

    return result;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        handle_error();
    }

    Calculator calc(argv[1]);
    std::cout << calc.calculate() << std::endl;

    return 0;
}
