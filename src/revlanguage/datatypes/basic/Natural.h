/**
 * @file
 * This file contains the declaration of Natural, which is
 * the primitive RevBayes type for natural numbers (including 0).
 *
 * @brief Declaration of Natural
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Natural_H
#define Natural_H

#include "Integer.h"

#include <ostream>
#include <string>


/**
 * Primitive type for Natural numbers (including 0).
 *
 * Note that we derive this from Integer. To make
 * sure inheritance is safe, we restrict the range
 * of natural numbers from 0 to to INT_MAX
 */

namespace RevLanguage {

class Natural : public Integer {

    public:
        Natural(void);                                                                                      //!< Default constructor (value is 0)
        Natural(RevBayesCore::TypedDagNode<int> *v);                                                        //!< Constructor with DAG node
        Natural(int x);                                                                                     //!< Constructor from int
        Natural(unsigned int x);                                                                            //!< Constructor from int
        Natural(unsigned long x);                                                                           //!< Constructor from size_t

        // Overloaded operators

        // Basic utility functions
        Natural*                    clone(void) const;                                                      //!< Clone object
        RbLanguageObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?

};
    
}

#endif

