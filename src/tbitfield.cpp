// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len>-1) {
	MemLen = len/(sizeof(TELEM)*BITS)+1;
	pMem = new TELEM[MemLen];
	BitLen=len;
	if (pMem != NULL)
		for (int i=0; i<MemLen; i++)
			pMem[i]=0;
	}
	else throw std::logic_error("Invalid length");
}
TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	int i;
	BitLen = bf.BitLen;
	MemLen=bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
	   for (i=0;i<MemLen;i++)
		   pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n>-1)
	   return (n/(sizeof(TELEM)*BITS));
	else throw std::logic_error("Invalid index");
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n>-1)
		{
			int i = (n%(sizeof(TELEM)*BITS));
	        TELEM temp = (UNIT<<i);
            return temp;
	}
	else throw std::logic_error("Invalid index");
}

// доступ к битам битового поля

int TBitField::GetLength() const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n>-1)&&(n<BitLen))
		pMem[GetMemIndex(n)]|=GetMemMask(n);
	else throw std::logic_error("Invalid index");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n>-1)&&(n<BitLen))
		pMem[GetMemIndex(n)]&=~GetMemMask(n);
	else throw std::logic_error("Invalid index");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n>-1)&&(n<BitLen))
		return pMem[GetMemIndex(n)]&GetMemMask(n);
	else throw std::logic_error("Invalid index");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen=bf.MemLen;
		if (pMem != NULL) delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL)
		for (int i=0; i<MemLen; i++)
			pMem[i]=bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res=1;
	if (BitLen != bf.BitLen)
		res=0;
	else 
		for (int i=0; i<MemLen; i++)
			if (pMem[i] != bf.pMem[i])
			{
				res=0;
				break;
			}
  return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res=1;
	if (BitLen != bf.BitLen)
		res=1;
	else 
		for (int i=0; i<MemLen; i++)
			if (pMem[i] == bf.pMem[i])
			{
				res=0;
				break;
			}
  return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (i=0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (i=0; i < bf.MemLen; i++)
		temp.pMem[i] |= bf.pMem[i];
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (i=0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (i=0; i < bf.MemLen; i++)
		temp.pMem[i] &= bf.pMem[i];
	if (BitLen>bf.BitLen)
		for (i=bf.MemLen+1; i<MemLen; i++)
			temp.pMem[i]=0;
    return temp;
}

TBitField TBitField::operator~() // отрицание
{
	int i;
	int len=BitLen;
	TBitField temp(len);
	TBitField invert(len);
	for (i=0;i<len;i++)
		invert.SetBit(i);
	for (i=0; i < MemLen; i++)
        temp.pMem[i] = (invert.pMem[i]&(~pMem[i]));
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0;
	char c;
	while (1)
	{
		istr>>c;
		if (c=='0')
			bf.ClrBit(i++);
		else if (c =='1')
			bf.SetBit(i++);
		    else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len=bf.GetLength();
	for (int i=0; i<len; i++)
		if (bf.GetBit(i))
			ostr<<'1';
		else ostr<<'0';
	return ostr;
}