/**
 * Simple example program showing how to used this Fixed class
 * in an embedded programme.
 * 
 * Copyright (c) Tim Molteno. 2003-2019.
 * */

#include "Fixed.h"
#include "FixedVector.h"

using namespace std;

int main(int argc, char **argv)
{
	Fixed16 a(4);
	Fixed16 b(5);
	
	Fixed16 c = sqrt(a);
	c = sqrt(b);
	c = sin(a/b);
	
	c = arctan(tan(Fixed16::PI()/4));

	long j = 12;
	long k = j / 17;
	
	FixedVector v1(a,b,a);
	FixedVector v2(b,a,b);

	Fixed16 n = norm(v1);

	FixedVector v3 = v1 * b;
	c = dot(v1,v2);

	return 0;
}
