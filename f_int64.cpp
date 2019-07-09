/*
    f_int64.cpp

    Based on the wxWindows 64-bit integer classes. With some fixes.
    Licence:     wxWindows licence
*/


#include "f_int64.h"
using namespace std;


#ifdef IOSTREAMS
#include <cmath>
#include <cstdlib>
#include <climits>

f_int64& f_int64::FromDouble(double d)
{
    bool positive = d >= 0;
    d = fabs(d);
    if ( d <= LONG_MAX )    // fixed from ULONG_MAX
    {
        m_hi = 0;
        m_lo = (f_int32)d;
    }
    else
    {
        m_hi = (f_uint32)(d / (1.0 + (double)ULONG_MAX));
        m_lo = (f_uint32)(d - ((double)m_hi * (1.0 + (double)ULONG_MAX)));
    }

    if ( !positive )
        Negate();

    return *this;
}

double f_int64::ToDouble() const
{
    double d = m_hi;
    d *= 1.0 + (double)ULONG_MAX;
    d += m_lo;
    return d;
}


template <class Type> void test_result(const char* message, const Type& x, const Type& y)
{
    if (x != y)
    {
        cout << "FAILED " << message << ": " << x << " != " << y << endl;
        exit(1);
    }
    else
    {
        cout << "Passed " << message << " = " << y << endl;
    }
}





void f_int64::testharness()
{
    cout << endl << endl << "f_int64 testharness" << endl;
    
    {
        f_int64 x(123);
        f_int64 y(456);
        
        cout << "x = " << (x) << endl;
        cout << "y = " << (y) << endl;

        test_result("x+y",(x+y), f_int64(579));
        test_result("y-x",(y-x), f_int64(333));
        test_result("x-y",(x-y), f_int64(-333));
        test_result("x>>3",(x >> 3), f_int64(15));
        test_result("x<<2",(x << 2), f_int64(492));
        test_result("x*y",(x*y), f_int64(56088));
        test_result("x*-1",(x*f_int64(-1)), f_int64(-123));
        test_result("x|y",(x|y), f_int64(507));
        test_result("x/8",(x/f_int64(8)), f_int64(15));
    }
    {
        f_int64 x(123,0);
        f_int64 y(0,456);
        
        cout << "x = " << (x) << endl;
        cout << "y = " << (y) << endl;

        test_result("x+y",(x+y), f_int64(0x7b,0x1c8)); // 528280977864
        test_result("x-y",(x-y), f_int64(0x7a,0xFFFFFe38)); // 528280976952
        test_result("y-x",(y-x), f_int64(0xFFFFFF85,0x000001c8)); // -528280976952
        test_result("x>>3",(x >> 3), f_int64(0xf, 0x60000000));
        test_result("x<<2",(x << 2), f_int64(0x1ec,0));
        test_result("x*y",(x*y), f_int64(0xdb18,0));
        test_result("x|y",(x|y), f_int64(123,456));
        test_result("x/8",(x/f_int64(8)), f_int64(0xf, 0x60000000));
    }

    cout << "f_int64 testharness complete." << endl << endl;
}

#endif

 // force the use of the 64-bit multiply
inline int64_t smull(int32_t a, int32_t b)
{
    return (int64_t) a * (int64_t) b;
}

/*!\brief Calculate the 64-bit product of two signed 32 bit words.
    The words are in big-endian order.
    Derived from Knuth's Algorithm M from [Knu2] section 4.3.1.
*/
#ifdef NO_64BIT_MULTIPLY
f_int64 f_int64::mult32(f_int32 u, f_int32 v)
{
    f_int32 u0 = u >> 16;
    f_uint32 u1 = u & 0xFFFF;
    f_int32 v0 = v >> 16;
    f_uint32 v1 = v & 0xFFFF;
    
    f_uint32 t = u1*v1;
    f_uint32 w3 = t & 0xFFFF;
    f_uint32 k = t >> 16;
    
    t = u0*v1 + k;
    f_uint32 w2 = t & 0xFFFF;
    f_uint32 w1 = (int32_t)t >> 16;
    
    t = u1*v0 + w2;
    k = (int32_t)t >> 16;
    
    return f_int64(u0*v0 + w1 + k, (t << 16) + w3);
}
#endif



f_int64 f_int64::operator<<(int32_t shift) const
{
    f_int64 ll(*this);
    ll <<= shift;

    return ll;
}

f_uint64 f_uint64::operator<<(int32_t shift) const
{
    f_uint64 ll(*this);
    ll <<= shift;

    return ll;
}

f_int64& f_int64::operator<<=(int32_t shift)
{
    if (shift != 0)
    {
        if (shift < 32)
        {
            m_hi <<= shift;
            m_hi |= m_lo >> (32 - shift);
            m_lo <<= shift;
        }
        else
        {
            m_hi = m_lo << (shift - 32);
            m_lo = 0;
        }
    }
    return *this;
}

f_uint64& f_uint64::operator<<=(int32_t shift)
{
    if (shift != 0)
    {
        if (shift < 32)
        {
            m_hi <<= shift;
            m_hi |= m_lo >> (32 - shift);
            m_lo <<= shift;
        }
        else
        {
            m_hi = m_lo << (shift - 32);
            m_lo = 0;
        }
    }
    return *this;
}

f_int64 f_int64::operator>>(int32_t shift) const
{
    f_int64 ll(*this);
    ll >>= shift;

    return ll;
}

f_uint64 f_uint64::operator>>(int32_t shift) const
{
    f_uint64 ll(*this);
    ll >>= shift;

    return ll;
}

f_int64& f_int64::operator>>=(int32_t shift)
{
    if (shift != 0)
    {
        if (shift < 32)
        {
            m_lo >>= shift;
            m_lo |= m_hi << (32 - shift);
            m_hi >>= shift;
        }
        else
        {
            m_lo = m_hi >> (shift - 32);
            m_hi = (m_hi < 0 ? -1L : 0);
        }
    }
    return *this;
}

f_uint64& f_uint64::operator>>=(int32_t shift)
{
    if (shift != 0)
    {
        if (shift < 32)
        {
            m_lo >>= shift;
            m_lo |= m_hi << (32 - shift);
            m_hi >>= shift;
        }
        else
        {
            m_lo = m_hi >> (shift - 32);
            m_hi = 0;
        }
    }
    return *this;
}

f_int64 f_int64::operator+(const f_int64& ll) const
{
    f_int64 res(*this);
    res += ll;

    return res;
}

f_uint64 f_uint64::operator+(const f_uint64& ll) const
{
    f_uint64 res(*this);
    res += ll;

    return res;
}

f_int64 f_int64::operator+(f_int32 l) const
{
    f_int64 res(*this);
    res += l;

    return res;
}

f_uint64 f_uint64::operator+(f_uint32 l) const
{
    f_uint64 res(*this);
    res += l;

    return res;
}

f_int64& f_int64::operator+=(const f_int64& ll)
{
    f_uint32 previous = m_lo;

    m_lo += ll.m_lo;
    m_hi += ll.m_hi;

    if ((m_lo < previous) || (m_lo < ll.m_lo))
        m_hi++;

    return *this;
}

f_uint64& f_uint64::operator+=(const f_uint64& ll)
{
    f_uint32 previous = m_lo;

    m_lo += ll.m_lo;
    m_hi += ll.m_hi;

    if ((m_lo < previous) || (m_lo < ll.m_lo))
        m_hi++;

    return *this;
}

f_int64& f_int64::operator+=(f_int32 l)
{
    f_uint32 previous = m_lo;

    m_lo += l;
    if (l < 0)
        m_hi += -1l;

    if ((m_lo < previous) || (m_lo < (f_uint32)l))
        m_hi++;

    return *this;
}

f_uint64& f_uint64::operator+=(f_uint32 l)
{
    f_uint32 previous = m_lo;

    m_lo += l;

    if ((m_lo < previous) || (m_lo < l))
        m_hi++;

    return *this;
}

// pre increment
f_int64& f_int64::operator++()
{
    m_lo++;
    if (m_lo == 0)
        m_hi++;

    return *this;
}

f_uint64& f_uint64::operator++()
{
    m_lo++;
    if (m_lo == 0)
        m_hi++;

    return *this;
}

// negation
f_int64 f_int64::operator-() const
{
    f_int64 res(*this);
    res.Negate();

    return res;
}

f_int64& f_int64::Negate()
{
    m_hi = ~m_hi;
    m_lo = ~m_lo;

    m_lo++;
    if ( m_lo == 0 )
        m_hi++;

    return *this;
}

// subtraction

f_int64 f_int64::operator-(const f_int64& ll) const
{
    f_int64 res(*this);
    res -= ll;

    return res;
}


#include <limits.h>     // for LONG_MAX

f_int64 f_uint64::operator-(const f_uint64& ll) const
{
#ifdef IOSTREAMS
    DEBUG_ASSERT_MESS(m_hi <= LONG_MAX,"m_hi <= LONG_MAX " );
    DEBUG_ASSERT_MESS(ll.m_hi <= LONG_MAX,"ll.m_hi <= LONG_MAX" );
#endif
    f_int64 res( (f_int32)m_hi , m_lo );
    f_int64 op( (f_int32)ll.m_hi , ll.m_lo );
    res -= op;

    return res;
}

f_int64& f_int64::operator-=(const f_int64& ll)
{
    f_uint32 previous = m_lo;

    m_lo -= ll.m_lo;
    m_hi -= ll.m_hi;

    if (previous < ll.m_lo)
        m_hi--;

    return *this;
}

f_uint64& f_uint64::operator-=(const f_uint64& ll)
{
    f_uint32 previous = m_lo;

    m_lo -= ll.m_lo;
    m_hi -= ll.m_hi;

    if (previous < ll.m_lo)
        m_hi--;

    return *this;
}

// pre decrement
f_int64& f_int64::operator--()
{
    m_lo--;
    if (m_lo == 0xFFFFFFFF)
        m_hi--;

    return *this;
}

f_uint64& f_uint64::operator--()
{
    m_lo--;
    if (m_lo == 0xFFFFFFFF)
        m_hi--;

    return *this;
}

// comparison operators

bool f_int64::operator<(const f_int64& ll) const
{
    if ( m_hi < ll.m_hi )
        return true;
    else if ( m_hi == ll.m_hi )
        return m_lo < ll.m_lo;
    else
        return false;
}

bool f_uint64::operator<(const f_uint64& ll) const
{
    if ( m_hi < ll.m_hi )
        return true;
    else if ( m_hi == ll.m_hi )
        return m_lo < ll.m_lo;
    else
        return false;
}

bool f_int64::operator>(const f_int64& ll) const
{
    if ( m_hi > ll.m_hi )
        return true;
    else if ( m_hi == ll.m_hi )
        return m_lo > ll.m_lo;
    else
        return false;
}

bool f_uint64::operator>(const f_uint64& ll) const
{
    if ( m_hi > ll.m_hi )
        return true;
    else if ( m_hi == ll.m_hi )
        return m_lo > ll.m_lo;
    else
        return false;
}

// bitwise operators

f_int64 f_int64::operator&(const f_int64& ll) const
{
    return f_int64(m_hi & ll.m_hi, m_lo & ll.m_lo);
}

f_uint64 f_uint64::operator&(const f_uint64& ll) const
{
    return f_uint64(m_hi & ll.m_hi, m_lo & ll.m_lo);
}

f_int64 f_int64::operator|(const f_int64& ll) const
{
    return f_int64(m_hi | ll.m_hi, m_lo | ll.m_lo);
}

f_uint64 f_uint64::operator|(const f_uint64& ll) const
{
    return f_uint64(m_hi | ll.m_hi, m_lo | ll.m_lo);
}

f_int64 f_int64::operator^(const f_int64& ll) const
{
    return f_int64(m_hi ^ ll.m_hi, m_lo ^ ll.m_lo);
}

f_uint64 f_uint64::operator^(const f_uint64& ll) const
{
    return f_uint64(m_hi ^ ll.m_hi, m_lo ^ ll.m_lo);
}

f_int64& f_int64::operator&=(const f_int64& ll)
{
    m_lo &= ll.m_lo;
    m_hi &= ll.m_hi;

    return *this;
}

f_uint64& f_uint64::operator&=(const f_uint64& ll)
{
    m_lo &= ll.m_lo;
    m_hi &= ll.m_hi;

    return *this;
}

f_int64& f_int64::operator|=(const f_int64& ll)
{
    m_lo |= ll.m_lo;
    m_hi |= ll.m_hi;

    return *this;
}

f_uint64& f_uint64::operator|=(const f_uint64& ll)
{
    m_lo |= ll.m_lo;
    m_hi |= ll.m_hi;
    return *this;
}

f_int64& f_int64::operator^=(const f_int64& ll)
{
    m_lo ^= ll.m_lo;
    m_hi ^= ll.m_hi;

    return *this;
}

f_uint64& f_uint64::operator^=(const f_uint64& ll)
{
    m_lo ^= ll.m_lo;
    m_hi ^= ll.m_hi;

    return *this;
}

f_int64 f_int64::operator~() const
{
    return f_int64(~m_hi, ~m_lo);
}

f_uint64 f_uint64::operator~() const
{
    return f_uint64(~m_hi, ~m_lo);
}

// multiplication

f_int64 f_int64::operator*(const f_int64& ll) const
{
    f_int64 res(*this);
    res *= ll;

    return res;
}

f_uint64 f_uint64::operator*(const f_uint64& ll) const
{
    f_uint64 res(*this);
    res *= ll;

    return res;
}

/*!\brief Long multiplication

            A.hi        A.lo
x            B.hi        B.lo
===================================================
            A.hi B.lo    A.lo B.lo
             B.hi A.lo
    A.hi B.hi
 */
f_int64& f_int64::operator*=(const f_int64& ll)
{
    bool negative = false;
    
    f_int64 A(*this);
    f_int64 B(ll);
    
    if (A < 0)
    {
        A.Negate();
        negative = !negative;
    }
    if (B < 0)
    {
        B.Negate();
        negative = !negative;
    }
    
    f_int64 t1 = mult32(A.m_lo, B.m_lo);
    f_int64 t2 = mult32(A.m_hi, B.m_lo) + mult32(B.m_hi, A.m_lo);
    
#ifdef DEBUGGING
    f_int64 t3 = mult32(A.m_hi, B.m_hi);
    if (t3.abs() > 0)
    {
        cout << "Error. Overflow in 64-bit multiplication." << endl;
        cout << "A=" << (*this) << " B=" << ll << endl;
        throw -1;
    }
#endif
    f_int64 prod = t1 + (t2 << 32);
    if (negative)
        prod.Negate();
    
    m_hi = prod.m_hi;
    m_lo = prod.m_lo;
    return *this;
#if 0
    f_int64 t(m_hi, m_lo);
    f_int64 q(ll.m_hi, ll.m_lo);

    m_hi = m_lo = 0;

    int32_t counter = 0;
    do
    {
        if ((q.m_lo & 1) != 0)
            *this += t;
        q >>= 1;
        t <<= 1;
        counter++;
    }
    while ((counter < 64) && ((q.m_hi != 0) || (q.m_lo != 0)));

    return *this;
#endif
}

/*!\brief Long multiplication
 */
f_uint64& f_uint64::operator*=(const f_uint64& ll)
{
    f_uint64 t(m_hi, m_lo);
    f_uint64 q(ll.m_hi, ll.m_lo);

    m_hi = m_lo = 0;

    int32_t counter = 0;
    do
    {
        if ((q.m_lo & 1) != 0)
            *this += t;
        q >>= 1;
        t <<= 1;
        counter++;
    }
    while ((counter < 64) && ((q.m_hi != 0) || (q.m_lo != 0)));

    return *this;
}

// division

#define IS_MSB_SET(ll)  ((ll.GetHi()) & (1 << (8*sizeof(f_int32) - 1)))

void f_int64::Divide(const f_int64& divisorIn,
                          f_int64& quotient,
                          f_int64& remainderIO) const
{
    if ((divisorIn.m_lo == 0) && (divisorIn.m_hi == 0))
    {
#ifdef DEBUGGING
        throw -1;
#endif
        return;
    }

    // VZ: I'm writing this in a hurry and it's surely not the fastest way to
    //     do this - any improvements are more than welcome
    //
    //     code inspired by the snippet at
    //          http://www.bearcave.com/software/divide.htm
    //
    //     Copyright notice:
    //
    //     Use of this program, for any purpose, is granted the author, Ian
    //     Kaplan, as f_int32 as this copyright notice is included in the source
    //     code or any source code derived from this program. The user assumes
    //     all responsibility for using this code.

    // init everything
    f_uint64 dividend, divisor, remainder;

    quotient = 0l;
    remainder = 0l;

    // always do unsigned division and adjust the signs later: in C integer
    // division, the sign of the remainder is the same as the sign of the
    // dividend, while the sign of the quotient is the product of the signs of
    // the dividend and divisor. Of course, we also always have
    //
    //      dividend = quotient*divisor + remainder
    //
    // with 0 <= abs(remainder) < abs(divisor)
    bool negRemainder = GetHi() < 0;
    bool negQuotient = false;   // assume positive
    if ( GetHi() < 0 )
    {
        negQuotient = !negQuotient;
        dividend = f_uint64(-*this);
    } else {
        dividend = f_uint64(*this);
    }
    if ( divisorIn.GetHi() < 0 )
    {
        negQuotient = !negQuotient;
        divisor = f_uint64(-divisorIn);
    } else {
        divisor = f_uint64(divisorIn);
    }

    // check for some particular cases
    if ( divisor > dividend )
    {
        remainder = dividend;
    }
    else if ( divisor == dividend )
    {
        quotient = 1l;
    }
    else
    {
        // here: dividend > divisor and both are positive: do unsigned division
        f_uint32 nBits = 64u;
        f_uint64 d;
        f_uint64 oneULL(1);
        f_int64 oneLL(1);

        while ( remainder < divisor )
        {
            remainder <<= 1;
            if ( IS_MSB_SET(dividend) )
            {
                remainder |= oneULL;
            }

            d = dividend;
            dividend <<= 1;

            nBits--;
        }

        // undo the last loop iteration
        dividend = d;
        remainder >>= 1;
        nBits++;

        for ( f_uint32 i = 0; i < nBits; i++ )
        {
            remainder <<= 1;
            if ( IS_MSB_SET(dividend) )
            {
                remainder |= oneULL;
            }

            f_int64 t = remainder - divisor;
            dividend <<= 1;
            quotient <<= 1;
            if ( !IS_MSB_SET(t) )
            {
                quotient |= oneLL;

                remainder = f_uint64(t);
            }
        }
    }

    remainderIO = remainder.toLongLong();

    // adjust signs
    if ( negRemainder )
    {
        remainderIO = -(remainder.toLongLong());
    }

    if ( negQuotient )
    {
        quotient = -quotient;
    }
}

void f_uint64::Divide(const f_uint64& divisorIn,
                           f_uint64& quotient,
                           f_uint64& remainder) const
{
    if ((divisorIn.m_lo == 0) && (divisorIn.m_hi == 0))
    {
#ifdef DEBUGGING
        throw -1;
#endif
        return;
    }

    // VZ: I'm writing this in a hurry and it's surely not the fastest way to
    //     do this - any improvements are more than welcome
    //
    //     code inspired by the snippet at
    //          http://www.bearcave.com/software/divide.htm
    //
    //     Copyright notice:
    //
    //     Use of this program, for any purpose, is granted the author, Ian
    //     Kaplan, as f_int32 as this copyright notice is included in the source
    //     code or any source code derived from this program. The user assumes
    //     all responsibility for using this code.

    // init everything
    f_uint64 dividend = *this,
                divisor = divisorIn;

    quotient = 0l;
    remainder = 0l;

    // check for some particular cases
    if ( divisor > dividend )
    {
        remainder = dividend;
    }
    else if ( divisor == dividend )
    {
        quotient = 1l;
    }
    else
    {
        // here: dividend > divisor
        f_uint32 nBits = 64u;
        f_uint64 d;
        f_uint64 oneULL(1);
        f_int64 oneLL(1);

        while ( remainder < divisor )
        {
            remainder <<= 1;
            if ( IS_MSB_SET(dividend) )
            {
                remainder |= oneULL;
            }

            d = dividend;
            dividend <<= 1;

            nBits--;
        }

        // undo the last loop iteration
        dividend = d;
        remainder >>= 1;
        nBits++;

        for ( f_uint32 i = 0; i < nBits; i++ )
        {
            remainder <<= 1;
            if ( IS_MSB_SET(dividend) )
            {
                remainder |= oneULL;
            }

            f_int64 t = remainder - divisor;
            dividend <<= 1;
            quotient <<= 1;
            if ( !IS_MSB_SET(t) )
            {
                quotient |= oneULL;

                remainder = f_uint64(t);
            }
        }
    }
}

f_int64 f_int64::operator/(const f_int64& ll) const
{
    f_int64 quotient, remainder;

    Divide(ll, quotient, remainder);

    return quotient;
}

f_uint64 f_uint64::operator/(const f_uint64& ll) const
{
    f_uint64 quotient, remainder;

    Divide(ll, quotient, remainder);

    return quotient;
}

f_int64& f_int64::operator/=(const f_int64& ll)
{
    f_int64 quotient, remainder;

    Divide(ll, quotient, remainder);

    *this = quotient;

    return *this;
}

f_uint64& f_uint64::operator/=(const f_uint64& ll)
{
    f_uint64 quotient, remainder;

    Divide(ll, quotient, remainder);

    *this = quotient;

    return *this;
}

f_int64 f_int64::operator%(const f_int64& ll) const
{
    f_int64 quotient, remainder;

    Divide(ll, quotient, remainder);

    return remainder;
}

f_uint64 f_uint64::operator%(const f_uint64& ll) const
{
    f_uint64 quotient, remainder;

    Divide(ll, quotient, remainder);

    return remainder;
}

#ifdef IOSTREAMS

ostream& operator<< ( ostream& o, const f_int64& x)
{
    if (x < 0)
        return o << "-" << f_uint64(-x);
    return o << f_uint64(x);
}


ostream& operator<< ( ostream& o, const f_uint64& in_ll)
{
    std::string result;
    f_uint64 zero(0);
    f_uint64 ten(10);
    f_uint64 ll(in_ll);

    while ( ll != zero )
    {
        result.insert(result.begin(),(char)('0' + (ll % ten).ToULong()));
        ll /= ten;
    }

    if ( result.empty() )
        result = '0';

    return o << result;
}

#endif // IOSTREAMS
