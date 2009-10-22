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
           virtual ~RbAbstractDataType();                                      //!< Destructor; delete attributes
       	virtual bool                isConvertible(const RbDataType& dt) const; //!< is this data type convertible into the data type dt
       	virtual bool                isConvertible(const RbTypeInfo& dt) const; //!< is this data type convertible into the data type dt
       	virtual RbDataType*         convertTo(const RbDataType& dt) const;     //!< convert this data type into the datat type dt
       	virtual RbDataType*         convertTo(const RbTypeInfo& dt) const;     //!< convert this data type into the datat type dt

       	virtual int                     getDimensions() const;                     //!< get the dimensions
       	virtual int*                    getLength() const;                         //!< get number of elements per dimension
        virtual const std::type_info&   getObjectType() const;                     //!< get the type of object
        virtual const std::type_info&   getElementType() const;                    //!< get the type of elements
//        virtual std::string*        getLabels() const;                         //!< get the labels for the different attributes of this data type

        virtual bool                isComparable() const;                      //!< can we compare instances of this type?
        // Overloaded comparison functions
        virtual bool                operator<(const RbDataType& o) const;      //!< Less than operator

        // Overloaded arithmetic functions
        virtual RbDataType&         operator+(const RbDataType& o) const;      //!< Addition
        virtual RbDataType&         operator-(const RbDataType& o) const;      //!< Subtraction
        virtual RbDataType&         operator*(const RbDataType& o) const;      //!< Multiplication
        virtual RbDataType&         operator/(const RbDataType& o) const;      //!< Division
        virtual RbDataType&         operator^(const RbDataType& o) const;      //!< power

        // Return types of arithmetic functions
        virtual RbTypeInfo&         operator+(const RbTypeInfo& t) const;      //!< Addition
        virtual RbTypeInfo&         operator-(const RbTypeInfo& t) const;      //!< Subtraction
        virtual RbTypeInfo&         operator*(const RbTypeInfo& t) const;      //!< Multiplication
        virtual RbTypeInfo&         operator/(const RbTypeInfo& t) const;      //!< Division
        virtual RbTypeInfo&         operator^(const RbTypeInfo& t) const;      //!< Power

    protected:
        RbAbstractDataType(const char* name);                   //!< Constructor from type name (C style)
        RbAbstractDataType(const std::string& name);            //!< Constructor from type name (string)
        int                         dimensions;
        int*                        length;
        const std::type_info&       elementType;


    private:
};

#endif
