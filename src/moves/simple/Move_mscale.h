/**
 * @file
 * This file contains the declaration of Move_mscale, which scales a
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Declaration of Move_mscale
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

#ifndef Move_mscale_H
#define Move_mscale_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "Workspace.h"

class Real;
class RbObject;

class Move_mscale : public SimpleMove {

    public:
                                    Move_mscale(void);                                                      //!< Parser constructor
                                    Move_mscale(StochasticNode* node, double lambda, double weight = 1.0, 
									RandomNumberGenerator* rng = Workspace::globalWorkspace().get_rng());   //!< Internal constructor

        // Basic utility functions
        Move_mscale*                clone(void) const;                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                   //!< Get class vector

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules

	protected:
        RbObject*                   convertTo(const std::string& type) const;                               //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;                         //!< Is convertible to type and dim?
        double                      perform(std::set<StochasticNode*>& affectedNodes);                      //!< Perform move
};

#endif

