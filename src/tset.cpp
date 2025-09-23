#include "tset.h"
#include <iostream>
#include <stdexcept>



// Конструкторы
TSet::TSet(int mp)
    : MaxPower(mp), BitField(mp){

}

TSet::TSet(const TSet& s)
    : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

TSet::TSet(const TBitField& bf)
    : MaxPower(bf.GetLength()), BitField(bf)
{
}

// Преобразование к TBitField
TSet::operator TBitField() const {
    return BitField;
}

// Методы
int TSet::GetMaxPower() const {
    return MaxPower;
}

int TSet::IsMember(const int Elem) const {
	if (Elem < 0 || Elem >= MaxPower) {
		return 0; // элемент не может быть в множестве, если он вне границ
	}
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) {
	if (Elem < 0 || Elem >= MaxPower) {
		return; // игнорируем элементы вне границ
	}
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
	if (Elem < 0 || Elem >= MaxPower) {
		return; // игнорируем элементы вне границ
	}
	BitField.ClrBit(Elem);
}

// Операторы множеств
TSet& TSet::operator=(const TSet& s) {
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const {
    return MaxPower == s.MaxPower && BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) const {
    TBitField bf = BitField | s.BitField;
    return TSet(bf);
}

TSet TSet::operator+(const int Elem) const {
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Element is out of range");
	}
	TSet tmp(*this);
	tmp.InsElem(Elem);
	return tmp;
}

TSet TSet::operator-(const int Elem) const {
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Element is out of range");
	}
	TSet tmp(*this);
	tmp.DelElem(Elem);
	return tmp;
}

TSet TSet::operator*(const TSet& s) const {
    TBitField bf = BitField & s.BitField;
    return TSet(bf);
}

TSet TSet::operator~() const {
    TBitField bf = ~BitField;
    return TSet(bf);
}

// Ввод/вывод
std::istream& operator>>(std::istream& istr, TSet& s) {
    int i = 0;
    if (!(istr >> i)) return istr;
    while ((i >= 0) && (i < s.MaxPower)) {
        s.InsElem(i);
        if (!(istr >> i)) break;
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) {
    for (int i = 0; i < s.GetMaxPower(); ++i) {
        if (s.IsMember(i)) {
            ostr << i << ' ';
        }
    }
    return ostr;
}