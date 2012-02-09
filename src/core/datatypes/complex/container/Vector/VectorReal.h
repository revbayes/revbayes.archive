/**
 * @file
 * This file contains the declaration of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Declaration of VectorReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#ifndef VectorReal_H
#define VectorReal_H

#include "Real.h"
#include "AbstractVector.h"

#include <iostream>
#include <vector>

const std::string VectorReal_name = "Real vector";

class VectorRealPos;


/**
 * @brief Class used for real vectors.
 *
 * @note VectorRealPos cannot be derived
 *       from this class without giving
 *       inappropriate access to elements.
 */

class VectorReal : public AbstractVector {

    public:
                                VectorReal(void);                                               //!< Default constructor (empty vector)
                                VectorReal(const std::string& elemType);                        //!< Default constructor for derived classes
                                VectorReal(double x);                                           //!< Construct vector with one double x
                                VectorReal(size_t n, double x);                                 //!< Construct vector with n doubles x
                                VectorReal(const std::vector<double>& x);                       //!< Constructor from double vector
                                VectorReal(const VectorRealPos& x);                             //!< Constructor from VectorRealPos

    // Overloaded operators
    double&                     operator[](size_t i);                                           //!< Index op allowing change
    const double&               operator[](size_t i) const;                                     //!< Const index op
    bool                        operator==(const VectorReal& x) const;                          //!< Equals comparison
    bool                        operator!=(const VectorReal& x) const;                          //!< Not equals comparison
    
    // Basic utility functions
    VectorReal*                 clone(void) const;                                              //!< Clone object
    RbObject*                   convertTo(const TypeSpec& type) const;                          //!< Convert to type
    const VectorString&         getClass(void) const;                                           //!< Get class
    const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
    bool                        isConvertibleTo(const TypeSpec& type) const;                    //!< Is convertible to type?
    void                        printValue(std::ostream& o) const;                              //!< Print value (for user)

        // Vector functions, including STL-like functions
    void                        clear(void);                                                    //!< Clear
    const RbObject&             getElement(size_t index) const;                                 //!< Get element
    RbObject&                   getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    std::vector<double>         getValue(void) const;                                           //!< Get value
    void                        pop_back(void);                                                 //!< Drop element at back
    void                        pop_front(void);                                                //!< Drop element from front
    void                        push_back(RbObject* x);                                   //!< Append element to end
    void                        push_back(double x);                                            //!< Append element to end
    void                        push_front(RbObject* x);                                  //!< Add element in front
    void                        push_front(double x);                                           //!< Add element in front
    void                        resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                        setElement(const size_t index, RbLanguageObject* elem);   //!< Set element with type conversion
    void                        setValue(const VectorReal& x);                                  //!< Set the value
    void                        setValue(const std::vector<double>& x);                         //!< Set the value
    void                        sort(void);                                                     //!< sort the AbstractVector
    size_t                      size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                        unique(void);                                                   //!< removes consecutive duplicates
    
    private:
    std::vector<double>         elements;
    static const TypeSpec       typeSpec;
};

#endif

