// ННГУ, ВМК, Курс "Методы программирования-2", C++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)                                    // (#О1)
{
    if (len < 0) {
        throw std::invalid_argument("Длина битового поля не может быть отрицательной");
    }

    BitLen = len;

    if (len == 0) {
        MemLen = 0;
        pMem = nullptr;
    }
    else {
        MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField& bf)                        // (#П1)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    else {
        pMem = nullptr;
    }
}

TBitField::~TBitField()                                          // (#С)
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const                    // (#О2)
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const                   // (#О3)
{
    return TELEM(1) << (n % (sizeof(TELEM) * 8));
}

int TBitField::GetLength(void) const                             // (#О)
{
    return BitLen;
}

void TBitField::SetBit(const int n)                              // (#О4)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Номер бита вне диапазона");
    }

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n)                              // (#П2)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Номер бита вне диапазона");
    }

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const                         // (#Л1)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Номер бита вне диапазона");
    }

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

TBitField& TBitField::operator=(const TBitField& bf)             // (#П3)
{
    if (this == &bf) {
        return *this;
    }

    delete[] pMem;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    else {
        pMem = nullptr;
    }

    return *this;
}

int TBitField::operator==(const TBitField& bf) const             // (#О5)
{
    if (BitLen != bf.BitLen) {
        return 0;
    }

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }

    return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
    return !(*this == bf);
}


TBitField TBitField::operator|(const TBitField& bf)              // (#О6)
{
    int newLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(newLen);

    int minMem = (MemLen < result.MemLen) ? MemLen : result.MemLen;
    for (int i = 0; i < minMem; i++) {
        result.pMem[i] = pMem[i];
    }

    minMem = (bf.MemLen < result.MemLen) ? bf.MemLen : result.MemLen;
    for (int i = 0; i < minMem; i++) {
        result.pMem[i] |= bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf)              // (#Л2)
{

    int newLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(newLen);

    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator~(void)                             // (#С2)
{
    TBitField result(BitLen);

    for (int i = 0; i < MemLen - 1; i++) {
        result.pMem[i] = ~pMem[i];
    }

    if (MemLen > 0) {
        int bitsInLast = BitLen % (sizeof(TELEM) * 8);
        if (bitsInLast == 0) bitsInLast = sizeof(TELEM) * 8;

        // Создаем маску из единиц для значащих битов
        TELEM mask = (bitsInLast == sizeof(TELEM) * 8) ? ~TELEM(0) : (TELEM(1) << bitsInLast) - 1;

        result.pMem[MemLen - 1] = (~pMem[MemLen - 1]) & mask;
    }

    return result;
}


std::istream& operator>>(std::istream& istr, TBitField& bf)      // (#О7)
{
    int bitNum;
    while (istr >> bitNum) {
        if (bitNum >= 0 && bitNum < bf.BitLen) {
            bf.SetBit(bitNum);
        }
    }
    istr.clear();
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf)// (#П4)
{
    ostr << "{";
    bool first = true;
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i)) {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }
    ostr << "}";
    return ostr;
}