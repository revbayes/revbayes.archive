/**
 * @file
 * This file contains the declaration of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Declaration of VectorReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef VectorReal_H
#define VectorReal_H

#include "Vector.h"

#include <iostream>
#include <vector>

class VectorReal : public Vector {

    public:
        // Constructors and destructor
                                    VectorReal(void);                                             //!< Default constructor (empty vector)
                                    VectorReal(double x);                                         //!< Construct vector with one double x
                                    VectorReal(size_t n, double x);                               //!< Construct vector with n doubles x
                                    VectorReal(const std::vector<double>& x);                     //!< Constructor from double vector
                                    VectorReal(const ContainerIterator& x);                       //!< Constructor from container iterator

        // Operators
        double&                     operator[](size_t i) { return value[i]; }                     //!< Index op allowing change
        const double&               operator[](size_t i) const { return value[i]; }               //!< Const index op
        bool                        operator==(const VectorReal& x) const;                        //!< Equals comparison
        bool                        operator!=(const VectorReal& x) const;                        //!< Not equals comparison
        
        // Basic utility functions, pure virtual in RbComplex and implemented here
        virtual VectorReal*         clone(void) const;                                            //!< Clone object
        virtual bool                equals(const RbObject* obj) const;                            //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                         //!< Get class
        virtual void                printValue(std::ostream& o) const;                            //!< Print value (for user)
        virtual std::string         toString(void) const;                                         //!< Complete info about object

        // Element access functions for parser
        virtual const std::string&  getElementType(void) const;                                   //!< Get element type
        const RbObject*             getElement(const VectorInteger& index) const;                 //!< Get element (read-only)
        const VectorInteger&        getLength(void) const;                                        //!< Get length in each dim
        void                        resize(const VectorInteger& len);                             //!< Resize
        virtual void                setElement(const VectorInteger& index, RbObject* val);        //!< Set element
        void                        setLength(const VectorInteger& len);                          //!< Set length in each dim

        // Regular functions, including STL-like functions
        void                        clear(void) { value.clear(); }                                //!< Clear
        const std::vector<double>&  getValue(void) const { return value; }                        //!< Get value
        void                        pop_back(void) { value.pop_back(); }                          //!< Drop element
        virtual void                push_back(double x) { value.push_back(x); }                   //!< Append element to end
        virtual void                push_front(double x) { value.insert(value.begin(), x); }      //!< Add element in front
        void                        resize(size_t n) { value.resize(n); }                         //!< Resize
        virtual void                setValue(const VectorReal& x);                                //!< Set the value
        virtual void                setValue(const std::vector<double>& x);                       //!< Set the value
        size_t                      size(void) const { return value.size(); }                     //!< Get size

    protected:
        // Member variable protected so it can be accessed directly by derived members
        std::vector<double>         value;                                                     //!< Vector of values
};

#endif


