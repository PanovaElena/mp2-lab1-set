// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len): BitLen(len)
{
    if (BitLen >= 0) {
        MemLen = ((BitLen - 1) / (sizeof(TELEM) * 8)) + 1;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    }
    else
        throw "-";
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n/(sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < BitLen) {
        const TELEM x = 1;
        int m = sizeof(TELEM) * 8;
        return x << m - (n % m) - 1;
    }
    else
        throw "large";
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= 0) {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
    else
        throw "-";
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= 0) {
        TELEM inv_mask = ~(GetMemMask(n));
        pMem[GetMemIndex(n)] &= inv_mask;
    }
    else
        throw "-";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= 0) {
        if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
            return 1;
        else
            return 0;
    }
    throw "-";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf == this)
        return *this;
    delete[] pMem;
    //if(BitLen )
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int k = 0;
    for (int i = 0; i < MemLen; i++) 
        if (pMem[i] != bf.pMem[i])
            k++;
    if (k == 0)
        return 1;
    else
        return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (*this == bf)
        return 0;
    else
        return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int size = 0;
    TBitField tmp(size);
    if (BitLen > bf.BitLen) {
        size = BitLen;
        tmp = *this;
        for (int i = 0; i < bf.BitLen; i++) {
            if ((this->GetBit(i) | bf.GetBit(i)) == 1)
                tmp.SetBit(i);
            //else
                //tmp.ClrBit(i);
        }
    }
    else {
        size = bf.BitLen;
        tmp = bf;
        for (int i = 0; i < BitLen; i++)
            if ((this->GetBit(i) | bf.GetBit(i)) == 1)
                tmp.SetBit(i);
    }
    return tmp;
    
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField tmp(bf);
    return ~((~(*this)) | (~tmp));
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (this->GetBit(i) == 0)
            tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    std::string BitStr;
    istr >> BitStr;
    bf.BitLen = BitStr.size();
    for (int i = 0; i < BitStr.size(); i++)
        if(BitStr[i] == '1')
            bf.SetBit(i);
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
        ostr << bf.GetBit(i);
    ostr << '\n';
    return ostr;
}
