// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if (mp < 0)
        throw std::invalid_argument("Negative power is not allowed");

    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{

}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
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
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of set bounds");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of set bounds");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of set bounds");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField; // Копируем данные битового поля
    }
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

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField tempBF = BitField | s.BitField;
    return TSet(tempBF);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of set bounds");
    }
    TSet tempSet(*this);// Создаем копию текущего множества с помощью конструктора копирования
    tempSet.InsElem(Elem);// Добавляем элемент в нашу временную копию
    return tempSet;

}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of set bounds");
    }
    TSet tempSet(*this);// Создаем копию текущего множества с помощью конструктора копирования
    tempSet.DelElem(Elem);// Добавляем элемент в нашу временную копию
    return tempSet;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TBitField tempBF = BitField & s.BitField;
    return TSet(tempBF);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField tempBF = ~BitField;
    return TSet(tempBF);
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) // вывод
{
    ostr << "{ ";
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            ostr << i << " ";
        }
    }
    ostr << "}";
    return ostr;
}
