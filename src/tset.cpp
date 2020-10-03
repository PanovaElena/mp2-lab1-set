// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem > -1) && (Elem < MaxPower))
        return BitField.GetBit(Elem);
    else throw "incorrect Elem";
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem > -1) && (Elem < MaxPower))
        BitField.SetBit(Elem);
    else throw "incorrect Elem";
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem > -1) && (Elem < MaxPower))
        BitField.ClrBit(Elem);
    else throw "incorrect Elem";
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if ((BitField == s.BitField) && (MaxPower == s.MaxPower))
        return 1;
    else return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (*this == s) return 0;
    else
        return 1;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet temp(BitField | s.BitField);
    return temp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet temp(*this);
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet temp(BitField & s.BitField);
    return temp;
}

TSet TSet::operator~(void) // дополнение
{
    TSet temp(~BitField);
    return temp;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int temp;
    char ch;
    do { istr >> ch; } while (ch != '{');
    do {
        istr >> temp; s.InsElem(temp);
        do { istr >> ch; } while ((ch != ',') && (ch != '}'));
    } while (ch != '}');
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    int i, n;
    char ch = ' ';
    ostr << '{';
    n = s.GetMaxPower();
    for (int i = 0; i < n; i++) {
        if (s.IsMember(i)) { ostr << ch << ' ' << i; ch = ','; }
    }
    ostr << '}';
    return ostr;
}
