// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw std::invalid_argument("negative lenght");
    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    if (MemLen == 0) MemLen = 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
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

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("unsuitable index");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("unsuitable index");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("unsuitable index");
    return ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)return 0;
  for (int i = 0; i < MemLen; i++)
      if (pMem[i] != bf.pMem[i]) return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen;
    if (BitLen > bf.BitLen) maxLen = BitLen;
    else maxLen = bf.BitLen;
    TBitField res(maxLen);
    for (int i = 0; i < res.MemLen; i++) {
        TELEM x1 = 0, x2 = 0;
        if (i < MemLen) x1 = pMem[i];
        if (i < bf.MemLen) x2 = bf.pMem[i];
        res.pMem[i] = x1 | x2;
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen;
    if (BitLen > bf.BitLen) maxLen = BitLen;
    else maxLen = bf.BitLen;
    TBitField res(maxLen);
    for (int i = 0; i < res.MemLen; i++) {
        TELEM x1 = 0, x2 = 0;
        if (i < MemLen) x1 = pMem[i];
        if (i < bf.MemLen) x2 = bf.pMem[i];
        res.pMem[i] = x1 & x2;
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (int i = 0; i < res.MemLen; i++) {
        res.pMem[i] = ~res.pMem[i];
    }
    int t = res.BitLen % (sizeof(TELEM) * 8);//колич.знач.битов в посл-ем элементе
    if (t != 0)
        res.pMem[res.MemLen - 1] &= ((TELEM)1 << t) - 1;//чтобы мусорные биты не менялись
    return res;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    std::string s;
    istr >> s;
    int len = bf.BitLen;
    int n = (int)s.size();
    if (n < len) len = n;
    for (int i = 0; i < len; i++) {
        if (s[i] == '1')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    for (int i = len; i < bf.BitLen; i++)
        bf.ClrBit(i);
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.BitLen - 1; i >= 0; i--)
        ostr << bf.GetBit(i);
    return ostr;
}
