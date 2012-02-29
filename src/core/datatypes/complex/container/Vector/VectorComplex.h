/**
 * @file
 * This file contains the declaration of VectorComplex, a complex type
 * used to hold complex vectors.
 *
 * @brief Declaration of VectorComplex
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

#ifndef VectorComplex_H
#define VectorComplex_H

#include "Complex.h"
#include "AbstractVector.h"
#include <iostream>
#include <vector>


class VectorComplex : public AbstractVector {

    public:
                                        VectorComplex(void);                                            //!< Default constructor (empty vector)
                                        VectorComplex(const std::string& elemType);                     //!< Default constructor for derived classes
                                        VectorComplex(double x);                                        //!< Construct vector with one double x
                                        VectorComplex(size_t n, double x);                              //!< Construct vector with n doubles x
                                        VectorComplex(const std::vector<double>& x);                    //!< Constructor from double vector
                                        VectorComplex(const std::vector<std::complex<double> >& x);     //!< Constructor from double vector
                                        VectorComplex(const VectorComplex& x);                          //!< Constructor from VectorComplex
                                        VectorComplex(size_t n, std::complex<double> x);                //!< Construct vector with n complex x

    // Overloaded operators
    std::complex<double>&               operator[](size_t i);                                           //!< Index op allowing change
    const std::complex<double>&         operator[](size_t i) const;                                     //!< Const index op
    bool                                operator==(const VectorComplex& x) const;                       //!< Equals comparison
    bool                                operator!=(const VectorComplex& x) const;                       //!< Not equals comparison
        
    // Basic utility functions
    VectorComplex*                      clone(void) const;                                              //!< Clone object
    static const std::string&           getClassName(void);                                             //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                        //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                              //!< Print value (for user)
    
    // Member object functions
    virtual const MethodTable&          getMethods(void) const;                                         //!< Get methods

    // Vector functions, including STL-like functions
    void                                clear(void);                                                    //!< Clear
    const RbObject&                     getElement(size_t index) const;                                 //!< Get element
    RbObject&                           getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    std::vector<std::complex<double> >  getValue(void) const;                                           //!< Get value
    void                                pop_back(void);                                                 //!< Drop element at back
    void                                pop_front(void);                                                //!< Drop element from front
    void                                push_back(RbObject* x);                                         //!< Append element to end
    void                                push_back(std::complex<double> x);                              //!< Append element to end
    void                                push_front(RbObject* x);                                        //!< Add element in front
    void                                push_front(std::complex<double> x);                             //!< Add element in front
    void                                resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                                setElement(const size_t index, RbLanguageObject* elem);         //!< Set element with type conversion
    void                                setValue(const VectorComplex& x);                               //!< Set the value
    void                                setValue(const std::vector<std::complex<double> >& x);          //!< Set the value
    void                                sort(void);                                                     //!< sort the AbstractVector
    size_t                              size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                                unique(void);                                                   //!< removes consecutive duplicates
        
    
    
    private:
    
    std::vector<std::complex<double> >  elements;

};

#endif

