// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)  //конструктор 
{
    if (len < 0) {
        throw "Error - bad length";
    }

    BitLen = len;
    int bitsInElem = sizeof(TELEM) * 8; // 32
    MemLen = (len + bitsInElem - 1) / bitsInElem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf)  // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()  // деструктор 
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of range";
    }
    return n / (sizeof(TELEM) * 8); // n / 32 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of range";
    }
    int pos = n % (sizeof(TELEM) * 8); // n % 32
    return 1 << pos; // 000..001 двигает 1 на pos влево
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of range";
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] | mask; 
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of range";
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] & (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of range";
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    if ((pMem[index] & mask) != 0) {
        return 1;
    }
    else {
        return 0;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        if (MemLen != bf.MemLen) {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
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
    if ((*this) == bf) {
        return 0;
    }
    else {
        return 1;
    }
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxLen = BitLen;
    if (bf.BitLen > BitLen) {
        maxLen = bf.BitLen;
    }

    TBitField temp(maxLen);

    int minMemLen = MemLen;
    if (bf.MemLen < MemLen) {
        minMemLen = bf.MemLen;
    }

    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (BitLen > bf.BitLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            temp.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            temp.pMem[i] = bf.pMem[i];
        }
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxLen = BitLen;
    if (bf.BitLen > BitLen) {
        maxLen = bf.BitLen;
    }

    TBitField temp(maxLen);

    int minMemLen = MemLen;
    if (bf.MemLen < MemLen) {
        minMemLen = bf.MemLen;
    }

    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i];
    }
    int lastBits = BitLen % (sizeof(TELEM) * 8);
    if (lastBits != 0) {
        TELEM mask = (1 << lastBits) - 1;
        temp.pMem[MemLen - 1] = temp.pMem[MemLen - 1] & mask;
    }
    return temp;
}

// ввод/вывод

std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    int bit;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> bit;
        if (bit != 0) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i) != 0) {
            ostr << '1';
        }
        else {
            ostr << '0';
        }
    }
    return ostr;
}

