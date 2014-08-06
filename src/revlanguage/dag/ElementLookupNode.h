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

        typedef std::vector< const RevBayesCore::TypedDagNode<typename Natural::valueType>* >::iterator         indexIterator;
        typedef std::vector< const RevBayesCore::TypedDagNode<typename Natural::valueType>* >::const_iterator   const_indexIterator;
    
    public:
        ElementLookupNode(const std::string&                        n,
                          const rlType*                             var,
                          const std::vector< RevPtr<Variable> >&    ind );                                              //!< Basic constructor
        ElementLookupNode(const ElementLookupNode<rlType,rlElemType>& n);                                               //!< Copy constructor
        
        virtual                                 ~ElementLookupNode(void);                                               //!< Virtual destructor
        
        // Assignment operator
        ElementLookupNode&                      operator=(const ElementLookupNode& x);                                  //!< Assignment operator
        
        // Public methods
        ElementLookupNode<rlType,rlElemType>*   clone(void) const;                                                      //!< Type-safe clone
        double                                  getLnProbability(void) { return 0.0; }                                  //!< Get ln prob
        double                                  getLnProbabilityRatio(void) { return 0.0; }                             //!< Get ln prob ratio
        typename rlElemType::valueType&         getValue(void);                                                         //!< Get the value
        const typename rlElemType::valueType&   getValue(void) const;                                                   //!< Get the value (const)
        bool                                    isConstant(void) const;                                                 //!< Is this DAG node constant?
        virtual void                            printStructureInfo(std::ostream& o, bool verbose=false) const;          //!< Print structure info
        void                                    redraw(void) {}                                                         //!< Redraw (or not)
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
        rlType*                                         baseVariable;                                                   //!< The base variable
        std::vector< const RevBayesCore::TypedDagNode<typename Natural::valueType>* >    oneOffsetIndices;              //!< The indices
        bool                                            touched;                                                        //!< Are we dirty?
        typename rlElemType::valueType*                 value;                                                          //!< Current value
    };
    
}


#include "Natural.h"
#include "RevPtr.h"
#include "Variable.h"

using namespace RevLanguage;


/**
 * Basic constructor of element lookup. We need to add us as a child
 * of our parents.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>::ElementLookupNode( const std::string&                        n,
                                                          const rlType*                             var,
                                                          const std::vector< RevPtr<Variable> >&    ind ) :
    RevBayesCore::DynamicNode<typename rlElemType::valueType>( n ),
    baseVariable( var->clone() ),
    oneOffsetIndices(),
    touched( true ),
    value( NULL )
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Make sure we have a properly connected shallow copy of the base variable node
    RevBayesCore::DagNode* origVarNode = var->getDagNode();
    baseVariable->setDagNode( origVarNode );
    origVarNode->addChild( this);
    origVarNode->incrementReferenceCount();
    
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
 * Copy constructor. We make a semi-shallow copy here, that is,
 * the copies will have independent base variables but the base
 * variable objects will share the same internal DAG node. This
 * is necessary for the cloneDAG function to work properly.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>::ElementLookupNode( const ElementLookupNode<rlType, rlElemType>& n ) :
    RevBayesCore::DynamicNode<typename rlElemType::valueType>( n ),
    baseVariable( n.baseVariable->clone() ),
    oneOffsetIndices( n.oneOffsetIndices ),
    touched( true ),
    value( NULL )
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;

    // Make sure we have a properly connected shallow copy of the base variable node
    RevBayesCore::DagNode* copyVarNode = n.baseVariable->getDagNode();
    baseVariable->setDagNode( copyVarNode );
    copyVarNode->addChild( this);
    copyVarNode->incrementReferenceCount();

    // Add us as a child to the index variables
    for( indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        const RevBayesCore::TypedDagNode<typename Natural::valueType>* indexNode = (*it);
        indexNode->addChild( this );
        indexNode->incrementReferenceCount();
    }
}


/**
 * Destructor. We need to delete the base variable and the value. There
 * are also some DAG node management tasks to be performed here.
 */
template<typename rlType, typename rlElemType>
ElementLookupNode<rlType, rlElemType>::~ElementLookupNode( void )
{
    // Detach our bond to the variable DAG node
    RevBayesCore::DagNode* varNode = baseVariable->getDagNode();
    varNode->removeChild( this );
    
    // The baseVariable will take varNode with it when it dies,
    // so we need not try to delete it here
    varNode->decrementReferenceCount();

    // Detach our bond to the element indices
    for ( const_indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
    {
        const RevBayesCore::TypedDagNode<typename Natural::valueType>* indexNode = (*it);
        indexNode->removeChild( this );
        if ( indexNode->decrementReferenceCount() == 0 )
            delete indexNode;
    }
    
    // Delete the base variable and value
    delete baseVariable;
    delete value;
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
        RevBayesCore::DagNode* oldVarNode = baseVariable->getDagNode();
        oldVarNode->removeChild( this );
        
        // The baseVariable will take oldVarNode with it when it dies,
        // so we need not try to delete it here
        oldVarNode->decrementReferenceCount();
        
        // Detach our bond to the element indices
        for ( indexIterator it = oneOffsetIndices.begin(); it != oneOffsetIndices.end(); ++it )
        {
            const RevBayesCore::TypedDagNode<typename rlElemType::valueType>* indexNode = (*it);
            indexNode->removeChild( this );
            if ( indexNode->decrementReferenceCount() == 0 )
                delete indexNode;
        }
        
        // Delete the base variable
        delete baseVariable;

        // Now copy the baseVariable and indices
        baseVariable = x.baseVariable->clone();
        oneOffsetIndices = x.oneOffsetIndices;
        
        // Make sure we have a properly connected shallow copy of the new base variable node
        RevBayesCore::DagNode* newVarNode = x.baseVariable.getDagNode();
        baseVariable->setDagNode( newVarNode );
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
 * Get the affected nodes.
 * This call is started by the parent. We need to delegate this call to all our children.
 */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::getAffected( std::set<RevBayesCore::DagNode *>& affected, RevBayesCore::DagNode* affecter )
{
    this->getAffectedNodes( affected );
}


/**
 * Get the parents of this node. We simply return the baseVariable DAG node and
 * all the index DAG nodes.
 */
template<typename rlType, typename rlElemType>
std::set<const RevBayesCore::DagNode*> ElementLookupNode<rlType, rlElemType>::getParents( void ) const
{
    std::set<const RevBayesCore::DagNode*> parents;

    parents.insert( baseVariable->getDagNode() );

    for ( const_indexIterator it = this->oneOffsetIndices.begin(); it != this->oneOffsetIndices.end(); ++it )
        parents.insert( (*it) );
    
    return parents;
}


/** Get the value (non-const version) */
template<typename rlType, typename rlElemType>
typename rlElemType::valueType& ElementLookupNode<rlType, rlElemType>::getValue( void )
{
    if ( touched )
        update();
    
    return *value;
}


/**
 * Get the value (const version). Note that we need to type-cast here to get access to the
 * update function in case we have been touched and have not updated before. This is required
 * by the lazy evaluation mechanism.
 */
template<typename rlType, typename rlElemType>
const typename rlElemType::valueType& ElementLookupNode<rlType, rlElemType>::getValue( void ) const {
    
    if ( touched )
        const_cast<ElementLookupNode<rlType, rlElemType>*>( this )->update();
    
    return *value;
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
    
    // Check the base variable
    if ( !baseVariable->getDagNode()->isConstant() )
        return false;
    
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
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::keepMe( RevBayesCore::DagNode* affecter )
{
    // We just pass the call on
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
        o << "_dagNode      = " << this << ">" << std::endl;
    }
    o << "_dagType      = Element lookup DAG node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
    }
    
    o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    
    o << "_parents      = ";
    this->printParents(o, 16, 70);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o, 16, 70);
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
    // we probably need to recompute our value; this will clear any touched flags
    this->update();
    
    // dispatch call to others
    this->restoreAffected();
}


/** Touch this node for recalculation */
template<typename rlType, typename rlElemType>
void ElementLookupNode<rlType, rlElemType>::touchMe( RevBayesCore::DagNode *toucher )
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
 * Swap parent. We get this call just before the base variable is being replaced by another variable
 * and when the DAG is being cloned. We need to make sure we detach ourselves as a child of the old
 * node and add ourselves as a child to the new node.
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
        if ( oldParent != baseVariable->getDagNode() )
            throw RbException( "Invalid attempt to swap non-parent" );

        // TODO: Find a more elegant solution that avoids a const_cast
        baseVariable->setDagNode( const_cast< RevBayesCore::DagNode* >( newParent ) );
    }

    // Detach old parent and adopt new parent
    oldParent->removeChild( this );
    if( oldParent->decrementReferenceCount() == 0 )
        delete oldParent;

    newParent->addChild( this );
    newParent->incrementReferenceCount();

    // Tell everybody we have been changed
    this->touch();
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
    
    RevPtr<Variable> theElement = baseVariable->getElement( theOneOffsetIndices );
    
    value = RevBayesCore::Cloner<typename rlElemType::valueType, IsDerivedFrom<typename rlElemType::valueType, RevBayesCore::Cloneable>::Is >::createClone( static_cast<rlElemType&>( theElement->getRevObject() ).getValue() );

    // We are clean!
    touched = false;
}


#endif
