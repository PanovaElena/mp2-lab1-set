// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) {
	if (len <= 0) 
		throw "Incorrect length";
	BitLen = len;
	MemLen = (BitLen / (sizeof(TELEM) * b_length)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) 
			pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen))
		throw "Error";
	return (n / (sizeof(TELEM) * b_length));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen))
		throw "Error";
	return unit << (n & (sizeof(TELEM) * b_length - unit));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen))
		throw "Error";
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen))
		throw "Error";
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))
		throw "Error";
	int i = GetMemIndex(n);
	if (pMem[i] & GetMemMask(n)) return 1;
	else return 0;
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
		return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 1;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 1;
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField res(len);
	for (int i = 0; i < MemLen; i++)
		res.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		res.pMem[i] |= bf.pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (bf.BitLen > BitLen) {
		TBitField res(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			res.pMem[i] = bf.pMem[i] & pMem[i];
		for (int i = MemLen; i < bf.MemLen; i++)
			res.pMem[i] = 0;
		return res;
	}
	else {
		TBitField res(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			res.pMem[i] = bf.pMem[i] & pMem[i];;
		for (int i = bf.MemLen; i < MemLen; i++)
			res.pMem[i] = 0;
		return res;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) == 0)
			res.SetBit(i);
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ch;
	while (1) {
		istr >> ch;
		if (ch == '0')
			bf.ClrBit(i++);
		else
			if (ch == '1')
				bf.SetBit(i++);
			else
				break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
