
// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Конструктор
TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// Конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// Конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

// Преобразование TSet -> TBitField
TSet::operator TBitField()
{
    return BitField;
}

// Получить максимальную мощность множества
int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

// Проверить, принадлежит ли элемент множеству
int TSet::IsMember(const int Elem) const
{
    return BitField.GetBit(Elem);
}

// Включить элемент в множество
void TSet::InsElem(const int Elem)
{
    BitField.SetBit(Elem);
}

// Удалить элемент из множества
void TSet::DelElem(const int Elem)
{
    BitField.ClrBit(Elem);
}

// Присваивание
TSet& TSet::operator=(const TSet &s)
{
    if (this != &s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }

    return *this;
}

// Сравнение
int TSet::operator==(const TSet &s) const
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

// Неравенство
int TSet::operator!=(const TSet &s) const
{
    return !(*this == s);
}

// Объединение с элементом
TSet TSet::operator+(const int Elem)
{
    TSet tmp(*this);
    tmp.InsElem(Elem);
    return tmp;
}

// Разность с элементом
TSet TSet::operator-(const int Elem)
{
    TSet tmp(*this);
    tmp.DelElem(Elem);
    return tmp;
}

// Объединение множеств
TSet TSet::operator+(const TSet &s)
{
    int newMaxPower = MaxPower > s.MaxPower ? MaxPower : s.MaxPower;

    TSet tmp(newMaxPower);
    tmp.BitField = BitField | s.BitField;

    return tmp;
}

// Пересечение множеств
TSet TSet::operator*(const TSet &s)
{
    int newMaxPower = MaxPower > s.MaxPower ? MaxPower : s.MaxPower;

    TSet tmp(newMaxPower);
    tmp.BitField = BitField & s.BitField;

    return tmp;
}

// Дополнение
TSet TSet::operator~(void)
{
    TSet tmp(*this);
    tmp.BitField = ~BitField;
    return tmp;
}

// Ввод
std::istream &operator>>(std::istream &istr, TSet &s)
{
    istr >> s.BitField;
    return istr;
}

// Вывод
std::ostream &operator<<(std::ostream &ostr, const TSet &s)
{
    ostr << s.BitField;
    return ostr;
}
