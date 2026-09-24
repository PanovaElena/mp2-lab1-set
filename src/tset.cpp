// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
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
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) const // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) const // объединение с элементом
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) const // разность с элементом
{
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s) const // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) const // дополнение
{
    return TSet(~BitField);
}

TSet TSet::operator^(const TSet& s) const // симметрическая разность
{
    return TSet(BitField ^ s.BitField);
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    int elem;
    s.BitField = TBitField(s.MaxPower);
    while (istr >> elem) {
        if (elem >= 0 && elem < s.MaxPower) {
            s.InsElem(elem);
        }
        else {
            break;
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << "{";
    bool isFirst = true;
    for (int i = 0; i < s.MaxPower; ++i) {
        if (s.IsMember(i)) {
            if (!isFirst) ostr << ", ";
            ostr << i;
            isFirst = false;
        }
    }
    ostr << "}";
    return ostr;
}
