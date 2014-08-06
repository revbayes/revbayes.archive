#ifndef ContainerNode_H
#define ContainerNode_H

#include "DynamicNode.h"
#include "RevPtr.h"
#include "TypedFunction.h"

#include <vector>

namespace RevLanguage {
    
    /**
     * @brief DAG node for composite containers
     *
     * This class is used for composite Rev model containers. These differ from
     * other model containers in that their value is not determined by a single
     * DAG node. Instead, each element has its own DAG node determining its value.
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
        ContainerNode(const std::string& n, std::vector<RevObject*> elems, const std::vector<size_t>& lengths);         //!< Construct container from Rev objects
        ContainerNode(const ContainerNode<rlElemType, valueType>& n);                                                   //!< Copy constructor

        virtual                                 ~ContainerNode(void);                                                   //!< Virtual destructor

        // Assignment operator
        ContainerNode&                          operator=(const ContainerNode& x);                                      //!< Assignment operator

        // STL-like container functions
        void                                    pop_back(void);                                                         //!< Drop element from back
        void                                    pop_front(void);                                                        //!< Drop element from front
        void                                    push_back(rlElemType* x);                                               //!< Push element onto back
        void                                    push_front(rlElemType* x);                                              //!< Push element onto front
        
        // Public methods
        void                                    addChild(RevBayesCore::DagNode *child) const;                           //!< Protect NA containers to form part of DAGs
        ContainerNode<rlElemType,valueType>*    clone(void) const;                                                      //!< Type-safe clone
        RevBayesCore::DagNode*                  cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*> &nodesMap) const;   //!< Clone the entire DAG which is connected to this node
        RevPtr<Variable>&                       getElement(size_t index);                                               //!< Return an element
        RevPtr<const Variable>&                 getElement(size_t index) const;                                         //!< Return an element (const version)
        double                                  getLnProbability(void);                                                 //!< Get ln prob (0.0)
        double                                  getLnProbabilityRatio(void);                                            //!< Get ln prob ratio (0.0)
        valueType&                              getValue(void);                                                         //!< Get the value
        const valueType&                        getValue(void) const;                                                   //!< Get the value (const)
        bool                                    isConstant(void) const;                                                 //!< Is this DAG node constant?
        virtual void                            printStructureInfo(std::ostream& o) const;                              //!< Print structure info
        void                                    printValue(std::ostream& o) const;                                      //!< Print value
        void                                    redraw(void);                                                           //!< Redraw (or not)
        void                                    setName(const std::string& n);                                          //!< Set the name of myself and of elements
        size_t                                  size(void) const { return elements.size(); }                            //!< Return number of elements
        void                                    update(void);                                                           //!< Update current value
        
        // Parent DAG nodes management functions
        std::set<const RevBayesCore::DagNode*>  getParents(void) const;                                                                     //!< Get the set of parents
        void                                    swapParent(const RevBayesCore::DagNode *oldParent, const RevBayesCore::DagNode *newParent); //!< Exchange the parent (element variable)
        
    protected:
        void                                    getAffected(std::set<RevBayesCore::DagNode *>& affected, RevBayesCore::DagNode* affecter);  //!< Mark and get affected nodes
        void                                    keepMe(RevBayesCore::DagNode* affecter);                                                    //!< Keep value of this and affected nodes
        void                                    restoreMe(RevBayesCore::DagNode *restorer);                                                 //!< Restore value of this nodes
        void                                    touchMe(RevBayesCore::DagNode *toucher);                                                    //!< Touch myself and tell affected nodes value is reset
        
    private:
        std::vector< RevPtr<Variable> >         elements;                                                               //!< The elements
        bool                                    touched;                                                                //!< Are we dirty?
        valueType                               value;                                                                  //!< Current value
    };
    
}


#include "ModelObject.h"
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


#if 0
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
        this->push_back( new rlElemType( v[i] ) );
}
#endif


/**
 * Constructor of container node from vector of Rev objects and
 * a lengths specification. For now, we assume that we are dealing
 * with a vector. We assume ownership of the elements.
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
    {
        if ( !elems[i]->isTypeSpec( rlElemType::getClassTypeSpec() ) )
            throw RbException( "Invalid attempt to set container element of type '" + rlElemType::getClassType() + "' with object of type '" + elems[i]->getClassType() + "'" );
        this->push_back( static_cast<rlElemType*>( elems[i] ) );
    }
}


/**
 * Copy constructor. We make a deep copy here and override cloneDAG, which would
 * otherwise need a shallow copy. We need to pay special attention to NA elements.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::ContainerNode( const ContainerNode<rlElemType, valueType>& n ) :
    RevBayesCore::DynamicNode<valueType>( n ),
    elements(),
    touched( true ),
    value()
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Deep copy constructor
    for ( std::vector< RevPtr<Variable> >::const_iterator it = n.elements.begin(); it != n.elements.end(); ++it )
    {
        if ( (*it)->isNAVar() )
            push_back( NULL );
        else
            push_back( static_cast<rlElemType&>( (*it)->getRevObject() ).clone() );
    }
}


/**
 * Destructor. By calling our own pop_back, we ensure that we deal with
 * all of the DAG node management tasks correctly.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::~ContainerNode( void )
{
   // Deep copy destructor
    while ( elements.size() > 0 )
        pop_back();
}


/**
 * Assignment operator. We make a deep copy here using our
 * own push_back and pop_back functions to ensure proper
 * management of DAG node connections.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>& ContainerNode<rlElemType, valueType>::operator=( const ContainerNode<rlElemType, valueType>& x )
{
    if (this != &x)
    {
        RevBayesCore::DynamicNode<valueType>::operator=( x );

        // Pop old elements (deep copy destructor)
        while ( elements.size() > 0 )
            pop_back();

        // Push back new elements (deep copy constructor)
        for ( std::vector< RevPtr<Variable> >::iterator it = x.elements.begin(); it != x.elements.end(); ++it )
        {
            if ( (*it)->isNAVar() )
                push_back( NULL );
            else
                push_back( (*it)->getRevObject().clone() );
        }

        // Tell everybody we have been changed
        this->touch();
    }

    return *this;
}


/** Type-safe clone function */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>* ContainerNode<rlElemType, valueType>::clone( void ) const
{
    return new ContainerNode<rlElemType, valueType>( *this );
}


/**
 * Clone the entire graph: clone children, swap parents. Because our parents are doubly managed by
 * us and the variables holding the Rev objects that contain them, we need a special version of
 * the cloneDAG function.
 */
template<typename rlElemType, typename valueType>
RevBayesCore::DagNode* ContainerNode<rlElemType, valueType>::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode* >& newNodes ) const
{
    // Return our clone if we have already been cloned
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // We avoid the conflict between having to make a deep copy in our clone
    // function and the need to have a shallow copy (identical parents) in
    // this function by starting out with an empty container, and not filling
    // it until we have the element clones we need.
    ContainerNode<rlElemType, valueType>* copy = new ContainerNode<rlElemType, valueType>( this->name );
    
    // Add this node and its copy to the map
    newNodes[ this ] = copy;
    
    // Now make sure the copy has its own Rev objects with their DAG nodes being the clones of our parents
    for ( std::vector< RevPtr<Variable> >::const_iterator it = elements.begin(); it != elements.end(); ++it )
    {
        // Get the i-th parent
        const RevBayesCore::DagNode *theParent = (*it)->getRevObject().getDagNode();
        
        // Get its clone
        RevBayesCore::DagNode* theParentClone = theParent->cloneDAG( newNodes );
        
        // Now we can safely make a copy of the element
        rlElemType* elemCopy = static_cast<rlElemType&>( (*it)->getRevObject()).clone();
        
        // Replace its DAG node with the clone of our parent
        elemCopy->setDagNode( theParentClone );
        
        // Push it onto the copy
        copy->push_back( elemCopy );
    }
    
    /* Make sure the children clone themselves */
    for( std::set<RevBayesCore::DagNode*>::const_iterator it = this->children.begin(); it != this->children.end(); ++it )
        (*it)->cloneDAG( newNodes );
    
    return copy;
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
 * Return a reference to a specific element. The caller can modify the
 * element; the element will tell us through its DAG node if it has
 * been modified, so that our touched flag gets set, so this should be safe.
 * The user can even replace the RevObject inside the Variable instance,
 * and the integrity of the container node is still ensured through the
 * call to the swapParent function during variable object replacement.
 */
template<typename rlElemType, typename valueType>
RevPtr<Variable>& ContainerNode<rlElemType, valueType>::getElement( size_t index )
{
    return elements[ index ];
}


/**
 * Return a reference to a specific element. This is the const version, so
 * the caller cannot modify the element.
 */
template<typename rlElemType, typename valueType>
RevPtr<const Variable>& ContainerNode<rlElemType, valueType>::getElement( size_t index ) const
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


/**
 * Get the parents of this node. We simply walk through the elements and return
 * a set with their DAG nodes.
 */
template<typename rlElemType, typename valueType>
std::set<const RevBayesCore::DagNode*> ContainerNode<rlElemType, valueType>::getParents( void ) const
{
    std::set<const RevBayesCore::DagNode*> parents;

    for ( std::vector< RevPtr<Variable> >::const_iterator it = this->elements.begin(); it != this->elements.end(); ++it )
    {
        if ( !(*it)->isNAVar() )
            parents.insert( (*it)->getRevObject().getDagNode() );
    }
    
    return parents;
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
 */
template<typename rlElemType, typename valueType>
bool ContainerNode<rlElemType, valueType>::isConstant( void ) const
{
    // iterate over all parents and only if all parents are constant then this node is constant too
    for ( std::vector< RevPtr<Variable> >::const_iterator it = elements.begin(); it != elements.end(); ++it )
    {
        if ( !(*it)->getRevObject().isConstant() )
            return false;
    }
    
    return true;
}


/**
 * Keep the current value of the node. If we have been touched
 * but no one asked for our value, we just leave our touched flag
 * set, which should be safe. We do not want to set the touched
 * flag to false without calling update, as done in 
 * RevBayesCore::DeterministicNode.
 *
 * @todo Check whether behavior in RevBayesCore::DeterministicNode is
 *       correct, or if there is some subtle point I have missed -- FR
 */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::keepMe( RevBayesCore::DagNode* affecter )
{
    // We just pass the call on
    this->keepAffected();
}


/** Drop an element from the back */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::pop_back( void )
{
    // Detach our bond to the element DAG node
    RevBayesCore::DagNode* theElementNode = ( *elements.rbegin() )->getRevObject().getDagNode();
    if ( theElementNode != NULL )
    {
        theElementNode->removeChild( this );
        theElementNode->decrementReferenceCount();      // Still referenced by variable/RevObject so we need not delete it
    }

    // Now get rid of it
    elements.pop_back();
    
    // Tell everybody we have been changed
    this->touch();
}


/** Drop an element from the front */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::pop_front( void )
{
    // Detach our bond to the element DAG node
    RevBayesCore::DagNode* theElementNode = ( *elements.begin() )->getRevObject().getDagNode();
    if ( theElementNode != NULL )
    {
        theElementNode->removeChild( this );
        theElementNode->decrementReferenceCount();      // Still referenced by variable/RevObject so we need not delete it
    }
    
    // Now get rid of it
    elements.erase( elements.begin() );

    // Tell everybody we have been changed
    this->touch();
}


/**
 * Print struct for user.
 */
template< typename rlElemType, typename valueType >
void ContainerNode< rlElemType, valueType >::printStructureInfo( std::ostream& o ) const
{
    o << "_dagNode      = " << this->name << " <" << this << ">" << std::endl;
    o << "_dagType      = Container DAG node" << std::endl;
    o << "_refCount     = " << this->getReferenceCount() << std::endl;
    
    o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    
    o << "_parents      = ";
    this->printParents(o, 16, 70);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o, 16, 70);
    o << std::endl;
}


/** Push an element onto the back */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::push_back( rlElemType* x )
{
    // Connect the new element (beware of NA (NULL) objects)
    if ( x != NULL )
    {
        RevBayesCore::DagNode* theParent = x->getDagNode();
        if ( theParent != NULL )
        {
            theParent->addChild( this );
            theParent->incrementReferenceCount();
        }
    }
    
    // Guard the type of object put in the variable
    Variable* newVar = new Variable( x );
    newVar->setRevObjectTypeSpec( rlElemType::getClassTypeSpec() );
    
    // Name the new element
    std::ostringstream s;
    s << this->name;
    s << "[" << elements.size() + 1 << "]";
    newVar->setName( s.str() );
    
    // Push it onto the end of the vector
    elements.push_back( newVar );
    
    // Tell everybody we have been changed
    this->touch();
}


/** Push an element onto the front */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::push_front( rlElemType* x )
{
    // Connect the new element (beware of NA (NULL) objects)
    if ( x != NULL )
    {
        RevBayesCore::DagNode* theParent = x->getDagNode();
        if ( theParent != NULL )
        {
            theParent->addChild( this );
            theParent->incrementReferenceCount();
        }
    }

    // Guard the type of object put in the variable
    Variable* newVar = new Variable( x );
    newVar->setRevObjectTypeSpec( rlElemType::getClassTypeSpec() );
    
    // Push it onto the front of the vector
    elements.insert( elements.begin(), new Variable( x ) );
    
    // Rename all the elements because their names are now incorrect
    for ( size_t i = 0; i < elements.size(); ++i )
    {
        std::stringstream s;
        s << this->name;
        s << "[" << i + 1 << "]";
        elements[ i ]->setName( s.str() );
    }
    
    // Tell everybody we have been changed
    this->touch();
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
    // we probably need to recompute our value; this will clear any touched flags
    this->update();
    
    // dispatch call to others
    this->restoreAffected();
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
        std::stringstream s;
        s << this->name;
        s << "[" << i + 1 << "]";
        elements[ i ]->setName( s.str() );
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
 * Swap parent. We get this call just before an element is being replaced by another element.
 * We need to make sure we detach ourselves as a child of the old node and add ourselves as
 * a child to the new node. The actual replacement is managed by the Variable instance holding
 * the RevObject instance that is our element.
 *
 * Normally, this function is also used by cloneDAG, but we override cloneDAG here to use our own
 * mechanism for replacing parents (elements).
 */
template <typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::swapParent(const RevBayesCore::DagNode* oldParent, const RevBayesCore::DagNode* newParent)
{
    if ( oldParent != NULL )
    {
        oldParent->removeChild( this );
        oldParent->decrementReferenceCount();       // No need to delete the element; the Variable instance does that
    }

    if ( newParent != NULL )
    {
        newParent->addChild( this );
        newParent->incrementReferenceCount();
    }
    
    // Tell everybody we have been changed
    this->touch();
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
    {
        if ( (*it)->isNAVar() )
            throw RbException( "Illegal attempt to retrieve value of NA element" );

        value.push_back( static_cast<rlElemType&>( (*it)->getRevObject() ).getValue() );
    }
    
    // We are clean!
    touched = false;
}


#endif
