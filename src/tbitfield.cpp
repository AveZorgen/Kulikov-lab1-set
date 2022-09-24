// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw length_error("length is negative");
    BitLen = len;
    MemLen = (BitLen + (1 << shift) - 1) >> shift;
    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> shift;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & ((1 << shift) - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    return bool(pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf == this) return *this;
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen) {
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
    }
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (MemLen != bf.MemLen) return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen >= bf.BitLen) {
        TBitField tb(*this);
        for (int i = 0; i < bf.MemLen; i++)
            tb.pMem[i]|= bf.pMem[i];
        return tb;
    }
    else {
        TBitField tb(bf);
        for (int i = 0; i < MemLen; i++)
            tb.pMem[i] |= pMem[i];
        return tb;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen >= bf.BitLen) {
        TBitField tb(*this);
        for (int i = 0; i < bf.MemLen; i++)
            tb.pMem[i] &= bf.pMem[i];
        return tb;
    }
    else {
        TBitField tb(bf);
        for (int i = 0; i < MemLen; i++)
            tb.pMem[i] &= pMem[i];
        return tb;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tb(*this);
    for (int i = 0; i < MemLen - 1; i++)
        tb.pMem[i] = ~tb.pMem[i];
    tb.pMem[MemLen - 1] = (~tb.pMem[MemLen - 1]) & ((1 << (BitLen & ((1 << shift) - 1))) - 1);
    return tb;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len = bf.BitLen;
    bool sgn;
    for (int i = 0; i < len; i++) {
        istr >> sgn;
        if (sgn)
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.BitLen;
    for (int i = len-1; i > -1; i--)
        ostr << bf.GetBit(i);
    return ostr;
}
