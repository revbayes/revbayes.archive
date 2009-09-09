/**
 * @file
 * This file contains the declaration of RbDataType, which is
 * the REvBayes interface for the data types handled by the parser.
 *
 * @brief Declaration of RbDataType
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @interface RbDataType
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#ifndef RbDataType_H
#define RbDataType_H

#include "RbObject.h"
#include <iostream>
#include <string>

/* Forward declarations */
class RbInt;
class RbString;

class RbDataType : public RbObject {

    public:
            virtual ~RbDataType() {}    //!< Virtual destructor because of virtual functions
    
        virtual void                addElements(RbDataType& x) = 0;         //!< Add elements to vector
	    virtual RbDataType*         copy(void) const = 0;                   //!< Copy
        virtual void                erase(void) = 0;                        //!< Erase vector
	    virtual const RbInt*        getDim(void) const = 0;                 //!< Get dim attribute
	    virtual const RbString*     getNames(void) const = 0;               //!< Get names attribute
	    virtual int                 getSize(void) const = 0;                //!< Get size of vector
	    virtual const std::string&  getType(void) const = 0;                //!< Get type name
        virtual void                print(std::ostream &c = std::cout) const = 0;  //!< Print to pecified ostream
	    virtual void                setDim(const RbInt& newDim) = 0;        //!< Get dim attribute
	    virtual void                setNames(const RbString& newNames) = 0; //!< Get names attribute
};

#endif
