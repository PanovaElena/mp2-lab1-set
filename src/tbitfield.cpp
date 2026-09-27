// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <iostream>
#include <string>
#include <stdexcept>

#define BITS (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
    if (len < 0) throw std::out_of_range("negative length");
    BitLen = len;
    MemLen = (len == 0) ? 1 : (len + BITS - 1) / BITS;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n)const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
    return n / BITS;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
    return (TELEM)1 << (n % BITS);
}

// доступ к битам битового поля

int TBitField::GetLength(void)const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) return *this;
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) //операция или
{
    int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(max_len);
    for (int i = 0; i < max_len; i++) {
        int b1 = (i < BitLen) ? this->GetBit(i) : 0;
        int b2 = (i < bf.BitLen) ? bf.GetBit(i) : 0;
        if (b1 || b2) res.SetBit(i);
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция и
{
    int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(max_len);
    for (int i = 0; i < max_len; i++) {
        int b1 = (i < BitLen) ? this->GetBit(i) : 0;
        int b2 = (i < bf.BitLen) ? bf.GetBit(i) : 0;
        if (b1 && b2) res.SetBit(i);
    }
    return res;
}

TBitField TBitField::operator~(void) //отрицание 
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) res.pMem[i] = ~pMem[i];

    int rem = BitLen % BITS;
    if (rem != 0) {
        TELEM mask = ((TELEM)1 << rem) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

// ввод/вывод

std::istream& operator>>(std::istream& istr, TBitField& bf) //ввод
{
    std::string s;
    istr >> s;
    for (int i = 0; i <bf.BitLen; i++) bf.ClrBit(i);
    for (size_t i = 0; i < s.length() && i < (size_t)bf.BitLen; i++) {
        if (s[i] == '1')bf.SetBit(i);
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) ostr << bf.GetBit(i);
    return ostr;
}