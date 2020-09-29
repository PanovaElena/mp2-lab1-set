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
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
	MaxPower = s.MaxPower;
	BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
	MaxPower = bf.GetLength();
	BitField = bf;
}

TSet::operator TBitField()
{
	TBitField temp = (TBitField)*this;
	return temp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	if ((Elem < 0) || (Elem >= MaxPower))
		throw "Error";
	if (BitField.GetBit(Elem))
		return 1;
	return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	if ((Elem < 0) || (Elem >= MaxPower))
		throw "Error";
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	if ((Elem < 0) || (Elem >= MaxPower))
		throw "Error";
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
	BitField = s.BitField;
	MaxPower = s.GetMaxPower();
	return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
	if (MaxPower != s.MaxPower)
		return 0;
	if (BitField != s.BitField)
		return 0;
	return 1;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
	if (MaxPower != s.MaxPower)
		return 1;
	if (BitField != s.BitField)
		return 1;
	return 0;
}

TSet TSet::operator+(const TSet &s) // объединение
{
	int max = MaxPower;
	if (MaxPower < s.MaxPower)
		max = s.MaxPower;
	TSet res(max);
	res.BitField = BitField | s.BitField;
	return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	if ((Elem < 0) || (Elem >= MaxPower))
		throw "Error";
	TSet res(MaxPower);
	for (int i = 0; i < res.MaxPower; i++)
		if (BitField.GetBit(i))
			res.BitField.SetBit(i);
	res.BitField.SetBit(Elem);

	return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	if ((Elem < 0) || (Elem >= MaxPower))
		throw "Error";
	BitField.ClrBit(Elem);
	return *this;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
	int len = MaxPower;
	if (s.MaxPower > MaxPower)
		len = s.MaxPower;
	TSet res(len);
	res = BitField & s.BitField;
	return res;
}

TSet TSet::operator~(void) // дополнение
{
	TSet res(MaxPower);
	res.BitField = ~BitField;
	return res;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
	char ch;
	int tmp;
	do
	{
		istr >> ch;
	} while (ch != '{');
	do
	{
		istr >> tmp;
		s.InsElem(tmp);
		istr >> ch;
	} while (ch != '}');
	return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
	ostr << s.BitField << ' ';
    return ostr;
}
