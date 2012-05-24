/**
 * @file
 * This file contains the declaration of RlContinuousCharacterState, which is
 * the class for the continuous data types in RevBayes.
 *
 * @brief Declaration of RlContinuousCharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlContinuousCharacterState_H
#define RlContinuousCharacterState_H

#include "RlCharacterState.h"
#include "ContinuousCharacterState.h"

#include <ostream>
#include <vector>



class RlContinuousCharacterState : public RlCharacterState {

    public:
                                        RlContinuousCharacterState(void);                                       //!< Default constructor
                                        RlContinuousCharacterState(const RlContinuousCharacterState& s);        //!< Copy constructor
                                        RlContinuousCharacterState(const ContinuousCharacterState &x);          //!< Constructor with mean value

        bool                            operator==(const CharacterState& x) const;                              //!< Equality
        bool                            operator!=(const CharacterState& x) const;                              //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)

        // Basic utility functions you have to override
        RlContinuousCharacterState*            clone(void) const;                                               //!< Clone object
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object


    private:
        ContinuousCharacterState        state;                                                                  //!< Internal value
    
};

#endif

