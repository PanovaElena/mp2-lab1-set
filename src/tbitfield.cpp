// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw std::runtime_error("Can't create bitfield with negative length");
    }
    BitLen = len;
    MemLen = (BitLen + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM [ MemLen ];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM [ MemLen ];
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
    if (n < 0 || n >= BitLen) {
        throw std::runtime_error("Bit index out of range");
    }
    return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) {
        throw std::runtime_error("Bit mask out of range");
    }
    int bitInTelem = n % (sizeof(TELEM) * 8);
    TELEM mask = 1;
    return mask << bitInTelem;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    TELEM *temp = new TELEM [bf.MemLen];
    for (int i = 0; i < bf.MemLen; i++) {
        temp[i] = bf.pMem[i];
    }
    delete[] pMem;
    pMem = temp;
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i< MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);
    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    int maxMemLen = (MemLen > bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    TELEM* pMemSource = (MemLen < bf.MemLen) ? bf.pMem : pMem;
    for (int i = minMemLen; i < maxMemLen; i++) {
        res.pMem[i] = pMemSource [i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i< MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }
    int lastBitLen = BitLen % (sizeof(TELEM) * 8);
    if (lastBitLen != 0) {
        TELEM mask = (TELEM(1) << lastBitLen) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;
    do {
        istr >> ch;
    } while (ch != '0' && ch != '1');
    while ((ch == '0' || ch == '1') && i < bf.BitLen) {
        if (ch == '1') bf.SetBit(i);
        else bf.ClrBit(i);
        i++;
        if (i < bf.BitLen) istr >> ch;
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i)) ostr << '1';
        else ostr << '0';
    }
    return ostr;
}
