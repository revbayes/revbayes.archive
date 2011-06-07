/**
 * @file
 * This file contains the declaration of RNA, which is
 * the data type for RNA characters in RevBayes.
 *
 * @brief Declaration of RNA
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RNA_H
#define RNA_H

#include "Nucleotide.h"

#include <ostream>
#include <string>

class DNA;


class RNA : public Nucleotide {

    public:
                                RNA(void);                                                              //!< Default constructor (NA)
                                RNA(int x);                                                             //!< Constructor from int value
                                RNA(const char c);                                                      //!< Constructor from char value
                                RNA(const DNA& x);                                                      //!< Constructor from DNA character

        // Basic utility functions
        RNA*                    clone(void) const;                                                      //!< Clone object
        RbObject*               convertTo(const std::string& type, int dim) const;                      //!< Convert to type and dim
        const VectorString&     getClass(void) const;                                                   //!< Get class vector
        bool                    isConvertibleTo(const std::string& type, int dim, bool once) const;     //!< Is convertible to type and dim?
        std::string             richInfo(void) const;                                                   //!< Complete info about object

        // Categorical functions
        const std::string&      getStateLabels(void) const;                                             //!< Get valid state labels
};

#endif

