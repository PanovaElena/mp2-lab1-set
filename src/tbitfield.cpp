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
        throw std::invalid_argument("Negative length is not allowed");
    BitLen = len;
    
    int bitsInTelem = sizeof(TELEM) * 8;
    MemLen = (len + bitsInTelem - 1) / bitsInTelem;

    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    }
    else {
        pMem = nullptr;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    else {
        pMem = nullptr;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Bit index out of range");

    int bitsInTelem = sizeof(TELEM) * 8;
    return n / bitsInTelem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Bit index out of range");

    int bitsInTelem = sizeof(TELEM) * 8;
    int bitPos = n % bitsInTelem;
    return 1U << bitPos;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] = pMem[index] & (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);

    if ((pMem[index] & mask) != 0) {
        return 1;
    }
    else {
        return 0;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        if (MemLen != bf.MemLen) {
            delete[] pMem;         // Освобождаем старый массив
            MemLen = bf.MemLen;    // Запоминаем новый физический размер
            if (MemLen > 0) 
                pMem = new TELEM[MemLen];
            else {
                pMem = nullptr;
            }
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i]; // Копируем сразу по 32 бита за шаг цикла
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0; // Как только нашли хотя бы одно отличие — сразу возвращаем "не равны"
        }
    }

    //Если проверки выше не сработали, значит поля абсолютно идентичны
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField temp(maxLen);
    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            temp.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            temp.pMem[i] = bf.pMem[i];
        }
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;

    //Создаем временный объект temp этой максимальной длины.
    //Конструктор temp автоматически выделит нужный размер памяти и занулит её.
    TBitField temp(maxLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);

    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i];
    }

    if (BitLen > 0) {
        int bitsInTelem = sizeof(TELEM) * 8;     // 32
        int lastElemBits = BitLen % bitsInTelem; // Сколько реальных бит в последней ячейке

        if (lastElemBits != 0) {
            for (int i = lastElemBits; i < bitsInTelem; i++) {
                temp.pMem[MemLen - 1] = temp.pMem[MemLen - 1] & (~(1U << i));
            }
        }
    }

    return temp;
}

// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;

    // Перебираем позиции битов, пока не заполним всё поле
    while (i < bf.BitLen) {
        // Извлекаем один символ из потока (игнорируя ведущие пробелы)
        if (!(istr >> ch)) {
            break; // Если ввод внезапно прервался (например, конец файла)
        }

        if (ch == '1') {
            bf.SetBit(i);
            i++;
        }
        else if (ch == '0') {
            bf.ClrBit(i);
            i++;
        }
        else {
            // Если встретился любой другой символ (пробел, буква, знак препинания),
            // мы возвращаем его обратно в поток ввода с помощью putback,
            // чтобы не испортить последующее чтение других переменных, и завершаем ввод поля.
            istr.putback(ch);
            break;
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    // Поочередно считываем каждый бит и отправляем его в поток вывода
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr; // Возвращаем поток для возможности цепочек вида cout << bf1 << bf2;
}
