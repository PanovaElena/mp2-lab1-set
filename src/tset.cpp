// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>

TSet::TSet(int mp) : BitField(0), MaxPower(mp)
{
    TBitField tmp(MaxPower);
    BitField = tmp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(0)
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return TBitField(BitField);
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
    else
        return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (*this == s)
        return 0;
    else
        return 1;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int sz;
    if (MaxPower > s.MaxPower)
        sz = MaxPower;
    else
        sz = s.MaxPower;
    TSet tmp(sz);
    tmp.BitField = BitField | s.BitField;
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(MaxPower);
    tmp = *this;
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(MaxPower);
    tmp = *this;
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int sz;
    if (MaxPower > s.MaxPower) {
        sz = s.MaxPower;
        TSet tmp(MaxPower);
        TSet thisTmp(MaxPower);
        for (int i = 0; i < sz; i++) {
            if (this->IsMember(i) == 1)
                thisTmp.InsElem(i);
        }
        tmp.BitField = thisTmp.BitField & s.BitField;
        return tmp;
    }
    else {
        sz = MaxPower;
        TSet tmp(s.MaxPower);
        TSet sTmp(s.MaxPower);
        for (int i = 0; i < sz; i++) {
            if(s.IsMember(i) == 1)
                sTmp.InsElem(i);
        }
        tmp.BitField = BitField & sTmp.BitField;
        return tmp;

    }
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(MaxPower);
    tmp.BitField = ~(BitField);
    return tmp;
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    int res = 0;
    std::string str;
    istr >> str;
    for (int i = 0; i < s.MaxPower; i++) {
        std::string c = std::to_string(i);
        int ind = str.find(c);
        if ((ind != 0) && (ind != str.size() - c.size())) {
            if ((ind < str.size()) && (str[ind - 1] == ',') && (str[ind + c.size()] == ','))
                s.InsElem(i);
        }
        else if (ind == 0) {
            if (str[ind + c.size()] == ',')
                s.InsElem(i);
        }
        else if (ind == str.size() - c.size()) {
            if (str[ind - 1] == ',')
                s.InsElem(i);
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
