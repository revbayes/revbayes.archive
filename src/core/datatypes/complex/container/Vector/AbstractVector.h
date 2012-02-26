/**
 * @file
 * This file contains the declaration of AbstractVector, a container type
 * that acts as a base class for all constant AbstractVectors.
 *
 * @brief Declaration of AbstractVector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id$
 */

#ifndef AbstractVector_H
#define AbstractVector_H

#include "Container.h"

#include <iostream>
#include <vector>


class AbstractVector : public Container {
    
    public:
        virtual                        ~AbstractVector(void);                                               //!< Virtual destructor 
        
        // Basic utility functions you have to override
        virtual RbObject*               convertTo(const TypeSpec &type) const;
        virtual AbstractVector*         clone(void) const = 0;                                              //!< Clone object
        static const std::string&       getClassName(void);                                                 //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                             //!< Get class type spec
        virtual void                    printValue(std::ostream& o) const;                                  //!< Print value for user
 
        // Member method inits
        virtual const MethodTable&      getMethods(void) const;                                             //!< Get methods

        // Container functions you have to override
        virtual void                    pop_back(void) = 0;                                                 //!< Drop element at back
        virtual void                    pop_front(void) = 0;                                                //!< Drop element from front
        virtual void                    push_back(RbObject* x) = 0;                                         //!< Append element to end
        virtual void                    push_front(RbObject* x) = 0;                                        //!< Add element in front
        virtual void                    resize(size_t n) = 0;                                               //!< Resize to new AbstractVector of length n
        virtual size_t                  size(void) const = 0;                                               //!< get the number of elements in the AbstractVector
        
        virtual void                    clear(void) = 0;                                                    //!< Clear
        virtual const RbObject&         getElement(size_t index) const = 0;                                 //!< Get element
        virtual RbObject&               getElement(size_t index) = 0;                                       //!< Get element (non-const to return non-const element)
        virtual void                    setElement(const size_t index, RbLanguageObject* elem) = 0;         //!< Set element with type conversion
        virtual void                    sort(void) = 0;                                                     //!< sort the AbstractVector
        virtual void                    unique(void) = 0;                                                   //!< removes consecutive duplicates
        
        // AbstractVector functions you do not have to override
        void                            setElement(const size_t index, RbObject* elem);                     //!< Set element with type conversion
        
    protected:
        AbstractVector(const TypeSpec &elemType);                                                           //!< Set type spec of container from type of elements
        AbstractVector(const TypeSpec &elemType, const MemberRules& memberRules);                           //!< Set type spec of container from type of elements
        AbstractVector(const AbstractVector &v);                                                            //!< Copy Constructor
        
        AbstractVector&                 operator=(const AbstractVector& x);                                 //!< Assignment operator

        // the member functions
        virtual const RbLanguageObject&     executeOperationSimple(const std::string& name, const std::vector<Argument>& args);//!< Execute method
    
};

#endif

