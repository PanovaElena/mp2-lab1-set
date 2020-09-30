// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw logic_error("Invalid size");
	BitLen = len;
	MemLen = (len / (sizeof(TELEM) * 8))+1;
	pMem = new TELEM[MemLen];
	if (pMem!=NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int i = (n % (sizeof(TELEM) * 8));
	TELEM c = 1;
	TELEM mask = c << i;
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen)) throw logic_error("Invalid position");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) throw logic_error("Invalid position");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n > BitLen-1)) throw logic_error("Invalid position");
	if (pMem[GetMemIndex(n)] & GetMemMask(n)) return 1;
	else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) {
		MemLen = bf.MemLen;
		if (pMem != NULL) delete [] pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL)
		for (int i=0;i<MemLen;i++) pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen) return 0;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) res=0;
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res = 0;
	if (BitLen != bf.BitLen) return 1;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) res = 1;
	return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (bf.BitLen > BitLen)
		BitLen = bf.BitLen;
	TBitField temp(BitLen);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (bf.BitLen > BitLen) {
		TBitField temp(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			temp.pMem[i] = bf.pMem[i] & pMem[i];
		for (int i = MemLen; i < bf.MemLen; i++)
			temp.pMem[i] = 0;
		return temp;
	}
	else {
		TBitField temp(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] = bf.pMem[i] & pMem[i];;
		for (int i = bf.MemLen; i < MemLen; i++)
			temp.pMem[i] = 0;
		return temp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(*this);
	TBitField mask(BitLen);
	for (int i = 0; i < mask.BitLen; i++)
	{
		mask.SetBit(i);
	}
	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = ~temp.pMem[i]& mask.pMem[i];
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ch;
	while (1) {
		istr >> ch;
		if (ch == '0') bf.ClrBit(i++);
		else
			if (ch == '1') bf.SetBit(i++);
			else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++) {
		if (bf.GetBit(i)) ostr << '1';
		else ostr << '0';
	}
    return ostr;
}
