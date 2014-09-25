#ifndef ModelContainer_H
#define ModelContainer_H

#include "Container.h"
#include "IndirectReferenceNode.h"
#include "ElementLookupNode.h"
#include "RlUserInterface.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "UserFunctionNode.h"

#include <iostream>
#include <vector>


namespace RevLanguage {
    

    /**
     * @brief ModelContainer: base class for Rev containers of model objects
     *
     * The most important difference between this class and WorkspaceContainer
     * is that this class keeps an internal DAG node that is in charge of the
     * value. The distinction between WorkspaceContainer and ModelContainer is
     * analogous to that between WorkspaceObject and ModelObject.
     * 
     * The template type is the Rev model object element type (rlType) and dim, the dimension
     * of the container (1 for vector, 2 for matrix, etc).
     *
     * Because of the need for a specialized value type for containers of abstract
     * elements, the template arguments also include the desired internal value type (rbType).
     *
     * Because of the dependency of some functionality on the number of
     * dimensions, we provide specializations for supported number of dimensions
     * (currently 1 and 2) for some functions, but no generic implementation, ensuring
     * that this class can only be used with those dimensions.
     */
    template <typename rlType, size_t dim, typename rbType>
    class ModelContainer : public Container {

    public:
        // The value type and other useful type definitions
        typedef rbType valueType;
        typedef typename rbType::iterator        iterator;
        typedef typename rbType::const_iterator  const_iterator;
        
        virtual                                         ~ModelContainer(void);                                               //!< Destructor
        
        // STL vector-like functions
        iterator                                        begin(void);                                                        //!< Iterator to the beginning of the Vector
        const_iterator                                  begin(void) const;                                                  //!< Const-iterator to the beginning of the Vector
        iterator                                        end(void);                                                          //!< Iterator to the end of the Vector
        const_iterator                                  end(void) const;                                                    //!< Const-iterator to the end of the Vector
        
        // Basic utility functions you have to override
        virtual ModelContainer<rlType,dim,rbType>*      clone(void) const = 0;                                              //!< Clone object
        static const std::string&                       getClassType(void);                                                 //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                             //!< Get class type spec
        virtual const TypeSpec&                         getTypeSpec(void) const = 0;                                        //!< Get the object type spec of the instance
        virtual void                                    makeCompositeValue() = 0;                                           //!< Convert the container to a composite value
        virtual RevObject*                              makeIndirectReference(void) = 0;                                    //!< Make an object referencing the dag node of this object
        virtual void                                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        
        // Basic utility functions you should not have to override
        RevObject*                                      cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*>& nodesMap ) const;  //!< Clone the model DAG connected to this node
        RevBayesCore::TypedDagNode<rbType>*             getDagNode(void) const;                                             //!< Get the DAG node
        const rbType&                                   getValue(void) const;                                               //!< Get the internal value
        bool                                            hasDagNode(void) const;                                             //!< Do we have a DAG node?
        bool                                            isAssignable(void) const;                                           //!< Is object or upstream members assignable?
        bool                                            isComposite(void) const;                                            //!< Is this a composite container?
        bool                                            isConstant(void) const;                                             //!< Is this variable and the internally stored deterministic node constant?
        void                                            makeConstantValue();                                                //!< Convert to constant object
        void                                            makeConversionValue(RevPtr<Variable> var);                          //!< Convert to conversion object
        void                                            makeUserFunctionValue(UserFunction* fxn);                           //!< Convert to user-defined Rev function objec
        void                                            printStructure(std::ostream& o, bool verbose=false) const;          //!< Print structure of language object for user
        void                                            replaceVariable(RevObject *newVar);                                 //!< Prepare to replace the internal DAG node
        void                                            setDagNode(RevBayesCore::DagNode *newNode);                         //!< Set or replace the internal dag node (and keep me)
        void                                            setName(const std::string &n);                                      //!< Set the name of the variable (if applicable)
        
        // Member object functions you may want to override
        virtual RevPtr<Variable>                        executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Map member methods to internal functions
        virtual const MethodTable&                      getMethods(void) const = 0;                                                 //!< Get member methods (const)
        virtual MethodTable                             makeMethods(void) const;                                                    //!< Make member methods (const)
        
        // Container functions you have to override
        virtual RevPtr<Variable>                        findOrCreateElement(const std::vector<size_t>& oneOffsetIndices) = 0;               //!< Find or create element variable
        virtual RevPtr<Variable>                        getElement(const std::vector<size_t>& oneOffsetIndices) = 0;                        //!< Get element variable
        virtual void                                    setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths) = 0; //!< Set elements from Rev objects

        // Container functions you should not have to override
        size_t                                          getDim(void) const;                                                 //!< Get the dimensions
        RevObject*                                      makeElementLookup(const RevPtr<Variable>&                var,
                                                                          const std::vector< RevPtr<Variable> >& oneOffsetIndices); //!< Get dynamic element variable lookup
        size_t                                          size(void) const;                                                   //!< Get the number of elements

    protected:
        ModelContainer(void);                                                                                               //!< Construct empty container
        ModelContainer(const rbType& v);                                                                                    //!< Construct constant model container
        ModelContainer(RevBayesCore::TypedDagNode<rbType>* n);                                                              //!< Construct model container from DAG node
        ModelContainer(const ModelContainer<rlType,dim,rbType>& c);                                                         //!< Copy constructor
        
        // Assignment operator
        ModelContainer&                                 operator=(const ModelContainer& x);                                 //!< Assignment operator
        
        // Member variable (protected and not private to make it available to derived classes)
        RevBayesCore::TypedDagNode<rbType>*             dagNode;                                                            //!< The DAG node keeping the value

    };

}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ContainerNode.h"
#include "ConverterNode.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RlUtils.h"
#include "StochasticNode.h"

using namespace RevLanguage;


/**
 * Default constructor. Construct empty model container. We do not
 * need to store the type of element because that can be obtained
 * from the type spec of the derived class instance.
 */
template <typename rlType, size_t dim, typename rbType>
ModelContainer<rlType, dim, rbType>::ModelContainer( void ) :
    Container(),
    dagNode( new RevBayesCore::ConstantNode<rbType>( "", new rbType() ) )
{
    dagNode->incrementReferenceCount();
}


/**
 * Construct constant model container from appropriate container value,
 * which needs to be of rbType.
 */
template <typename rlType, size_t dim, typename rbType>
ModelContainer<rlType, dim, rbType>::ModelContainer( const rbType& v ) :
    Container(),
    dagNode( new RevBayesCore::ConstantNode<rbType>( "", new rbType( v ) ) )
{
    dagNode->incrementReferenceCount ();
}


/**
 * Construct a container wrapper around an appropriate DAG node.
 */
template <typename rlType, size_t dim, typename rbType>
ModelContainer<rlType, dim, rbType>::ModelContainer( RevBayesCore::TypedDagNode<rbType>* n ) :
    Container(),
    dagNode( n )
{
    dagNode->incrementReferenceCount();
}


/**
 * Copy constructor. We make an independent copy of the DAG node
 * here (independent elements), and deal with DAG node memory management.
 */
template <typename rlType, size_t dim, typename rbType>
ModelContainer<rlType, dim, rbType>::ModelContainer( const ModelContainer& c) :
    Container( c ),
    dagNode( NULL )
{
    if ( c.dagNode != NULL )
    {
        if ( c.isComposite() )
            dagNode = static_cast< ContainerNode<rlType, rbType>* >( c.dagNode )->deepCopy();
        else
            dagNode = c.dagNode->clone();
        
        dagNode->incrementReferenceCount();
    }
}


/** Destructor needs to deal with DAG node memory management */
template <typename rlType, size_t dim, typename rbType>
ModelContainer<rlType, dim, rbType>::~ModelContainer()
{
    if ( dagNode != NULL && dagNode->decrementReferenceCount() == 0 )
        delete dagNode;
}


/**
 * Assignment operator. We make sure we have an independent clone of the
 * DAG node here, and deal with DAG node memory management.
 */
template <typename rlType, size_t dim, typename rbType>
ModelContainer<rlType, dim, rbType>& ModelContainer<rlType, dim, rbType>::operator=(const ModelContainer& c)
{
    
    if ( this != &c )
    {
        // Free the memory
        if ( dagNode->decrementReferenceCount() == 0 )
            delete dagNode;

        dagNode = NULL;
        
        // Create my own copy
        if ( c.dagNode != NULL )
        {
            if ( c.isComposite() )
                dagNode = static_cast< ContainerNode<rlType, rbType>* >( c.dagNode )->deepCopy();
            else
                dagNode = c.dagNode->clone();
            dagNode->incrementReferenceCount();
        }
    }
    
    return *this;
}


/**
 * Get iterator to the beginning of the container value elements. We
 * assume that the rbType class has an iterator.
 *
 * @todo Check that this is safe. Is it not possible to assign to value using this iterator?
 */
template <typename rlType, size_t dim, typename rbType>
typename rbType::iterator ModelContainer<rlType, dim, rbType>::begin( void )
{
    return dagNode->getValue().begin();
}


/**
 * Get const_iterator to the beginning of the container value elements. We
 * assume that the rbType class has a const_iterator.
 */
template <typename rlType, size_t dim, typename rbType>
typename rbType::const_iterator ModelContainer<rlType, dim, rbType>::begin( void ) const
{
    return dagNode->getValue().begin();
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
template<typename rlType, size_t dim, typename rbType>
RevLanguage::RevObject* RevLanguage::ModelContainer<rlType, dim, rbType>::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*>& nodesMap ) const
{
    ModelContainer<rlType, dim, rbType>* theClone = clone();
    
    theClone->setDagNode( NULL );
    
    RevBayesCore::DagNode* theNodeClone = dagNode->cloneDAG( nodesMap );

    theClone->setDagNode( theNodeClone );
    
    return theClone;
}


/**
 * Get iterator to the end of the container value elements. We assume
 * that the rbType class has an iterator.
 *
 * @todo Check that this is safe. Is it not possible to assign to value using this iterator?
 */
template <typename rlType, size_t dim, typename rbType>
typename rbType::iterator ModelContainer<rlType, dim, rbType>::end( void )
{
    return dagNode->getValue().end();
}


/**
 * Get const_iterator to the end of the container value elements. We assume
 * that the rbType class has a const_iterator.
 */
template <typename rlType, size_t dim, typename rbType>
typename rbType::const_iterator ModelContainer<rlType, dim, rbType>::end( void ) const
{
    return dagNode->getValue().end();
}


/**
 * Map calls to member methods. This deals with the stochastic variable methods.
 */
template <typename rlType, size_t dim, typename rbType>
RevPtr<Variable> ModelContainer<rlType, dim, rbType>::executeMethod( std::string const &name, const std::vector<Argument> &args )
{
    if ( name == "clamp" )
    {
        // Check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() ) {
            throw RbException( "Only stochastic variables can be clamped." );
        }
        
        // Convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast< RevBayesCore::StochasticNode<rbType>* >( dagNode );
        
        // Get the observation
        const rbType &observation = static_cast< const ModelContainer<rlType, 1, rbType>& >( args[0].getVariable()->getRevObject() ).getValue();
        
        // Clamp
        stochNode->clamp( new rbType( observation ) );
        
        return NULL;
    }
    else if (name == "redraw")
    {
        // Check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException("You can only redraw the value of a stochastic variable.");
        }
        
        // Convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast< RevBayesCore::StochasticNode<rbType>* >( dagNode );
        
        // Redraw the value
        stochNode->redraw();
        
        return NULL;
    }
    else if ( name == "setValue" )
    {
        // Check whether the variable is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException( "You can only set the value of a stochastic variable." );
        }
        
        // Convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast< RevBayesCore::StochasticNode<rbType>* >( dagNode );
        
        // Get the observation
        const rbType &observation = static_cast< const ModelContainer<rlType, 1, rbType>& >( args[0].getVariable()->getRevObject() ).getValue();
        
        // Set the value
        stochNode->setValue( new rbType(observation) );
        
        return NULL;
    }
    else if ( name == "unclamp" )
    {
        // Check whether the DAG node is actually a stochastic node
        if ( !dagNode->isStochastic() )
        {
            throw RbException( "Only stochastic variables can be unclamped." );
        }
        
        // Convert the node
        RevBayesCore::StochasticNode<rbType>* stochNode = static_cast< RevBayesCore::StochasticNode<rbType>* >( dagNode );
        
        // Unclamp
        stochNode->unclamp();
        
        return NULL;
    }

    return Container::executeMethod( name, args );
}


/** Get Rev type of object */
template <typename rlType, size_t dim, typename rbType>
const std::string& ModelContainer<rlType, dim, rbType>::getClassType(void)
{
    static std::string revType = "ModelContainer";
    
	return revType;
}


/** Get class type spec describing type of object */
template <typename rlType, size_t dim, typename rbType>
const TypeSpec& ModelContainer<rlType, dim, rbType>::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Container::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get the DAG node holding the value */
template <typename rlType, size_t dim, typename rbType>
RevBayesCore::TypedDagNode<rbType>* ModelContainer<rlType, dim, rbType>::getDagNode( void ) const
{
    return dagNode;
}


/**
 * Get dimension of container. Returns 1 for vector, 2 for matrix etc.
 * Because the dimension is part of the template, we can do this in a
 * generic way.
 */
template <typename rlType, size_t dim, typename rbType>
size_t ModelContainer<rlType, dim, rbType>::getDim( void ) const
{
    return dim;
}


/**
 * Get the value of the node. We only give out a const reference to make
 * sure that nobody else messes with our value. Only the DAG node should
 * be able to do this.
 */
template <typename rlType, size_t dim, typename rbType>
const rbType& ModelContainer<rlType, dim, rbType>::getValue( void ) const
{
    if ( dagNode == NULL )
        throw RbException( "Invalid attempt to get the value of an NA object" );
    
    return dagNode->getValue();
}


/** Make sure users understand we have an internal DAG node */
template <typename rlType, size_t dim, typename rbType>
bool ModelContainer<rlType, dim, rbType>::hasDagNode( void ) const
{
    return true;
}


/**
 * Is the object or any of its upstream members or elements
 * modifiable by the user through assignment? We simply ask
 * our DAG node.
 */
template <typename rlType, size_t dim, typename rbType>
bool ModelContainer<rlType, dim, rbType>::isAssignable( void ) const
{
    if ( this->dagNode == NULL )
        return false;
    
    return dagNode->isAssignable();
}


/** Check whether this container is composite by checking the DAG node type. */
template <typename rlType, size_t dim, typename rbType>
bool ModelContainer<rlType, dim, rbType>::isComposite( void ) const
{
    return dynamic_cast< ContainerNode<rlType, rbType>* >( dagNode ) != NULL;
}


/** Check whether this node has a constant value by asking the DAG node. */
template <typename rlType, size_t dim, typename rbType>
bool ModelContainer<rlType, dim, rbType>::isConstant( void ) const
{
    return dagNode->isConstant();
}


/** Convert this node to a constant value if it is not already a constant value. */
template <typename rlType, size_t dim, typename rbType>
void ModelContainer<rlType, dim, rbType>::makeConstantValue( void )
{
    if ( dynamic_cast< RevBayesCore::ConstantNode<rbType>* >( dagNode ) == NULL )
    {
        RevBayesCore::ConstantNode<rbType>* newNode = new RevBayesCore::ConstantNode<rbType>( dagNode->getName(), new rbType( dagNode->getValue() ) );

        setDagNode( newNode );
    }
}


/**
 * Convert a model object to a conversion object, the value of which is determined by a type
 * conversion from a specified variable.
 */
template <typename rlType, size_t dim, typename rbType>
void ModelContainer<rlType, dim, rbType>::makeConversionValue( RevPtr<Variable> var )
{
    // Create the converter node
    ConverterNode< ModelContainer<rlType, dim, rbType> >* newNode = new ConverterNode< ModelContainer<rlType, dim, rbType> >( "", var, getTypeSpec() );
    
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
 * Make element lookup. This is relevant for the dynamic evaluation context, where
 * we try to retrieve an element of this object, for instance
 *
 *    a := b[i]
 *
 * We need to make sure that the element is looked up dynamically based on the
 * current value of i in this evaluation context.
 */
template <typename rlType, size_t dim, typename rbType>
RevObject* ModelContainer<rlType, dim, rbType>::makeElementLookup( const RevPtr<Variable>& var, const std::vector< RevPtr<Variable> >& oneOffsetIndices ) {
    
    ElementLookupNode< ModelContainer<rlType, dim, rbType>, rlType >* newNode =
        new ElementLookupNode< ModelContainer<rlType, dim, rbType>, rlType >( "", var, oneOffsetIndices );

    rlType* newObj = new rlType( newNode );
    
    return newObj;
}


/**
 * Make member methods for this class. Here we provide argument rules for the stochastic
 * variable functions.
 */
template <typename rlType, size_t dim, typename rbType>
MethodTable ModelContainer<rlType, dim, rbType>::makeMethods( void ) const
{
    MethodTable methods = MethodTable();
    
    // Stochastic variable member functions
    ArgumentRules* clampArgRules = new ArgumentRules();
    clampArgRules->push_back( new ArgumentRule("x", getTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("clamp", new MemberProcedure( RlUtils::Void, clampArgRules) );
    
    ArgumentRules* redrawArgRules = new ArgumentRules();
    methods.addFunction("redraw", new MemberProcedure( RlUtils::Void, redrawArgRules) );
    
    ArgumentRules* setValueArgRules = new ArgumentRules();
    setValueArgRules->push_back( new ArgumentRule("x", getTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("setValue", new MemberProcedure( RlUtils::Void, setValueArgRules) );
    
    ArgumentRules* unclampArgRules = new ArgumentRules();
    methods.addFunction("unclamp", new MemberProcedure( RlUtils::Void, unclampArgRules) );
    
    // Insert inherited methods
    methods.insertInheritedMethods( Container::makeMethods() );
    
    return methods;
}


/** Convert a model object to a deterministic object, the value of which is determined by a user-defined Rev function */
template <typename rlType, size_t dim, typename rbType>
void ModelContainer<rlType, dim, rbType>::makeUserFunctionValue( UserFunction* fxn )
{
    UserFunctionNode< ModelContainer<rlType, dim, rbType> >*  detNode  = new UserFunctionNode< ModelContainer<rlType, dim, rbType> >( "", fxn );
    
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


/**
 * Print structure info for user  We use a combination of information
 * from different sources.
 */
template <typename rlType, size_t dim, typename rbType>
void ModelContainer<rlType, dim, rbType>::printStructure( std::ostream &o, bool verbose ) const
{
    Container::printStructure( o, verbose );

    dagNode->printStructureInfo( o, verbose );
    
    printMemberInfo( o );
}


/**
 * Replace variable. This is called in Variable just prior to replacing this object
 * by another object with a different DAG node. We deal with this situation by telling
 * all children of the current DAG node that they are getting a new parent so that
 * they can adjust accordingly.
 */
template <typename rlType, size_t dim, typename rbType>
void ModelContainer<rlType, dim, rbType>::replaceVariable( RevObject *newObj )
{
    RevBayesCore::DagNode* newParent = newObj->getDagNode();
    
    if ( dagNode != NULL )
    {
        while ( dagNode->getNumberOfChildren() > 0 )
            dagNode->getFirstChild()->swapParent(dagNode, newParent);
    }
}


/**
 * Replace the DAG node. We throw an error if the new node does
 * not have the right value type. This is based on a dynamic
 * cast to the expected value type.
 */
template <typename rlType, size_t dim, typename rbType>
void RevLanguage::ModelContainer<rlType, dim, rbType>::setDagNode( RevBayesCore::DagNode* newNode )
{
    RevBayesCore::TypedDagNode<rbType>* newDagNode = dynamic_cast< RevBayesCore::TypedDagNode<rbType>* >( newNode );
    if ( newNode != NULL && newDagNode == NULL )
            throw RbException( "Illegal attempt to set type '" + this->getType() + "' with DAG node of wrong value type" );
    
    // Take care of the old value node
    if ( dagNode != NULL )
    {
        if ( newNode != NULL )
            newNode->setName( dagNode->getName() );
        dagNode->replace( newDagNode );
        
        if ( dagNode->decrementReferenceCount() == 0 )
            delete dagNode;
    }
    
    // Set the new value node
    dagNode = newDagNode;
    
    // Increment the reference count to the new value node
    if ( dagNode != NULL )
        dagNode->incrementReferenceCount();
}


/** Name the object by naming the DAG node inside it. */
template <typename rlType, size_t dim, typename rbType>
void ModelContainer<rlType, dim, rbType>::setName( std::string const& n )
{
    if ( dagNode != NULL )
        dagNode->setName( n );
}


/**
 * Return size of container. This relies on there being a member
 * function size() in rbType. If we are a composite container,
 * we want to ask our container node so that we can get the size
 * without having to compute an internal value. This is because there
 * may be NA elements, which make it impossible to compute an internal
 * value representation.
 */
template <typename rlType, size_t dim, typename rbType>
size_t ModelContainer<rlType, dim, rbType>::size( void ) const
{
    if ( isComposite() )
        return static_cast< ContainerNode<rlType, rbType>* >( dagNode )->size();
    else
        return getValue().size();
}


#endif
