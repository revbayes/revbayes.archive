/*!
 * \file
 * This file contains the declaration of RbInt, which is
 * a REvBayes wrapper around a regular int.
 *
 * \brief Declaration of RbInt
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbInt_H
#define RbInt_H

#include "RbAbstractDataType.h"
#include "RbDataType.h"
#include "RbTypeInfo.h"

class RbInt : public RbAbstractDataType {

public:

    RbInt(const int v);                                            //!< Constructor from double
    RbInt(const int v, std::string& name);                         //!< Constructor from double
    RbInt(const RbInt& s);                                         //!< Copy constructor
	virtual ~RbInt();

    // implemented abstract/virtual functions from base classes
	RbObject*           clone(void) const ;                                 //!< clone this object
	void                print(std::ostream &c) const;                       //!< Print the value to ostream c
	void                dump(std::ostream& c);                              //!< Dump to ostream c
	void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
	bool                isConvertible(const RbDataType& dt) const;          //!< is this data type convertible into the data type dt
   	bool                isConvertible(const RbTypeInfo& dt) const;          //!< is this data type convertible into the data type dt
   	RbDataType*         convertTo(const RbDataType& dt) const;              //!< convert this data type into the datat type dt
   	RbDataType*         convertTo(const RbTypeInfo& dt) const;              //!< convert this data type into the datat type dt

	//overloaded operators
	bool                operator==(const RbObject& o) const;                //!< Comparison
	bool                operator==(const RbDataType& o) const;              //!< Comparison
	bool                operator==(const RbInt& o) const;                   //!< Comparison
	RbDataType&         operator+(const RbDataType& o) const;               //!< Addition
	RbInt&              operator+(const RbInt& o) const;                    //!< Addition


	// memeber functions
	void       setValue(int v);
	int        getValue(void) const;

private:
	int        value;      //!< value member
};


#endif
