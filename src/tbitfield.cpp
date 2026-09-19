// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len)
{
    if (len < 0) throw std::invalid_argument("lenght < 0!");

    BitLen = len;
    MemLen = (len - 1) / (sizeof(TELEM) * 8) + 1;
    pMem = new TELEM[(len - 1) / (sizeof(TELEM) * 8) + 1]{ 0 };
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen), pMem(new TELEM[bf.MemLen]{0})// конструктор копирования
{
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("bit index out of range");
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("bit index out of range");
    return TELEM(1) << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (pMem[GetMemIndex(n)] & GetMemMask(n)) return 1;
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) 
        if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return (*this == bf) == 1 ? 0 : 1;
}

TBitField TBitField::operator|(const TBitField &bf) const// операция "или"
{
    int len = BitLen >= bf.BitLen ? BitLen : bf.BitLen;

    TBitField res(len);

    for (int i = 0; i < res.MemLen; ++i)
        res.pMem[i] = (i < MemLen ? pMem[i] : 0) | (i < bf.MemLen ? bf.pMem[i] : 0);

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) const// операция "и"
{
    int len = BitLen >= bf.BitLen ? BitLen : bf.BitLen;

    TBitField res(len);

    for (int i = 0; i < res.MemLen; ++i)
        res.pMem[i] = (i < MemLen ? pMem[i] : 0) & (i < bf.MemLen ? bf.pMem[i] : 0);

    return res;
}

TBitField TBitField::operator~(void) const// отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < MemLen; ++i)
        res.pMem[i] = ~pMem[i];

    //если есть хвост, то делаем его 0, чтобы это не влияло на сравнение
    if (BitLen % (sizeof(TELEM) * 8) != 0) res.pMem[MemLen - 1] &= (TELEM(1) << BitLen % (sizeof(TELEM) * 8)) - 1; // -1 преваращает 00010000 в 00001111

    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    std::string bits;
    istr >> bits;

    if (bits.length() != bf.GetLength()) throw std::runtime_error("error reading data");

    for (int i = 0; i < bf.BitLen; i++) {
        if (bits[i] == '1') bf.SetBit(bf.BitLen - 1 - i);
        else if (bits[i] == '0') bf.ClrBit(bf.BitLen - 1 - i);
        else throw std::runtime_error("error reading data");
    }

    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.BitLen - 1; i >= 0; i--)
        ostr << bf.GetBit(i);

    return ostr;
}
