/**
 * @file
 * This file contains the declaration of StandardState, which is
 * the class for the Standard data types in RevBayes.
 *
 * @brief Declaration of StandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef StandardState_H
#define StandardState_H

#include "CharacterObservationDiscrete.h"
#include <ostream>
#include <string>
#include <vector>



class StandardState : public CharacterObservationDiscrete {

    public:
                                        StandardState(void);                                       //!< Default constructor
                                        StandardState(const StandardState& s);                     //!< Copy constructor
                                        StandardState(const std::string ss);                       //!< Constructor with the set of possible states
        bool                            operator==(const StandardState& x) const;                  //!< Equality
        bool                            operator!=(const StandardState& x) const;                  //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                         //!< Print value (for user)

        // Basic utility functions you have to override
        StandardState*                  clone(void) const;                                         //!< Clone object
        const VectorString&             getClass(void) const;                                      //!< Get class vector
        std::string                     richInfo(void) const;                                      //!< Complete info about object

        // Discrete character observation functions
        void                            addState(const char s);                                    //!< Add a character state to the set of character states
        const std::string&              getStateLabels(void) const { return stateLabels; }         //!< Get valid state labels
        const char                      getState(void) const;                                      //!< Get the discrete observation
        void                            setState(const char s);                                    //!< Set the discrete observation
        void                            setStateLabels(const std::string ss) { stateLabels = ss; } //!< Set the labels for the possible states

    protected:
        std::string                     stateLabels;                                               //!< The labels for the possible states
};

#endif

