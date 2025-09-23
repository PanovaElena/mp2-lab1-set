// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
  if (len <= 0) throw "Error";

  BitLen = len;
  MemLen = (BitLen + sizeof(TELEM)*8 - 1) / (sizeof(TELEM)*8);
  pMem = new TELEM[MemLen];

  for(int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];

  for(int i = 0; i< MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n < 0 || n > BitLen) throw "Error";
  return n/(sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n < 0 || n >= BitLen) throw "Error";
  
  int bitPosition = n % (sizeof(TELEM)* 8);
  return TELEM(1) << bitPosition;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if(n < 0 || n >= BitLen) throw "Error";

  int memIndex, mask;
  memIndex = GetMemIndex(n);
  mask = GetMemMask(n);
  pMem[memIndex] = pMem[memIndex] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if(n < 0 || n >= BitLen) throw "Error";

  int memIndex, mask;
  memIndex = GetMemIndex(n);
  mask = GetMemMask(n);
  pMem[memIndex] = pMem[memIndex] & ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
 if(n < 0 || n >= BitLen) throw "Error";

  int memIndex, mask;
  memIndex = GetMemIndex(n);
  mask = GetMemMask(n);
  if ((pMem[memIndex] & mask) > 0) return 1;
  return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
   if (this != &bf){
      if (MemLen != bf.MemLen)
      {
          delete[] pMem;
          MemLen = bf.MemLen;
          pMem = new TELEM[MemLen];
      }
      BitLen = bf.BitLen;
      
      for (int i = 0; i < MemLen; i++)
          pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen) return 0;

  int minMemLen = std::min(MemLen,bf.MemLen);

  for(int i = 0; i < minMemLen; i++){
    if(pMem[i] != bf.pMem[i]) return 0;
  }

  if (MemLen > bf.MemLen)
    {
        for (int i = bf.MemLen; i < MemLen; i++)
            if (pMem[i] != 0) return 0;
    }
    else
    {
        for (int i = MemLen; i < bf.MemLen; i++)
            if (bf.pMem[i] != 0) return 0;
    }
    
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // объединение
{
    int maxLen = std::max(BitLen, bf.BitLen);

    TBitField res(maxLen);

    for(int i = 0 ; i < BitLen ; i++){
      if(GetBit(i)) res.SetBit(i);
    }

    for(int i = 0; i< bf.BitLen; i++){
      if(bf.GetBit(i)) res.SetBit(i);
    }

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // переесечение
{
    int maxLen = std::max(BitLen, bf.BitLen);
    TBitField res(maxLen);

    int minLen = std::min(BitLen, bf.BitLen);
    for (int i = 0; i < minLen; i++) {
        if (GetBit(i) && bf.GetBit(i))
            res.SetBit(i);
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField fullSet(BitLen);

    for(int i = 0; i < BitLen; i++){
      fullSet.SetBit(i);
    }

    TBitField res(fullSet);

    for(int i = 0; i < BitLen; i++){
      if(this->GetBit(i)) res.ClrBit(i);
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for(int i = 0; i < bf.BitLen; i++){
      char bit;
      istr >> bit;
      if(bit == '1') bf.SetBit(i);
      else if(bit == '0') bf.ClrBit(i);
      else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for(int i = 0; i < bf.GetLength(); i++) ostr << bf.GetBit(i);

    return ostr;
}
