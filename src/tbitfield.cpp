// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw "Smth's Wrong";
	BitLen = len;
	MemLen = (BitLen / (sizeof(TELEM) * 8)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс ячейки памяти для бита n
{
	if ((n < 0) || (n >= BitLen))
		throw "Smth's Wrong";
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen))
		throw "Smth's Wrong";
	return (1 << ((n - 1) % (8 * sizeof(TELEM))));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen))
		throw "Smth's Wrong";
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen))
		throw "Smth's Wrong";
	{
		int i = GetMemIndex(n);
		pMem[i] = pMem[i] & ~GetMemMask(n);
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))
		throw "Smth's Wrong";
	int i = GetMemIndex(n);
	return (pMem[i] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		delete[]pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
	{
		return 0;
	}
	else
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i])
			{
				return 0;
			}
		}
		return 1;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение "не"
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen > len)
	{
		len = bf.BitLen;
	}
	TBitField temporary(len);
	for (int i = 0; i < MemLen; i++)
		temporary.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temporary.pMem[i] |= bf.pMem[i];
	return temporary;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (BitLen < bf.BitLen)
	{
		len = bf.BitLen;
	}
	TBitField temporary(len);
	for (int i = 0; i < MemLen; i++)
		temporary.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temporary.pMem[i] &= bf.pMem[i];
	return temporary;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temporary = *this;
	for (int i = 0; i < temporary.BitLen; i++)
	{
		if (temporary.GetBit(i))
			temporary.ClrBit(i);
		else
			temporary.SetBit(i);
	}
	return temporary;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char vvod;
	int i = 0;
	istr >> vvod;
	while ((vvod == '1') || (vvod == '0'))
	{
		switch (vvod)
		{
			case '1':
			{
				bf.SetBit(i);
				break;
			}
			case '0':
			{
				bf.ClrBit(i);
				break;
			}
			default:
			{
				cout << "Wrong";
				break;
			}
		}
		i++;
		istr >> vvod;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
