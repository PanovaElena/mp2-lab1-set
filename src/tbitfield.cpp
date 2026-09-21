// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <stdexcept>
#include "tbitfield.h"
#include "../include/tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0 ) {
        throw std:: invalid_argument("negative lenght");
    }
    BitLen = len;
    MemLen = (BitLen + 31) / 32;
    pMem = new TELEM[MemLen]{};
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
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
    return n/ (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1u << (n % (sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("bit index is out of range");

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("bit index is out of range");

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("bit index is out of range");

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;

    delete[] pMem;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int resultLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField result(resultLen);

    for (int i = 0; i < resultLen; i++)
    {
        bool firstHasBit = i < BitLen && GetBit(i);
        bool secondHasBit = i < bf.BitLen && bf.GetBit(i);

        if (firstHasBit || secondHasBit)
            result.SetBit(i);
    }

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int resultLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    int commonLen = BitLen < bf.BitLen ? BitLen : bf.BitLen;
    TBitField result(resultLen);

    for (int i = 0; i < commonLen; i++)
    {
        if (GetBit(i) && bf.GetBit(i))
            result.SetBit(i);
    }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);

    for (int i = 0; i < BitLen; i++)
    {
        if (!GetBit(i))
            result.SetBit(i);
    }

    return result;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    for (int i = bf.BitLen - 1; i >= 0; i--)
    {
        char bit;
        istr >> bit;

        if (bit == '1')
            bf.SetBit(i);
        else if (bit == '0')
            bf.ClrBit(i);
    }

    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.BitLen - 1; i >= 0; i--)
    {
        ostr << bf.GetBit(i);
    }

    return ostr;
}
