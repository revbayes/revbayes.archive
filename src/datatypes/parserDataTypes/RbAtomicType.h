/**
 * @file
 * This file contains the declaration of RbAtomicType, which is
 * the REvBayes base class for atomic data types.
 *
 * @brief Declaration of RbDataType
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#ifndef RbAtomicType_H
#define RbAtomicType_H

#include "RbObject.h"
#include <iostream>


class RbAtomicType : public RbObject {
    
    public:
            virtual ~RbAtomicType() {}

        virtual void            print(std::ostream& c=std::cout) const {}   //!< Print to specified ostream
};

/** @todo Overload << stream operator */

#endif
