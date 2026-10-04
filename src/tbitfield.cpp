// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0 || len>1e7) {
        throw "некорректный размер BitField";
        return;
    }
    BitLen = len;
    const int tmp = sizeof(TELEM) * 8;
    MemLen = (len + tmp - 1) / tmp;
    pMem = new TELEM[MemLen]();

}

TBitField::TBitField(const TBitField &bf)// конструктор копирования
    : MemLen(bf.MemLen)
    , BitLen(bf.BitLen)
    , pMem(new TELEM[bf.MemLen])
{
    
    if (bf.pMem!=nullptr)
        std::copy(bf.pMem, bf.pMem + MemLen, pMem);
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw "out_of_range";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return ((TELEM)1) << (n % (sizeof(TELEM) * 8));
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
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции
// реализации операций можно написать лучше, но сейчас 3 часа ночи
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    TBitField tmp=bf;
    std::swap(MemLen, tmp.MemLen);
    std::swap(BitLen, tmp.BitLen);
    std::swap(pMem, tmp.pMem);
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)return 0;
    int ret = 1;
    for (size_t i = 0; i < MemLen; i++) {
        if (bf.pMem[i] != pMem[i]) {
            ret = 0;
            break;
        }
    }
    return ret;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(bf == *this);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    //int len_min = std::min(BitLen, bf.BitLen);
    int len_max = std::max(BitLen, bf.BitLen);
    TBitField tmp(len_max);
    for (size_t i = 0; i < len_max; i++) {
        if ((i<BitLen && GetBit(i)) || (i < bf.BitLen && bf.GetBit(i)))
            tmp.SetBit(i);
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len_min = std::min(BitLen, bf.BitLen);
    int len_max = std::max(BitLen, bf.BitLen);
    TBitField tmp(len_max);
    for (size_t i = 0; i < len_min; i++) {
        if (GetBit(i) && bf.GetBit(i))
            tmp.SetBit(i);
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (size_t i = 0; i < BitLen; i++) {
        if (!GetBit(i)) tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    for (int i = bf.BitLen - 1; i >= 0; --i) {
        char c;
        if (!(istr >> c))
            return istr;

        if (c == '1')
            bf.SetBit(i);
        else if (c == '0')
            bf.ClrBit(i);
        else {
            istr.setstate(std::ios::failbit);//ошибка ввода
            return istr;
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.BitLen - 1; i > -1; --i) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
