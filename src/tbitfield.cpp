// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООПфференцированный зачет 
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len) 
{
    if (len < 0){
        throw std::out_of_range("Dlina ne mojet bit' <0");
    }
    if (len == 0){
        MemLen = BitLen = 0;
        pMem = nullptr;
        return;
    }
    
    BitLen = len;
    MemLen = (len + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen]();
        
    
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{   
    if (bf.MemLen == 0){
        MemLen = BitLen = 0;
        pMem = nullptr;
    }
    else{

        BitLen = bf.BitLen; 
        MemLen = bf.MemLen;

        pMem  = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {pMem[i] = bf.pMem[i];}
    }
}

TBitField::~TBitField()
{
    delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{   
    if (n < 0 || n >= BitLen){
        throw std::out_of_range("Index slishkom bol'shoi or <0, try another one:)");
    }
    return (n/(sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen){
        throw std::out_of_range("Index slishkom bol'shoi or <0, try another one:)");
    }

    unsigned int position_in_mem = n % (sizeof(TELEM) * 8);
    TELEM mask = TELEM(1) << position_in_mem;
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{

    TELEM mask = GetMemMask(n);
    int mem_index = GetMemIndex(n);

    pMem[mem_index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{

    TELEM invr_mask = ~GetMemMask(n);
    int mem_index = GetMemIndex(n);

    pMem[mem_index] &= invr_mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
   
    TELEM mask = GetMemMask(n);
    int mem_index = GetMemIndex(n);

    return (pMem[mem_index] & mask) != 0; //rezultatom pobitovoi kon'unkciy
                                          //budet chislo tipa 2^k (0 <= k <= sizeof(TELEM)*8) - 1) or 0, 
                                          //sravnivaem s 0, chtobi bilo libo 1 libo 0.
}

// битовые операции. 

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf == this) {return *this;}

    if (MemLen != bf.MemLen){
        
        delete [] pMem;

        if (bf.MemLen == 0){
            MemLen = BitLen = 0;
            pMem = nullptr;
            return *this;
        }
        
        pMem = new TELEM[bf.MemLen];
    }

    BitLen = bf.BitLen; 
    MemLen = bf.MemLen;
    for (int i = 0; i < MemLen; i++){pMem[i] = bf.pMem[i];}

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen){ return 0;}

    for (int i = 0; i < MemLen; i++){
        if (pMem[i] != bf.pMem[i]){
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) const// операция "или"
{
    int bitLen_temp = std::max(BitLen, bf.BitLen);
    TBitField bf_temp = TBitField(bitLen_temp);

    int min_memlen = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < min_memlen; i++){
        bf_temp.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if  (MemLen > min_memlen){
        
        for (int i = min_memlen; i < MemLen; i++){
            bf_temp.pMem[i] = pMem[i];
        }
    }

    else if (bf.MemLen > min_memlen){

        for (int i = min_memlen; i < bf.MemLen; i++){
            bf_temp.pMem[i] = bf.pMem[i];
        }
    }

    return bf_temp;
}

TBitField TBitField::operator&(const TBitField &bf) const // операция "и"
{
    int bitLen_temp = std::max(BitLen, bf.BitLen);
    TBitField bf_temp = TBitField(bitLen_temp);

    int min_memlen = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < min_memlen; i++){
        bf_temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    // tak kak konstructor dlya bf_temp zapolnyet vsoy pole 0-yami, 
    // to mojem ne rabotat' s "xvostom", eta chast' uje v nulyah' .
    return bf_temp;
}

TBitField TBitField::operator~(void) const // отрицание
{
    TBitField bf_temp = TBitField(BitLen);

    for (int i = 0 ; i < MemLen; i++){
        bf_temp.pMem[i] = ~pMem[i];
    }
    // if Bitlen ne kratno sizeof(TELEM) * 8, 
    // to eta operacia "empty's" nuli (> Bitlen) prevratit v 1,
    // tem samim, vsoy narushiv.
    int bad_bit_ind = BitLen % (sizeof(TELEM) * 8);

    if (bad_bit_ind != 0 && MemLen > 0){

        TELEM mask = ((TELEM)1 << bad_bit_ind) - (TELEM)1; //vichitaem 1, chtobi poluchilas'
                                                           //maska iz edinits na meste pMem[Memlen - 1]
        bf_temp.pMem[MemLen - 1] &= mask;                  //primer: 1000 - 1 = 0111
    }

    return bf_temp;
}

TBitField TBitField::operator^(const TBitField &bf) const{ //XOR
    
    int bitlen_temp = std::max(BitLen, bf.BitLen);
    TBitField bf_temp = TBitField(bitlen_temp);

    int min_memlen = std::min(MemLen, bf.MemLen);

    for (int i = 0; i < min_memlen; i++){
        bf_temp.pMem[i] = pMem[i] ^ bf.pMem[i];
    }

    if (MemLen > min_memlen) {

        for (int i = min_memlen; i < MemLen; i++) {
            bf_temp.pMem[i] = pMem[i];
        }

    } else if (bf.MemLen > min_memlen) {

        for (int i = min_memlen; i < bf.MemLen; i++) {
            bf_temp.pMem[i] = bf.pMem[i];
        }
    }

    return bf_temp;
}
// ввод/вывод

std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    char value;

    for (int i = bf.GetLength() - 1; i >= 0; i-- ){
        istr >> value;

        if (value == '1') {
            bf.SetBit(i);
        }

        else if (value == '0'){
            bf.ClrBit(i);
        }

        else{
            throw std::runtime_error("Vvedite 1 or 0");
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{   
    if (bf.GetLength() == 0) { return ostr;}
    ostr << "|";
    for (int i = bf.GetLength() - 1; i >= 0; i-- ){

        ostr << bf.GetBit(i);

        if( i % (sizeof(TELEM) * 8) == 0){
            ostr << "|";
        }
    }
    return ostr;
}


//     