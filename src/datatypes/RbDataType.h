/**
 * @file
 * This file contains the declaration of RbDataType, which is
 * the REvBayes base class for basic data types.
 *
 * @brief Declaration of RbDataType
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @interface RbDataType
 *
 * $Id$
 */

#ifndef RbDataType_H
#define RbDataType_H

#include <string>
#include "RbObject.h"
#include "RbTypeInfo.h"

class RbDataType : public RbObject{

public:
    virtual bool                isConvertible(const RbDataType& dt) const = 0;              //!< is this data type convertible into the data type dt
	virtual bool                isConvertible(const RbTypeInfo& t) const = 0;               //!< is this data type convertible into the data type t
	virtual RbDataType*         convertTo(const RbDataType& dt) const = 0;                  //!< convert this data type into the datat type dt
	virtual RbDataType*         convertTo(const RbTypeInfo& dt) const = 0;                 //!< convert this data type into the datat type dt

//	virtual int                 getDimension() = 0;                            //!< get the dimensions
//	virtual std::string*        getNames() = 0;                                //!< get the labels for the different attributes of this data type
//    virtual void                setDim(const int* newDim) = 0;                 //!< Get dim attribute
//    virtual void                setNames(const int* newNames) = 0;             //!< Get names attribute


	virtual bool                 isComparable() const =0;                      //!< can we compare instances of this type?
	// Overloaded comparison functions
    virtual bool                 operator<(RbDataType& o) const =0;            //!< Less than operator

    // Overloaded arithmetic functions
    virtual RbDataType&          operator+(RbDataType& o) const =0;            //!< Addition
    virtual RbDataType&          operator-(RbDataType& o) const =0;            //!< Subtraction
    virtual RbDataType&          operator*(RbDataType& o) const =0;            //!< Multiplication
    virtual RbDataType&          operator/(RbDataType& o) const =0;            //!< Division
    virtual RbDataType&          operator^(RbDataType& o) const =0;            //!< Power

    // Return types of arithmetic functions
    virtual RbTypeInfo&          operator+(RbTypeInfo& t) const =0;            //!< Addition
    virtual RbTypeInfo&          operator-(RbTypeInfo& t) const =0;            //!< Subtraction
    virtual RbTypeInfo&          operator*(RbTypeInfo& t) const =0;            //!< Multiplication
    virtual RbTypeInfo&          operator/(RbTypeInfo& t) const =0;            //!< Division
    virtual RbTypeInfo&          operator^(RbTypeInfo& t) const =0;            //!< Power

protected:
	RbDataType(const char* name) : RbObject(name) {}                           //!< Constructor from type name (C style)
	RbDataType(const std::string& name) : RbObject(name) {}                         //!< Constructor from type name (string)

private:
};

#endif
