// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len): BitLen(len)
{
    if (len < 0)
        throw ("error: lenght must be >= 0");

    MemLen = (len + sizeof(TELEM)*8 - 1) / (sizeof(TELEM)*8);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
 
}

TBitField::TBitField(const TBitField &bf):MemLen(bf.MemLen), BitLen(bf.BitLen) // конструктор копирования
{
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
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return TELEM(1 << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw ("error: index out of range");

    pMem[GetMemIndex(n)] = GetMemMask(n) | pMem[GetMemIndex(n)];

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw ("error: index out of range");

    pMem[GetMemIndex(n)] = ~GetMemMask(n) & pMem[GetMemIndex(n)];
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw ("error: index out of range");
    
    TELEM bit = pMem[GetMemIndex(n)] & GetMemMask(n);
    return bit != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;

    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }

    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;

}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int max_Bitlen, min_Memlen;

    if (BitLen > bf.BitLen) { max_Bitlen = BitLen; }
    else { max_Bitlen = bf.BitLen;}

    if (MemLen < bf.MemLen) { min_Memlen = MemLen; }
    else { min_Memlen = bf.MemLen; }

    TBitField new_field(max_Bitlen);

    for (int i = 0; i < min_Memlen; i++) {
        new_field.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = min_Memlen; i < MemLen; i++) {
            new_field.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = min_Memlen; i < bf.MemLen; i++) {
            new_field.pMem[i] = bf.pMem[i];
        }
    }
    return new_field;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int max_Bitlen = 0, min_Memlen = 0;

    if (BitLen > bf.BitLen) { max_Bitlen = BitLen; }
    else { max_Bitlen = bf.BitLen; }

    if (MemLen < bf.MemLen) { min_Memlen = MemLen; }
    else { min_Memlen = bf.MemLen; }

    TBitField new_field(max_Bitlen);

    for (int i = 0; i < min_Memlen; i++) {
        new_field.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return new_field;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField new_fild(BitLen);

    for (int i = 0; i < MemLen; i++) {
        new_fild.pMem[i] = ~pMem[i];
    }

    int need_bits = BitLen % (sizeof(TELEM) * 8);
    if (need_bits != 0  && MemLen > 0) {
        TELEM maska = (1 << need_bits) - 1;
        new_fild.pMem[MemLen - 1] &= maska;
    }

    return new_fild;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        char val;
        istr >> val;

        if (val == '1')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }

    return ostr;
}
