/**
 * @file
 * This file contains the declaration of Move_mscale, which scales a
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Declaration of Move_mscale
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

#ifndef Move_mscale_H
#define Move_mscale_H

#include <ostream>
#include <set>
#include <string>

#include "RandomNumberFactory.h"
#include "MoveSimple.h"


class Move_mscale : public MoveSimple {

    public:
                                    Move_mscale(void);                                                      //!< Parser constructor

        // Basic utility functions
        Move_mscale*                clone(void) const;                                                      //!< Clone object
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules

        // Return variable type
        const TypeSpec              getVariableType(void) const;                                            //!< Get move variable type

	protected:
        double                      perform(void);                                                          //!< Perform move

        // parameters
        RbVariablePtr               lambda;
};

#endif

