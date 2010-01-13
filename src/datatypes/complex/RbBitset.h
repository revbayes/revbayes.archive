/*! 
 * \file
 * This file declares MbBitfield, which is used to hold bitfields.
 *  
 * \brief Declaration of MbBitfield
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

#ifndef MbBitfield_H
#define MbBitfield_H

#include <iostream>

/*!
 * This is a class for bitfields. It stores the bits
 * in unsigned long ints.
 *
 * The class is inspired by the Template Numerical Toolkit
 * and implements garbage collection as well as shallow
 * assignment for the assignment operator and copy constructor.
 *
 * The class uses two const static members: bpl, which is the 
 * number of bits in an unsigned long (bits per long); and hb,
 * which is an unsigned long with the highest bit set (high bit).
 *
 * The most common operations on bitfields have been inlined.
 *
 * \brief Bitfield class
 */
class RbBitset {

public:
                	         RbBitset(void);                                             //!< null constructor
                	         RbBitset(int numBits);                                      //!< constructs bitfield of length n
	                  inline RbBitset(const RbBitset &A);                              //!< constructs shallow copy of bitfield A
	                         ~RbBitset(void);                                            //!< destructor with garbage collection

             unsigned long   &operator[](int i) { return v[i]; }                           //!< indexing of unsigned longs
             unsigned long   &operator[](int i) const { return v[i]; };                    //!< indexing of unsigned longs (const bitfield)
			    RbBitset   &operator=(const RbBitset &A);                                 //!< assignment operator (shallow copy, elements share bits)
			    	  bool   operator==(const RbBitset &A) const;                        //!< equality operator
	  		    	  bool   operator>(const RbBitset &A) const;                         //!< larger than operator
         inline RbBitset   operator&(const RbBitset &A) const;                         //!< operator &
         inline RbBitset   operator|(const RbBitset &A) const;                         //!< operator |
         inline RbBitset   operator^(const RbBitset &A) const;                         //!< operator ^
         inline RbBitset   &ref(const RbBitset &A);                                    //!< creates a reference to another bitfield (shallow copy)
                RbBitset   copy(void) const;                                             //!< creates a copy of another bitfield (deep copy, with separate bits)
                RbBitset   &inject(const RbBitset & A);                                //!< copy the bits from one bitfield to another
                       int   dim(void) const { return n; }                                 //!< get the dimension (number of bits)
                       int   dim_(void) const { return m; }                                //!< get the dimension (number of longs)
				       int   getRefCount(void) const {return *refCount; }                  //!< get the number of bitfields that share the same data
		    		  bool   isBitSet(int i) const {return ((v[i/bpl]&(hb>>(i%bpl)))!=0); }//!< is bit i set (on)?
                inline int   numOnBits (void) const;                                       //!< return number of bits that are on
                inline int   firstOnBit (void) const;                                      //!< return index of first bit that is on
	           inline void   setBits(void);                                                //!< set all bits
               inline void   clearBits(void);                                              //!< clear all bits
               inline void   flipBits(void);                                               //!< flip all bits
			          void   setBit(int i) { v[i/bpl]|=(hb>>(i%bpl)); }                    //!< set bit i
				      void   clearBit(int i) { v[i/bpl]&=((hb>>(i%bpl))^ULONG_MAX); }      //!< clear bit i
                      void   flipBit(int i) { v[i/bpl]^=((hb>>(i%bpl))); }                 //!< flip bit i

private:
          const static int   bpl = (int) sizeof (unsigned long) * 8;                       //!< number of bits per long (assumes 8 bits per char)
const static unsigned long   hb = (1UL << (bpl-1));                                        //!< highest bit in an unsigned long for shift ops
	   	     unsigned long   *v;                                                           //!< pointer to unsigned longs (ie, bitfield)
		               int   m;                                                            //!< number of unsigned longs
		               int   n;                                                            //!< number of bits
             unsigned long   mask;                                                         //!< mask to control trailing bits in some operations
			    	   int   *refCount;                                                    //!< number of instances of the vector
                      void   destroy(void);                                                //!< garbage collection

};


std::ostream& operator<<(std::ostream &s, const RbBitset &A);                            //!< operator <<
std::istream& operator>>(std::istream &s, RbBitset &A);                                  //!< operator >>
RbBitset    operator!(const RbBitset &A);                                              //!< operator !
RbBitset    &operator&=(RbBitset &A, const RbBitset &B);                             //!< operator &=
RbBitset    &operator|=(RbBitset &A, const RbBitset &B);                             //!< operator |=
RbBitset    &operator^=(RbBitset &A, const RbBitset &B);                             //!< operator ^=
bool          operator!=(const RbBitset &A, const RbBitset &B);                        //!< inequality operator
bool          operator< (const RbBitset &A, const RbBitset &B);                        //!< operator <
bool          operator<=(const RbBitset &A, const RbBitset &B);                        //!< operator <=
bool          operator>=(const RbBitset &A, const RbBitset &B);                        //!< operator >=


// Function definitions for inlined functions

/*!
 * Copy constructor, which creates a shallow copy of the
 * bitfield argument. Bitfield data are not copied but shared.
 * Thus, in RbBitset B(A), subsequent changes to A will be
 * reflected by changes in B. For an independent copy, use
 * RbBitset B(A.copy()), or B = A.copy(), instead. Note
 * the use of garbage collection in this class, through the
 * refCount argument
 *
 * \brief Shallow copy constructor
 * \param A Bitfield to copy
 */
inline RbBitset::RbBitset(const RbBitset &A)
    : v(A.v), m(A.m), n(A.n), mask(A.mask), refCount(A.refCount) {

	(*refCount)++;	

}

/*!
 * Create a reference (shallow assignment) to another existing bitfield.
 * In B.ref(A), B and A share the same data and subsequent changes
 * to the bits of one will be reflected in the other. Note that
 * the reference counter is always allocated, even for null bitfields,
 * so we need not test whether refCount is NULL.
 *
 * This is what operator= calls, and B=A and B.ref(A) are equivalent
 * operations.
 *
 * \brief Make this reference to A
 */
inline RbBitset &RbBitset::ref(const RbBitset &A) {

	if (this != &A) {
        (*refCount)--;
	    if ( *refCount < 1)
		    destroy();

		m = A.m;
		n = A.n;
		v = A.v;
		mask = A.mask;
		refCount = A.refCount;
		(*refCount)++;
	}
	return *this;
	
}

/*!
 * This function performs bitwise AND (&) of two bitfields
 * and returns the resulting bitfield.
 *
 * \brief operator&
 * \param A Bitfield to AND with this
 * \return Resulting bitfield
 */
RbBitset RbBitset::operator&(const RbBitset &A) const {

	if (A.dim() != n )
		return RbBitset();
	else {
		RbBitset B(n);
		for (int i=0; i<m; i++)
			B[i] = v[i] & A[i];
		return B;
	}

}

/*!
 * This function performs bitwise OR (|) of two bitfields
 * and returns the resulting bitfield.
 *
 * \brief operator|
 * \param A Bitfield to OR with this
 * \return Resulting bitfield
 */
RbBitset RbBitset::operator|(const RbBitset &A) const {

	if (A.dim() != n )
		return RbBitset();
	else {
		RbBitset B(n);
		for (int i=0; i<m; i++)
			B[i] = v[i] | A[i];
		return B;
	}

}

/*!
 * This function performs bitwise XOR (^) of two bitfields
 * and returns the resulting bitfield.
 *
 * \brief operator^
 * \param A Bitfield to XOR with this
 * \return Resulting bitfield
 */
RbBitset RbBitset::operator^(const RbBitset &A) const {

	if (A.dim() != n )
		return RbBitset();
	else {
		RbBitset B(n);
		for (int i=0; i<m; i++)
			B[i] = v[i] ^ A[i];
		return B;
	}

}

/*!
 * Set all bits.
 *
 * \brief Set bits
 */
inline void RbBitset::setBits(void) {
	
	for (int i=0; i<m; i++)
		v[i] = ULONG_MAX;
	v[m-1] &= mask;	// ensure clear trailing bits

}

/*!
 * Clear all bits.
 *
 * \brief Set bits
 */
inline void RbBitset::clearBits(void) {
	
	for (int i=0; i<m; i++)
		v[i] = 0;

}

/*!
 * Flip all bits.
 *
 * \brief Set bits
 */
inline void RbBitset::flipBits(void) {

	for (int i=0; i<m; i++)
		v[i] = v[i] ^ ULONG_MAX;
	v[m-1] &= mask;	// ensure clear trailing bits

}

/*!
 * Return number of on bits.
 *
 * \brief Number on bits
 * \return Number on bits
 */
inline int RbBitset::numOnBits(void) const {
	
#	if 1
	int count = 0;
	for (int i=0; i<n; i++)
		if ( (v[i/bpl] & (hb >> (i % bpl))) != 0 )
			count++;
	return count;
#	else
	int count = 0;
	for (int i=0; i<m; i++) {
		unsigned long x = v[i];
		while (x != 0) {
			count++;
			x &= (x-1);
		}
	}
	return count;
#	endif

}

/*!
 * Return 0-based index of first bit that
 * is on. If no bits are on, return -1.
 *
 * \brief First on bit
 * \return Index of first on bit or -1 if no bits are on
 */
inline int RbBitset::firstOnBit(void) const {
	
	int i, j;
	for (i=0; i<m; i++) {
		if (v[i] != 0)
			break;
	}
	if (i == m)
		return -1;
	for (j=0; j<bpl; j++) {
		if ((v[i]&(hb>>j))!=0)
			break;
	}
	return (i*bpl + j);

}

#endif
