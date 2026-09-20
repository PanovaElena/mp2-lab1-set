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
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField() const
{
    return BitField; 
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) const // объединение
{
    TSet temp(BitField | s.BitField);
    return temp;
}

TSet TSet::operator+(const int Elem) const// объединение с элементом
{
    TSet temp(*this);
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem)const // разность с элементом
{
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet &s) const// пересечение
{
    TSet temp(BitField & s.BitField);
    return temp;
}

TSet TSet::operator~(void) const // дополнение
{
    TSet temp(~BitField);
    return temp;
}

// перегрузка ввода/вывода

TSet TSet::operator^(const TSet &s) const {
    TSet temp(BitField ^ s.BitField);
    return temp;
}

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    int temp;
    while (istr >> temp && temp >= 0 && temp < s.GetMaxPower()) {
        s.InsElem(temp);
    }
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            ostr << i << " ";
        }
    }
    return ostr;
}
