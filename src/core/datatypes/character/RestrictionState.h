/**
 * @file
 * This file contains the declaration of RestrictionState, which is
 * the class for the Standard data types in RevBayes.
 *
 * @brief Declaration of RestrictionState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RestrictionState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef RestrictionState_H
#define RestrictionState_H

#include "StandardState.h"

namespace RevBayesCore {

    class RestrictionState : public StandardState {
    
    public:
                                        RestrictionState(void);                                        //!< Default constructor
                                        RestrictionState(const std::string &s);
    };

}

#endif

