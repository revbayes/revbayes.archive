/*!
 * \file
 * This file contains the definitions of the functions in the RbBitset class,
 * as well as related operations on bitfields. Note that manu functions in this
 * class are inlined and appear in the header file.
 *  
 * \brief Definitions for RbBitset and related operators
 *
 * MrBayes version 4.0 beta
 *
 * (c) Copyright 2005.
 * \version 4.0 Beta
 * \date Last modified: $Date: 2006/06/04 16:25:45 $
 * \author John Huelsenbeck (1)
 * \author Bret Larget (2)
 * \author Paul van der Mark (3)
 * \author Fredrik Ronquist (3)
 * \author Donald Simon (4)
 * \author (authors listed in alphabetical order)
 * (1) Division of Biological Science, University of California, San Diego
 * (2) Departments of Botany and of Statistics, University of Wisconsin - Madison
 * (3) School of Computational Science, Florida State University
 * (4) Department of Mathematics/Computer Science, Duquesne University
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (the file gpl.txt included with this
 * distribution or http: *www.gnu.org/licenses/gpl.txt) for more
 * details.
 *
 * $Id: $
 */

#include "RbBitset.h"
#include <iostream>

/*!
 * Null constructor. Creates a 0-length (NULL) bitfield.
 * Note that the reference count will be set to 1 for this
 * null bitfield. This is to simplify the rest of the
 * code at the cost of allocating and deleting an int
 * everytime a null bitfield is used.
 *
 * \brief Null constructor
 */
RbBitset::RbBitset(void)
    : v(0), m(0), n(0), mask(0), refCount(0) {

	refCount = new int;
	*refCount = 1;

}

/*!
 * Constructor, which creates a bitfield with numBits
 * bits in it. The bits will be set to zero. If numBits
 * is 0 or negative, we construct a null bitfield. Note
 * that the reference count will be set to 1 for this
 * null bitfield. This is to simplify the rest of the
 * code at the cost of allocating and deleting an int
 * every time a null bitfield is used.
 *
 * \brief Constructor of initialized (0) bitfield
 * \param numBits Number of bits in bitfield
 */
RbBitset::RbBitset(int numBits)
    : v(0), m(0), n(0), mask(0), refCount(0) {

	if (numBits >= 1) {
		n = numBits;

		// calculate number of unsigned longs needed
		m = (n-1) / bpl + 1;
		
		v = new unsigned long[m];
		for (int i=0; i<m; i++)
			v[i] = 0;

		// set mask
		if ( n % bpl != 0 )
			{
			mask = (hb >> (n % bpl));
			mask -= 1;
			mask ^= ULONG_MAX;
			}
		else
			mask = ULONG_MAX;
	}
	refCount = new int;
	*refCount = 1;

}

/*!
 * Destructor. Note that refCount is decreased and only if
 * refCount reaches 0 do we delete allocated memory. This is
 * garbage collection as implemented in JAVA and other languages.
 * Note that null bitfields also have a reference count allocated,
 * so that we can always access the value of refCount.
 *
 * \brief Destructor with garbage collection
 */
RbBitset::~RbBitset(void) {

    (*refCount)--;
    if (*refCount < 1)
	    destroy();
 
}

RbBitset& RbBitset::operator=(const RbBitset &A) {

	if (n != A.n)
		{
		destroy();
		n = A.n;
		m = A.m;
		v = new unsigned long[m];
		for (int i=0; i<m; i++)
			v[i] = 0;
		}
	for (int i=0; i<m; i++)
		v[i] = A.v[i];
}

/*!
 * Equality operator for bitfields. Note that this
 * function assumes that extraneous bits in the last
 * unsigned long are always kept in a consistent state
 * for all bitfield objects.
 *
 * \brief Equality operator.
 * \param A Bitfield to compare this with.
 * \return True if equal, false otherwise.
 */
bool RbBitset::operator==(const RbBitset &A) const {

	if (n != A.n)
		return false;
	for (int i=0; i<m; i++)
		if (v[i] != A.v[i])
			return false;
	return true;
	
}

/*!
 * Not operator for bitfields. This will
 * flip all bits.
 *
 * \brief operator ! (flip bits)
 * \param A First bitfield to compare.
 * \param B Second bitfield to compare.
 */
RbBitset operator!(const RbBitset &A) {

	RbBitset B = A.copy();
	B.flipBits();
	return B;
	
}

/*!
 * Inequality operator for bitfields.
 *
 * \brief operator != (inequality)
 * \param A First bitfield to compare.
 * \param B Second bitfield to compare.
 */
bool operator!=(const RbBitset &A, const RbBitset &B) {

	return !(A==B);
	
}

/*!
 * Less than or equal operator for bitfields.
 *
 * \brief operator <=
 * \param A First bitfield to compare.
 * \param B Second bitfield to compare.
 */
bool operator<=(const RbBitset &A, const RbBitset &B) {

	if (A > B)
		return false;
	else
		return true;
	
}

/*!
 * Less than operator for bitfields.
 *
 * \brief operator <
 * \param A First bitfield to compare.
 * \param B Second bitfield to compare.
 */
bool operator<(const RbBitset &A, const RbBitset &B) {

	if (A > B || A == B)
		return false;
	else
		return true;
	
}

/*!
 * Larger than or equal operator for bitfields.
 *
 * \brief operator >=
 * \param A First bitfield to compare.
 * \param B Second bitfield to compare.
 */
bool operator>=(const RbBitset &A, const RbBitset &B) {

	if (A > B || A == B)
		return true;
	else
		return false;
	
}

/*!
 * Larger than operator for bitfields. Note that this
 * function assumes that extraneous bits in the last
 * unsigned long are always kept in a consistent state
 * for all bitfield objects.
 *
 * \brief operator >
 * \param A Bitfield to compare this with.
 */
bool RbBitset::operator>(const RbBitset &A) const {

	int x = (m < A.m) ? m : A.m;
	for (int i=0; i<x; i++) {
        if (v[i] > A.v[i])
			return true;
		else if (v[i] < A.v[i])
			return false;
	}
	return false;
	
}

/*!
 * Create a new version of an existing bitfield.  Used in B = A.copy()
 * or in the construction of a new bitfield that does not share
 * data with the copied bitfield, e.g. in RbBitset B(A.copy()).
 *
 * \brief Create independent copy
 * \return Copy of this.
 */
RbBitset RbBitset::copy(void) const {

	RbBitset A(n);
	for (int i=0; i<m; i++)
		A.v[i] = v[i];
	return A;

}

/*
 * Copy the elements from one bitfield to another, in place.
 * That is, if you call B.inject(A), both A and B must conform
 * (i.e. have the same dimension).
 *
 * This differs from B = A.copy() in that references to B
 * before this assignment are also affected.  That is, if
 * we have 
 *
 * MbVector A(n);
 * MbVector C(n);
 * MbVector B(C);        (elements of B and C are shared) 
 *
 * then B.inject(A) affects both B and C, while B=A.copy() creates
 * a new bitfield B which shares no data with C or A.
 *
 * A is the bitfield from which bits will be copied
 * The function returns an instance of the modifed bitfield. That is, in 
 * B.inject(A), it returns B.  If A and B are not conformant, no 
 * modifications to B are made.
 *
 * \brief Inject elements of A into this
 * \param A Bitfield with elements to inject
 * \return Injected bitfield
 */
RbBitset &RbBitset::inject(const RbBitset &A) {

	if (A.n == n) {
		for (int i=0; i<m; i++) {
			v[i] = A.v[i];
		}
	}
	return *this;
	
}

/*!
 * This is a garbage collector, which is
 * called only when there is no more element
 * referencing this bitfield.
 *
 * \brief Garbage collection
 */
void RbBitset::destroy(void) {

	if (v != 0)
		delete [] (v);
	if (refCount != 0)
		delete refCount;

}

/*!
 * Printing of a bitfield to an ostream object. The
 * format used is '[<dim>] <bits>'.
 *
 * \brief operator<<
 * \param A Bitfield to output
 * \param s ostream to output to
 * \return ostream object (for additional printing)
 */
std::ostream& operator<<(std::ostream &s, const RbBitset &A) {

	int N = A.dim();
	s << "[" << N << "] ";
	for (int j=0; j<N; j++) {
		char c;
		if (A.isBitSet(j))
			c = '1';
		else
			c = '0';
		s << c;
	}
	return s;

}

/*!
 * Reading of a bitfield from an istream object.
 * We expect an int defining the length of the
 * bitfield in square brackets, followed by a
 * a space and then the bits, that is
 * '[<dim>] <bits>'. On incorrect format, we set
 * A to a null bitfield.
 *
 * \brief operator>>
 * \param A Bitfield to receive input
 * \param s istream to read from
 * \return istream object (for additional reading)
 */
std::istream& operator>>(std::istream &s, RbBitset &A) {

	A = RbBitset();
	int N;
	char c;
	s >> c;
	if (c != '[')
		return s;
	s >> N;
	RbBitset B(N);
	s >> c;
	if (c != ']')
		return s;
	s.ignore();  // ignore the space
	for (int i=0; i<N; i++) {
		s >> c;
		if (c == '1')
			B.setBit(i);
		else if (c != '0')
			return s;
	}
	A = B;
	return s;

}

/*!
 * This function performs bitwise AND (&=) of two bitfields
 * and places the result in the first bitfield.
 *
 * \brief operator&=
 * \param A Bitfield 1
 * \param B Bitfield 2
 * \return Resulting bitfield
 */
RbBitset &operator&=(RbBitset &A, const RbBitset &B) {

	if (B.dim() == A.dim()) {
		for (int i=0; i<A.dim_(); i++)
			A[i] &= B[i];
	}
	return A;

}

/*!
 * This function performs bitwise OR (|=) of two bitfields
 * and places the result in the first bitfield.
 *
 * \brief operator|=
 * \param A Bitfield 1
 * \param B Bitfield 2
 * \return Resulting bitfield
 */
RbBitset &operator|=(RbBitset &A, const RbBitset &B) {

	if (B.dim() == A.dim()) {
		for (int i=0; i<A.dim_(); i++)
			A[i] |= B[i];
	}
	return A;

}

/*!
 * This function performs bitwise XOR (^=) of two bitfields
 * and places the result in the first bitfield.
 *
 * \brief operator^=
 * \param A Bitfield 1
 * \param B Bitfield 2
 * \return Resulting bitfield
 */
RbBitset &operator^=(RbBitset &A, const RbBitset &B) {

	if (B.dim() == A.dim()) {
		for (int i=0; i<A.dim_(); i++)
			A[i] ^= B[i];
	}
	return A;
}
