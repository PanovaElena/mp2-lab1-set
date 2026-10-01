// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>

TBitField::TBitField(int len)
{
    this->BitLen = len;
    this->MemLen = (BitLen / 8) + 1;;
    this->pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
        pMem[i]=0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{  
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / 8;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int j = 7 - (n - 8 * GetMemIndex(n));
    TELEM mask = ((TELEM)1) << j;
    return mask;
}

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int i = (n - 8 * GetMemIndex(n));
    int j = 7 - (n - 8 * GetMemIndex(n));
    return (i>>j)%2;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if ((this->BitLen == bf.BitLen) && (this->MemLen == bf.MemLen) && (this->pMem == bf.pMem)) {
        return 1;
    }
    else
        return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int k = this->MemLen;
    TBitField Res(BitLen);
    for (int i = 0; i < k; i++) {
        Res.pMem[i] = this->pMem[i]|bf.pMem[i];
    }
    return Res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int k = this->MemLen;
    TBitField Res(BitLen);
    for (int i = 0; i < k; i++) {
        Res.pMem[i] = this->pMem[i] & bf.pMem[i];
    }
    return Res;
}

TBitField TBitField::operator~(void) // отрицание
{
    int k = this->MemLen;
    for (int i = 0; i < k; i++) {
        this->pMem[i] = ~pMem[i];
    }
    return *this;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
