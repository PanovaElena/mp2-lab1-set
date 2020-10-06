// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// конструктор по умолчанию
TSet::TSet(int mp): BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf): BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    TBitField result(BitField);
    return result;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // проверить наличие элемента в множестве
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    return BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    return BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.GetMaxPower();
    BitField = s.BitField;
    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet result(BitField | s.BitField);
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
        BitField.SetBit(Elem);
        TSet result(BitField);
        return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
        BitField.ClrBit(Elem);
        TSet result(BitField);
        return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet result(BitField & s.BitField);
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(~BitField);
    return result;
}

// перегрузка ввода/вывода ({ i1, i2, ..., in})

istream &operator>>(istream &istr, TSet &s) // ввод 
{
    int Elem;
    char c;
    do {
        istr >> c; 
    } while (c != '{');
    do {
        istr >> Elem;
        s.InsElem(Elem);
        do {
            istr >> c;
        } while ((c != ',') && (c != '}'));
    } while (c != '}');
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    char c = '{';
    for (int i = 0; i < s.GetMaxPower(); i++)
        if (s.IsMember(i))
        {
            ostr << c << ' ' <<  i;
            c = ',';
        }
    ostr << " }";
    return ostr;
}
