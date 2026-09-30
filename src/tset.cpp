// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
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
    if (Elem >= 0 && Elem < MaxPower) {
        return BitField.GetBit(Elem);
    }
    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        BitField.SetBit(Elem);
    }
    else {
        BitField.SetBit(Elem);
    }
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem >= 0 && Elem < MaxPower) {
        BitField.ClrBit(Elem);
    }
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s) return *this;

    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TBitField newBf = BitField | s.BitField;
    return TSet(newBf);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of range");
    }

    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TBitField newBf = BitField & s.BitField;
    return TSet(newBf);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

std::istream& operator>>(std::istream& istr, TSet& s) // ввод
{
    istr >> s.BitField;
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) // вывод
{
    ostr << s.BitField;
    return ostr;
}