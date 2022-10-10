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
TSet::TSet(const TSet& s) : BitField(s.MaxPower)
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf.GetLength())
{
    MaxPower = bf.GetLength();
    BitField = bf;
}

TSet::operator TBitField()
{
    TBitField rar(MaxPower);
    rar = BitField;

    return rar;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (BitField.GetBit(Elem) == 1)
        return 1;
    else
        return 0;
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

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (*this == s)
        return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;

    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower)
        return 0;
    else if ((MaxPower == s.MaxPower) && (BitField == s.BitField))
        return 1;

    return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{

    TSet rar(MaxPower > s.MaxPower ? MaxPower : s.MaxPower);
    rar.BitField = BitField | s.BitField;

    return rar;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet rar(MaxPower);
    rar.BitField = BitField;
    rar.InsElem(Elem);

    return rar;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet rar(MaxPower);
    rar.BitField = BitField;
    rar.DelElem(Elem);

    return rar;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet rar(MaxPower > s.MaxPower ? MaxPower : s.MaxPower);
    rar.BitField = BitField & s.BitField;

    return rar;
}

TSet TSet::operator~(void) // дополнение
{
    TSet rar(MaxPower);
    rar.BitField = ~BitField;

    return rar;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << s.BitField;

    return ostr;
}