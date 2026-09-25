// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
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
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Index out of bounds";
    }
    if (BitField.GetBit(Elem) != 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Index out of bounds";
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Index out of bounds";
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower) {
        return 0;
    }
    if (BitField == s.BitField) {
        return 1;
    }
    else {
        return 0;
    }
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if ((*this) == s) {
        return 0;
    }
    else {
        return 1;
    }
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TBitField tempField = BitField | s.BitField;
    TSet temp(tempField);
    return temp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Index out of bounds";
    }
    TSet temp(*this);
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw "Index out of bounds";
    }
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TBitField tempField = BitField & s.BitField;
    TSet temp(tempField);
    return temp;
}

TSet TSet::operator~(void) // дополнение
{
    TBitField tempField = ~BitField;
    TSet temp(tempField);
    return temp;
}

// перегрузка ввода/вывода

std::istream& operator>>(std::istream& istr, TSet& s) // ввод
{
    int elem;
    while (istr >> elem) {
        if (elem >= 0 && elem < s.MaxPower) {
            s.InsElem(elem);
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) // вывод
{
    int first = 1;
    ostr << "{";
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i) != 0) {
            if (first == 0) {
                ostr << ", ";
            }
            ostr << i;
            first = 0;
        }
    }
    ostr << "}";
    return ostr;
}
