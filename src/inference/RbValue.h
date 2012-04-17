/**
 * @file
 * This file contains the declaration of RbValue, which is
 * the RevBayes class for all values on the inference machinery part of RevBayes.
 *
 * RbValue can contain values of any dimensionality (e.g. scalars, vectors, matrices ...).
 * The value is stored by a pointer to the first element and a vector of length for each dimension.
 *
 * @brief Declaration of RbValue
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-04-17
 *
 * $Id: RbValue.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef RbValue_H
#define RbValue_H

#include <vector>

template<class valueType>
class RbValue {
    
public:
    virtual                            ~RbValue(void) {}                                                         //!< Virtual destructor
    
    valueType                           value;
    std::vector<size_t>                 lengths;
    
};

#endif
