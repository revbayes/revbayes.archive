/**
 * @file
 * This file contains the declaration of Move_mslide, which moves
 * a real value uniformly within a sliding window.
 *
 * @brief Declaration of Move_mslide
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

#ifndef Move_mslide_H
#define Move_mslide_H

#include <ostream>
#include <set>
#include <string>

#include "MoveSimple.h"
#include "RandomNumberFactory.h"


class Move_mslide : public MoveSimple {

    public:
                                    Move_mslide(void);                                                      //!< Parser constructor
    Move_mslide(const Move_mslide &ms);                                                                 //!< Copy Constructor

        // Basic utility functions
        Move_mslide*                clone(void) const;                                                      //!< Clone object
        static const std::string&   getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object

        // Member variable inits
        const MemberRules&          getMemberRules(void) const;                                             //!< Get member rules
        void                        setMemberVariable(const std::string& name, Variable* var);              //!< Set member variable (ensure topologyProb is updated)

        // Return variable type
        const TypeSpec              getVariableType(void) const;                                            //!< Get move variable type

	protected:
        double                      perform(void);                                                          //!< Perform move
    
    private:

        // parameters
        RbVariablePtr               delta;
};

#endif

