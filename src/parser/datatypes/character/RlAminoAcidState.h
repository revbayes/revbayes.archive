/**
 * @file
 * This file contains the declaration of RlAminoAcidState, 
 * that is the class holding amino acid states in the RevLanguage.
 * This class is simply a wrapper for the cores implementation of
 * the amino acid state.
 *
 * @brief Declaration of AminoAcidState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlAminoAcidState_H
#define RlAminoAcidState_H

#include "AminoAcidState.h"
#include "RlDiscreteCharacterState.h"

#include <ostream>
#include <string>
#include <vector>


class RlAminoAcidState : public RlDiscreteCharacterState {

    public:
                                        RlAminoAcidState(void);                             //!< Default constructor
                                        RlAminoAcidState(const RlAminoAcidState& s);        //!< Copy constructor
//                                        RlAminoAcidState(const char s);                     //!< Constructor with nucleotide observation

        bool                            operator==(const RlCharacterState& x) const;        //!< Equality

        // the value type definition
        typedef AminoAcidState valueType;

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                  //!< Print value (for user)

        // Basic utility functions you have to override
        RlAminoAcidState*               clone(void) const;                                  //!< Clone object
        static const std::string&       getClassName(void);                                 //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                             //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                            //!< Get language type of the object

        const AminoAcidState&           getValue(void) const;
    
    private:
        AminoAcidState                  state;                                              //!< The internal element
    
};

#endif

