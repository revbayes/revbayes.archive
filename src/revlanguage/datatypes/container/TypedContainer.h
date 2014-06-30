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

#ifndef TypedContainer_H
#define TypedContainer_H

#include "Container.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    template <typename rbType>
    class TypedContainer : public Container {
        
    public:
        // the value type definition
        typedef rbType                              valueType;
        typedef typename rbType::iterator           iterator;
        typedef typename rbType::const_iterator     const_iterator;

        virtual                                    ~TypedContainer(void);                                               //!< Destructor        

        // Basic utility functions you have to override
        virtual Container*                          clone(void) const = 0;                                              //!< Clone object

        // Basic utility functions you should not have to override
        const rbType&                               getValue(void) const;
        RevBayesCore::TypedDagNode<rbType>*         getDagNode(void) const;
        bool                                        isConstant(void) const;                                             //!< Is this variable and the internally stored deterministic node constant?
        void                                        makeConstantValue();                                                //!< Convert the stored variable to a constant variable (if applicable)
        void                                        printValue(std::ostream& o) const;                                  //!< Print value for user
        void                                        setName(const std::string &n);                                      //!< Set the name of the variable (if applicable)
        void                                        replaceVariable(RevObject *newVar);                          //!< Replace the internal DAG node

        // function you might want to overwrite
        virtual RevObject*                   convertTo(const TypeSpec& type) const;                          //!< Convert to type
        virtual RevObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        virtual const MethodTable&                  getMethods(void) const;                                                     //!< Get member methods (const)

        // Container functions you should not have to override
        iterator                                    begin(void);                                                    //!< Iterator to the beginning of the Vector
        const_iterator                              begin(void) const;                                              //!< Const-iterator to the beginning of the Vector
        void                                        clear(void);                                                    //!< Clear
        iterator                                    end(void);                                                      //!< Iterator to the end of the Vector
        const_iterator                              end(void) const;                                                //!< Const-iterator to the end of the Vector
        int                                         findIndex(const RevObject& x) const;                     //!< Find the index if the element being equal to that one
        void                                        pop_back(void);                                                 //!< Drop element at back
        void                                        pop_front(void);                                                //!< Drop element from front
        size_t                                      size(void) const;                                               //!< get the number of elements in the AbstractVector
  
        // Container functions you have to overwrite
        virtual RevObject*                   getElement(size_t index) = 0;                                   //!< Get element (non-const to return non-const element)
        virtual void                                sort(void) = 0;                                                 //!< sort the AbstractVector
        virtual void                                unique(void)= 0 ;                                               //!< removes consecutive duplicates

    protected:
        TypedContainer(const TypeSpec &elemType);                                                                       //!< Set type spec of container from type of elements
        TypedContainer(const TypeSpec &elemType, const rbType& v);                                                      //!< Set type spec of container from type of elements
        TypedContainer(const TypeSpec &elemType, RevBayesCore::TypedDagNode<rbType> *c);                                //!< Set type spec of container from type of elements
        TypedContainer(const TypedContainer<rbType> &c);                                                                //!< Set type spec of container from type of elements
        
        TypedContainer&                             operator=(const TypedContainer& x);                                 //!< Assignment operator

        RevBayesCore::TypedDagNode<rbType>*         value;
        
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
RevLanguage::TypedContainer<rbType>::TypedContainer(const TypeSpec &elemType) : Container(elemType), 
    value( new RevBayesCore::ConstantNode<rbType>( "", new rbType() ) ) 
{
    value->incrementReferenceCount();
}



template <typename rbType>
RevLanguage::TypedContainer<rbType>::TypedContainer(const TypeSpec &elemType, const rbType &v) : Container(elemType), 
    value( new RevBayesCore::ConstantNode<rbType>( "", new rbType(v) ) ) 
{
    value->incrementReferenceCount ();
    
}



template <typename rbType>
RevLanguage::TypedContainer<rbType>::TypedContainer(const TypeSpec &elemType, RevBayesCore::TypedDagNode<rbType> *v) : Container(elemType), 
    value( v ) 
{
    value->incrementReferenceCount();
    
}



template <typename rbType>
RevLanguage::TypedContainer<rbType>::TypedContainer(const TypedContainer &v) : Container( v ), 
    value( NULL ) 
{
    
    if ( v.value != NULL ) 
    {
        value = v.value->clone();
        value->incrementReferenceCount();
    }
    
}



template <typename rbType>
RevLanguage::TypedContainer<rbType>::~TypedContainer() {
    
    if ( value->decrementReferenceCount() == 0 ) 
    {
        delete value;
    }

}


template <typename rbType>
RevLanguage::TypedContainer<rbType>& RevLanguage::TypedContainer<rbType>::operator=(const TypedContainer &v) {
    
    if ( this != &v ) 
    {
        // free the memory
        if ( value->decrementReferenceCount() == 0 ) 
        {
            delete value;
        }
        value = NULL;
        
        // create own copy
        if ( v.value != NULL ) 
        {
            value = v.value->clone();
            value->incrementReferenceCount();
        }
    }
    
    return *this;
}


/** Get iterator to the beginning of the Container. */
template <typename rlType>
typename rlType::iterator RevLanguage::TypedContainer<rlType>::begin( void ) {
    
    return value->getValue().begin();
}


/** Get const-iterator to the beginning of the Container. */
template <typename rlType>
typename rlType::const_iterator RevLanguage::TypedContainer<rlType>::begin( void ) const {
    
    return value->getValue().begin();
}


/** Clear the container. */
template <typename rlType>
void RevLanguage::TypedContainer<rlType>::clear( void ) {
    
    return value->getValue().clear();
}


/** Clear the container. */
template <typename rlType>
RevLanguage::RevObject* RevLanguage::TypedContainer<rlType>::convertTo(const RevLanguage::TypeSpec &type ) const {
    
    return Container::convertTo( type );
}


/** Get iterator to the end of the Vector. */
template <typename rlType>
typename rlType::iterator RevLanguage::TypedContainer<rlType>::end( void ) {
    
    return value->getValue().end();
}


/** Get const-iterator to the end of the Vector. */
template <typename rlType>
typename rlType::const_iterator RevLanguage::TypedContainer<rlType>::end( void ) const {
    
    return value->getValue().end();
}


/* Map calls to member methods */
template <typename rbType>
RevLanguage::RevObject* RevLanguage::TypedContainer<rbType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "clamp") 
    {
        
        // check whether the variable is actually a stochastic node
        if ( !value->isStochastic() ) {
            throw RbException("Only stochastic variables can be clamped.");
        }
        // convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
        
        // get the observation
        const rbType &observation = static_cast<const TypedContainer<rbType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // clamp
        stochNode->clamp( new rbType(observation) );
        
        return NULL;
    } 
    else if (name == "setValue") 
    {
        
        // check whether the variable is actually a stochastic node
        if ( !value->isStochastic() ) 
        {
            throw RbException("You can only set the value of a stochastic variable.");
        }
        // convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
        
        // get the observation
        const rbType &observation = static_cast<const TypedContainer<rbType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // clamp
        stochNode->setValue( new rbType(observation) );
        
        return NULL;
    } 
    else if (name == "redraw") 
    {
        
        // check whether the variable is actually a stochastic node
        if ( !value->isStochastic() ) 
        {
            throw RbException("You can only set the value for stochastic variables.");
        }
        // convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
        
        // redraw the value
        stochNode->redraw();
        
        return NULL;
    } 
    else if ( name == "sort" ) 
    {
        sort();
        
        return NULL;
    }
    else if ( name == "unique" ) 
    {
        unique();
        
        return NULL;
    }
    
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
int RevLanguage::TypedContainer<rbType>::findIndex(const RevObject& x) const {
    
    // get the iterator to the first element
    typename rbType::const_iterator i;
    
    // initialize the index
    int index = 0;
    for ( i = value->getValue().begin(); i != value->getValue().end(); i++, index++) 
    {
        // test if the object matches
        // note that we rely on the implemented operator==
//        if ( *i == x.getValue() ) {
//            return index;
//        }
        throw RbException("Missing implementation of findIndex() in Container.\n");
    }
    
    return -1;
}


/* Get method specifications */
template <typename rbType>
const RevLanguage::MethodTable&  RevLanguage::TypedContainer<rbType>::getMethods(void) const {
    
    static MethodTable methods      = MethodTable();
    static bool        methodsSet   = false;
    
    if ( methodsSet == false ) 
    {
        
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("clamp", new MemberFunction( RlUtils::Void, clampArgRules) );
        
        ArgumentRules* setValueArgRules = new ArgumentRules();
        setValueArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("setValue", new MemberFunction( RlUtils::Void, setValueArgRules) );
        
        ArgumentRules* redrawArgRules = new ArgumentRules();
        methods.addFunction("redraw", new MemberFunction( RlUtils::Void, redrawArgRules) );
        
        // add method for call "x.sort()" as a function
        ArgumentRules* sortArgRules = new ArgumentRules();
        methods.addFunction("sort",  new MemberFunction( RlUtils::Void, sortArgRules) );
        
        // add method for call "x.unique()" as a function
        ArgumentRules* uniqueArgRules = new ArgumentRules();
        methods.addFunction("unique",  new MemberFunction( RlUtils::Void, uniqueArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &Container::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}



template <typename rbType>
const rbType& RevLanguage::TypedContainer<rbType>::getValue( void ) const {
    
    return value->getValue();
}



template <typename rbType>
RevBayesCore::TypedDagNode<rbType>* RevLanguage::TypedContainer<rbType>::getDagNode( void ) const {
    
    return value;
}


template <typename rbType>
bool RevLanguage::TypedContainer<rbType>::isConstant( void ) const {
    return value->isConstant();
}


template <typename rbType>
void RevLanguage::TypedContainer<rbType>::makeConstantValue( void ) {
    
    if ( value == NULL ) 
    {
        throw RbException("Cannot convert a variable without value to a constant value.");
    } else 
    {
        // @todo: we might check if this variable is already constant. Now we construct a new value anyways.
        RevBayesCore::ConstantNode<rbType>* newVal = new RevBayesCore::ConstantNode<rbType>(value->getName(), new rbType(value->getValue()) );
        value->replace(newVal);
        
        if ( value->decrementReferenceCount() == 0) {
            delete value;
        }
        
        value = newVal;
        value->incrementReferenceCount();
    }
}


template <typename rbType>
void RevLanguage::TypedContainer<rbType>::setName(std::string const &n) {
    
    if ( value == NULL ) 
    {
        throw RbException("Null-pointer-exception: Cannot set name of value.");
    } 
    else 
    {
        value->setName( n );
    }
    
}




/** Print value for user */
template <typename rbType>
void RevLanguage::TypedContainer<rbType>::printValue(std::ostream &o) const {
    
    o << "[ ";
    value->printValue(o,", ");
    o << " ]";
}


template <typename rbType>
void RevLanguage::TypedContainer<rbType>::replaceVariable(RevObject *newVar) {
    
    RevBayesCore::DagNode* newParent = newVar->getDagNode();
    
    while ( value->getNumberOfChildren() > 0 ) 
    {
        value->getFirstChild()->swapParent(value, newParent);
    }
    
}


/** Get the size of the vector */
template <typename rbType>
size_t RevLanguage::TypedContainer<rbType>::size( void ) const {
    
    return value->getValue().size();
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

