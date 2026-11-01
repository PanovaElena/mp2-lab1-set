// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp) // BitField(-1)
{
    this->MaxPower = mp;
    this->BitField = TBitField(mp);
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField) // BitField(-1)
{
    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf) // BitField(-1)
{
    this->MaxPower = bf.GetLength();
    this->BitField = bf;
}

TSet::operator TBitField() //создается копия битового поля множества
{

    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return this->MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (this->BitField.GetBit(Elem) == 1)
        return 1;
    else
        return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    this->BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    this->BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if ((this->MaxPower == s.MaxPower) && (this->BitField == s.BitField))
        return 1;
    else
        return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField newBitField = this->BitField | s.BitField;
    return TSet(newBitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    this->BitField.SetBit(Elem);
    return *this;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    this->BitField.ClrBit(Elem);
    return *this;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField newBitField = this->BitField & s.BitField;
    return TSet(newBitField);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField newBitField = ~this->BitField;
    return TSet(newBitField);
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i) == 1)
            ostr << i;
    }
    return ostr;
}
/*std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
*/