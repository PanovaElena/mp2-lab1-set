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
        throw std::invalid_argument("err");
    }
    BitLen = len;
    MemLen = (BitLen / (sizeof(TELEM) * 8)) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[bf.MemLen];
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

    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) {
        throw std::invalid_argument("err");
    }
    TELEM tmp = pMem[GetMemIndex(n)] | GetMemMask(n);
    pMem[GetMemIndex(n)] = tmp;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) {
        throw std::invalid_argument("err");
    }
    TELEM tmp = pMem[GetMemIndex(n)] & ~GetMemMask(n);
    pMem[GetMemIndex(n)] = tmp;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) {
        throw std::invalid_argument("err");
    }

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    TELEM* copy = new TELEM[bf.MemLen];
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    for (int i = 0; i < MemLen; i++) {
        copy[i] = bf.pMem[i];
    }
    delete[] pMem;
    pMem = copy;

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen || MemLen != bf.MemLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }

    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return ~(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField rar(BitLen > bf.BitLen ? BitLen : bf.BitLen);
    int minMem = MemLen > bf.MemLen ? bf.MemLen : MemLen;
    int maxMem = MemLen > bf.MemLen ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; i++) {
        rar.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if (maxMem == bf.MemLen) {
        for (int i = minMem; i < maxMem; i++) {
            rar.pMem[i] = bf.pMem[i];
        }
    }
    else {
        for (int i = minMem; i < maxMem; i++) {
            rar.pMem[i] = pMem[i];
        }
    }

    return rar;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField rar(BitLen > bf.BitLen ? BitLen : bf.BitLen);
    int minMem = MemLen > bf.MemLen ? bf.MemLen : MemLen;
    for (int i = 0; i < minMem; i++) {
        rar.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return rar;
}

TBitField TBitField::operator~(void) // отрицание
{
    return TBitField(0);
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }

    return ostr;
}