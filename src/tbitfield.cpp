// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw std::invalid_argument("Len can not be negative");
    MemLen = len / (sizeof(TELEM) * 8) + 1;
    BitLen = len;
    pMem = new TELEM[MemLen] {0};
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen] {};
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
    if (n < 0 || n > BitLen) throw std::out_of_range("n out of range");
    return static_cast<int>(n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen) throw std::out_of_range("n out of range");
    TELEM mask = 1;
    int pos = n % (sizeof(TELEM) * 8);
    return mask << (sizeof(TELEM) * 8 - pos - 1);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) throw std::out_of_range("n out of range");
    int ind = GetMemIndex(n);
    pMem[ind] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) throw std::out_of_range("n out of range");
    int ind = GetMemIndex(n);
    pMem[ind] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) throw std::out_of_range("n out of range");
    int ind = GetMemIndex(n);
    if ((pMem[ind] & GetMemMask(n)) != 0) return 1;
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen] {};
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen || MemLen != bf.MemLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (bf == *this) return 0;
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField bitfieldik(0);
    if (BitLen > bf.BitLen) {
        bitfieldik = *this;
    } else {
        bitfieldik = bf;
    }
    int bl = std::max(BitLen, bf.BitLen);
    bitfieldik.BitLen = bl;
    for (int i = 0; i < std::min(bf.MemLen, MemLen); i++) {
        bitfieldik.pMem[i] = bf.pMem[i] | pMem[i];
    }
    return bitfieldik;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField bitfieldik(0);
    if (BitLen > bf.BitLen) {
        bitfieldik = *this;
    } else {
        bitfieldik = bf;
    }
    int bl = std::max(BitLen, bf.BitLen);
    bitfieldik.BitLen = bl;
    for (int i = 0; i < std::max(bf.MemLen, MemLen); i++) {
        bitfieldik.pMem[i] = bf.pMem[i] & pMem[i];
    }
    return bitfieldik;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField bitfieldik = *this;
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 1) bitfieldik.ClrBit(i);
        else {
            bitfieldik.SetBit(i);
        }
    }
    return bitfieldik;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        int b;
        istr >> b;
        if (b == 1) bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i) << " ";
    }
    return ostr;
}
