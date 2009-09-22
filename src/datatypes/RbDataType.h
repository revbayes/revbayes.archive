/*!
 * \file
 * This file contains the declaration of RbDataType, which is
 * the REvBayes base class for basic data types.
 *
 * \brief Declaration of RbDataType
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef RbDataType_H
#define RbDataType_H

#include <string>
#include "../main/RbObject.h"

using namespace std;

class RbDataType : public RbObject{

public:
	//static const std::string         dataType;                                 //!< the name/description of the data type

	virtual bool                isConvertible(const RbDataType& dt) const = 0;             //!< is this data type convertible into the data type dt
	virtual bool                isConvertible(const std::string& dt) const = 0;            //!< is this data type convertible into the data type dt
	virtual RbDataType*         convertTo(const RbDataType& dt) const = 0;                 //!< convert this data type into the datat type dt
	virtual RbDataType*         convertTo(const std::string& dt) const = 0;                //!< convert this data type into the datat type dt

//	virtual int                 getDimension() = 0;                            //!< get the dimensions
//	virtual std::string*        getLabels() = 0;                               //!< get the labels for the different attributes of this data type
//    virtual void                setDim(const int* newDim) = 0;                 //!< Get dim attribute
//    virtual void                setNames(const int* newNames) = 0;             //!< Get names attribute
    virtual const std::string&  getType(void) const = 0;                       //!< Get type name

    // declare overloaded functions
    virtual RbDataType&          operator+(RbDataType& o) const =0;            //!< Addition
    virtual RbDataType&          operator-(RbDataType& o) const =0;            //!< Subtraction
    virtual RbDataType&          operator*(RbDataType& o) const =0;            //!< Multiplication
    virtual RbDataType&          operator/(RbDataType& o) const =0;            //!< Division
    virtual RbDataType&          operator^(RbDataType& o) const =0;            //!< power


protected:
	RbDataType(const char* name) : RbObject(name) {}                           //!< Constructor from type name (C style)
	RbDataType(const string& name) : RbObject(name) {}                         //!< Constructor from type name (string)
#pragma mark Attributes
	int dim;                                                   //!< the int-array with the number of dimensions
	std::string* labels;                                        //!< the string-array of labels for the different attributes of this data type

private:
};

#endif
