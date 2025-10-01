// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>

const int BITS_IN_TELEM = sizeof(TELEM) * 8;

// Конструктор
TBitField::TBitField(int len)
{
    if (len <= 0)
        throw "Error!";

    BitLen = len;
    MemLen = (BitLen + BITS_IN_TELEM - 1) / BITS_IN_TELEM;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

// Конструктор копирования
TBitField::TBitField(const TBitField& bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

// Деструктор
TBitField::~TBitField()
{
    delete[] pMem;
}

// Индекс в pMem для бита n
int TBitField::GetMemIndex(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw "Error!";

    return n / BITS_IN_TELEM;
}

// Битовая маска для бита n
TELEM TBitField::GetMemMask(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw "Error!";

    return TELEM(1) << (n % BITS_IN_TELEM);
}

// Получить длину (к-во битов)
int TBitField::GetLength(void) const
{
    return BitLen;
}

// Установить бит
void TBitField::SetBit(const int n)
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

// Очистить бит
void TBitField::ClrBit(const int n)
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

// Получить значение бита
int TBitField::GetBit(const int n) const
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

// Присваивание
TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this != &bf)
    {
        if (MemLen != bf.MemLen)
        {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }
        BitLen = bf.BitLen;

        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

// Сравнение
int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;

    return 1;
}

// Сравнение (не равно)
int TBitField::operator!=(const TBitField& bf) const
{
    return !(*this == bf);
}

// Операция "или"
TBitField TBitField::operator|(const TBitField& bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

    int commonMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < commonMemLen; i++)
        result.pMem[i] = pMem[i] | bf.pMem[i];

    if (MemLen > commonMemLen) {
        for (int i = commonMemLen; i < MemLen; i++)
            result.pMem[i] = pMem[i];
    }
    else if (bf.MemLen > commonMemLen) {
        for (int i = commonMemLen; i < bf.MemLen; i++)
            result.pMem[i] = bf.pMem[i];
    }

    return result;
}

// Операция "и"
TBitField TBitField::operator&(const TBitField& bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    for (int i = minMemLen; i < result.MemLen; i++)
        result.pMem[i] = 0;

    return result;
}

// Отрицание
TBitField TBitField::operator~(void)
{
    TBitField result(BitLen);

    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];

    int extraBits = BitLen % BITS_IN_TELEM;
    if (extraBits != 0)
    {
        TELEM mask = (TELEM(1) << extraBits) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}

// Ввод
istream& operator>>(istream& istr, TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        int bit;
        istr >> bit;
        if (bit == 1)
            bf.SetBit(i);
        else if (bit == 0)
            bf.ClrBit(i);
        else
            throw "Error!";
    }
    return istr;
}

// Вывод
ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}