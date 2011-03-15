/**
 * @file
 * This file contains the declaration of Move_msimplex, which changes
 * a simplex.
 *
 * @brief Declaration of Move_msimplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Move_msimplex_H
#define Move_msimplex_H

#include <ostream>
#include <set>
#include <string>

#include "RandomNumberFactory.h"
#include "SimpleMove.h"

class Real;
class RbObject;

class Move_msimplex : public SimpleMove {

    public:
                                    Move_msimplex(void);                                                             //!< Parser constructor
                                    Move_msimplex(StochasticNode* node, double tuning, int nc, double weight = 1.0); //!< Internal constructor

        // Basic utility functions
        Move_msimplex*              clone(void) const;                                                               //!< Clone object
        const VectorString&         getClass(void) const;                                                            //!< Get class vector

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                                      //!< Get member rules

	protected:
        double                      perform(std::set<StochasticNode*>& affectedNodes);                               //!< Perform move
};

#endif

