/**
 * @file
 * This file contains the declaration of RnaState, which is
 * the class for the RNA data types in RevBayes.
 *
 * @brief Declaration of RnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RnaState_H
#define RnaState_H

#include "NucleotideState.h"
#include <ostream>
#include <set>
#include <vector>



const std::string RnaState_name = "RNA";

class RnaState : public NucleotideState {

    public:
                                        RnaState(void);                                     //!< Default constructor
                                        RnaState(const RnaState& s);                        //!< Copy constructor
                                        RnaState(const char s);                             //!< Constructor with nucleotide observation
                                        RnaState(const std::set<char> s);                   //!< Constructor from a set of states
        bool                            operator==(const Character& x) const;               //!< Equality
        bool                            operator!=(const Character& x) const;               //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                  //!< Print value (for user)

        // Basic utility functions you have to override
        RnaState*                       clone(void) const;                                  //!< Clone object
        const VectorString&             getClass(void) const;                               //!< Get class vector
        const TypeSpec&                 getTypeSpec(void) const;                            //!< Get language type of the object

        // Discrete character observation functions
        const std::string&              getStateLabels(void) const { return stateLabels; }  //!< Get valid state labels
        const char                      getState(void) const;                               //!< Get the discrete observation
        virtual std::string             getStringValue(void) const;                         //!< Get a representation of the nucleotide as a string

    protected:
        const static std::string        stateLabels;                                        //!< The labels for the possible states
        
    private:
        const char                      getNucleotideCode(const std::set<char>& s) const;
        const char                      getNucleotideCode(const std::vector<bool>& sSet) const;
    
        static const TypeSpec           typeSpec;
};

#endif

