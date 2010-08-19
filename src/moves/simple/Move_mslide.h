/**
 * @file
 * This file contains the declaration of Move_mslide, which moves
 * a real value uniformly within a sliding window.
 *
 * @brief Declaration of Move_mslide
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

#ifndef Move_mslide_H
#define Move_mslide_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "Workspace.h"

class RbDouble;
class RbObject;

class Move_mslide : public SimpleMove {

    public:
                                    Move_mslide(void);                                                      //!< Parser constructor
                                    Move_mslide(StochasticNode* node, double delta, double weight = 1.0,
									RandomNumberGenerator* rng = Workspace::globalWorkspace().get_rng());   //!< Internal constructor

        // Basic utility functions
        RbObject*                   clone(void) const;                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                   //!< Get class vector

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules

	protected:
        RbObject*                   convertTo(const std::string& type) const;                               //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;                         //!< Is convertible to type and dim?
        double                      perform(std::set<StochasticNode*>& affectedNodes);                      //!< Perform move
};

#endif

