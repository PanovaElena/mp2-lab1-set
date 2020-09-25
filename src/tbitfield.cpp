// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{ 
    if (len > 0) {
        if (len % (sizeof(TELEM) * 8) == 0)
            MemLen = len / sizeof(TELEM) / 8;
        else MemLen = len / sizeof(TELEM) / 8 + 1;
        pMem = new TELEM[MemLen];
        BitLen = len;
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    }
    else throw "Non-positive bitfield length";
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    pMem = new TELEM[bf.MemLen];
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n > BitLen || n < 0)
        throw "Bit index out of the bitfield's scope";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n > BitLen || n < 0)
        throw "Bit index out of the bitfield's scope";
    int bitNumber = n % (sizeof(TELEM) * 8);
    return 1 << bitNumber;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > BitLen || n < 0)
        throw "Bit index out of the bitfield's scope";
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > BitLen || n < 0)
        throw "Bit index out of the bitfield's scope";
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > BitLen || n < 0)
        throw "Bit index out of the bitfield's scope";
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
        return 1;
    else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (bf != *this) {
        delete[] pMem;
        pMem = new TELEM[bf.MemLen];
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    bool flag = true;
    if (BitLen != bf.BitLen)
        return false;
    for (int i = 0; i < BitLen; i++)
        if (GetBit(i) != bf.GetBit(i))
            flag = false;
    return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int length;
    if (bf.BitLen > BitLen)
        length = bf.BitLen;
    else length = BitLen;
    TBitField res(length);
   /* for (int i = 0; i < res.MemLen; i++)
        res.pMem[i] = pMem[i] | bf.pMem[i]; */
    for (int i = 0; i < res.BitLen; i++)
        if (i < bf.BitLen && i < BitLen)
            if (GetBit(i) || bf.GetBit(i) == 1)
                res.SetBit(i);
            else res.ClrBit(i);
        else if (i < bf.BitLen)
            if (bf.GetBit(i)) res.SetBit(i);
            else res.ClrBit(i);
        else if (GetBit(i)) res.SetBit(i);
        else res.ClrBit(i);
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int length;
    if (bf.BitLen > BitLen)
        length = bf.BitLen;
    else length = BitLen;
    TBitField res(length);
    for (int i = 0; i < res.BitLen; i++)
        if (i < bf.BitLen && i < BitLen)
            if (GetBit(i) && bf.GetBit(i) == 1)
                res.SetBit(i);
            else res.ClrBit(i);
        else res.ClrBit(i);
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~(pMem[i]);
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
