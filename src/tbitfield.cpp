// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0)
	{
		BitLen = len;
		MemLen = BitLen / (sizeof(TELEM) * data_type_length) + 1;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = 0;
		}
	}
	else throw len;
}
TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
		throw - 1;
	else return (n / (sizeof(TELEM) * data_type_length));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen))	throw n;
	return unit << (n & (sizeof(TELEM) * data_type_length - unit));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);//побитовое ИЛИ
	else throw n;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);//побитовая инверсия
	else throw n;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= 0) && (n < BitLen))
	{
		if (pMem[GetMemIndex(n)] & GetMemMask(n)) return 1;
		else return 0;
	}
	else throw n;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf)
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}
	return (*this);
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i))
			return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i))
			return 1;
	}
	return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int i, len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++)
		temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int tempsize = 0;
	int n = 0;
	if (BitLen >= bf.BitLen)
	{
		tempsize = BitLen;
		n = bf.BitLen;
	}
	else
	{
		tempsize = bf.BitLen;
		n = BitLen;
	}

	TBitField tmp(tempsize);
	for (int i = 0; i < n; i++) {
		if (GetBit(i) && bf.GetBit(i))
			tmp.SetBit(i);
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~pMem[i];
	return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int k;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> k;
		if (k == 1)
		{
			bf.SetBit(i);
		}
		else if (k == 0)
		{
			bf.ClrBit(i);
		}
		else
			break;
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		if (bf.GetBit(i) == 0)
			ostr << "0 ";
		else
			ostr << "1 ";
	}
	return ostr;
}
