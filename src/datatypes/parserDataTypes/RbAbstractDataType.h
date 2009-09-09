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
 * @implements RbDataType (partially)
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#ifndef RbAbstractDataType_H
#define RbAbstractDataType_H

#include "RbDataType.h"
#include <iostream>
#include <string>

/* Forward declarations */
class RbInt;
class RbString;

using namespace std;

class RbAbstractDataType : public RbDataType {

    public:
	        RbAbstractDataType(const char* name);               //!< Constructor from type name (C style)
	        RbAbstractDataType(const string& name=dataType);    //!< Constructor from type name (string)
            virtual ~RbAbstractDataType();                      //!< Destructor; delete attributes

        static const string         dataType;                                   //!< An abstract type

        virtual void                addElements(RbDataType& x) {}               //!< Add elements to vector
	    virtual RbAbstractDataType* copy(void) const = 0;                       //!< Copy
        virtual void                erase(void) {}                              //!< Erase vector
	    const RbInt*                getDim(void) const { return dim; }          //!< Get dim attribute
	    const RbString*             getNames(void) const { return names; }      //!< Get names attribute
	    virtual int                 getSize(void) const = 0;                    //!< Get size of vector
	    const string&               getType(void) const { return typeName; }    //!< Get type name
        virtual void                print(std::ostream& c=std::cout) const {}   //!< Print to specified ostream
	    void                        setDim(const RbInt& newDim);                //!< Get dim attribute
	    void                        setNames(const RbString& newNames);         //!< Get names attribute

    protected:
#pragma mark Attributes
        RbInt*          dim;        //!< Dimensions
        RbString*       names;      //!< Labels of elements

    private:
#pragma mark Private member variables
	    const string    typeName;   //!< Name of data type (for type checking or debugging)
};

#endif
