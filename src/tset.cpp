// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <iostream>
#include <stdexcept>

// Конструктор
TSet::TSet(int mp) :BitField(mp)
{
    if(mp < 0) throw std::out_of_range("negative");
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
}

// конструктор преобразования типа (из битового поля)
TSet::TSet(const TBitField& bf) : BitField(bf)
{
}

// Оператор преобразования в TBitField
TSet::operator TBitField()
{
    return BitField;
}
int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return BitField.GetLength();
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem <0 || Elem >= GetMaxPower())  throw std::out_of_range("e out of range");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem <0 || Elem>=GetMaxPower()) throw std::out_of_range("e out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= GetMaxPower()) throw std::out_of_range("e out of range");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if(this == &s) return *this;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(BitField == s.BitField);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

std::istream& operator>>(std::istream& istr, TSet& s) // ввод
{
    int elem;
    while(istr >> elem) {
        if (elem >= 0 && elem < s.GetMaxPower()) {
            s.InsElem(elem);
        }
    }
    istr.clear();
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) // вывод
{
    ostr << '{';
    int f = 1;
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if(s.IsMember(i)) {
            if (!f) ostr << ' ';
            ostr << i;
            f = 0;
        }
    }
    ostr << '}';
    return ostr;
}