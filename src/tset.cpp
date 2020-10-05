// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if (mp > 0)
    {
        MaxPower = mp;
    }  
    else throw "Negative cardinality!"; //отрицательная мощность множества
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

//преобразование типа к битовому полю (к TBitField)
TSet::operator TBitField()
{
    TBitField tmp(this->BitField);
    return tmp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem >= 0) && (Elem < MaxPower))
    {
        int result;
        if (BitField.GetBit(Elem)) //если больше нуля - принадлежит
            result = true;
        else
            result = false;
        return result;
        //или просто return BitField.GetBit(Elem);
    }
    else throw "Element number is uncorrect!";
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem >= 0) && (Elem < MaxPower))
        BitField.SetBit(Elem);
    else throw "Element number is uncorrect!";
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem >= 0) && (Elem < MaxPower))
        BitField.ClrBit(Elem);
    else throw "Element number is uncorrect!";
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    int result = true;
    if (MaxPower != s.MaxPower)
        result = false;
    else
        if (BitField != s.BitField) //если не равны, то будет больше нуля, то сработает if, и результат будет false
            result = false;
    //или просто return (BitField==s.BitField)
    return result;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    int  result = false;
    if (MaxPower != s.MaxPower)
        result = true;
    else
        if (BitField != s.BitField)
            result = true;
    return result;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet a(*this);  //чтобы не портить исходное множество
    a.BitField = (a.BitField | s.BitField);
    a.MaxPower = a.BitField.GetLength();    //т.к. значение могло измениться в ходе суммирования
    return a;
}

TSet TSet::operator+(const int Elem) // объединение с элементом (включение элемента в множество)
{
    //проверка на правильность значения Elem не нужна, т.к. она есть в функции InsElem. Если что, исключение вызовется оттуда
    TSet tmp(*this);
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    //проверка на правильность значения Elem не нужна, т.к. она есть в функции DelElem. Если что, исключение вызовется оттуда
    TSet tmp(*this);
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet a(*this);
    a.BitField = (a.BitField & s.BitField);
    a.MaxPower = a.BitField.GetLength();    //т.к. значение могло измениться в ходе умножения
    return a;
}

TSet TSet::operator~(void) // дополнение
{
    TSet a(*this);
    a.BitField = (~this->BitField);
    return a;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{   //формат данных - { i1, i2, .., in }
    int temp; char ch;
    //поиск {
        do { istr >> ch; } while (ch != '{');
        do {     //ввод элементов и включение в множество
            istr >> temp; s.InsElem(temp);
            do { istr >> ch; } while ((ch != ',') && (ch != '}'));
        } while (ch != '}');
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{   //формат данных - { i1, i2, .., in }
    int i, n; char ch = ' ';
    ostr << "{";
    n = s.GetMaxPower();
    for ( i=0; i<n; i++)    //вывод элементов
        if (s.IsMember(i)) { ostr << ch << ' ' << i; ch = ','; }
    ostr << "}";
    return ostr;
}
