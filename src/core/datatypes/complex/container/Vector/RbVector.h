/**
 * @file
 * This file contains the declaration of Vector, a container type
 * that acts as a base class for all constant Vectors.
 *
 * This class is a wrapper for the stl-class Vector and we provide additional RevBayes functionality
 * (e.g. getTypeSpec and getClassTypeSpec for argument checking)
 *
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
#include "MemberFunction.h"
#include "MethodTable.h"

#include <algorithm>
#include <iostream>
#include <vector>


class RbVector : public Container {
    
public:
    
    typedef std::vector<RbObject *>::iterator iterator;
    typedef std::vector<RbObject *>::const_iterator const_iterator;
    
    RbVector(const TypeSpec &elemType);                                                                             //!< Default constructor with type RbLanguageObject
    RbVector(const TypeSpec &elemType, size_t n);                                                                   //!< Default constructor with type RbLanguageObject
    RbVector(const TypeSpec &elemType, size_t n, RbObject* x);                                                      //!< Default constructor with type RbLanguageObject
    RbVector(const RbVector& v);                                                                                    //!< Copy Constructor
    
    virtual                                        ~RbVector(void);                                                 //!< Virtual destructor 
    
    // Basic utility functions 
    RbVector*                                       clone(void) const;                                              //!< Clone object
    RbObject*                                       convertTo(const TypeSpec& type) const;                          //!< Convert to type
    static const std::string&                       getClassName(void);                                             //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
    virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                    //!< Is this object convertible to the asked one?
    virtual void                                    printValue(std::ostream& o) const;                              //!< Print value for user
    
    RbObject&                                       operator[](size_t i);                                           //!< Index op allowing change
    const RbObject&                                 operator[](size_t i) const;                                     //!< Const index op
    RbVector&                                       operator=(const RbVector& x);                                   //!< Assignment operator
    RbVector&                                       operator+=(const RbObject& x);                                  //!< Concatenate
    RbVector&                                       operator+=(const RbVector& x);                                  //!< Concatenate
    const RbVector                                  operator+(const RbObject& x) const;                             //!< Concatenate
    const RbVector                                  operator+(const RbVector& x) const;                             //!< Concatenate
    bool                                            operator==(const RbVector& x) const;                            //!< Equality
    bool                                            operator!=(const RbVector& x) const;                            //!< Inequality
    
    // Overwritten RbObject functions
    virtual void*                                   getLeanValue(std::vector<size_t> &length) const;                //!< Transform the object into a basic element pointer for fast access.
    virtual size_t                                  memorySize() const;                                             //!< Get the size

    
    // Member object function
    const MemberRules&                              getMemberRules(void) const;                                     //!< Get member rules
    const MethodTable&                              getMethods(void) const;                                         //!< Get methods
    void                                            setMemberVariable(const std::string& name, const RbPtr<RbLanguageObject> & var);//!< Vector member variable
    
    // Vector functions
    std::vector<RbObject *>::iterator               begin(void);                                                    //!< Iterator to the beginning of the Vector
    std::vector<RbObject *>::const_iterator         begin(void) const;                                              //!< Const-iterator to the beginning of the Vector
    void                                            clear(void);                                                    //!< Clear
    std::vector<RbObject *>::iterator               end(void);                                                      //!< Iterator to the end of the Vector
    std::vector<RbObject *>::const_iterator         end(void) const;                                                //!< Const-iterator to the end of the Vector
    const RbObject&                                 find(const RbObject& x) const;                                  //!< Find the element being equal to that one
    int                                             findIndex(const RbObject& x) const;                             //!< Find the index if the element being equal to that one
    const RbObject&                                 getElement(size_t index) const;                                 //!< Get element
    RbObject&                                       getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    const std::vector<RbObject *>&                  getValue(void) const;                                           //!< Get the stl Vector of elements
    void                                            pop_back(void);                                                 //!< Drop element at back
    void                                            pop_front(void);                                                //!< Drop element from front
    void                                            push_back(RbObject *x);                                         //!< Append element to end
    void                                            push_front(RbObject *x);                                        //!< Add element in front
    void                                            resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                                            setElement(const size_t index, RbObject *elem);                 //!< Set element with type conversion
    void                                            sort(void);                                                     //!< sort the AbstractVector
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                                            unique(void);                                                   //!< removes consecutive duplicates

protected:
    
    virtual RbPtr<RbLanguageObject>                 executeOperationSimple(const std::string& name, const std::vector<Argument>& args);//!< Execute method
    
    // We store internally pointers to our objects. This is necessary because elements can be also of the derived type and we need to be able to make proper copies of the Vector and all its elements
    std::vector<RbObject *>                         elements;

private:
    
    MemberRules                                     memberRules;
    MethodTable                                     methods;
    TypeSpec                                        typeSpec;
    
    struct comparator {
        bool operator() (RbLanguageObject* A, RbLanguageObject* B) const { return ( *A < *B);}
    } myComparator;
};


#endif