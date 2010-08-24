/**
 * @file
 * This file contains the declaration of VectorBoolean, a complex type
 * used to hold boolean vectors.
 *
 * @brief Declaration of VectorBoolean
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

#ifndef VectorBoolean_H
#define VectorBoolean_H

#include "Vector.h"

#include <iostream>
#include <vector>

class VectorBoolean : public Vector {

    public:
        // Constructors and destructor
                                    VectorBoolean(void);                                          //!< Default constructor (empty vector)
                                    VectorBoolean(bool x);                                        //!< Construct vector with one int x
                                    VectorBoolean(size_t n, bool x);                              //!< Construct vector with n ints x
                                    VectorBoolean(const std::vector<bool>& x);                    //!< Constructor from int vector
                                    VectorBoolean(const std::vector<int>& x);                     //!< Constructor from unsigned int vector
                                    VectorBoolean(const ContainerIterator& x);                    //!< Constructor from container iterator

        // Operators
        //bool&                     operator[](size_t i) { return value[i]; }                     //!< Good luck overloading these operators
        //const bool&               operator[](size_t i) const { return value[i]; }               //!< "    "    "           "     "
        bool                        operator==(const VectorBoolean& x) const;                     //!< Equals comparison
        bool                        operator!=(const VectorBoolean& x) const;                     //!< Not equals comparison
        
        // Basic utility functions, pure virtual in RbComplex and implemented here
        virtual VectorBoolean*      clone(void) const;                                            //!< Clone object
        virtual bool                equals(const RbObject* obj) const;                            //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                         //!< Get class
        virtual void                printValue(std::ostream& o) const;                            //!< Print value (for user)
        virtual std::string         toString(void) const;                                         //!< Complete info about object

        // Overloaded operators and built-in functions
        void                        clear(void) { value.clear(); }                                //!< Clear
        const std::vector<bool>&    getValue(void) const { return value; }                        //!< Get value
        void                        pop_back(void) { value.pop_back(); }                          //!< Drop element
        void                        push_back(int x) { value.push_back(x); }                      //!< Append element to end
        void                        push_front(int x) { value.insert(value.begin(), x); }         //!< Add element in front
        void                        resize(size_t n) { value.resize(n); }                         //!< Resize
        virtual void                setValue(const VectorBoolean& x);                             //!< Set the value
        virtual void                setValue(const std::vector<bool>& x) { value = x; }           //!< Set the value
        size_t                      size(void) const { return value.size(); }                     //!< Get size

        // Element access functions for parser
        virtual const std::string&  getElementType(void) const;                                   //!< Get element type
        const RbObject*             getElement(const VectorInteger& index) const;                 //!< Get element (read-only)
        const VectorInteger&        getLength(void) const;                                        //!< Get length in each dim
        void                        resize(const VectorInteger& len);                             //!< Resize
        void                        setElement(const VectorInteger& index, RbObject* val);        //!< Set element
        void                        setLength(const VectorInteger& len);                          //!< Set length in each dim

	protected:
        RbObject*                   convertTo(const std::string& type) const;                     //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;               //!< Is convertible to type and dim?

    private:
        std::vector<bool>           value;                                                        //!< Vector of values
};

#endif


