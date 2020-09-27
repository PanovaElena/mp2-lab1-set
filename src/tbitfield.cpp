// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len > 0)
    {
        BitLen = len;
        MemLen = (len + 15) >> 4; //т.к. в элементе каждая ячейка pMem 16 бит (TELEM - это unsigned int), поэтому нужно общую длину поделить на 16 бит, и узнаем требуемое кол-во ячеек. 
                    //Пишем +15, чтобы при сдвиге (целочисленном делении) всё было верно. Например, 7/16=0, но на самом деле нам нужна одна ячейка. Поэтому 7+15=22, 22/16=1. Всё верно
        pMem = new TELEM[MemLen];   //динамически создаём требуемое количество ячеек (массив)
        if (pMem != nullptr)
            for (int i = 0; i < MemLen; i++)
                pMem[i] = 0;
    }
    else throw "Negative lenght of bitfield!";
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != nullptr)
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    if (pMem)   //проверка на то, что указатель пришёл ненулевой. Если нулевой, то удаляли ранее, то удалять второй раз содержимое памяти нельзя
    {
        delete pMem;    //удаляем содержимое, на которое указывает указатель
        pMem = nullptr; //обнуляем указатель
    }
}

int TBitField::GetMemIndex(const int n) const // индекс ячейки Мем для бита n
{
    if ((n >= 0) && (n < BitLen))
        //преобразовать к int и разделить на 16, т.к. в pMem (в одной ячейке) 16 бит
        return n >> 4;  //грамотно писать return n / (sizeof(TELEM) * 8);
    else throw "Incorrect Bit Index";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 15);   //сдвигаем единицу влево в пределах одной ячейки. Максимум - на 15 позиций влево, будет 1000.0000.0000.0000
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n >= 0) && (n < BitLen))
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    else throw "Incorrect Bit Index";
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n >= 0) && (n < BitLen))
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    else throw "Incorrect Bit Index";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return TBitField(0);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int i, len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField temp(len);
    for (i = 0; i < MemLen; i++)
        temp.pMem[i] = pMem[i];
    for (i = 0; i < bf.MemLen; i++)
        temp.pMem[i] &= bf.pMem[i];
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    return TBitField(0);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
