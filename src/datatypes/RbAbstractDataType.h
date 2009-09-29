/**
 * @file
 * This file contains the declaration of RbAbstractDataType, which is
 * the REvBayes base class for the data types handled by the parser.
 *
 * @brief Declaration of RbAbstractDataType
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @implements RbDataType (abstract)
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#ifndef RbAbstractDataType_H
#define RbAbstractDataType_H

#include "RbDataType.h"
#include "RbTypeInfo.h"
#include <iostream>
#include <string>

class RbAbstractDataType : public RbDataType {

    public:
           virtual ~RbAbstractDataType();                       //!< Destructor; delete attributes
       	virtual bool isConvertible(const RbDataType& dt) const;       //!< is this data type convertible into the data type dt
       	virtual bool isConvertible(const RbTypeInfo& dt) const;      //!< is this data type convertible into the data type dt
       	virtual RbDataType* convertTo(const RbDataType& dt) const;    //!< convert this data type into the datat type dt
       	virtual RbDataType* convertTo(const RbTypeInfo& dt) const;   //!< convert this data type into the datat type dt

//    	int* getDimension();                                    //!< get the dimensions
//    	std::string* getLabels();                               //!< get the labels for the different attributes of this data type

        virtual bool                 isComparable() const;            //!< can we compare instances of this type?
        // Overloaded comparison functions
        virtual bool                 operator<(RbDataType& o) const;  //!< Less than operator

        // Overloaded arithmetic functions
        virtual RbDataType&          operator+(RbDataType& o) const;  //!< Addition
        virtual RbDataType&          operator-(RbDataType& o) const;  //!< Subtraction
        virtual RbDataType&          operator*(RbDataType& o) const;  //!< Multiplication
        virtual RbDataType&          operator/(RbDataType& o) const;  //!< Division
        virtual RbDataType&          operator^(RbDataType& o) const;  //!< power

        // Return types of arithmetic functions
        virtual RbTypeInfo&          operator+(RbTypeInfo& t) const;  //!< Addition
        virtual RbTypeInfo&          operator-(RbTypeInfo& t) const;  //!< Subtraction
        virtual RbTypeInfo&          operator*(RbTypeInfo& t) const;  //!< Multiplication
        virtual RbTypeInfo&          operator/(RbTypeInfo& t) const;  //!< Division
        virtual RbTypeInfo&          operator^(RbTypeInfo& t) const;  //!< Power

    protected:
        RbAbstractDataType(const char* name);                   //!< Constructor from type name (C style)
        RbAbstractDataType(const std::string& name);            //!< Constructor from type name (string)


    private:
};

#endif
