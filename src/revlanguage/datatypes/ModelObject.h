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

#include "AbstractModelObject.h"
#include "MethodTable.h"
#include "RevObject.h"
#include "TypedDagNode.h"
#include "UserFunction.h"

namespace RevLanguage {
    
    template <typename rbType>
    class ModelObject : public AbstractModelObject {
    
    public:
        virtual                                ~ModelObject(void);                                                          //!< Virtual destructor

        ModelObject&                            operator=(const ModelObject& x);                                            //!< Assignment operator
        
        // The value type definition
        typedef rbType valueType;
       
        // Basic utility functions you have to override
        virtual ModelObject*                    clone(void) const = 0;                                                      //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type (static)
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get Rev type spec (static)
        virtual const TypeSpec&                 getTypeSpec(void) const = 0;                                                //!< Get Rev type spec (instance)
    
        // Utility functions you might want to override
        virtual RevPtr<Variable>                executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        virtual void                            initializeMethods(void) const;                                              //!< Initialize member methods
        
        // Basic utility functions you should not have to override
        RevObject*                              cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*>& nodesMap ) const;  //!< Clone the model DAG connected to this node
        bool                                    isAssignable(void) const;                                                   //!< Is object or upstream members assignable?
        bool                                    isConstant(void) const;                                                     //!< Is this variable and the internally stored deterministic node constant?
        void                                    makeConstantValue(void);                                                    //!< Convert to constant object
        void                                    makeConversionValue(RevPtr<Variable> var);                                  //!< Convert to conversion object
        ModelObject<rbType>*                    makeIndirectReference(void);                                                //!< Make reference to object
        void                                    makeUserFunctionValue(UserFunction* fxn);                                   //!< Convert to user-defined Rev function object
        virtual void                            printStructure(std::ostream& o, bool verbose=false) const;                  //!< Print structure of language object for user
        void                                    printValue(std::ostream& o) const;                                          //!< Print value for user
        void                                    setDagNode(RevBayesCore::DagNode *newNode);                                 //!< Set or replace the internal dag node (and keep me)
        void                                    setName(const std::string &n);                                              //!< Set the name of the variable (if applicable)
        void                                    replaceVariable(RevObject *newVar);                                         //!< Replace the internal DAG node (and prepare to replace me...)
        
        // Getters and setters
        RevBayesCore::TypedDagNode<rbType>*     getDagNode(void) const;                                                     //!< Get the internal DAG node
        virtual const rbType&                   getValue(void) const;                                                       //!< Get the value
        void                                    setValue(rbType *x);                                                        //!< Set new constant value
        
    protected:
        ModelObject(void);
        ModelObject(rbType *v);
        ModelObject(RevBayesCore::TypedDagNode<rbType> *v);
        ModelObject(const ModelObject &v);
        
        RevBayesCore::TypedDagNode<rbType>*     dagNode;

    };
    
}


#include "AbstractCharacterData.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Cloner.h"
#include "ConstantNode.h"
#include "ConverterNode.h"
#include "IndirectReferenceNode.h"
#include "MemberProcedure.h"
#include "RlDeterministicNode.h"
#include "RlUtils.h"
#include "StochasticNode.h"
#include "UserFunctionNode.h"
#include "Variable.h"
#include "Workspace.h"

#include <cassert>
#include <cmath>

template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject() :
    AbstractModelObject(),
    dagNode( NULL )
{
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject(rbType *v) :
    AbstractModelObject(),
    dagNode( new RevBayesCore::ConstantNode<rbType>("",v) )
{
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject(RevBayesCore::TypedDagNode<rbType> *v) :
    AbstractModelObject(),
    dagNode( v )
{
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
}



template <typename rbType>
RevLanguage::ModelObject<rbType>::ModelObject(const ModelObject &v) :
    AbstractModelObject(),
    dagNode( NULL )
{
    if ( v.dagNode != NULL )
    {
        
        dagNode = v.dagNode->clone();
        
        // increment the reference count to the value
        dagNode->incrementReferenceCount();
    }
}


template <typename rbType>
RevLanguage::ModelObject<rbType>::~ModelObject() 
{
    
    // free the old value
    if ( dagNode != NULL )
    {
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
    }
}


template <typename rbType>
RevLanguage::ModelObject<rbType>& RevLanguage::ModelObject<rbType>::operator=(const ModelObject &v) {
    
    if ( this != &v ) 
    {
        // free the memory
        // free the old value
        if ( dagNode != NULL )
        {
            if ( dagNode->decrementReferenceCount() == 0 )
            {
                delete dagNode;
            }
            
            dagNode = NULL;
        }
        
        // create own copy
        if ( v.dagNode != NULL )
        {
            dagNode = v.dagNode->clone();
            
            // increment the reference count to the value
            dagNode->incrementReferenceCount();
        }
    }
    
    return *this;
}


/**
 * Clone the model DAG connected to this object. This function is used
 * by the DAG node cloneDAG function, for DAG node types belonging to the
 * RevLanguage layer and handling Rev objects.
 *
 * @todo This is a temporary hack that makes different Rev objects sharing
 *       the same internal DAG node keeping their value. Replace with code
 *       that actually clones the model DAG with the included Rev objects
 *       (and possibly also the included variables).
 */
template<typename rbType>
RevLanguage::RevObject* RevLanguage::ModelObject<rbType>::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*>& nodesMap ) const
{
    ModelObject<rbType>* theClone = clone();

    theClone->setDagNode( NULL );
    
    RevBayesCore::DagNode* theNodeClone = dagNode->cloneDAG( nodesMap );
    
    theClone->setDagNode( theNodeClone );
    
    return theClone;
}


/* Map calls to member methods */
template <typename rbType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::ModelObject<rbType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "clamp") 
    {
        // check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException("Only stochastic variables can be clamped.");
        }
        // convert the pointer to the DAG node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( dagNode );
        
        // get the observation
        const rbType &observation = static_cast<const ModelObject<rbType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // clamp
        stochNode->clamp( RevBayesCore::Cloner<rbType, IsDerivedFrom<rbType, RevBayesCore::Cloneable>::Is >::createClone( observation ) );
        
        return NULL;
    } 
    else if (name == "redraw")
    {
        // check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException("You can only set the value for stochastic variables.");
        }
        // convert the pointer to the DAG node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( dagNode );
        
        // redraw the value
        stochNode->redraw();
        
        return NULL;
    }
    else if (name == "setValue")
    {
        // check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException("You can only set the value for stochastic variables.");
        }
        // convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( dagNode );
        
        // get the observation
        const rbType &observation = static_cast<const ModelObject<rbType> &>( args[0].getVariable()->getRevObject() ).getValue();
        
        // set value
        stochNode->setValue( RevBayesCore::Cloner<rbType, IsDerivedFrom<rbType, RevBayesCore::Cloneable>::Is >::createClone( observation ) );
        
        return NULL;
    }
    else if (name == "unclamp")
    {
        // Check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException("Only stochastic variables can be clamped.");
        }
        // Convert the pointer to the DAG node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast<RevBayesCore::StochasticNode<rbType> *>( dagNode );
        
        // Unclamp
        stochNode->unclamp();
        
        return NULL;
    }
    
    if ( dagNode->isStochastic() )
    {
        
    }
    
    return RevObject::executeMethod( name, args );
}


template <typename rbType>
const std::string& RevLanguage::ModelObject<rbType>::getClassType(void) {
    
    static std::string revType = "ModelObject";
    
	return revType;
}


/** Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::ModelObject<rlType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &RevObject::getClassTypeSpec() );
    
	return revTypeSpec;
}


template <typename rbType>
const rbType& RevLanguage::ModelObject<rbType>::getValue( void ) const {
    
    if ( dagNode == NULL )
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    
    return dagNode->getValue();
}



template <typename rbType>
RevBayesCore::TypedDagNode<rbType>* RevLanguage::ModelObject<rbType>::getDagNode( void ) const {
    
    return dagNode;
}


/**
 * Is the object or any of its upstream members or elements
 * modifiable by the user through assignment? We simply ask
 * our DAG node.
 */
template <typename rbType>
bool RevLanguage::ModelObject<rbType>::isAssignable( void ) const
{
    if ( dagNode == NULL )
        return false;
    
    return dagNode->isAssignable();
}


template <typename rbType>
bool RevLanguage::ModelObject<rbType>::isConstant( void ) const {
    
    return dagNode->isConstant();
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::makeConstantValue( void ) {
    
    if ( dagNode == NULL )
    {
        throw RbException("Cannot convert a variable without value to a constant value.");
    }
    else
    {
        // @todo: we might check if this variable is already constant. Now we construct a new value anyways.
        RevBayesCore::ConstantNode<rbType>* newNode = new RevBayesCore::ConstantNode<rbType>(dagNode->getName(), RevBayesCore::Cloner<rbType, IsDerivedFrom<rbType, RevBayesCore::Cloneable>::Is >::createClone( dagNode->getValue() ) );
        dagNode->replace(newNode);
        
        // delete the value if there are no other references to it.
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
        dagNode = newNode;
        
        // increment the reference counter
        dagNode->incrementReferenceCount();
    }
    
}


/**
 * Convert a model object to a conversion object, the value of which is determined by a type
 * conversion from a specified variable.
 */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::makeConversionValue( RevPtr<Variable> var )
{
    // Create the converter node
    ConverterNode< ModelObject<rbType> >* newNode = new ConverterNode< ModelObject<rbType> >( "", var, getTypeSpec() );
    newNode->setHidden( true );

    // Signal replacement and delete the value if there are no other references to it.
    if ( dagNode != NULL )
    {
        dagNode->replace( newNode );
        if ( dagNode->decrementReferenceCount() == 0 )
            delete dagNode;
    }
    
    // Shift the actual node
    dagNode = newNode;
    
    // Increment the reference counter
    dagNode->incrementReferenceCount();
}


/**
 * Make an indirect reference to the variable. This is appropriate for the contexts
 * where the object occurs on the righ-hand side of expressions like a := b
 */
template <typename rbType>
RevLanguage::ModelObject<rbType>* RevLanguage::ModelObject<rbType>::makeIndirectReference(void)
{
    IndirectReferenceNode< RevLanguage::ModelObject<rbType> >* newNode = new IndirectReferenceNode< RevLanguage::ModelObject<rbType> >( "", this->getDagNode() );
    
    RevLanguage::ModelObject< rbType >* newObj = this->clone();
    
    newObj->setDagNode( newNode );
    
    return newObj;
}


/**
 * In this function we make member methods that belong to this level to serve
 * derived classes when they construct their static member method tables.
 * Using this mechanism, we ensure that the methods constructed at this
 * level for each derived class can use the appropriate type specification
 * for the derived class in its argument rules, if necessary. See the setValue
 * function for an example.
 *
 */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::initializeMethods(void) const
{
    // add the inherited rules
    AbstractModelObject::initializeMethods();
    
    if ( this->dagNode != NULL && this->dagNode->isStochastic() )
    {
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back( new ArgumentRule("x", getTypeSpec(), ArgumentRule::BY_VALUE ) );
        this->methods.addFunction("clamp", new MemberProcedure( RlUtils::Void, clampArgRules) );
    
        ArgumentRules* redrawArgRules = new ArgumentRules();
        this->methods.addFunction("redraw", new MemberProcedure( RlUtils::Void, redrawArgRules) );
    
        ArgumentRules* setValueArgRules = new ArgumentRules();
        setValueArgRules->push_back( new ArgumentRule("x", getTypeSpec(), ArgumentRule::BY_VALUE ) );
        this->methods.addFunction("setValue", new MemberProcedure( RlUtils::Void, setValueArgRules) );
    
        ArgumentRules* unclampArgRules = new ArgumentRules();
        this->methods.addFunction("unclamp", new MemberProcedure( RlUtils::Void, unclampArgRules) );
   
    }
    
}


/** Convert a model object to a deterministic object, the value of which is determined by a user-defined Rev function */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::makeUserFunctionValue( UserFunction* fxn )
{
    UserFunctionNode< ModelObject<rbType> >*  detNode = new UserFunctionNode< ModelObject<rbType> >( "", fxn );
    
    // Signal replacement and delete the value if there are no other references to it.
    if ( dagNode != NULL )
    {
        dagNode->replace( detNode );
        if ( dagNode->decrementReferenceCount() == 0 )
            delete dagNode;
    }
    
    // Shift the actual node
    dagNode = detNode;
    
    // Increment the reference counter
    dagNode->incrementReferenceCount();
}


/** Print structure info for user */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::printStructure( std::ostream &o, bool verbose ) const
{
    o << "_RevType      = " << getType() << std::endl;
    o << "_RevTypeSpec  = [ " << getTypeSpec() << " ]" << std::endl;
    o << "_value        = ";
    
    std::ostringstream o1;
    printValue( o1 );
    o << StringUtilities::oneLiner( o1.str(), 54 ) << std::endl;

    dagNode->printStructureInfo( o, verbose );
    
    
    const MethodTable& methods = getMethods();
    for ( MethodTable::const_iterator it = methods.begin(); it != methods.end(); ++it )
    {
        o << "." << (*it).first << " = ";
        (*it).second->printValue( o );
        o << std::endl;
    }
}


/**
 * Print value for user. The DAG node pointer may be NULL, in which
 * case we print "NA".
 */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::printValue(std::ostream &o) const
{
    if ( dagNode == NULL )
    {
        o << "NA";
    }
    else
    {
        dagNode->printValue( o );
    }
    
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::replaceVariable(RevObject *newVar) {
    
    RevBayesCore::DagNode* newParent = newVar->getDagNode();
    
    if ( dagNode != NULL )
    {
        while ( dagNode->getNumberOfChildren() > 0 )
        {
            dagNode->getFirstChild()->swapParent(dagNode, newParent);
        }
    }
}


/** Copy name of variable onto DAG node, if it is not NULL */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::setName(std::string const &n)
{
    if ( dagNode != NULL )
        dagNode->setName( n );
}


/**
 * Set dag node. We also accommodate the possibility of setting the DAG node
 * to null.
 */
template <typename rbType>
void RevLanguage::ModelObject<rbType>::setDagNode(RevBayesCore::DagNode* newNode) {
    
    // Take care of the old value node
    if ( dagNode != NULL )
    {
        if ( newNode != NULL )
            newNode->setName( dagNode->getName() );
        dagNode->replace(newNode);
        
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
    }
    
    // Set the new value node
    dagNode = static_cast< RevBayesCore::TypedDagNode<rbType>* >( newNode );
    
    // Increment the reference count to the new value node
    if ( dagNode != NULL )
        dagNode->incrementReferenceCount();
    
}


template <typename rbType>
void RevLanguage::ModelObject<rbType>::setValue(rbType *x) {
    
    RevBayesCore::ConstantNode<rbType>* newNode;
    
    if ( dagNode == NULL )
    {
        newNode = new RevBayesCore::ConstantNode<rbType>("",x);
    }
    else
    {
        newNode = new RevBayesCore::ConstantNode<rbType>(dagNode->getName(),x);
        dagNode->replace(newNode);
        
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
    }
    
    dagNode = newNode;
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
}


#endif
