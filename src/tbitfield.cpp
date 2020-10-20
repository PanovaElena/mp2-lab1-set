// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) : BitLen(len)
{
    if (len <= 0)
        throw "Negative length";

    int a = 8 * sizeof(TELEM);  
    MemLen = (BitLen / a) + 1;  
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != NULL)
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    int a = 8 * sizeof(TELEM);
    if ((n < 0) || (n >= BitLen))
        throw "Negative or Too Large index";
    return (n / a);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int a = 8 * sizeof(TELEM);
    return TELEM(1) << (n % a);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
        throw "Negative or Too Large index";
    int i = GetMemIndex(n);
    pMem[i] |= GetMemMask(n);

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
        throw "Negative or Too Large index";
    int i = GetMemIndex(n);
    pMem[i] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n <= -1) || (n >= BitLen))
        throw "Negative or Too Large index";
    int i = GetMemIndex(n);
    return int(pMem[i] & GetMemMask(n));

}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (BitLen != bf.BitLen) {
        BitLen = bf.BitLen;
        //MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    else for(int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int res = 1;
    if (BitLen != bf.BitLen)
        res = 0;
    else
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                res = 0;
    return res;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    /*int i, max, len = BitLen;
    if (len < bf.BitLen)
        len = bf.BitLen;
    
    TBitField temp1(len);
    for (i = 0; i < len; i++)
        temp1.pMem[i] = pMem[i];
    for (i = BitLen; i < MemLen; i++)
        temp1.ClrBit(i);

    TBitField temp2(len);
    for (i = 0; i < bf.BitLen; i++)
        temp2.pMem[i] = bf.pMem[i];
    for (i = bf.BitLen; i < bf.MemLen; i++)
        temp2.ClrBit(i);
    
    if (MemLen > bf.MemLen)
        max = MemLen;
    else max = bf.MemLen;

    for (i = 0; i < max; i++)
        temp1.pMem[i] |= temp2.pMem[i];
    return temp1;*/

    ///////------------------//////
    
    int i, len = BitLen;
    if (len < bf.BitLen)
        len = bf.BitLen;
    TBitField temp(len);
    for ( i=0; i < MemLen; i++ )
        temp.pMem[i] = pMem[i];
    for ( i=0; i < bf.MemLen; i++ ) 
        temp.pMem[i] |= bf.pMem[i];
    return temp; 
    
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int i,max, len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField temp(len);
    for (i = 0; i < MemLen; i++)
        temp.pMem[i] = pMem[i];

    max = this->MemLen;
    if (this->MemLen < bf.MemLen)
        max = bf.MemLen;

    for (i = 0; i < max; i++)
        temp.pMem[i] &= bf.pMem[i];

    for (int i = bf.MemLen; i < MemLen; i++)
        temp.pMem[i] = 0;

    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(*this);
    for (int i = 0; i < temp.BitLen; i++) {
        if (temp.GetBit(i))
            temp.ClrBit(i);
        else temp.SetBit(i);
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char ch;
    do {
        istr >> ch;
    } while (ch != ' ');
    while (1) {
        istr >> ch;
        if (ch == '0')
            bf.ClrBit(i++);
        else if (ch == '1')
            bf.SetBit(i++);
        else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.GetLength();
    for (int i = 0; i < len; i++)
        if (bf.GetBit(i))
            ostr << '1';
        else ostr << '0';
    return ostr;
}
