#define uint32 unsigned long

inline void divnorm(long& num, long& den, int& sign)
{
	if (num < 0)
	{
		num = -num;
		sign = 1;
	}
	else
	{
		sign = 0;
	}
	
	if (den < 0)
	{
		den = - den;
		sign = 1 - sign;
	}
}

unsigned long divmodsi4(bool modwanted, uint32_t num, uint32_t den)
{
	long int bit = 1;
	long int res = 0;
	while (den < num && bit && !(den & (1L<<31)))
	{
		den <<=1;
		bit <<=1;
	}
	while (bit)
	{
		if (num >= den)
		{
			num -= den;
			res |= bit;
		}
		bit >>=1;
		den >>=1; 
	}
	if (modwanted) return num;
	return res;
}

long __modsi3 (long numerator, long denominator)
{
	int sign = 0;
	
	if (numerator < 0) 
	{
		numerator = -numerator;
		sign = 1;
	}
	if (denominator < 0)
	{
		denominator = -denominator;
	}  
	
	long modul =  divmodsi4 (true, numerator, denominator);
	if (0 != sign)
		return -modul;
	return modul;
}

long __divsi3 (long numerator, long denominator)
{
	int sign;
	divnorm (numerator, denominator, sign);
	
	long dividend = divmodsi4 (false,  numerator, denominator);
	if (0 != sign)
		return -dividend;
	return dividend;
}

long __umodsi3 (uint32_t numerator, uint32_t denominator)
{
	return divmodsi4 (true,  numerator, denominator);
}

uint32_t divu32_u32 (uint32_t numerator, uint32_t denominator)
{
	return divmodsi4 (false, numerator, denominator);
}

// Result goes into n, remainder is returned 
uint32_t divu64_u32(uint64_t *n, uint32_t base)
{
	uint64_t rem = *n;
	uint64_t b = base;
	uint64_t res, d = 1;
	uint32_t high = rem >> 32;

	// Reduce the thing a bit first
	res = 0;
	if (high >= base)
	{
		high /= base;
		res = (uint64_t) high << 32;
		rem -= (uint64_t) (high*base) << 32;
	}

	while ((int64_t)b > 0 && b < rem)
	{
		b = b+b;
		d = d+d;
	}

	do
	{
		if (rem >= b)
		{
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
 }

/* Incumbent version from the 2.5 tree */

static inline uint32_t fls32(uint32_t x)
{
		int r = 32;

		if (!x)
				return 0;
		if (!(x & 0xffff0000u)) {
				x <<= 16;
				r -= 16;
		}
		if (!(x & 0xff000000u)) {
				x <<= 8;
				r -= 8;
		}
		if (!(x & 0xf0000000u)) {
				x <<= 4;
				r -= 4;
		}
		if (!(x & 0xc0000000u)) {
				x <<= 2;
				r -= 2;
		}
		if (!(x & 0x80000000u)) {
				x <<= 1;
				r -= 1;
		}
		return r;
}
// 64bit divisor, dividend and result. dynamic precision 
static inline u_int64_t divu64_u64(u_int64_t dividend, u_int64_t divisor)
{
	u_int32_t d = divisor;
	if (divisor > 0xffffffffULL)
	{
		unsigned int shift = fls32(divisor >> 32);
		d = divisor >> shift;
		dividend >>= shift;
	}
	// avoid 64 bit division if possible
	if (dividend >> 32)
		div64_32(dividend, d);
	else
	{
		// dividend = (uint32_t) dividend / d;
		dividend = div32_32(dividend,d);
	}
	return dividend;
}
 
