// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>

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
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("elem out of range");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("elem out of range");
    if (Elem >= 0 && Elem < MaxPower) BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) throw std::out_of_range("elem out of range");
    if (Elem >= 0 && Elem < MaxPower) BitField.ClrBit(Elem);
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
    if (MaxPower == s.MaxPower && BitField == s.BitField) return 1;
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return (*this == s) == 1 ? 0 : 1;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int len = MaxPower >= s.MaxPower ? MaxPower : s.MaxPower;
    TSet res(len);

    res.BitField = BitField | s.BitField;

    return res;
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

TSet TSet::operator*(const TSet &s) // пересечение
{
    int len = MaxPower >= s.MaxPower ? MaxPower : s.MaxPower;
    TSet res(len);

    res.BitField = BitField & s.BitField;

    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet res(MaxPower);

    res.BitField = ~BitField;

    return res;
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
