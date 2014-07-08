/**
 * @file
 * This file contains the declaration of TypedContainer, a container type
 * that acts as a specialized base class for all constant Containers. Additionally to the interface
 * container, this class provides the wrapper functionality for DAG node values.
 *
 * @brief Declaration of TypedContainer
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id: Container.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef TypedWorkspaceObjectContainer_H
#define TypedWorkspaceObjectContainer_H

#include "Container.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    
    template <typename rbType>
    class TypedWorkspaceObjectContainer : public Container {
        
    public:
        // the value type definition
        typedef rbType valueType;
        typedef typename rbType::iterator iterator;
        typedef typename rbType::const_iterator const_iterator;
        
        virtual                                    ~TypedWorkspaceObjectContainer(void);                                //!< Destructor        
        
        // Basic utility functions you have to override
        virtual TypedWorkspaceObjectContainer*      clone(void) const = 0;                                              //!< Clone object
        
        // Basic utility functions you should not have to override
        const rbType&                               getValue(void) const;
        bool                                        isConstant(void) const;                                             //!< Is this variable and the internally stored deterministic node constant?
        void                                        makeConstantValue();                                                //!< Convert the stored variable to a constant variable (if applicable)
        void                                        printValue(std::ostream& o) const;                                  //!< Print value for user
        void                                        setName(const std::string &n);                                      //!< Set the name of the variable (if applicable)
        void                                        replaceVariable(RevObject *newVar);                          //!< Replace the internal DAG node
        
        // function you might want to overwrite
        virtual RevObject*                          convertTo(const TypeSpec& type) const;                          //!< Convert to type
        virtual RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        virtual const MethodTable&                  getMethods(void) const;                                                     //!< Get member methods (const)
        
        // Container functions you should not have to override
        iterator                                    begin(void);                                                    //!< Iterator to the beginning of the Vector
        const_iterator                              begin(void) const;                                              //!< Const-iterator to the beginning of the Vector
        void                                        clear(void);                                                    //!< Clear
        iterator                                    end(void);                                                      //!< Iterator to the end of the Vector
        const_iterator                              end(void) const;                                                //!< Const-iterator to the end of the Vector
        int                                         findIndex(const RevObject& x) const;                     //!< Find the index if the element being equal to that one
        //        const std::vector<elementType>&             getValue(void) const;                                           //!< Get the stl Vector of elements
        void                                        pop_back(void);                                                 //!< Drop element at back
        void                                        pop_front(void);                                                //!< Drop element from front
        size_t                                      size(void) const;                                               //!< get the number of elements in the AbstractVector
        
        // Container functions you have to overwrite
        virtual RevPtr<Variable>                    getElement(size_t index) = 0;                                   //!< Get element (non-const to return non-const element)
        
    protected:
        TypedWorkspaceObjectContainer(const TypeSpec &elemType);                                                                       //!< Set type spec of container from type of elements
        TypedWorkspaceObjectContainer(const TypeSpec &elemType, const rbType& v);                                                      //!< Set type spec of container from type of elements
        TypedWorkspaceObjectContainer(const TypedWorkspaceObjectContainer<rbType> &c);                                                                //!< Set type spec of container from type of elements
        
        TypedWorkspaceObjectContainer&              operator=(const TypedWorkspaceObjectContainer& x);                                 //!< Assignment operator
        
        rbType                                      value;
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RlUtils.h"
#include "StochasticNode.h"

template <typename rbType>
RevLanguage::TypedWorkspaceObjectContainer<rbType>::TypedWorkspaceObjectContainer(const TypeSpec &elemType) : Container(elemType), value() {
    
}



template <typename rbType>
RevLanguage::TypedWorkspaceObjectContainer<rbType>::TypedWorkspaceObjectContainer(const TypeSpec &elemType, const rbType &v) : Container(elemType), value( v ) {
    
}



template <typename rbType>
RevLanguage::TypedWorkspaceObjectContainer<rbType>::TypedWorkspaceObjectContainer(const TypedWorkspaceObjectContainer &v) : Container( v ), value( v.value ) {
    
}



template <typename rbType>
RevLanguage::TypedWorkspaceObjectContainer<rbType>::~TypedWorkspaceObjectContainer() {
    
//    delete value;
}


template <typename rbType>
RevLanguage::TypedWorkspaceObjectContainer<rbType>& RevLanguage::TypedWorkspaceObjectContainer<rbType>::operator=(const TypedWorkspaceObjectContainer &v) {
    
    if ( this != &v ) {
        // delegate to base class
        Container::operator=( v );
        
        // just copy the value
        value = v.value;
    }
    
    return *this;
}


/** Get iterator to the beginning of the Container. */
template <typename rlType>
typename rlType::iterator RevLanguage::TypedWorkspaceObjectContainer<rlType>::begin( void ) {
    return value.begin();
}


/** Get const-iterator to the beginning of the Container. */
template <typename rlType>
typename rlType::const_iterator RevLanguage::TypedWorkspaceObjectContainer<rlType>::begin( void ) const {
    return value.begin();
}


/** Clear the container. */
template <typename rlType>
void RevLanguage::TypedWorkspaceObjectContainer<rlType>::clear( void ) {
    return value.clear();
}


/** Clear the container. */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::TypedWorkspaceObjectContainer<rlType>::convertTo(const RevLanguage::TypeSpec &type ) const {
    return Container::convertTo( type );
}


/** Get iterator to the end of the Vector. */
template <typename rlType>
typename rlType::iterator RevLanguage::TypedWorkspaceObjectContainer<rlType>::end( void ) {
    return value.end();
}


/** Get const-iterator to the end of the Vector. */
template <typename rlType>
typename rlType::const_iterator RevLanguage::TypedWorkspaceObjectContainer<rlType>::end( void ) const {
    return value.end();
}


/* Map calls to member methods */
template <typename rbType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::TypedWorkspaceObjectContainer<rbType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    return Container::executeMethod( name, args );
}



/**
 * Find the index of the given element.
 * We rely on overloaded operator== in the element classes to check for matches.
 * 
 * \param x the element we are looking for. 
 * \return The index or -1 if we didn't find it.
 */
template <typename rbType>
int RevLanguage::TypedWorkspaceObjectContainer<rbType>::findIndex(const RevObject& x) const {
    
    // get the iterator to the first element
    typename rbType::const_iterator i;
    
    // initialize the index
    int index = 0;
    for ( i = value->getValue().begin(); i != value->getValue().end(); i++, index++) {
        // test if the object matches
        // note that we rely on the implemented operator==
        //        if ( *i == x.getValue() ) {
        //            return index;
        //        }
        throw RbException("Missing implementation of findIndex() in Container.");
    }
    
    return -1;
}


/* Get method specifications */
template <typename rbType>
const RevLanguage::MethodTable&  RevLanguage::TypedWorkspaceObjectContainer<rbType>::getMethods(void) const {
    
    static MethodTable methods      = MethodTable();
    static bool        methodsSet   = false;
    
    if ( methodsSet == false ) {
        
        // necessary call for proper inheritance
        methods.setParentTable( &Container::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}



template <typename rbType>
const rbType& RevLanguage::TypedWorkspaceObjectContainer<rbType>::getValue( void ) const {
    
    return value;
}


template <typename rbType>
bool RevLanguage::TypedWorkspaceObjectContainer<rbType>::isConstant( void ) const {
    return true;
}


template <typename rbType>
void RevLanguage::TypedWorkspaceObjectContainer<rbType>::makeConstantValue( void ) {
    
    // nothing to do
}


template <typename rbType>
void RevLanguage::TypedWorkspaceObjectContainer<rbType>::setName(std::string const &n) {
    
}




/** Print value for user */
template <typename rbType>
void RevLanguage::TypedWorkspaceObjectContainer<rbType>::printValue(std::ostream &o) const {
    
    o << "Some container";
}


template <typename rbType>
void RevLanguage::TypedWorkspaceObjectContainer<rbType>::replaceVariable(RevObject *newVar) {
    
//    value = newVar;
    
}


/** Get the size of the vector */
template <typename rbType>
size_t RevLanguage::TypedWorkspaceObjectContainer<rbType>::size( void ) const {
    
    return value.size();
    
}



//template <typename rbType>
//void RevLanguage::TypedContainer<rbType>::setValue(const rbType &x) {
//    
//    RevBayesCore::ConstantNode<rbType>* newVal;
//    if ( value == NULL ) {
//        newVal = new RevBayesCore::ConstantNode<rbType>("",x);
//    } else {
//        newVal = new RevBayesCore::ConstantNode<rbType>(value->getName(),x);
//        value->replace(newVal);
//        delete value;
//    }
//    value = newVal;
//}

#endif

