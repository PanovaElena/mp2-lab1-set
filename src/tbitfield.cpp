#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "tbitfield.h"
TBitField::TBitField(int len)
	: BitLen(len),
	pMem(nullptr),
	MemLen(0)
{
	if (len < 0) {
		throw std::range_error("Bitfield length cannot be negative");
	}
	constexpr int bitsInElement = sizeof(TELEM) * 8;
	MemLen = (BitLen + bitsInElement - 1) / bitsInElement;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; ++i) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf)
	: BitLen(bf.BitLen),
	pMem(new TELEM[bf.MemLen]),
	MemLen(bf.MemLen)
{
	for (int i = 0; i < MemLen; ++i) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField() {
	delete[] pMem;
	pMem = nullptr;
	BitLen = 0;
	MemLen = 0;
}

// Вспомогательные методы
int TBitField::GetMemIndex(const int n) const {
	constexpr int bitsInElement = sizeof(TELEM) * 8;
	return n / bitsInElement;
}

TELEM TBitField::GetMemMask(const int n) const {
	constexpr int bitsInElement = sizeof(TELEM) * 8;
	const int bit = n % bitsInElement;
	return 1u << bit;
}

// Доступ к битам
int TBitField::GetLength(void) const {
	return BitLen;
}

void TBitField::SetBit(const int n) {
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("Index bite out of range");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("Index bite out of range");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const {
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("Index bite out of range");
	}
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// Операции/сравнения
TBitField& TBitField::operator=(const TBitField& bf) {
	if (this != &bf) {
		if (MemLen != bf.MemLen) {
			delete[] pMem;
			MemLen = bf.MemLen;
			pMem = new TELEM[MemLen];
		}
		BitLen = bf.BitLen;
		for (int i = 0; i < MemLen; ++i) {
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const {
	if (BitLen != bf.BitLen) {
		return 0;
	}
	for (int i = 0; i < MemLen; ++i) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) const {
	const int maxLen = std::max(BitLen, bf.BitLen);
	TBitField res(maxLen);

	// копируем свою часть
	for (int i = 0; i < MemLen; ++i) {
		res.pMem[i] = pMem[i];
	}
	// добавляем часть bf
	for (int i = 0; i < bf.MemLen; ++i) {
		res.pMem[i] |= bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) const {
	const int maxLen = std::max(BitLen, bf.BitLen);
	TBitField res(maxLen);

	// пересечение в общей области
	const int minMemLen = std::min(MemLen, bf.MemLen);
	for (int i = 0; i < minMemLen; ++i) {
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	// оставшиеся элементы res уже нули
	return res;
}

// Отрицание: возвращаем новый инвертированный битфилд
TBitField TBitField::operator~(void) const {
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; ++i) {
		if (!GetBit(i)) {
			res.SetBit(i);
		}
	}
	return res;
}

// Ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) {
	char ch;
	for (int i = 0; i < bf.BitLen; ++i) {
		if (!(istr >> ch)) {
			break;
		}
		if (ch == '1') {
			bf.SetBit(i);
		}
		else if (ch == '0') {
			bf.ClrBit(i);
		}
		else {
			istr.putback(ch);
			break;
		}
	}
	return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
	for (int i = bf.GetLength() - 1; i >= 0; --i) {
		ostr << bf.GetBit(i);
	}
	return ostr;
}
