// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw "Error: len<0";
    }

    BitLen = len;
    MemLen = (BitLen - 1 + sizeof(TELEM) * 8) / ((sizeof(TELEM)) * 8);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    int MemIndex = n / (sizeof(TELEM) * 8);
    return MemIndex;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM mask = (TELEM)1 << (n % (sizeof(TELEM) * 8));
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Error: Bit out of range";
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Error: Bit out of range";
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw "Error: Bit out of range";
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }

    if (BitLen != bf.BitLen) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen){
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return (*this == bf)==0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int MaxBitLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(MaxBitLen);
    int MinMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
    for (int i = 0; i < MinMemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = MinMemLen; i < MemLen;i++) {
            res.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = MinMemLen; i < bf.MemLen;i++) {
            res.pMem[i] = bf.pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int MaxBitLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(MaxBitLen);

    int MinMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;

    for (int i = 0; i < MinMemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }
    int LastBits = BitLen % (sizeof(TELEM) * 8);
    if (LastBits != 0) {
        TELEM mask = ((TELEM)1 << LastBits) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char v;
    for (int j = 0; j < bf.BitLen;j++) {
        istr >> v;
        if (v == '1') {
            bf.SetBit(j);
        }
        else {
            bf.ClrBit(j);
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int j = 0; j < bf.BitLen; j++) {
        ostr << bf.GetBit(j);
    }
    return ostr;
}
