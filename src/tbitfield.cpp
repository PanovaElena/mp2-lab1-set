// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "../include/tbitfield.h"

#include <math.h>
#include <stddef.h>
#include <stdexcept>
#include <string>


TBitField::TBitField(int len): BitLen(len)
{
    if (len >= 0) {
        int SizeOfTelem = sizeof(TELEM) * 8;
        MemLen = (BitLen + SizeOfTelem - 1) / SizeOfTelem; // округление вверх
        pMem = new TELEM[MemLen];
        for (size_t i = 0; i < MemLen; ++i) { // тк массив telem'ов
            pMem[i] = 0;
        }
    }
    else {
        throw std::out_of_range("length is negative");
    }

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8); //тк индексация идет с нуля, не прибавляем 1
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int IndexInMem = n % (sizeof(TELEM) * 8);
    TELEM Mask = ((TELEM)1 << IndexInMem);
    return Mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("index out of range");
    }

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("index out of range");
    }

    pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("index out of range");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0 ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (size_t i = 0; i < MemLen; ++i) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this == &bf) {
        return 1;
    }

    if (BitLen != bf.BitLen) {
        return 0;
    }

    int bitsInLastEl = BitLen % (sizeof(TELEM)*8);

    if (bitsInLastEl == 0) {
        for (size_t i = 0; i < MemLen; ++i) {
            if (pMem[i] != bf.pMem[i]) {
                return 0;
            }
        }
        return 1;
    }
    else {
        for (size_t i = 0; i < MemLen - 1; ++i) {
            if (pMem[i] != bf.pMem[i]) {
                return 0;
            }
        }
        TELEM tmp = pMem[MemLen - 1];
        TELEM Mask = ((TELEM)1 << bitsInLastEl) - 1;
        tmp = tmp & Mask;
        TELEM tmp2 = bf.pMem[MemLen - 1];
        tmp2 = tmp2 & Mask;
        if (tmp != tmp2) {
            return 0;
        }
        return 1;
    }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) const // операция "или"
{
    const TBitField &longer = (BitLen >= bf.BitLen) ? *this : bf;
    const TBitField &shorter = (BitLen >= bf.BitLen) ? bf : *this;

    TBitField tmp = longer;
    int bitsInLastElShort = shorter.BitLen % (sizeof(TELEM)*8);
    if (bitsInLastElShort == 0) {
        for (size_t i = 0; i < shorter.MemLen; ++i) {
            tmp.pMem[i] |= shorter.pMem[i];
        }
        return tmp;
    }

    for (size_t i = 0; i < shorter.MemLen - 1; ++i) {
        tmp.pMem[i] |= shorter.pMem[i];
    }
    TELEM LastElShort =  shorter.pMem[shorter.MemLen - 1];
    TELEM Mask = ((TELEM)1 << bitsInLastElShort) - 1;
    LastElShort = LastElShort & Mask;
    tmp.pMem[shorter.MemLen - 1] |= LastElShort;

    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) const // операция "и"
{
    const TBitField &longer = (BitLen >= bf.BitLen) ? *this : bf;
    const TBitField &shorter = (BitLen >= bf.BitLen) ? bf : *this;

    TBitField tmp = longer;
    int bitsInLastElShort = shorter.BitLen % (sizeof(TELEM)*8);
    if (bitsInLastElShort == 0) {
        for (size_t i = 0; i < shorter.MemLen; ++i) {
            tmp.pMem[i] &= shorter.pMem[i];
        }

        for (size_t i = shorter.MemLen; i < longer.MemLen; ++i) {
            tmp.pMem[i] &= (TELEM)0;
        }

        return tmp;
    }

    for (size_t i = 0; i < shorter.MemLen - 1; ++i) {
        tmp.pMem[i] &= shorter.pMem[i];
    }
    TELEM LastElShort =  shorter.pMem[shorter.MemLen - 1];
    TELEM Mask = ((TELEM)1 << bitsInLastElShort) - 1;
    LastElShort = LastElShort & Mask;
    tmp.pMem[shorter.MemLen - 1] &= LastElShort;

    for (size_t i = shorter.MemLen; i < longer.MemLen; ++i) {
        tmp.pMem[i] &= (TELEM)0;
    }

    return tmp;
}

TBitField TBitField::operator~(void) const // отрицание
{
    TBitField tmp(*this);
    for (size_t i = 0; i < MemLen; ++i) {
        tmp.pMem[i] = ~pMem[i];
    }
    return tmp;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf)  // ввод
{
    std::string str;
    std::getline(istr, str);

    for (size_t i = 0; i < str.size() && i < (size_t)bf.GetLength(); ++i) {
        if (str[i] == '1') {
            bf.SetBit(i);
        } else {
            bf.ClrBit(i);
        }
    }

    for (size_t i = str.size(); i < (size_t)bf.GetLength(); ++i) {
        bf.ClrBit(i);
    }

    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); ++i) {
        ostr << bf.GetBit(i); //побитово выводим, а не блоком
    }
    return ostr;
}
