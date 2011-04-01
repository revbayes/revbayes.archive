/**
 * @file
 * This file contains the declaration of Simplex, a complex type
 * used to hold a simplex.
 *
 * @brief Declaration of Simplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Simplex_H
#define Simplex_H

#include "MemberObject.h"

#include <ostream>
#include <string>
#include <vector>

class VectorRealPos;


/**
 * @brief Simplex class
 *
 * This class is used to hold a simplex. Note that a simplex cannot
 * be a container because then it would be a loose collection of
 * RealPos numbers. Such a collection could not be associated with
 * a single distribution. Nor would type checking work properly then,
 * because the language type would then be +Real[] and not Simplex.
 *
 * For these reasons, we derive simplex from MemberObject. We
 * implement subscripting so that you can access the elements through
 * subscripting (their values, no modify access). The subscript operator
 * is also implemented to give the RevBayes source code access to element
 * values, but not references.
 */
class Simplex : public MemberObject {

    public:
                                Simplex(const size_t n = 2);                                        //!< Simplex of length (size) n
                                Simplex(const std::vector<double>& x);                              //!< Simplex from double vector
                                Simplex(const VectorRealPos& x);                                    //!< Simplex from positive real vector

        // Overloaded operators
        double                  operator[](size_t i) const;                                         //!< Index op giving copy - no element mod allowed

        // Basic utility functions
        Simplex*                clone(void) const;                                                  //!< Clone object
        const VectorString&     getClass(void) const;                                               //!< Get class
        void                    printValue(std::ostream& o) const;                                  //!< Print value for user
        std::string             richInfo(void) const;                                               //!< Complete info about object

        // Subscript access functions
        bool                    hasSubscript(void) { return true; }                                 //!< We support subscripting
        DAGNode*                getSubelement(VectorInteger& index) const;                          //!< Return subscript[](index) element
        size_t                  getSubelementsSize(void) { return value.size(); }                   //!< Number of subscript elements

        // Simplex functions
        void                    setValue(const VectorRealPos& x);                                   //!< Set value from VectorRealPos & rescale
        void                    setValue(const std::vector<double>& x);                             //!< Set value from vector<double>, check & rescale
        std::vector<double>     getValue(void) const { return value; }                              //!< Get value

    private:
        void                    rescale(void);                                                      //!< Rescale the simplex

        std::vector<double>     value;                                                              //!< Vector containing values
};

#endif
