/**
 * @file Cloneable.h
 *
 *
 * @brief Declaration of template cloneable. The only method this interface defines
 * is T* clone(void) const. The clone function is required in e.g. DAG nodes if the
 * type is abstract (e.g. RateMatrix, Function, ...)
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-03-07 15:57:47 +0100 (Wed, 07 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2013-04-05, version 1.0
 *
 * $Id: RbPtr.h 1325 2012-03-07 14:57:47Z hoehna $
 */

#ifndef Cloneable_H
#define Cloneable_H

#include <string>

namespace RevBayesCore {
    
    class Cloneable {
        
    public:
        virtual                         ~Cloneable(void) {}
        
        virtual Cloneable*              clone( void ) const = 0;                                    //!< Create a clone/copy of the object
    };
    
}

#endif

