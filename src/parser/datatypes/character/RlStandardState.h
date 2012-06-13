/**
 * @file
 * This file contains the declaration of RlStandardState, which is
 * the class for the Standard data types in RevBayes.
 *
 * @brief Declaration of RlStandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlStandardState_H
#define RlStandardState_H

#include "RlDiscreteCharacterState.h"
#include "StandardState.h"

#include <ostream>
#include <string>
#include <vector>


class RlStandardState : public RlDiscreteCharacterState {

    public:
                                        RlStandardState(void);                                     //!< Default constructor
                                        RlStandardState(const StandardState& s);                   //!< Copy constructor
//                                        RlStandardState(const std::string ss);                     //!< Constructor with the set of possible states

        bool                            operator==(const RlCharacterState& x) const;               //!< Equality

        // the value type definition
        typedef StandardState valueType;

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                         //!< Print value (for user)

        // Basic utility functions you have to override
        RlStandardState*                clone(void) const;                                         //!< Clone object
        static const std::string&       getClassName(void);                                        //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                    //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                   //!< Get language type of the object
        RbValue<void*>                  getLeanValue(void) const;                                  //!< Transform the object into a basic element pointer for fast access.
        size_t                          memorySize(void) const;                                    //!< Get the size

        const StandardState&            getValue(void) const;

    private:
        StandardState                   state;                                                      //!< The internal state
    
};

#endif

