// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw std::invalid_argument("TBitField::TBitField: len must be non-negative");

    BitLen = len;
    int MemSize = sizeof(TELEM) * 8;
    MemLen = (BitLen + MemSize - 1) / MemSize;
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
    pMem = new TELEM[MemLen]();
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return TELEM(static_cast<TELEM>(1) << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("TBitField::SetBit: index is out of range");

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("TBitField::ClrBit: index is out of range");

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("TBitField::GetBit: index is out of range");

    TELEM res = pMem[GetMemIndex(n)] & GetMemMask(n);
    return res != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;

    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen]();
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; ++i) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) const // операция "или"
{
    TBitField res(BitLen >= bf.BitLen ? *this : bf);
    for (int i = 0; i < std::min(MemLen, bf.MemLen); ++i) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) const // операция "и"
{
    TBitField res(std::max(BitLen, bf.BitLen));
    for (int i = 0; i < std::min(MemLen, bf.MemLen); ++i) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) const // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; ++i) {
        res.pMem[i] = ~pMem[i];
    }
    int lastBits = BitLen % (sizeof(TELEM) * 8);
    if (lastBits != 0) {
        TELEM mask = (static_cast<TELEM>(1) << lastBits) - static_cast<TELEM>(1);
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

TBitField TBitField::operator^(const TBitField& bf) const // исключающее "или"
{
    TBitField res(BitLen >= bf.BitLen ? *this : bf);
    for (int i = 0; i < std::min(MemLen, bf.MemLen); ++i) {
        res.pMem[i] = pMem[i] ^ bf.pMem[i];
    }
    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    std::string input;
    char ch;
    istr >> std::ws;
    while (istr.get(ch)) {
        if (ch == '1' || ch == '0') {
            input.push_back(ch);
        }
        else {
            istr.putback(ch);
            break;
        }
    }
    TBitField tmp(input.length());
    for (int i = 0; i < tmp.BitLen; ++i) {
        if (input[i] == '1') {
            tmp.SetBit(i);
        }
    }
    bf = tmp;
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); ++i) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
