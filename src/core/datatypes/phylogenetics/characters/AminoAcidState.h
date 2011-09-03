/**
 * @file
 * This file contains the declaration of AminoAcidState, which is
 * the class for the Amino Acid data types in RevBayes.
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

#ifndef AminoAcidState_H
#define AminoAcidState_H

#include "CharacterStateDiscrete.h"
#include <ostream>
#include <vector>



const std::string AminoAcidState_name = "Amino Acid";

class AminoAcidState : public CharacterStateDiscrete {

    public:
                                        AminoAcidState(void);                               //!< Default constructor
                                        AminoAcidState(const AminoAcidState& s);            //!< Copy constructor
                                        AminoAcidState(const char s);                       //!< Constructor with nucleotide observation
        bool                            operator==(const Character& x) const;               //!< Equality
        bool                            operator!=(const Character& x) const;               //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                  //!< Print value (for user)

        // Basic utility functions you have to override
        AminoAcidState*                 clone(void) const;                                  //!< Clone object
        const VectorString&             getClass(void) const;                               //!< Get class vector
        const TypeSpec&                 getTypeSpec(void) const;                            //!< Get language type of the object
        std::string                     richInfo(void) const;                               //!< Complete info about object

        // Discrete character observation functions
        void                            addState(const char s);                             //!< Add a character state to the set of character states
        const std::string&              getStateLabels(void) const { return stateLabels; }  //!< Get valid state labels
        const char                      getState(void) const;                               //!< Get the amino acid observation
        void                            setState(const char s);                             //!< Set the amino acid observation

    protected:
        const static std::string        stateLabels;                                        //!< The labels for the possible states
    
    private:
        static const TypeSpec           typeSpec;
};

#endif

