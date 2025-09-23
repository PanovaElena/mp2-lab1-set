// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    if (mp < 0) throw "Error";
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField),MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf),MaxPower(bf.GetLength())
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
    if(Elem<0 || Elem>= MaxPower) return 0;

    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if(Elem<0 || Elem >= MaxPower) throw "Error: index out of";

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if(Elem < 0 || Elem >= MaxPower) throw "Error: index out of";

    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet &TSet::operator=(const TSet &s) // присваивание
{
    if(this != &s) {MaxPower = s.MaxPower; BitField = s.BitField;}

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
    int newMaxPower = std::max(MaxPower,s.MaxPower);

    TBitField tmp1(newMaxPower);
    TBitField tmp2(newMaxPower);

    for(int i = 0 ; i<MaxPower; i++){
        if(BitField.GetBit(i)){tmp1.SetBit(i);}
    }

    for(int i = 0 ; i<MaxPower; i++){
        if(BitField.GetBit(i)){tmp2.SetBit(i);}
    }

    TSet res(tmp1 | tmp2);
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if(Elem < 0 || Elem >= MaxPower) throw "Error: index out of";

    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if(Elem < 0 || Elem >= MaxPower) throw "Error: out of";

    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int minPower = std::min(MaxPower,s.MaxPower);

    TBitField result(minPower);

    for(int i = 0;i<minPower; i++){
        if(BitField.GetBit(i) && BitField.GetBit(i))
            result.SetBit(i);
    }

    TSet res(result);
    return res;
}

TSet TSet::operator~(void) // дополнение
{
   TSet res(MaxPower);

   for(int i = 0; i < MaxPower; i++){
    if(!BitField.GetBit(i)) res.InsElem(i);
   }

   return res;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int count;
    istr>>count;

    for(int i = 0; i<count; i++){
        int element;
        istr>>element;
        if(element >= 0 && element <s.MaxPower){
            s.InsElem(element);
        }
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{ ";
    int count;
    bool flag = true;

    for(int i = 0; i<s.MaxPower; i++){
        if(s.BitField.GetBit(i)){
            if(!flag) ostr<<", ";
            ostr<<i;
            flag = false;
            count++;
        }
    }

    if (count == 0) ostr<<"isEmpty";

    ostr<<" }";

    return ostr;
}
