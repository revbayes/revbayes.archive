/**
 * @file
 * This file contains the declaration of CharacterStateDiscrete, which is
 * the abstract base class for character observations, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of CharacterStateDiscrete
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterStateDiscrete_H
#define CharacterStateDiscrete_H

#include "RlCharacterState.h"
#include <ostream>
#include <vector>


class RlDiscreteCharacterState : public RlCharacterState {

    public:
        virtual bool                            operator==(const RlCharacterState& x) const = 0;    //!< Equality

        // Basic utility functions you can override
        virtual const MemberRules&              getMemberRules(void) const;                         //!< Get member rules
        virtual void                            initialize(const std::vector<RbObject*>& attributes);//!< Initialize this object with the values inside the vector

        // Basic utility functions you have to override
        virtual RlDiscreteCharacterState*       clone(void) const = 0;                              //!< Clone object
        static const std::string&               getClassName(void);                                 //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                             //!< Get class type spec
        virtual void                            printValue(std::ostream& o) const = 0;              //!< Print value (for user)


    protected:
                                                RlDiscreteCharacterState();                         //!< Constructor taking the number of states as input
};

#endif

