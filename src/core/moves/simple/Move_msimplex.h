/**
 * @file
 * This file contains the declaration of Move_msimplex, which changes
 * a simplex.
 *
 * @brief Declaration of Move_msimplex
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

#ifndef Move_msimplex_H
#define Move_msimplex_H

#include <ostream>
#include <set>
#include <string>

#include "MoveSimple.h"
#include "RandomNumberFactory.h"

class Real;
class RbObject;

const std::string Move_msimplex_name = "msimplex move";

class Move_msimplex : public MoveSimple {

    public:
                                    Move_msimplex(void);                                                                //!< Parser constructor
                                    Move_msimplex(StochasticNode* node, double tuning, int nc, double weight = 1.0);    //!< Internal constructor

        // Basic utility functions
        Move_msimplex*              clone(void) const;                                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                                               //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                            //!< Get language type of the object

        // Member variable inits
        RbPtr<const MemberRules>    getMemberRules(void) const;                                                         //!< Get member rules

        // Return variable type
        const TypeSpec              getVariableType(void) const;                                                        //!< Get variable type for move

    protected:
        double                      perform(void);                                                                      //!< Perform move
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

