/**
 * @file
 * This file contains the declaration of the Rev language ModelObject, which is
 * the Rev abstract base class for all language objects wrapping core datatypes
 * that can be stored inside DAG nodes and hence used in model graphs.
 *
 * @brief Declaration of ModelObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-08-06
 *
 * $Id: RevObject.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef ModelObject_H
#define ModelObject_H

#include "MethodTable.h"
#include "RevObject.h"
#include "TypedDagNode.h"
#include "UserFunctionArgs.h"
#include "UserFunctionCall.h"

namespace RevLanguage {
    
    template <typename rbType>
    class ModelObject : public RevObject {
    
    public:
        virtual                                ~ModelObject(void);                                                          //!< Virtual destructor

        ModelObject&                            operator=(const ModelObject& x);                                            //!< Assignment operator
        
        // the value type definition
        typedef rbType valueType;
       
        // Basic utility functions you have to override
        virtual ModelObject*                    clone(void) const = 0;                                                      //!< Clone object
    
        // function you might want to overwrite
        virtual RevObject*                      executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        virtual RevObject*                      getMember(const std::string& name) const;                                   //!< Get member variable
        virtual const MethodTable&              getMethods(void) const;                                                     //!< Get member methods (const)
        virtual bool                            hasMember(const std::string& name) const;                                   //!< Has this object a member with name

        // Basic utility functions you should not have to override
        RevBayesCore::TypedDagNode<rbType>*     getDagNode(void) const;                                                     //!< Get the internal DAG node
        bool                                    hasDagNode(void) const;                                                     //!< Return true because we have an internal DAG node
        bool                                    isConstant(void) const;                                                     //!< Is this variable and the internally stored deterministic node constant?
        void                                    makeConstantValue();                                                        //!< Convert the stored variable to a constant variable (if applicable)
        void                                    makeDeterministicValue(UserFunctionCall* call, UserFunctionArgs* args);     //!< Make deterministic clone with a userdefined Rev function
        ModelObject<rbType>*                    makeDagReference(void);                                                     //!< Make reference to object
        virtual void                            printStructure(std::ostream& o) const;                                      //!< Print structure of language object for user
        void                                    printValue(std::ostream& o) const;                                          //!< Print value for user
        void                                    setName(const std::string &n);                                              //!< Set the name of the variable (if applicable)
        void                                    replaceVariable(RevObject *newVar);                                         //!< Replace the internal DAG node (and prepare to replace me...)

        ModelObject<rbType>*                    shallowCopy(void);      // Testing...
        
        // getters and setters
        virtual const rbType&                   getValue(void) const;                                                       //!< Get the value
        void                                    setValue(rbType *x);                                                        //!< Set new constant value
        void                                    setDagNode(RevBayesCore::DagNode *newNode);                                 //!< Set or replace the internal dag node (and keep me)
        
    protected:
        ModelObject(void);
        ModelObject(rbType *v);
        ModelObject(RevBayesCore::TypedDagNode<rbType> *v);
        ModelObject(const ModelObject &v);
        
        RevBayesCore::TypedDagNode<rbType>*     value;
        mutable MethodTable                     methods;

    private:
        
        void                                    initMethods(void);
    };
    
}


#include "AbstractCharacterData.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Cloner.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "MemberFunction.h"
#include "RlDeterministicNode.h"
#include "RlUtils.h"
#include "StochasticNode.h"
#include "TypedReferenceFunction.h"
#include "TypedUserFunction.h"

#include <cassert>

template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject() : RevObject(), 
    value( NULL ), 
    methods() 
{
    
//    initMethods();
    
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject(rbType *v) : RevObject(), 
    value( new RevBayesCore::ConstantNode<rbType>("",v) ), 
    methods() 
{
    // increment the reference count to the value
    value->incrementReferenceCount();
    
//    initMethods();
    
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject(RevBayesCore::TypedDagNode<rbType> *v) : RevObject(), 
    value( v ), 
    methods() 
{
    
    // increment the reference count to the value
    value->incrementReferenceCount();
    
    
//    initMethods();
    
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject(const ModelObject &v) : RevObject(), 
    value( NULL ), 
    methods() 
{
    
    if ( v.value != NULL ) 
    {
        
        value = v.value->clone();
        
        // increment the reference count to the value
        value->incrementReferenceCount();
    }
    
//    initMethods();
    
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::~ModelObject() 
{
    
    // free the old value
    if ( value != NULL ) 
    {
        if ( value->decrementReferenceCount() == 0 ) 
        {
            delete value;
        }
    }
}


template <typename rbType>
RevLanguage::ModelObject<rbType>& RevLanguage::ModelObject<rbType>::operator=(const ModelObject &v) {
    
    if ( this != &v ) 
    {
        // free the memory
        // free the old value
        if ( value != NULL ) 
        {
            if ( value->decrementReferenceCount() == 0 ) 
            {
                delete value;
            }
            
            value = NULL;
        }
        
        // create own copy
        if ( v.value != NULL ) 
        {
            value = v.value->clone();
            
            // increment the reference count to the value
            value->incrementReferenceCount();
        }
    }
    
    return *this;
}


/* Map calls to member methods */
template <typename rbType>
RevLanguage::RevObject* RevLanguage::ModelObject<rbType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "clamp") 
    {
        
        // check whether the variable is actually a stochastic node
        if ( !value->isStochastic() ) 
        {
            throw RbException("Only stochastic variables can be clamped.");
        }
        // convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
        
        // get the observation
        const rbType &observation = static_cast<const ModelObject<rbType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // clamp
        stochNode->clamp( RevBayesCore::Cloner<rbType, IsDerivedFrom<rbType, RevBayesCore::Cloneable>::Is >::createClone( observation ) );
        
        return NULL;
    } 
    else if (name == "setValue") 
    {
        
        // check whether the variable is actually a stochastic node
        if ( !value->isStochastic() ) 
        {
            throw RbException("You can only set the value for stochastic variables.");
        }
        // convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
        
        // get the observation
        const rbType &observation = static_cast<const ModelObject<rbType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // set value
        stochNode->setValue( RevBayesCore::Cloner<rbType, IsDerivedFrom<rbType, RevBayesCore::Cloneable>::Is >::createClone( observation ) );
        
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
    
    return RevObject::executeMethod( name, args );
}


/* Find member variables */
template <typename rbType>
RevLanguage::RevObject* RevLanguage::ModelObject<rbType>::getMember(std::string const &name) const
{
    
    // check whether the variable is actually a stochastic node
    if ( value->isStochastic() ) 
    {
        if ( name == "prob" || name == "probability" ) 
        {
            // convert the node
            RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
            double lnProb = stochNode->getLnProbability();
            RevObject *p = RlUtils::RlTypeConverter::toReal( exp(lnProb) );
            
            return p;
        } 
        else if ( name == "lnProb" || name == "lnProbability" ) 
        {
            // convert the node
            RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( value );
            double lnProb = stochNode->getLnProbability();
            RevObject *p = RlUtils::RlTypeConverter::toReal( lnProb );
            
            return p;
            
        }
    }

    
    return RevObject::getMember( name );
}


/* Get method specifications */
template <typename rbType>
const RevLanguage::MethodTable&  RevLanguage::ModelObject<rbType>::getMethods(void) const {
    
//    static MethodTable methods      = MethodTable();
    // Sebastian: Static variables don't work because derived classes, e.g. PosReal from Real
    // require different types but only one static variable will be set for both classes!!!
//    static bool        methodsSet   = false;
    
//    if ( methodsSet == false ) 
//    {
    
    methods = MethodTable();
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("clamp", new MemberFunction( RlUtils::Void, clampArgRules) );
        
        ArgumentRules* setValueArgRules = new ArgumentRules();
        setValueArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("setValue", new MemberFunction( RlUtils::Void, setValueArgRules) );
        
        ArgumentRules* redrawArgRules = new ArgumentRules();
        methods.addFunction("redraw", new MemberFunction( RlUtils::Void, redrawArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
//        methodsSet = true;
//    }
    
    return methods;
}



template <typename rbType>
const rbType& RevLanguage::ModelObject<rbType>::getValue( void ) const {
    
    return value->getValue();
}



template <typename rbType>
RevBayesCore::TypedDagNode<rbType>* RevLanguage::ModelObject<rbType>::getDagNode( void ) const {
    
    return value;
}


/** Make sure users understand that we have an internal DAG node */
template <typename rbType>
bool RevLanguage::ModelObject<rbType>::hasDagNode( void ) const {
    
    return true;
}


/**
 * Has this object a member with the given name?
 *
 */
template<typename rbType>
bool RevLanguage::ModelObject<rbType>::hasMember(std::string const &name) const 
{
    // first the general members ...
    // if ( name == )
    
    // members that all stochastic variables have
    if ( value->isStochastic() )  
    {
        if ( name == "prob" || name == "probability" ) 
        {
            return true;
        } 
        else if ( name == "lnProb" || name == "lnProbability" ) 
        {
            return true;
        }
    } 
//    else 
//    {
//        <#statements-if-false#>
//    }
    
    return false;
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::initMethods( void )
{
    
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
    methods.addFunction("clamp", new MemberFunction( RlUtils::Void, clampArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
    methods.addFunction("setValue", new MemberFunction( RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    methods.addFunction("redraw", new MemberFunction( RlUtils::Void, redrawArgRules) );
    
    // necessary call for proper inheritance
    methods.setParentTable( &RevObject::getMethods() );

}


template <typename rbType>
bool RevLanguage::ModelObject<rbType>::isConstant( void ) const {
    
    return value->isConstant();
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::makeConstantValue( void ) {
    
    if ( value == NULL )
    {
        throw RbException("Cannot convert a variable without value to a constant value.");
    }
    else
    {
        // @todo: we might check if this variable is already constant. Now we construct a new value anyways.
        RevBayesCore::ConstantNode<rbType>* newVal = new RevBayesCore::ConstantNode<rbType>(value->getName(), RevBayesCore::Cloner<rbType, IsDerivedFrom<rbType, RevBayesCore::Cloneable>::Is >::createClone( value->getValue() ) );
        value->replace(newVal);
        
        // delete the value if there are no other references to it.
        if ( value->decrementReferenceCount() == 0 )
        {
            delete value;
        }
        
        value = newVal;
        
        // increment the reference counter
        value->incrementReferenceCount();
    }
    
}


template <typename rbType>
RevLanguage::ModelObject<rbType>* RevLanguage::ModelObject<rbType>::makeDagReference(void) {
    
    RevBayesLanguage::TypedReferenceFunction< rbType >* f = new RevBayesLanguage::TypedReferenceFunction< rbType >(value);

    RevBayesCore::DeterministicNode< rbType >* newVal = new RevBayesCore::DeterministicNode< rbType >( "", f );
    
    RevLanguage::ModelObject<rbType>* newObj = this->clone();
    
    // replace value with deterministic reference and attempt memory management
    newObj->value->decrementReferenceCount();
    newObj->value = newVal;
    newObj->value->incrementReferenceCount();
    
    return newObj;
}


/** Convert a model object to a deterministic object, the value of which is determined by a userdefined Rev function */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::makeDeterministicValue( UserFunctionCall* call, UserFunctionArgs* args )
{
    TypedUserFunction< rbType >*  fxn      = new TypedUserFunction< rbType >( call );
    DeterministicNode< rbType >*  detNode  = new DeterministicNode< rbType >("", fxn, args );
    
    if ( value != NULL && value->decrementReferenceCount() == 0 )
        delete value;

    value = detNode;
    value->incrementReferenceCount();
}


/** Print structure info for user */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::printStructure(std::ostream &o) const {
    
    value->printStructureInfo(o);
}


/** Print value for user */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::printValue(std::ostream &o) const {
    
    value->printValue(o,"");
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::replaceVariable(RevObject *newVar) {
    
    RevBayesCore::DagNode* newParent = newVar->getDagNode();
    
    while ( value->getNumberOfChildren() > 0 ) 
    {
        value->getFirstChild()->swapParent(value, newParent);
    }
    
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::setName(std::string const &n) {
    
    if ( value == NULL ) 
    {
        throw RbException("Null-pointer-exception: Cannot set name of value.");
    } 
    else 
    {
        value->setName( n );
    }
    
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::setValue(rbType *x) {
    
    RevBayesCore::ConstantNode<rbType>* newVal;
    
    if ( value == NULL ) 
    {
        newVal = new RevBayesCore::ConstantNode<rbType>("",x);
    } 
    else 
    {
        newVal = new RevBayesCore::ConstantNode<rbType>(value->getName(),x);
        value->replace(newVal);

        if ( value->decrementReferenceCount() == 0 ) 
        {
            delete value;
        }
        
    }
    
    value = newVal;
    
    // increment the reference count to the value
    value->incrementReferenceCount();
    
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::setDagNode(RevBayesCore::DagNode* newNode) {
    
    assert( dynamic_cast< RevBayesCore::TypedDagNode<rbType>* >(newNode) != NULL );
    
    // Take care of the old value node
    if ( value != NULL )
    {
        newNode->setName( value->getName() );
        value->replace(newNode);
        
        if ( value->decrementReferenceCount() == 0 )
        {
            delete value;
        }
        
    }
    
    // Set the new value node
    value = static_cast< RevBayesCore::TypedDagNode<rbType>* >( newNode );
    
    // Increment the reference count to the new value node
    value->incrementReferenceCount();
    
}


template <typename rbType>
RevLanguage::ModelObject<rbType>* RevLanguage::ModelObject<rbType>::shallowCopy(void) {
    
}

#endif
