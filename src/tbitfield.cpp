// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h" 

const TELEM k = 8;
const TELEM e = 1;

TBitField::TBitField(int len)
{
    if (len > 0)
    {
        BitLen = len;

        //*частный случай!*/ MemLen = (len + 15) >> 4; //т.к. в элементе каждая ячейка pMem 16 бит (TELEM - это unsigned int), поэтому нужно общую длину поделить на 16 бит, и узнаем требуемое кол-во ячеек. 
                    //Пишем +15, чтобы при сдвиге (целочисленном делении) всё было верно. Например, 7/16=0, но на самом деле нам нужна одна ячейка. Поэтому 7+15=22, 22/16=1. Всё верно
        
        if (len % (sizeof (TELEM) * k) == 0) //если делится нацело (без остатка), то делаем так
            MemLen = len / sizeof(TELEM) / k;   //целочисленное деление, находим количество бит в каждой ячейке
        else
            MemLen = (len / sizeof(TELEM) / k) + 1; //целочисленное деление плюс единица
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
        delete[] pMem;    //удаляем содержимое, на которое указывает указатель
        pMem = nullptr; //обнуляем указатель
    }
}

int TBitField::GetMemIndex(const int n) const // индекс ячейки Мем для бита n
{
    if ((n >= 0) && (n < BitLen))
        //преобразовать к int и разделить на 16, т.к. в pMem (в одной ячейке) 16 бит
        return n / (sizeof (TELEM) * k);  //грамотно писать return n / (sizeof(TELEM) * 8);
    else throw "Incorrect Bit Index";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n - в пределах ОДНОЙ ячейки
{
    if ((n >= 0) && (n < BitLen))
        return e << (n & ((sizeof(TELEM) * k) - e));   //сдвигаем единицу влево в пределах одной ячейки. Максимум - на 15 позиций влево, будет 1000.0000.0000.0000
    else throw "Incorrect Bit Index";
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
    if ((n >= 0) && (n < BitLen))
    {
        int requiredbit;    //искомый бит
        if (pMem[GetMemIndex(n)] & GetMemMask(n)) //если это пересечение больше нуля (т.е. равно 1), то бит равен 1, иначе нулю 
            requiredbit = 1;    //например, 101101001, ищем для третьего бита. Битовая маска равна 000001000. 101101001&=000001000=000001000 не равно нулю. Бит равен 1
        else
            requiredbit = 0;
        return requiredbit;    
    }                           
    else throw "Incorrect Bit Index";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    TELEM* temp = new TELEM[bf.MemLen];
    for (int i = 0; i < bf.MemLen; i++)
        temp[i] = bf.pMem[i];
    delete[] pMem;
    pMem = temp;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int result = true;
    if (BitLen != bf.BitLen)    //если длины битовых строк не равны
        result = false;
    else
    {
        for (int i=0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
            {
                result = false;
                break;
            }
    }
  return result;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int result;
    if (*this == bf)    //когда *this=bf, результат будет ненулевой. Значит, они не "не равны"
        result = false ;
    else                //иначе - "не равны"
        result = true;
  return result;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int i;
    int TotalLen = max(BitLen, bf.BitLen);
    TBitField tmp(TotalLen);    //создаём битовое поле на максимальное из двух данных значений
    for (i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];  //скопировали все данные нашего поля *this
    for (i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] |= bf.pMem[i]; //сделали ИЛИ для всех ячеек копии исходного поля c переданным в параметрах полем
    return tmp;
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
    TBitField tmp(*this);
    for (int i = 0; i < tmp.BitLen; i++)
    {
        if (tmp.GetBit(i))
            tmp.ClrBit(i);
        else
            tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    //формат данных - последовательность из 0 и 1 без пробелов
    //начальные пробелы игнорируются
    //при получении не 0 и не 1 - завершение ввода
    int i=0; char ch;
    //поиск
    do
    {
        istr >> ch;
    } while (ch != ' ');
    //ввод элементов и включение в множество
    while (1)
    {
        istr >> ch;
        if (ch == '0') bf.ClrBit(i++);
        else if (ch == '1') bf.SetBit(i++);
        else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    /*for (int i = 0; i < bf.GetMemIndex(bf.GetLength()); i++)    //ищем номер ячейки для максимального номера бита (который нашли как GetLenght(). Это будет числом необходимых ячеек
        ostr << bf.pMem[i];*/

    for (int i = 0; i < bf.GetLength(); i++)
        if (bf.GetBit(i)) ostr << '1';  else ostr << '0';
    return ostr;
}
