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
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Simplex_H
#define Simplex_H

#include "VectorRealPos.h"

#include <ostream>
#include <string>
#include <vector>


/**
 * This class is used to hold a simplex. Note that a simplex cannot
 * be a container because then it would be a loose collection of
 * RealPos numbers. Such a collection could not be associated with
 * a single distribution. Nor would type checking work properly then,
 * because the language type would then be +Real[] and not Simplex.
 *
 * For these reasons, we derive simplex directly from RbComplex. We
 * implement subscripting so that you can access the elements through
 * subscripting (their values, no modify access). The subscript operator
 * is also implemented to give the RevBayes source code access to element
 * values, but not references.
 *
 * @note Right now, this is the only subscript-enabled data type. In the
 *       future, we probably want to separate out some of the functionality
 *       into an abstract base class for subscript-enabled data types. We
 *       override all of RbComplex element access functions, but for all
 *       access functions, we just modify the error message.
 *
 * @note Note that the object has dim 0 even though it support subscripting.
 *       This is because a simplex has the language type Simplex, and not
 *       +Real[]. We do not override getTypeSpec(), which will return dim
 *       0 and type Simplex.
 */
class Simplex : public RbComplex {

    public:
                                Simplex(const size_t n);                            //!< Simplex of length (size) n
                                Simplex(const std::vector<double>& x);              //!< Simplex from double vector
                                Simplex(const VectorRealPos& x);                    //!< Simplex from positive real vector

        // Overloaded operators
        double                  operator[](size_t i) const;                                         //!< Index op giving copy - no element mod allowed

        // Basic utility functions
        Simplex*                clone(void) const;                                                  //!< Clone object
        const VectorString&     getClass(void) const;                                               //!< Get class
        void                    printValue(std::ostream& o) const;                                  //!< Print value for user
        std::string             richInfo(void) const;                                               //!< Complete info about object

        // Element access functions
        const RbObject*         getElement(const VectorInteger& index) const;                       //!< Get element (read-only)
        virtual void            setElement(const VectorInteger& index, RbObject* val);              //!< Set value element (throw error)
        virtual bool            supportsSubscripting(void) const { return true; }                   //!< Does object support subscripting?

        // Simplex functions
        void                    setValue(const VectorRealPos& x);                                   //!< Set value from VectorRealPos & rescale
        void                    setValue(const std::vector<double>& x);                             //!< Set value from vector<double>, check & rescale
        std::vector<double>     getValue(void) const { return value; }                              //!< Get value

    private:
        void                    rescale(void);                                                      //!< Rescale the simplex

        std::vector<double>     value;                                                              //!< Vector containing values
};

#endif
