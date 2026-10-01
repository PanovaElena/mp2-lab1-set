// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::out_of_range("Negative length"); 
    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8); //приём, для округления вверх при делении нацело
    pMem = new TELEM[MemLen] (); 
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс ячейки Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return TELEM(1U << n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Index out of range");
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Index out of range");
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] & ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Index out of range");
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) 
        return *this;

    if (BitLen != bf.BitLen)
    {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int MaxBitLen = std::max(BitLen, bf.BitLen);
    TBitField result(MaxBitLen);

    int MinMemLen = std::min(MemLen, bf.MemLen);

    for (int i = 0; i < MinMemLen; i++)
        result.pMem[i] = pMem[i] | bf.pMem[i];

    if (MemLen >= bf.MemLen) // копирование остатка из более длинного объекта
    {
        for (int i = MinMemLen; i < MemLen; i++)
            result.pMem[i] = pMem[i];
    }
    else
    {
        for (int i = MinMemLen; i < bf.MemLen; i++)
            result.pMem[i] = bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxBitLen = std::max(BitLen, bf.BitLen);
    TBitField result(maxBitLen);

    int MinMemLen = std::min(MemLen, bf.MemLen);

    for (int i = 0; i < MinMemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    // хвост от minMemLen до result.MemLen автоматически остается нулями!

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];

    /*
    for (int i = BitLen; i < MemLen * sizeof(TELEM) * 8; i++) //обработка несиспользуемых битов BitLen = 35, MemLen = 64; 
        result.ClrBit(i);
    */

    int lastBitPos = BitLen % (sizeof(TELEM) * 8); // cоздаем маску, оставляющую только нужные биты
    if (lastBitPos != 0)
    {
        TELEM mask = (1U << lastBitPos) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}   

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;

    while (istr >> ch)
    {
        if (ch == '0')
        {
            bf.ClrBit(i);
            i++;
        }
        else if (ch == '1')
        {
            bf.SetBit(i);
            i++;
        }
        else
        {
            break; // прерываемся, если встретили сторонний символ
        }

        if (i >= bf.BitLen)
            break; // не выходим за границы поля
    }

    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
