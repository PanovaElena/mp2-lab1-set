// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw "It's impossible to create TBitField with negative length";
    BitLen = len;
    if (len == 0) {
        MemLen = 0;
        pMem = nullptr;
    }
    else {
        //BitInElem = sizeof(TELEM) << 3;
        MemLen = (len + BitInElem - 1) / BitInElem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    //BitInElem = bf.BitInElem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс pМем для бита n
{
    return n / BitInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (TELEM)1 << (BitInElem - n - 1);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Bit position is out of range";
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n % BitInElem);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Bit position is out of range";
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n % BitInElem);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw "Bit position is out of range";
    }
  return GetMemMask(n % BitInElem) & pMem[GetMemIndex(n)];
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        //BitInElem = bf.BitInElem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this == &bf) return 1;
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(this->operator==(bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen != bf.BitLen) {
        TBitField m(std::max(BitLen, bf.BitLen));
        if (BitLen < bf.BitLen) {
            for (int i = 0; i < MemLen; i++) {
                m.pMem[i] = pMem[i];
            }
            TBitField res = bf;
            for (int i = 0; i < MemLen; i++) {
                res.pMem[i] = m.pMem[i] | bf.pMem[i];
            }
            return res;
        }
        else {
            for (int i = 0; i < bf.MemLen; i++) {
                m.pMem[i] = bf.pMem[i];
            }
            TBitField res = *this;
            for (int i = 0; i < MemLen; i++) {
                res.pMem[i] = pMem[i] | m.pMem[i];
            }
            return res;
        }
    }
    TBitField res = *this;
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen != bf.BitLen) {
        TBitField m(std::max(BitLen, bf.BitLen));
        if (BitLen < bf.BitLen) {
            for (int i = 0; i < MemLen; i++) {
                m.pMem[i] = pMem[i];
            }
            TBitField res = bf;
            for (int i = 0; i < MemLen; i++) {
                res.pMem[i] = m.pMem[i] & bf.pMem[i];
            }
            return res;
        }
        else {
            for (int i = 0; i < bf.MemLen; i++) {
                m.pMem[i] = bf.pMem[i];
            }
            TBitField res = *this;
            for (int i = 0; i < MemLen; i++) {
                res.pMem[i] = pMem[i] & m.pMem[i];
            }
            return res;
        }
    }
    TBitField res = *this;
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res = *this;
    int i = 0;
    for (; i < MemLen - 1; i++) {
        res.pMem[i] = ~pMem[i];
    }
    for (int j = i * BitInElem; j < BitLen; j++) {
        if (GetBit(j)) res.ClrBit(j);
        else res.SetBit(j);
    }
    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    int x = 0;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> x;
        if (x) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
