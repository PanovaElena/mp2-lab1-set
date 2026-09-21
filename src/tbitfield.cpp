// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) : BitLen(len), MemLen((len + BitsInElem - 1) / BitsInElem)
{
    if (len < 0) throw std::length_error("The length of a bit field cannot be negative");
    if (len == 0) pMem = nullptr;
    else {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen)// конструктор копирования
{
    if (MemLen == 0) pMem = nullptr;
    else {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit index out of range");
    return n / BitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit index out of range");
    return 1u << (n % BitsInElem);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit index out of range");
    pMem[GetMemIndex(n)[] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;

    if (MemLen != bf.MemLen) {
        if (pMem) delete[] pMem;
        MemLen = bf.MemLen;
        if (MemLen == 0) pMem = nullptr;
        else pMem = new TELEM[MemLen];
    }
    
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 1;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 1;
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int MaxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(MaxBitLen);

    int MinMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < MinMemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen < bf.MemLen) {
        for (int i = MinMemLen; i < bf.MemLen; i++) {
            res.pMem[i] = bf.pMem[i];
        }
    }
    else {
        for (int i = MinMemLen; i < MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int MaxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(MaxBitLen);

    int MinMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
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

    if (BitLen > 0) {
        int CntLastBits = BitLen % BitsInElem;
        if (CntLastBits != 0) {
            TELEM mask = (1u << CntLastBits) - 1;
            res.pMem[MemLen - 1] &= mask;
        }
    }

    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    std::string str;
    istr >> str;

    int len = str.size();
    TBitField tmp(len);
    for (int i = 0; i < len; i++) {
        if (str[i] == '1') {
            tmp.SetBit(i);
        }
        else if (str[i] != '0') {
            throw std::invalid_argument("An attempt was made to write a value other than 0 or 1 to a bit field");
        }
    }
    bf = tmp;

    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
