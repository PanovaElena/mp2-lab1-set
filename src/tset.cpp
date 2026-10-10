// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "../include/tset.h"

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

//оператор приведения типа
TSet::operator TBitField() const //создается копия битового поля множества
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    bool found = BitField.GetBit(Elem);
    return found;
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
    if (this == &s)
        return true;
    if (BitField != s.BitField)
        return false;
    return true;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField tmpBitField = BitField | s.BitField;
    return TSet(tmpBitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmpSet(*this);
    tmpSet.InsElem(Elem);
    return tmpSet;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmpSet(*this);
    tmpSet.DelElem(Elem);
    return tmpSet;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField tmpBitField = BitField & s.BitField;
    return TSet(tmpBitField);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField tmpBitField = ~BitField;
    return TSet(tmpBitField);
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    s.MaxPower = s.BitField.GetLength();
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
