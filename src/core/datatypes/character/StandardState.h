/**
 * @file
 * This file contains the declaration of StandardState, which is
 * the class for the Standard data types in RevBayes.
 *
 * @brief Declaration of StandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: StandardState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef StandardState_H
#define StandardState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {

    class StandardState : public DiscreteCharacterState {
    
    public:
                                        StandardState(void);                                        //!< Default constructor
                                        StandardState(const StandardState &s);                      //!< Copy constructor
                                        StandardState(const std::string &s);                        //!< Constructor with nucleotide observation
                                        StandardState(const std::string& s, const std::string& l);
        bool                            operator==(const CharacterState& x) const;                  //!< Equality
        bool                            operator!=(const CharacterState& x) const;                  //!< Inequality
        bool                            operator<(const CharacterState& d) const;                   //!< Less than
        void                            operator++();                                               //!< Increment
        void                            operator++(int i);                                          //!< Increment
        void                            operator+=(int i);                                          //!< Increment
        void                            operator--();                                               //!< Decrement
        void                            operator--(int i);                                          //!< Decrement
        void                            operator-=(int i);                                          //!< Decrement
    
        StandardState*                  clone(void) const;                                          //!< Get a copy of this object
    
        // Discrete character observation functions
        void                            addState(const std::string &symbol);                        //!< Add a character state to the set of character states
        std::string                     getDataType(void) const;                                    //!< Get the datatype as a common string.
        unsigned int                    getNumberObservedStates(void) const;                        //!< How many states are observed for the character
        const std::string&              getStateLabels(void) const;                                 //!< Get valid state labels
        std::string                     getStringValue(void) const;                                 //!< Get a representation of the character as a string
        size_t                          getNumberOfStates(void) const;                              //!< Get the number of discrete states for the character
        unsigned long                   getState(void) const;                                       //!< Get the discrete observation
        size_t                          getStateIndex(void) const;
        bool                            isAmbiguous(void) const;                                    //!< Is the character missing or ambiguous
        void                            setStateByIndex(size_t index);                              //!< Set the discrete observation
        void                            setState(const std::string &symbol);                        //!< Set the discrete observation from string of state symbols
//        void                            setState(size_t pos, bool val);                             //!< Set the discrete observation from position and boolean flag
        void                            setToFirstState(void);                                      //!< Set this character state to the first (lowest) possible state
 
        
    private:
        double                          computeRootFilteredLikelihood2Nodes();
        double                          computeRootFilteredLikelihood3Nodes();
        unsigned long                   computeStateIndex(const std::string &symbol) const;         //!< Compute the internal state value for this character.
        unsigned long                   computeState(const std::string &symbol) const;

        std::string                     labels;
        unsigned long                   state;
        
    };

}

#endif

