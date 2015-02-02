#ifndef ElementLookupNode_H
#define ElementLookupNode_H

#include "Natural.h"
#include "DynamicNode.h"
#include "RevPtr.h"

#include <vector>

namespace RevLanguage {
    
    /**
     * @brief DAG node for element lookups
     *
     * This DAG node class is used for dynamic Rev lookups of elements. Such
     * dynamic lookups are created by statements like:
     *
     *    a := b[ i ]
     *
     * where a is supposed to change value dynamically as the value of i changes.
     *
     * The parent nodes of the ElementLookupNode simply correspond to the container
     * variable (or variable supporting the subscript operator), and the variables
     * producing the indices to the element.
     */
    template<typename rlType, typename rlElemType>
    class ElementLookupNode : public RevBayesCore::DynamicNode<typename rlElemType::valueType> {

        typedef std::vector< const RevBayesCore::TypedDagNode<typename Natural::valueType>* >   IndexVector;
        typedef IndexVector::iterator                                                           indexIterator;
        typedef IndexVector::const_iterator                                                     const_indexIterator;
    
    public:
        ElementLookupNode(const std::string&                        n,
                          const RevPtr<Variable>&                   var,
                          const std::vector< RevPtr<Variable> >&    ind );                                              //!< Basic constructor
        ElementLookupNode(const ElementLookupNode<rlType,rlElemType>& n);                                               //!< Copy constructor
        
        virtual                                 ~ElementLookupNode(void);                                               //!< Virtual destructor
        
        // Assignment operator
        ElementLookupNode&                      operator=(const ElementLookupNode& x);                                  //!< Assignment operator
        
        // Public methods
        ElementLookupNode<rlType,rlElemType>*   clone(void) const;                                                      //!< Type-safe clone
        RevBayesCore::DagNode*                  cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*> &nodesMap) const;   //!< Clone the entire DAG connected to this node
        typename rlElemType::valueType&         getValue(void);                                                         //!< Get the value
        const typename rlElemType::valueType&   getValue(void) const;                                                   //!< Get the value (const)
        bool                                    isConstant(void) const;                                                 //!< Is this DAG node constant?
        virtual void                            printStructureInfo(std::ostream& o, bool verbose=false) const;          //!< Print structure info
        void                                    update(void);                                                           //!< Update current value
        
        // Parent DAG nodes management functions
        std::set<const RevBayesCore::DagNode*>  getParents(void) const;                                                                     //!< Get the set of parents
        void                                    swapParent(const RevBayesCore::DagNode *oldParent, const RevBayesCore::DagNode *newParent); //!< Exchange the parent (element variable)
        
    protected:
        void                                    keepMe(RevBayesCore::DagNode* affecter);                                                    //!< Keep value of this and affected nodes
        void                                    restoreMe(RevBayesCore::DagNode *restorer);                                                 //!< Restore value of this nodes
        void                                    touchMe(RevBayesCore::DagNode *toucher);                                                    //!< Touch myself and tell affected nodes value is reset
        
    private:
        RevPtr<Variable>                        variable;                                           //!< The base variable
        IndexVector                             oneOffsetIndices;                                   //!< The indices
        RevPtr<Variable>                        element;                                            //!< Current element
    };
    
}


#include "Variable.h"

using namespace RevLanguage;


/**
 * Basic constructor of element lookup. We need to add us as a child
 * of our parents.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>::ElementLookupNode( const std::string&                        n,
                                                          const RevPtr<Variable>&                   var,
                                                          const std::vector< RevPtr<Variable> >&    ind ) :
    RevBayesCore::DynamicNode<typename rlElemType::valueType>( n ),
    variable( var ),
    oneOffsetIndices(),
    element( NULL )
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Add us as a child to the variable DAG node
    RevBayesCore::DagNode* varNode = variable->getRevObject().getDagNode();
    varNode->addChild( this);
    varNode->incrementReferenceCount();
    
    // Retrieve the element index nodes and add us as a child to them
    for ( std::vector< RevPtr<Variable> >::const_iterator it = ind.begin(); it != ind.end(); ++it )
    {
        const RevBayesCore::TypedDagNode<typename Natural::valueType>* indexNode;
        indexNode = static_cast< RevBayesCore::TypedDagNode<typename Natural::valueType>* >( (*it)->getRevObject().getDagNode() );
        indexNode->addChild( this );
        indexNode->incrementReferenceCount();
        oneOffsetIndices.push_back( indexNode );
    }
}


/**
 * Copy constructor. The two copies will point to the same
 * variable and use the same index nodes.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>::ElementLookupNode( const ElementLookupNode<rlType, rlElemType>& n ) :
    RevBayesCore::DynamicNode<typename rlElemType::valueType>( n ),
    variable( n.variable ),
    oneOffsetIndices( n.oneOffsetIndices ),
    element( NULL )
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;

    // Add us as a child to the variable DAG node
    RevBayesCore::DagNode* varNode = variable->getRevObject().getDagNode();
    varNode->addChild( this);
    varNode->incrementReferenceCount();

    // Add us as a child to the index variables
    for( indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        const RevBayesCore::TypedDagNode<typename Natural::valueType>* indexNode = (*it);
        indexNode->addChild( this );
        indexNode->incrementReferenceCount();
    }
}


/**
 * Destructor. We need to delete the value. There are also
 * some DAG node management tasks to be performed here.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>::~ElementLookupNode( void )
{
    // Detach our bond to the variable DAG node
    RevBayesCore::DagNode* varNode = variable->getRevObject().getDagNode();
    varNode->removeChild( this );
    
    // We adhere to standard even though reference count should not be 0 here
    if ( varNode->decrementReferenceCount() == 0 )
        delete varNode;

    // Detach our bond to the element indices
    for ( const_indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        const RevBayesCore::TypedDagNode<typename Natural::valueType>* indexNode = (*it);
        indexNode->removeChild( this );
        if ( indexNode->decrementReferenceCount() == 0 )
            delete indexNode;
    }
    
    // The variable and the element will delete themselves if needed (smart pointers)
}


/**
 * Assignment operator. Important parent DAG node management issues
 * to be dealt with here. We need to make sure we detach ourselves
 * from the old base variable and index variables. We also need to 
 * ensure that we have a semi-shallow copy of the new base variable.
 *
 * There is no need to delete or update our value, as long as we mark
 * ourselves as dirty.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>& ElementLookupNode<rlType, rlElemType>::operator=( const ElementLookupNode<rlType, rlElemType>& x )
{
    if (this != &x)
    {
        RevBayesCore::DynamicNode<typename rlType::valueType>::operator=( x );
        
        // Detach our bond to the old variable DAG node
        RevBayesCore::DagNode* oldVarNode = variable->getRevObject().getDagNode();
        oldVarNode->removeChild( this );
        
        // We adhere to standard even though reference count should not be 0 here
        if ( oldVarNode->decrementReferenceCount() == 0 )
            delete oldVarNode;
        
        // Detach our bond to the element indices
        for ( indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
        {
            const RevBayesCore::TypedDagNode<typename rlElemType::valueType>* indexNode = (*it);
            indexNode->removeChild( this );
            if ( indexNode->decrementReferenceCount() == 0 )
                delete indexNode;
        }
        
        // Now copy the baseVariable and indices
        variable = x.variable;
        oneOffsetIndices = x.oneOffsetIndices;
        
        // Add us as a child to the variable DAG node
        RevBayesCore::DagNode* newVarNode = variable->getRevObject().getDagNode();
        newVarNode->addChild( this);
        newVarNode->incrementReferenceCount();
        
        // Add us as a child to the index variables
        for( indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
        {
            const RevBayesCore::DagNode* indexNode = (*it);
            indexNode->addChild( this );
            indexNode->incrementReferenceCount();
        }

        // Tell everybody we have been changed (and mark ourselves as dirty)
        this->touch();
    }
    
    return *this;
}


/** Type-safe clone function */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>* ElementLookupNode<rlType, rlElemType>::clone( void ) const
{
    return new ElementLookupNode<rlType, rlElemType>( *this );
}


/**
 * Clone the entire graph: clone children, swap parents. Because the variable DAG node is
 * doubly managed by us and by the variable wrapper around the variable object, we need
 * a special version of the cloneDAG function. Otherwise we cannot distinguish between the
 * call to swapParent from cloneDAG (where we need to replace the DAG node inside the
 * variable) and the call from the variable wrapper (where we simply should update the
 * connections, and the variable wrapper replaces the DAG node inside it).
 *
 * @todo For this to be perfectly safe, we need all DAG nodes to be connected to their
 *       Rev object variables. The hack below (two variables sharing the same DAG node)
 *       should be safe for the current implementation of model, however, which does
 *       not expose its model variables to the parser.
 */
template<typename rlType, typename rlElemType>
RevBayesCore::DagNode* ElementLookupNode<rlType, rlElemType>::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode* >& newNodes ) const
{
    // Return our clone if we have already been cloned
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // We avoid the conflict between having to make a deep copy in our clone
    // function and the need to have a shallow copy (identical parents) in
    // this function by replacing the variable using special code
    ElementLookupNode<rlType, rlElemType>* copy = this->clone();
    
    // Add this node and its copy to the map
    newNodes[ this ] = copy;
    
    // First swap variable using non-standard code
    
    // Swap argument variable using non-standard code
    
    // Get our variable DAG node
    const RevBayesCore::DagNode *theVariableNode = variable->getRevObject().getDagNode();
    
    // Remove the copy as a child to our variable DAG node so the cloning works
    theVariableNode->removeChild( copy );
    
    // Make sure the copy has its own Rev object variable with its DAG node being the clone of our variable DAG node
    copy->variable = new Variable( variable->getRevObject().cloneDAG( newNodes ), variable->getName() );
    
    // Now swap copy parents: detach the copy node from its old parent and attach it to the new parent
    // If we called swapParent here, the swapParent function would not find the old parent in its RevObject
    // variable wrapper because that wrapper has already been exchanged above. Note that we already removed
    // the copy as a child of the variable DAG node above. Just to conform to the pattern we check for
    // the need to delete even though the test should never be true here.
    if ( theVariableNode->decrementReferenceCount() == 0 )
        delete theVariableNode;
    RevBayesCore::DagNode* theVariableNodeClone = copy->variable->getRevObject().getDagNode();
    theVariableNodeClone->addChild( copy );
    theVariableNodeClone->incrementReferenceCount();
    
    // We use the standard code (below) for the index nodes
    
    // We need to remove the copy as a child of our index nodes in order to stop recursive calls to
    // cloneDAG on our copy, its copy, etc, when we call cloneDAG on our index nodes
    for ( const_indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        (*it)->removeChild( copy );
    }
    
    // Now replace the index nodes of the copy (which are now the same as our parents) with the index node clones
    for ( const_indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        // Get the i-th index node
        const RevBayesCore::DagNode *theIndexNode = (*it);
        
        // Get its clone if we already have cloned this parent (parameter), then we will get the previously created clone
        RevBayesCore::DagNode* theIndexNodeClone = theIndexNode->cloneDAG( newNodes );
        
        // Add the copy back as a child of this index node so that the swapping works
        theIndexNode->addChild( copy );
        
        // Swap the copy parent with its clone. This will remove the copy again as the child of our index node.
        copy->swapParent( theIndexNode, theIndexNodeClone);
    }
    
    /* Make sure the children clone themselves */
    for( std::set<RevBayesCore::DagNode*>::const_iterator it = this->children.begin(); it != this->children.end(); ++it )
        (*it)->cloneDAG( newNodes );
    
    return copy;
}


/**
 * Get the parents of this node. We simply return the variable DAG node and
 * all the index DAG nodes.
 */
template<typename rlType, typename rlElemType>
std::set<const RevBayesCore::DagNode*> ElementLookupNode<rlType, rlElemType>::getParents( void ) const
{
    std::set<const RevBayesCore::DagNode*> parents;

    parents.insert( variable->getRevObject().getDagNode() );

    for ( const_indexIterator it = this->oneOffsetIndices.begin(); it != this->oneOffsetIndices.end(); ++it )
        parents.insert( (*it) );
    
    return parents;
}


/** Get the value (non-const version) */
template<typename rlType, typename rlElemType>
typename rlElemType::valueType& ElementLookupNode<rlType, rlElemType>::getValue( void )
{
    if ( this->touched )
        update();
    
    return static_cast< RevBayesCore::TypedDagNode<typename rlElemType::valueType>* >( element->getRevObject().getDagNode() )->getValue();
}


/**
 * Get the value (const version). Note that we need to type-cast here to get access to the
 * update function in case we have been touched and have not updated before. This is required
 * by the lazy evaluation mechanism.
 */
template<typename rlType, typename rlElemType>
const typename rlElemType::valueType& ElementLookupNode<rlType, rlElemType>::getValue( void ) const
{
    if ( this->touched )
        const_cast<ElementLookupNode<rlType, rlElemType>*>( this )->update();
    
    return static_cast< RevBayesCore::TypedDagNode<typename rlElemType::valueType>* >( element->getRevObject().getDagNode() )->getValue();
}



/**
 * Is this node constant? Only if all parents are constant.
 */
template<typename rlType, typename rlElemType>
bool ElementLookupNode<rlType, rlElemType>::isConstant( void ) const
{
    // Check the indices
    for ( const_indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        if ( !(*it)->isConstant() )
            return false;
    }
    
    // Check the variable
    if ( !variable->getRevObject().getDagNode()->isConstant() )
        return false;
    
    return true;
}


/**
 * Keep the current value of the node.
 *
 * @note We cannot change the touched flag, because if we have been
 *       touched and not updated, we need to stay touched until
 *       someone asks for our value.
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::keepMe( RevBayesCore::DagNode* affecter )
{
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In keepMe of element lookup node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // Pass the call on
    this->keepAffected();
}


/**
 * Print struct for user. Because of lazy evaluation we need to make a
 * const cast, but this should be perfectly safe.
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::printStructureInfo( std::ostream& o, bool verbose ) const
{
    
    if ( verbose == true )
    {
        o << "_dagNode      = " << this->name << " <" << this << ">" << std::endl;
    }
    else
    {
        if ( this->name != "")
            o << "_dagNode      = " << this->name << std::endl;
        else
            o << "_dagNode      = <" << this << ">" << std::endl;
    }
    o << "_dagType      = Element lookup DAG node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
    }
    
    o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    
    o << "_parents      = ";
    this->printParents( o, 16, 70, verbose );
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren( o, 16, 70, verbose );
    o << std::endl;
}


/**
 * Restore the old value of the node and tell affected. We simply copy behavior
 * in DeterministicNode. However, it is doubtful that this call is needed.
 *
 * @todo Is this call really needed?
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::restoreMe( RevBayesCore::DagNode *restorer )
{

#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In restoreMe of element lookup node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // we probably need to recompute our value; this will clear any touched flags
    this->update();
    
    // dispatch call to others
    this->restoreAffected();
}


/**
 * Swap parent. We get this call just before the variable object is being replaced by another variable
 * object and when the DAG is being cloned. In the latter algorithm, the swapParent function is not
 * called (see comments for this function).
 *
 * We throw an error if the old parent cannot be found.
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::swapParent(const RevBayesCore::DagNode* oldParent, const RevBayesCore::DagNode* newParent)
{
    // Find old parent and replace it
    indexIterator it;
    for( it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        if ( (*it) == oldParent )
        {
            (*it) = static_cast< const RevBayesCore::TypedDagNode<typename Natural::valueType>* >( newParent );

            break;
        }
    }

    if ( it == oneOffsetIndices.end() )
    {
        if ( oldParent != variable->getRevObject().getDagNode() )
            throw RbException( "Invalid attempt to swap non-parent" );
    }

    // Detach old parent and delete it if needed
    oldParent->removeChild( this );
    if( oldParent->decrementReferenceCount() == 0 )
        delete oldParent;
    
    // Adopt new parent
    newParent->addChild( this );
    newParent->incrementReferenceCount();
    
    // Tell everybody we have been changed
    this->touch();
}


/**
 * Touch this node for recalculation.
 *
 * @todo Can we test here for being touched and only pass the call
 *       on if we are not touched? It is not safe in DeterministicNode
 *       so we always pass the call on here, to be safe.
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::touchMe( RevBayesCore::DagNode *toucher )
{
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In touchMe of element lookup node " << this->getName() << " <" << this << ">" << std::endl;
#endif

    // Touch myself
    this->touched = true;
    
    // Dispatch the touch message to downstream nodes
    this->touchAffected();
}


/**
 * Update our value. We assume that the caller knows
 * that an update is really needed.
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::update()
{
    // Compute indices
    std::vector<size_t> theOneOffsetIndices;
    for( indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
        theOneOffsetIndices.push_back( size_t( (*it)->getValue() ) );
    
    // Get the element
    element = variable->getRevObject().getElement( theOneOffsetIndices );
    
    // We are clean!
    this->touched = false;
}


#endif
