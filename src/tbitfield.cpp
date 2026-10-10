// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

 //конструктор с параметром - максимальной длиной битового поля (BitLen)
TBitField::TBitField(int len) : BitLen(len)
{
    if (len < 0) {
        throw "Negative number of bytes. Try another one";
    }
    MemLen = (len + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

// конструктор копирования
TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

//деструктор, должен лишь освободить динамическую память
TBitField::~TBitField()
{
    delete[] pMem;
}

// индекс Мем для бита n (индекс блока-элемента TELEM в массиве pMem)
int TBitField::GetMemIndex(const int n) const
{
    if (n < 0 || n >= BitLen) {
        throw "Index is out of range. Try another one";
    }
    return (n / (sizeof(TELEM) * 8));
}

// битовая маска для бита n
TELEM TBitField::GetMemMask(const int n) const
{
    if (n < 0 || n >= BitLen) {
        throw "Index is out of range. Try another one";
    }
    TELEM mask = (TELEM)1 << (n % (sizeof(TELEM) * 8));
    return mask;
}

// доступ к битам битового поля

// получить длину (к-во битов)
int TBitField::GetLength(void) const
{
  return BitLen;
}

// установить бит
void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen) {
        throw "Index is out of range. Try another one";
    }
    int MemIndex = GetMemIndex(n);
    TELEM MemMask = GetMemMask(n);
    pMem[MemIndex] = pMem[MemIndex] | MemMask; 
}

// очистить бит
void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen) {
        throw "Index is out of range. Try another one";
    }
    int MemIndex = GetMemIndex(n);
    TELEM MemMask = GetMemMask(n);
    pMem[MemIndex] = pMem[MemIndex] & ~MemMask;
}

// получить значение бита
int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen) {
        throw "Index is out of range. Try another one";
    }
    int MemIndex = GetMemIndex(n);
    TELEM MemMask = GetMemMask(n);
    TELEM temp = pMem[MemIndex];
    temp = temp & MemMask;
    temp = temp >> (n % (sizeof(TELEM) * 8));
    return temp;
}

// битовые операции

// присваивание
TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this == &bf) {
        return *this;
    }
    BitLen = bf.BitLen;
    if (MemLen == bf.MemLen) {
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
        return *this;
    }
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

// сравнение
int TBitField::operator==(const TBitField &bf) const 
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

// сравнение
int TBitField::operator!=(const TBitField &bf) const
{
  return !(*this == bf);
}

// операция "или"
//почему этот оператор не константный? он же создает новый объект, не изменяя исходные
TBitField TBitField::operator|(const TBitField &bf)
{
    TBitField temp = TBitField(std::max(BitLen, bf.BitLen));
    if (MemLen <= bf.MemLen) {
        temp = bf;
        for (int i = 0; i < MemLen; i++) {
            temp.pMem[i] = temp.pMem[i] | pMem[i];
        }
    }
    else {
        temp = *this;
        for (int i = 0; i < bf.MemLen; i++) {
            temp.pMem[i] = temp.pMem[i] | bf.pMem[i];
        }
    }
    return temp;
}

// операция "и"
TBitField TBitField::operator&(const TBitField &bf)
{
    TBitField temp = TBitField(std::max(BitLen, bf.BitLen));
    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return temp;
}

// отрицание
TBitField TBitField::operator~(void)
{
    TBitField temp = TBitField(BitLen);
    for (int i = 0; i < temp.MemLen; i++) {
        temp.pMem[i] = ~pMem[i];
    }
    int RestBits = temp.BitLen % (sizeof(TELEM) * 8);
    if (RestBits != 0) {
        TELEM mask = (TELEM(1) << RestBits) - 1;
        temp.pMem[temp.MemLen - 1] = temp.pMem[temp.MemLen - 1] & mask;
    }
    return temp;
}

// ввод/вывод
// ввод
std::istream &operator>>(std::istream &istr, TBitField &bf)
{
    char value;
    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> value;
        if (value == '1') {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

// вывод
std::ostream &operator<<(std::ostream &ostr, const TBitField &bf)
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
