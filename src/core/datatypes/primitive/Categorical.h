/**
 * @file
 * This file contains the declaration of Categorical, which is
 * the abstract base class for categorical data types in RevBayes.
 *
 * @brief Declaration of Categorical
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef Categorical_H
#define Categorical_H

#include "RbObject.h"

#include <ostream>
#include <string>


class Categorical : public RbObject {

    public:
        // Overloaded operators
                                        operator int(void) const { return value; }                          //!< Type conversion to int

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                                  //!< Print value (for user)

        // Basic utility functions you have to override
        virtual Categorical*            clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class vector
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Categorical functions you do not have to override
        int                             convertSymbolToState(char c) const;                                 //!< Convert symbol to state code
        int                             getNumStates(void) { return int( getStateLabels().size() ); }       //!< Get number of states
        int                             getValue(void) const { return value; }                              //!< Get value as an int
        bool                            isValidState(int x) const;                                          //!< Is valid state value?
        void                            setValue(int x);                                                    //!< Set value from int

        // Categorical functions you have to override
        virtual const std::string&      getStateLabels(void) const = 0;                                     //!< Get valid state labels
        virtual const std::string&      getNALabels(void) const = 0;                                        //!< Get NA labels corresponding to value = -1, -2, etc

    protected:
                                        Categorical(void);                                                  //!< Default constructor
                                        Categorical(int x);                                                 //!< Constructor from int value
                                        Categorical(char c);                                                //!< Constructor from char value (state code or symbol)

        // Member variables
        int                             value;                                                              //!< Current value, in [0,numStates]
};

#endif

