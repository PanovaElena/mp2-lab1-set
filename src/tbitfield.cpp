// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

const TELEM byte = 8;
const TELEM one = 1;
const TELEM zero = 0;

TBitField::TBitField(int len) // конструктор по умолчанию
{
    if (len > 0)
    {
        BitLen = len;
        if (BitLen > byte * sizeof(TELEM))
        {
            if (BitLen % (byte * sizeof(TELEM)) == 0)
                MemLen = BitLen / (byte * sizeof(TELEM));
            else
                MemLen = BitLen / (byte * sizeof(TELEM)) + 1;
        }
        else
            MemLen = 1;
        pMem = new TELEM[MemLen];
        if (pMem != NULL)
            for (int i = 0; i < MemLen; i++)
                pMem[i] = 0;
    }
    else throw "Negative Lenght of bitfield!";
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

TBitField::~TBitField() // деструктор
{
    if (pMem != NULL)
    {
        delete[]pMem;
        pMem = NULL;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n >= 0) && (n < BitLen))
        return n / (byte * sizeof(TELEM));
    else throw "Incorrect index of bit!";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n >= 0) && (n < BitLen))
        return one << (n & (byte * sizeof(TELEM) - one));
    else throw "Incorrect index of bit!";
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
        return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n >= 0) & (n < BitLen))
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    else throw "Incorrect index of bit!";
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n >= 0) && (n < BitLen))
            pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    else throw "Incorrect index of bit!";
}

int TBitField::GetBit(const int n) const // получить значение бита
{ 
    if ((n >= 0) && (n < BitLen))
        if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
            return one;
        else
            return zero;
    else throw "Incorrect index of bit!";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
        BitLen = bf.BitLen;
        if (MemLen != bf.MemLen)
        {
            MemLen = bf.MemLen;
            if (pMem != NULL)
                delete[]pMem;
            pMem = new TELEM[MemLen];
        }
        if (pMem != NULL)
            for (int i = 0; i < MemLen; i++)
                pMem[i] = bf.pMem[i];
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return false;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return false;
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen)
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return true;
            else
                return false;
    return true;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len;
    len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField result(len);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        result.pMem[i] |= bf.pMem[i];
    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len;
    len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField result(len);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        result.pMem[i] &= bf.pMem[i];
    for (int i = bf.MemLen; i < result.MemLen; i++)
        result.pMem[i] = 0;
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField negative(*this);
    for (int i = 0; i < BitLen; i++)
        if (negative.GetBit(i))
            negative.ClrBit(i);
        else
            negative.SetBit(i);
    return negative;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char c;
    do 
{ 
        istr >> c; 
    } while (c != ' ');
    while (1)
    {
        istr >> c;
        if (c == '0')
            bf.ClrBit(i++);
        else
            if (c == '1')
                bf.SetBit(i++);
            else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
        if (bf.GetBit(i) != 0)
            ostr << '1';
        else
            ostr << '0';
    return ostr;
}
