#ifndef ContainerNode_H
#define ContainerNode_H

#include "DynamicNode.h"
#include "RevPtr.h"
#include "TypedFunction.h"

#include <vector>

namespace RevLanguage {
    
    /**
     * @brief ContainerNode: DAG node for Rev containers
     *
     * This class is used for composite Rev model containers. These differ from
     * other model containers in that their value is not determined by a single
     * function. Instead, each element has its own DAG node determining its value.
     * The value of the container as a whole is simply composed by the values of
     * the individual elements.
     * 
     * A key element of composite model containers is the vector of variables, which
     * is kept inside the ContainerNode object because other types of model containers
     * do not keep the full variable or RevObject info of the elements.
     *
     * The template type valueType is the type of the value of the container as a whole,
     * not of the individual elements. It is typically std::vector<elemType> but could
     * also be RbVector<elemType>. In both cases we can access the elements through
     * index operations. The rlElemType template argument is the Rev type of the container
     * elements.
     *
     * For now, we only support vectors, although it is straightforward to extend the class
     * to containers of multiple dimensions.
     */
    template< typename rlElemType, typename valueType >
    class ContainerNode : public RevBayesCore::DynamicNode< valueType > {

    public:
        ContainerNode(const std::string& n);                                                                            //!< Constructor of empty container node
        ContainerNode(const std::string& n, const valueType& v);                                                        //!< Construct container from valueType
        ContainerNode(const std::string& n, std::vector<RevObject*> elems, const std::vector<size_t>& lengths);         //!< Construct container from Rev objects
        ContainerNode(const ContainerNode<rlElemType, valueType>& n);                                                   //!< Copy constructor

        virtual                                 ~ContainerNode(void);                                                   //!< Virtual destructor
        
        // STL-like container functions
        void                                    pop_back(void);                                                         //!< Drop element from back
        void                                    pop_front(void);                                                        //!< Drop element from front
        void                                    push_back(rlElemType* x);                                               //!< Push element onto back
        void                                    push_front(rlElemType* x);                                              //!< Push element onto front
        
        
        // Public methods
        ContainerNode<rlElemType,valueType>*    clone(void) const;                                                      //!< Type-safe clone
        RevPtr<Variable>&                       getElement(size_t index);                                               //!< Return an element
        double                                  getLnProbability(void);                                                 //!< Get ln prob (0.0)
        double                                  getLnProbabilityRatio(void);                                            //!< Get ln prob ratio (0.0)
        valueType&                              getValue(void);                                                         //!< Get the value
        const valueType&                        getValue(void) const;                                                   //!< Get the value (const)
        bool                                    isConstant(void) const;                                                 //!< Is this DAG node constant?
        virtual void                            printStructureInfo(std::ostream& o) const;                              //!< Print structure info
        void                                    redraw(void);                                                           //!< Redraw (or not)
        void                                    setName(const std::string& n);                                          //!< Set the name of myself and of elements
        void                                    swapParameter(const RevBayesCore::DagNode* oldP, const RevBayesCore::DagNode* newP);    //!< Swap parameter: just touch ourselves to be on the safe side
        void                                    update(void);                                                           //!< Update current value
        
    protected:
        void                                    getAffected(std::set<RevBayesCore::DagNode *>& affected, RevBayesCore::DagNode* affecter);  //!< Mark and get affected nodes
        void                                    keepMe(RevBayesCore::DagNode* affecter);                                                    //!< Keep value of this and affected nodes
        void                                    restoreMe(RevBayesCore::DagNode *restorer);                                                 //!< Restore value of this nodes
        void                                    touchMe(RevBayesCore::DagNode *toucher);                                                    //!< Touch myself and tell affected nodes value is reset
        
    private:
        std::vector< RevPtr<Variable> >         elements;                                                               //!< The elements
        Container*                              myOwner;
        bool                                    touched;                                                                //!< Are we dirty?
        valueType                               value;                                                                  //!< Current value
    };
    
}


#include "RevPtr.h"
#include "Variable.h"

using namespace RevLanguage;


/**
 * Constructor of empty container node.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::ContainerNode( const std::string &n ) :
    RevBayesCore::DynamicNode<valueType>( n ),
    elements(),
    touched( false ),
    value()
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
}


/**
 * Constructor of constant value container node. We take a
 * value of valueType and set our elements vector from that
 * value.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::ContainerNode( const std::string &n, const valueType& v ) :
    RevBayesCore::DynamicNode<valueType>( n ),
    elements(),
    touched( true ),
    value()
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;

    for ( size_t i = 0; i < v.size(); ++i )
        elements.push_back( new Variable( new rlElemType( v[i] ) ) );
}


/**
 * Constructor of container node from vector of Rev objects and
 * a lengths specification. For now, we assume that we are dealing
 * with a vector.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::ContainerNode( const std::string &n, std::vector<RevObject*> elems, const std::vector<size_t>& lengths ) :
    RevBayesCore::DynamicNode<valueType>( n ),
    elements(),
    touched( true ),
    value()
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    if ( lengths.size() != 1 || lengths[0] != elems.size() )
        throw RbException( "Invalid lengths specification in ContainerNode" );
    
    for ( size_t i = 0; i < elems.size(); ++i )
        elements.push_back( new Variable( elems[i] ) );
}


/**
 * Copy constructor. We make a deep copy of the variable elements here.
 * Simply mark ourselves as dirty and recalculate value later, when asked
 * about the value.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::ContainerNode( const ContainerNode<rlElemType, valueType>& n ) :
    RevBayesCore::DynamicNode<valueType>( n ),
    elements(),
    touched( true ),
    value()
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Make sure we have independent copies of the elements, not just smart pointers to the same elements
    for ( std::vector< RevPtr<Variable> >::const_iterator it = n.elements.begin(); it != n.elements.end(); ++it )
    {
        Variable* newVar = new Variable( (*it)->getRevObject().clone() );
        elements.push_back( newVar );
    }
    
}


/** Destructor. No need to delete variables here because the variables are smart pointers. */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::~ContainerNode( void )
{
}


/** Type-safe clone function */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>* ContainerNode<rlElemType, valueType>::clone( void ) const
{
    return new ContainerNode<rlElemType, valueType>( *this );
}


/**
 * Get the affected nodes.
 * This call is started by the parent. We need to delegate this call to all our children.
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::getAffected( std::set<RevBayesCore::DagNode *>& affected, RevBayesCore::DagNode* affecter )
{
    this->getAffectedNodes( affected );
}


/**
 * Return a reference to the vector of elements. The caller can modify the
 * elements; the elements will tell us through their DAG nodes if they have
 * been modified, so that our touched flag gets set, so this should be safe.
 */
template<typename rlElemType, typename valueType>
RevPtr<Variable>& ContainerNode<rlElemType, valueType>::getElement( size_t index )
{
    return elements[ index ];
}


/** 
 * Calculate ln prob.
 * @todo Do we really need this?
 */
template<typename rlElemType, typename valueType>
double ContainerNode<rlElemType, valueType>::getLnProbability( void )
{
    return 0.0;
}


/**
 * Calculate ln prob ratio.
 * @todo Do we really need this?
 */
template<typename rlElemType, typename valueType>
double ContainerNode<rlElemType, valueType>::getLnProbabilityRatio( void )
{
    return 0.0;
}


/** Get the value (non-const version) */
template<typename rlElemType, typename valueType>
valueType& ContainerNode<rlElemType, valueType>::getValue( void )
{
    if ( touched )
        update();

    return value;
}


/**
 * Get the value (const version). Note that we need to type-cast here to get access to the
 * update function in case we have been touched and have not updated before. This is required
 * by the lazy evaluation mechanism.
 */
template<typename rlElemType, typename valueType>
const valueType& ContainerNode<rlElemType, valueType>::getValue( void ) const {
    
    if ( touched )
        const_cast<ContainerNode<rlElemType, valueType>*>( this )->update();

    return value;
}



/**
 * Is this node constant? Only if all parents are constant.
 *
 * @todo It seems like this could be taken care of by the DagNode base class.
 */
template<typename rlElemType, typename valueType>
bool ContainerNode<rlElemType, valueType>::isConstant( void ) const
{
    // iterate over all parents and only if all parents are constant then this node is constant too
    for (std::set<const RevBayesCore::DagNode*>::iterator it = this->parents.begin(); it != this->parents.end(); ++it)
    {
        if ( !(*it)->isConstant() )
            return false;
    }
    
    return true;
}


/**
 * Keep the current value of the node.
 * At this point, we just delegate to the children.
 *
 * @todo This is just a copy of what is in the deterministic node.
 *       I do not fully understand this. Is it not dangerous to
 *       mark ourselves as clean when we have not checked first if
 *       we are dirty and then update ourselves if so?
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::keepMe( RevBayesCore::DagNode* affecter )
{
    // we just mark ourselves as clean
    if ( touched )
        throw RbException( "Inconsistent touched state in ContainerNode" );
    this->touched = false;
    
    // delegate call
    this->keepAffected();
    
    // clear the list of touched element indices
    this->touchedElements.clear();
}


/** Drop an element from the back */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::pop_back( void )
{
    // Detach our bond to the element DAG node
    RevBayesCore::DagNode* theElementNode = elements.rbegin()->getRevObject().getDagNode();
    this->removeParent( theElementNode );
    theElementNode->removeChild( this );
    
    // Now get rid of it
    elements.pop_back();
    
    // We are dirty
    touched = true;
}


/** Drop an element from the front */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::pop_front( void )
{
    // Detach our bond to the element DAG node
    RevBayesCore::DagNode* theElementNode = elements.begin()->getRevObject().getDagNode();
    this->removeParent( theElementNode );
    theElementNode->removeChild( this );
    
    // Now get rid of it
    elements.erase( elements.begin() );

    // We are dirty
    touched = true;
}


/** Push an element onto the back */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::push_back( rlElemType* x )
{
    // Connect the new element
    x->getDagNode()->addChild( this );
    this->addParent( x->getDagNode() );
    
    // Guard the type of object put in the variable
    Variable* newVar = new Variable( x );
    newVar->setRevObjectTypeSpec( rlElemType::getClassTypeSpec() );
    
    // Name the new element
    std::ostringstream o;
    o << this->name;
    o << "[" << elements.size() + 1 << "]";
    newVar->setName( o.str() );

    // Push it onto the end of the vector
    elements.push_back( newVar );

    // We are dirty
    touched = true;
}


/** Push an element onto the front */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::push_front( rlElemType* x )
{
    // Connect the new element
    x->getDagNode()->addChild( this );
    this->addParent( x->getDagNode() );
    
    // Guard the type of object put in the variable
    Variable* newVar = new Variable( x );
    newVar->setRevObjectTypeSpec( rlElemType::getClassTypeSpec() );

    // Push it onto the front of the vector
    elements.insert( elements.begin(), new Variable( x ) );

    // Rename all the elements because their names are now incorrect
    for ( size_t i = 0; i < elements.size(); ++i )
    {
        std::ostringstream o;
        o << this->name;
        o << "[" << i + 1 << "]";
        elements[ i ]->setName( o.str() );
    }
    
    // We are dirty
    touched = true;
}


/**
 * Print struct for user. Because of lazy evaluation we need to make a
 * const cast, but this should be perfectly safe.
 */
template< typename rlElemType, typename valueType >
void ContainerNode< rlElemType, valueType >::printStructureInfo( std::ostream& o ) const
{
    o << "_dagType      = Container node" << std::endl;
    
    o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    o << "_value        = ";
    RevBayesCore::TypedDagNode<valueType>::printValue(o, ", ");
    o << std::endl;
    
    o << "_parents      = ";
    this->printParents(o);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o);
    o << std::endl;
}


/**
 * Redraw function.
 *
 * @todo Check if this is really needed.
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::redraw( void )
{
    // nothing to do
    // the touch should have called our update
}


/**
 * Restore the old value of the node and tell affected. We simply copy behavior
 * in DeterministicNode. However, it is doubtful that this call is needed.
 *
 * @todo Is this call really needed?
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::restoreMe( RevBayesCore::DagNode *restorer )
{
    // we need to recompute our value?!
    this->update();
    
    // we just mark ourselves as clean (actually done by update)
    this->touched = false;
    
    // delegate call
    this->restoreAffected();
    
    // clear the list of touched element indices
    this->touchedElements.clear();
}


/**
 * Swap parameter. This should be done automatically by the reassignment.
 * We just touch ourselves to make sure we are updated later, in case the
 * touch is not called by the reassignment as it should.
 *
 * @todo Check whether this is needed.
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::swapParameter( const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP )
{
    this->touched = true;
}


/**
 * Set the name of this node. We also need to update the name of our
 * elements.
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::setName( const std::string& n )
{
    RevBayesCore::DynamicNode<valueType>::setName( n );
    
    // Rename all the elements
    for ( size_t i = 0; i < elements.size(); ++i )
    {
        std::ostringstream o;
        o << this->name;
        o << "[" << i + 1 << "]";
        elements[ i ]->setName( o.str() );
    }
}


/** Touch this node for recalculation */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::touchMe( RevBayesCore::DagNode *toucher )
{
    if ( !this->touched )
    {
        // Touch myself
        this->touched = true;
    
        // Dispatch the touch message to downstream nodes
        this->touchAffected();
    }
}


/**
 * Update our value. We assume that the caller knows
 * that an update is really needed.
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::update()
{
    // Recompute our internal value
    value.clear();
    size_t i;
    std::vector< RevPtr<Variable> >::iterator it;
    for ( i = 0, it = elements.begin(); it != elements.end(); ++it, ++i )
        value.push_back( static_cast<rlElemType&>( (*it)->getRevObject() ).getValue() );
    
    // We are clean!
    touched = false;
}


#endif
