/**
 * @file
 * This file contains the declaration of Move_mmultinomial, which changes
 * a vector of positive reals.
 *
 * @brief Declaration of Move_mmultinomial
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Move_mmultinomial_H
#define Move_mmultinomial_H

#include <ostream>
#include <set>
#include <string>

#include "MoveSimple.h"
#include "RandomNumberFactory.h"

class Real;
class RbObject;

const std::string Move_mmultinomial_name = "mmultinomial move";

class Move_mmultinomial : public MoveSimple {

    public:
                                    Move_mmultinomial(void);                                                                //!< Parser constructor
                                    Move_mmultinomial(StochasticNode* node, double tuning, int nc, double weight = 1.0);    //!< Internal constructor

        // Basic utility functions
        Move_mmultinomial*          clone(void) const;                                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                                   //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                                //!< Get language type of the object

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                                             //!< Get member rules

        // Return variable type
        const TypeSpec              getVariableType(void) const;                                                            //!< Get move variable type

	protected:
        double                      perform(void);                                                                          //!< Perform move
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

