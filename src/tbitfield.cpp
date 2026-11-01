// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw 1;
    }
    this->BitLen = len;
    this->MemLen = (BitLen / (sizeof(TELEM)*8)+1);
    this->pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
        pMem[i]=0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{  
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int j = (sizeof(TELEM) * 8) - (n - (sizeof(TELEM) * 8) * GetMemIndex(n)) - 1;
    TELEM mask = ((TELEM)1) << j;
    return mask;
}

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0)
        throw 1;
    if (n > this->BitLen)
        throw 1;
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0)
        throw 1;
    if (n > this->BitLen)
        throw 1;
    pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0)
        throw 1;
    if (n > this->BitLen)
        throw 1;
    int i = n - (sizeof(TELEM) * 8) * GetMemIndex(n); //номер элемента слева
    int j = ((sizeof(TELEM) * 8) - 1) - i;
    return (pMem[GetMemIndex(n)] >> j) % 2;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
    else {
        this->BitLen = bf.BitLen;
        this->MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < this->MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
        return *this;
    }
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this->BitLen == bf.BitLen) { 
        bool flag = true;
        for (int i = 0; i < BitLen; i++) {
            if (this->GetBit(i) != bf.GetBit(i))
                flag = false;
        }
        if (flag == true)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение (переделать)
{
    if (this->BitLen == bf.BitLen) {
        bool flag = true;
        for (int i = 0; i < BitLen; i++) {
            if (this->GetBit(i) != bf.GetBit(i))
                flag = false;
        }
        if (flag == true)
            return 0;
        else
            return 1;
    }
    else
        return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (this->BitLen != bf.BitLen) {
        if (this->BitLen < bf.BitLen) {
            TBitField newthis(bf.BitLen);
            for (int i = 0; i < this->MemLen; i++) {
                newthis.pMem[i] = this->pMem[i];
            }
            int k = bf.MemLen;
            TBitField Res(bf.BitLen);
            for (int i = 0; i < k; i++) {
                Res.pMem[i] = (newthis.pMem[i] | bf.pMem[i]);
            }
            return Res;
        }
        else if (this->BitLen > bf.BitLen) {
            TBitField newbf(this->BitLen);
            for (int i = 0; i < bf.MemLen; i++) {
                newbf.pMem[i] = bf.pMem[i];
            }
            int k = this->MemLen;
            TBitField Res(this->BitLen);
            for (int i = 0; i < k; i++) {
                Res.pMem[i] = (newbf.pMem[i] | this->pMem[i]);
            }
            return Res;
        }
    }
    else {
        int k = this->MemLen;
        TBitField Res(BitLen);
        for (int i = 0; i < k; i++) {
            Res.pMem[i] = (this->pMem[i] | bf.pMem[i]);
        }
        return Res;
    }
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и" (хвост)
{
    if (this->BitLen != bf.BitLen) {
        if (this->BitLen != bf.BitLen) {
            if (this->BitLen < bf.BitLen) {
                TBitField newthis(bf.BitLen);
                for (int i = 0; i < this->MemLen; i++) {
                    newthis.pMem[i] = this->pMem[i];
                }
                int k = bf.MemLen;
                TBitField Res(bf.BitLen);
                for (int i = 0; i < k; i++) {
                    Res.pMem[i] = (newthis.pMem[i] & bf.pMem[i]);
                }
                return Res;
            }
            else if (this->BitLen > bf.BitLen) {
                TBitField newbf(this->BitLen);
                for (int i = 0; i < bf.MemLen; i++) {
                    newbf.pMem[i] = bf.pMem[i];
                }
                int k = this->MemLen;
                TBitField Res(this->BitLen);
                for (int i = 0; i < k; i++) {
                    Res.pMem[i] = (newbf.pMem[i] & this->pMem[i]);
                }
                std::cout << newbf << std::endl;
                std::cout << *this << std::endl;
                std::cout << Res << std::endl;
                return Res;
            }
        }
    }
    else {
        int k = this->MemLen;
        TBitField Res(BitLen);
        for (int i = 0; i < k; i++) {
            Res.pMem[i] = this->pMem[i] & bf.pMem[i];
        }
        return Res;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    int k = this->MemLen;
    TBitField Res(BitLen);
    if(k == 0)
        return Res;
    for (int i = 0; i < k-1; i++) {
        Res.pMem[i]  = ~this->pMem[i];
    }
    for (int j = (MemLen-1) * (sizeof(TELEM)*8); j < BitLen; j++) {
        if (GetBit(j) == 1)
            Res.ClrBit(j);
        else
            Res.SetBit(j);
    }
    return Res;
}

// ввод/вывод

std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}

/*
std::istream &operator>>(std::istream &istr, TBitField &bf)  // ввод
{
    std::string str;
    std::getline(istr, str);

    for (size_t i = 0; i < str.size() && i < (size_t)bf.GetLength(); ++i) {
        if (str[i] == '1') {
            bf.SetBit(i);
        } else {
            bf.ClrBit(i);
        }
    }

    for (size_t i = str.size(); i < (size_t)bf.GetLength(); ++i) {
        bf.ClrBit(i);
    }

    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); ++i) {
        ostr << bf.GetBit(i); //побитово выводим, а не блоком
    }
    return ostr;
}
*/