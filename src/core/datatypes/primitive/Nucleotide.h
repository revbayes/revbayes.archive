/**
 * @file
 * This file contains the declaration of Nucleotide, which is
 * the abstract base class for nucleotide data types in
 * RevBayes.
 *
 * @brief Declaration of Nucleotide
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Nucleotide_H
#define Nucleotide_H

#include "CharacterState.h"

#include <ostream>
#include <string>


class Nucleotide : public CharacterState {

    public:
        // Basic utility functions you have to override
        virtual Nucleotide*             clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class vector
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Categorical function you should not have to override
        const std::string&              getNALabels(void) const;                                            //!< Get NA labels corresponding to value = -1, -2, etc

        // Categorical function you have to override
        virtual const std::string&      getStateLabels(void) const = 0;                                     //!< Get valid state labels

    protected:
                                        Nucleotide(void);                                                   //!< Default constructor
                                        Nucleotide(int x);                                                  //!< Constructor from int value
                                        Nucleotide(char c);                                                 //!< Constructor from char value (state code or symbol)
};

#endif

