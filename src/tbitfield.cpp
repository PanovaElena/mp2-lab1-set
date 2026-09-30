// ННГУ, ВМК, Курс "Методы программирования-2", C++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)                // (#О1)
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

TBitField::TBitField(const TBitField& bf)    // (#П1)
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

TBitField::~TBitField()                      // (#С)
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс в pMem для бита n       (#О2)
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n       (#О3)
{
    return TELEM(1) << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const      // получить длину (к-во битов)           (#О)
{
    return BitLen;
}

void TBitField::SetBit(const int n)       // установить бит                       (#О4)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Номер бита вне диапазона");
    }

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n)       // очистить бит                         (#П2)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Номер бита вне диапазона");
    }

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int  TBitField::GetBit(const int n) const // получить значение бита               (#Л1)
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Номер бита вне диапазона");
    }

    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

// битовые операции

int TBitField::operator==(const TBitField& bf) const // сравнение                 (#О5)
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

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf) // присваивание              (#П3)
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

TBitField  TBitField::operator|(const TBitField& bf) // операция "или"            (#О6)
{
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("Длины битовых полей не совпадают");
    }

    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }

    return result;
}

TBitField  TBitField::operator&(const TBitField& bf) // операция "и"              (#Л2)
{
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("Длины битовых полей не совпадают");
    }

    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;
}

TBitField  TBitField::operator~(void)                // отрицание                  (#С2)
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }

    return result;
}

// ввод/вывод

std::istream& operator>>(std::istream& istr, TBitField& bf)       //      (#О7)
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

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) //      (#П4)
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