// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>
#include <string>
#include <sstream>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    if (mp < 0) throw std::length_error("The power of a set cannot be negative");
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
    if (Elem < 0 || Elem >= MaxPower) return 0;
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("Element index out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("Element index out of range");
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
    return (MaxPower != s.MaxPower) || (BitField != s.BitField);
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

TSet TSet::operator+(const TSet& s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    for (int i = 0; i < s.MaxPower; i++) {
        s.DelElem(i);
    }

    std::string str;
    if (std::getline(istr, str)) {
        std::stringstream ss(str);
        int elem;
        while (ss >> elem) {
            s.InsElem(elem);
        }
    }

    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << '{';
    int i = 0;
    while (!s.IsMember(i)) i++;
    ostr << i;

    while (i < s.MaxPower) {
        if (s.IsMember(i)) ostr << ", " << i;
        i++;
    }
    ostr << '}';

    return ostr;
}
