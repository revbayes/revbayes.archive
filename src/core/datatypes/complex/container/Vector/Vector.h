/**
 * @file
 * This file contains the declaration of Vector, a container type
 * that acts as a base class for all constant vectors.
 *
 * @brief Declaration of Vector
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

#ifndef Vector_H
#define Vector_H

#include "Container.h"

#include <iostream>
#include <vector>


const std::string Vector_name = "Vector";

class Vector : public Container {

    public:
        virtual                        ~Vector(void);                                                       //!< Virtual destructor 

        // Basic utility functions you have to override
        virtual Vector*                 clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class
        virtual void                    printValue(std::ostream& o) const;                                  //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object
        
        Vector&                         operator=(const Vector& x);                                         //!< Assignment operator
    
        // Vector functions you do not have to override
        void                            pop_back(void);                                                     //!< Drop element at back
        void                            pop_front(void);                                                    //!< Drop element from front
        void                            resize(size_t n);                                                   //!< Resize to new vector of length n
        size_t                          size(void) const;                                                   //!< Get the number of elements in the vector
    
    
        // Container functions you have to override
        virtual void                    clear(void);                                                        //!< Clear
        virtual RbLanguageObject*       getElement(size_t index);                                           //!< Get element
        virtual void                    setElement(const size_t index, RbLanguageObject* elem);             //!< Set element with type conversion
        virtual void                            sort(void) = 0;                                                         //!< Sort the vector
        virtual void                            unique(void) = 0;                                                       //!< Remove duplicates and resize the vector
        
    
    
        protected:
                                        Vector(const std::string& elemType);                                //!< Set type spec of container from type of elements
                                        Vector(const Vector &v);                                            //!< Copy Constructor
//        virtual bool comparisonFunction (RbLanguageObject* i,RbLanguageObject* j);                                            //!< Utilitary function for sort

    
        std::vector<RbLanguageObject*>  elements;
};



#endif

