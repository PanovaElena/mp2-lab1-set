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
        throw "Invalid length";
    }
    BitLen = len;
    MemLen = (len + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);
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
    if ((n < 0) || (n >= BitLen)) {
        throw "Index out of range";
    }
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n >= BitLen)) {
        throw "Index out of range";
    }
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
    if ((n < 0) || (n >= BitLen)) {
        throw "Index out of range";
    }
    int MemIndex = GetMemIndex(n);
    TELEM MemMask = GetMemMask(n);
    pMem[MemIndex] = pMem[MemIndex] | MemMask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen)) {
        throw "Index out of range";
    }
    int MemIndex = GetMemIndex(n);
    TELEM MemMask = GetMemMask(n);
    pMem[MemIndex] = pMem[MemIndex] & ~MemMask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) {
        throw "Index out of range";
    }
    int MemIndex = GetMemIndex(n);
    TELEM MemMask = GetMemMask(n);
    TELEM tmp = pMem[MemIndex];
    tmp = tmp & MemMask;
    tmp = tmp >> (n % (sizeof(TELEM) * 8));
    return tmp;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }
    BitLen = bf.BitLen;
    if (MemLen == bf.MemLen) {
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
        return *this;
    }
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
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

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField tmp(std::max(BitLen, bf.BitLen));
    if (BitLen <= bf.BitLen) {
        tmp = bf;
        for (int i = 0; i < MemLen; i++) {
            tmp.pMem[i] = tmp.pMem[i] | pMem[i];
        }
    }
    else {
        tmp = *this;
        for (int i = 0; i < bf.MemLen; i++) {
            tmp.pMem[i] = tmp.pMem[i] | bf.pMem[i];
        }
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField tmp(std::min(BitLen, bf.BitLen));
    for (int i = 0; i < tmp.MemLen; i++) {
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    int res = BitLen % (sizeof(TELEM) * 8);
    if (res != 0) {
        TELEM mask = ((TELEM)1 << res) - 1;
        result.pMem[MemLen - 1] = result.pMem[MemLen - 1] & mask;
    }
    return result;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char val;
    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> val;
        if (val == '0') {
            bf.ClrBit(i);
        }
        else if(val == '1') {
            bf.SetBit(i);
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
