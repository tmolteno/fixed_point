/**
 * Simple Testharness
 * in an embedded programme.
 * 
 * Copyright (c) Tim Molteno. 2003-2019.
 * */

#include <iostream>


#include "Fixed.h"
#include "FixedVector.h"
#include "FixedMatrix.h"
#include "Quaternion.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << " "  << endl;
	cout << " "  << endl;
	
// 	f_int64::testharness();
	Fixed16::testharness();
	Fixed32::testharness();
 	FixedVector::testharness();
 	Quaternion::testharness();
	FixedMatrix::testharness();

	return 0;
}


