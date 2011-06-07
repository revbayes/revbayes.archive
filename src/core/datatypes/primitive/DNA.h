/**
 * @file
 * This file contains the declaration of DNA, which is
 * the data type for DNA characters in RevBayes.
 *
 * @brief Declaration of DNA
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef DNA_H
#define DNA_H

#include "Nucleotide.h"

#include <ostream>
#include <string>

class RNA;


class DNA : public Nucleotide {

    public:
                                DNA(void);                                                              //!< Default constructor (NA)
                                DNA(int x);                                                             //!< Constructor from int value
                                DNA(const char c);                                                      //!< Constructor from char value
                                DNA(const RNA& x);                                                      //!< Constructor from RNA character

        // Basic utility functions
        DNA*                    clone(void) const;                                                      //!< Clone object
        RbObject*               convertTo(const std::string& type, int dim) const;                      //!< Convert to type and dim
        const VectorString&     getClass(void) const;                                                   //!< Get class vector
        bool                    isConvertibleTo(const std::string& type, int dim, bool once) const;     //!< Is convertible to type and dim?
        std::string             richInfo(void) const;                                                   //!< Complete info about object

        // Categorical functions
        const std::string&      getStateLabels(void) const;                                             //!< Get valid state labels
};

#endif

