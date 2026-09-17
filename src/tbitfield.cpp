// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len<0) {
        throw "Error";
    }

    BitLen = len;

    int bitsInElement = sizeof(TELEM) * 8;
    MemLen = BitLen / bitsInElement;
    if (BitLen % bitsInElement != 0 ) {
        MemLen+=1;
    }

    pMem = new TELEM[MemLen];
    for (int i = 0; i< MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];
    for (int i = 0; i< MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr; // ??
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n>= BitLen) {
        throw "Bit index out of range";
    }

    int bitsInElement = sizeof(TELEM) * 8;

    return n/bitsInElement;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n<0 || n>= BitLen) {
        throw "Bit index out of range";
    }

    int bitsInElement = sizeof(TELEM) * 8;
    int bitPosition = n%bitsInElement;

    return static_cast<TELEM>(1) << bitPosition;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n<0 || n>= BitLen) {
        throw "Bit index out of range";
    }

    pMem[GetMemIndex(n)] |=GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n<0 || n>= BitLen) {
        throw "Bit index out of range";
    }

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n<0 || n>= BitLen) {
        throw "Bit index out of range";
    }

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }

    delete[] pMem;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];
    for (int i = 0; i< MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
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

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) const// операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen: bf.BitLen;
    TBitField temp(maxLen);

    for (int i = 0; i< MemLen; i++) {
        temp.pMem[i] = pMem[i];
    }

    for (int i = 0; i< bf.MemLen; i++) {
        temp.pMem[i] |= bf.pMem[i];
    }

    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) const // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen: bf.BitLen;
    TBitField temp(maxLen);

    for (int i = 0; i< MemLen; i++) {
        temp.pMem[i] = pMem[i];
    }

    for (int i = 0; i< bf.MemLen; i++) {
        temp.pMem[i] &= bf.pMem[i];
    }

    return temp;
}

TBitField TBitField::operator~(void) const// отрицание
{
    TBitField temp(BitLen);
    
    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i]; //
    }

    int bitsInElem = sizeof(TELEM) * 8;
    int lastBits = BitLen % bitsInElem;
    
    if (lastBits != 0) {
        TELEM mask = (static_cast<TELEM>(1) << lastBits) - 1;
        temp.pMem[MemLen - 1] &= mask;
    }
    return temp;
}
TBitField TBitField::operator^(const TBitField &bf) const 
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField temp(maxLen);
    
    // Копируем элементы первого поля
    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = pMem[i];
    }
    
    // Применяем XOR (^) с элементами второго поля
    for (int i = 0; i < bf.MemLen; i++) {
        temp.pMem[i] ^= bf.pMem[i];
    }
    
    return temp;
}


// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char c;
    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> c;
        if (c == '1') {
            bf.SetBit(i);
        } else if (c == '0') {
            bf.ClrBit(i);
        } else {
            break; // Если встретили не 0 и не 1
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
