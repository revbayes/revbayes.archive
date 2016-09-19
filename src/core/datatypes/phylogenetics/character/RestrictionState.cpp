/**
 * @file
 * This file contains the implementation of RestrictionState, which is
 * the base class for the Standard character data type in RevBayes.
 *
 * @brief Implementation of RestrictionState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RestrictionState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "RestrictionState.h"

using namespace RevBayesCore;

/** Default constructor */
RestrictionState::RestrictionState(void) : StandardState("0", "01")
{

}


/** Default constructor */
RestrictionState::RestrictionState(const std::string &s) : StandardState(s, "01")
{ 
}


