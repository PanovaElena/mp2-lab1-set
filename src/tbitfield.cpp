// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) : BitLen(len), MemLen(len + BitsInElem - 1) / BitsInElem)
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

TBitField::TBitField(const TBitField &bf) BitLen(bf.BitLen), MemLen(bf.MemLen)// конструктор копирования
{
    if (len == 0) pMem = nullptr;
    else {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    delete[] pMEM;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("Bit index out of range");
    return n / BitsInElem
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return TELEM();
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
}

void TBitField::ClrBit(const int n) // очистить бит
{
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  return 0;
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
    return TBitField(0);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return TBitField(0);
}

TBitField TBitField::operator~(void) // отрицание
{
    return TBitField(0);
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
