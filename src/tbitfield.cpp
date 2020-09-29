// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len): BitLen(len)
{
    if (len < 0) { 
        throw "incorrect length in input";
    }
    else {
        if (len > 8 * sizeof(TELEM))
            MemLen = len / sizeof(TELEM)* 8;
        else MemLen = 1;
        pMem = new TELEM[MemLen];
        if (pMem != nullptr)
            for (size_t i = 0; i < MemLen; i++) pMem[i] = 0;
    }
}
TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != nullptr)
        for (size_t i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    if (pMem) {
        delete[] pMem;
        pMem = nullptr;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n > -1) && (n < BitLen))
        return n / sizeof(TELEM) / 8;
    else throw "incorrect index";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n > -1) && (n < BitLen))
        return 1 << (n & 15);
    else throw  "Error";
}


int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n > -1) && (n < BitLen)) {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
    else throw "incorrect index";
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n > -1) && (n < BitLen)) {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
    else throw "incorrect index";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n > -1) && (n < BitLen))
        if (pMem[GetMemIndex(n)] & GetMemMask(n)) return 1;
        else return 0;
    else throw "incorrect index";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    TELEM* temp = new TELEM[bf.MemLen];
    for (int i = 0; i < bf.MemLen; i++) {
        temp[i] = bf.pMem[i];
    }
    delete[] pMem;
    pMem = temp;
    BitLen = bf.BitLen;
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{ 
    if (BitLen != bf.BitLen) return 0;
    else {
        int f = 1;
        for (int i = 0; i < MemLen; i++) 
            if (pMem[i] != bf.pMem[i]){
                f = 0;
                break; 
            }
        return f;
    }
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (*this == bf) return 0;
    else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int i, len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField temp(len);
    for (i = 0; i < MemLen; i++)
        temp.pMem[i] = pMem[i];
    for (i = 0; i < bf.MemLen; i++)
        temp.pMem[i] |= bf.pMem[i];
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int i, len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField temp(len);
    for (i = 0; i < MemLen; i++)
        temp.pMem[i] = pMem[i];
    for (i = 0; i < bf.MemLen; i++)
        temp.pMem[i] &= bf.pMem[i];
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    int i;
    TBitField temp(*this);
    TBitField mask(BitLen);
    for (int i = 0; i < mask.BitLen; i++) {
        mask.SetBit(i);
    }
    for (i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i] & mask.pMem[i];
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char ch;
    do { istr >> ch; } while (ch != ' ');
    while (1) {
        istr >> ch;
        if (ch == '0') bf.ClrBit(i++);
        else if (ch == '1') bf.SetBit(i++); 
        else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.GetLength();
    for (int i = 0; i < len; i++)
        if (bf.GetBit(i)) ostr << "1"; 
        else ostr << "0";
    return ostr;
}
