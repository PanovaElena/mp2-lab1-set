// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <stdexcept>
#include "tbitfield.h"

TBitField::TBitField(const int len)
    : BitLen(len),
    pMem(nullptr),
    MemLen(0)
{
    if (len<0) {
        throw std::range_error("Длина битового поля не может быть отрицательной");
    }
    constexpr int bitsInElement = sizeof(TELEM) * 8;
    MemLen = (BitLen + bitsInElement - 1) / bitsInElement;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf)
    : BitLen(bf.BitLen),
    pMem(new TELEM[bf.MemLen]),
    MemLen(bf.MemLen)
{
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField() {
    delete[] pMem;
    BitLen = 0;
    MemLen = 0;
}

// доступ к битам битового поля

int TBitField::GetLength() const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Индекс бита выходит за границы поля");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Индекс бита выходит за границы поля");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Индекс бита выходит за границы поля");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) {
    if (this != &bf) {
        if (MemLen != bf.MemLen) {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }
        BitLen = bf.BitLen;

        for (int i = 0; i < MemLen; ++i) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) {
        return 0;
    }

    for (int i = 0; i < MemLen; ++i) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) {
    const int maxLen = std::max(BitLen, bf.BitLen);
    TBitField res(maxLen);
    for (int i = 0; i < MemLen; ++i) {
        res.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; ++i) {
        res.pMem[i] |= bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) {
    const int maxBitLen = std::max(BitLen, bf.BitLen);
    TBitField tmp(maxBitLen);
    const int MemLen_ = std::max(MemLen, bf.MemLen);
    for (int i = 0; i < MemLen_; ++i) {
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return tmp;
}

TBitField& TBitField::operator~() {
    for (int i = 0; i < BitLen; ++i) {
        pMem[GetMemIndex(i)] ^= GetMemMask(i);
    }
    return *this;
}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) {
    char ch;
    for (int i = 0; i < bf.BitLen; ++i) {
        istr >> ch;
        if (ch == '1') {
            bf.SetBit(i);
        } else if (ch == '0') {
            bf.ClrBit(i);
        } else {
            istr.putback(ch);
            break;
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int i = bf.GetLength() - 1; i >= 0; --i) {
        ostr << bf.GetBit(i) << ' ';
    }
    ostr << '\n';
    return ostr;
}

// статик метОды
int TBitField::GetMemIndex(const int n) {
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) {
    return static_cast<TELEM>(1) << (n % (sizeof(TELEM) * 8));
}