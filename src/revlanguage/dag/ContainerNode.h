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

        // Assignment operator
        ContainerNode&                          operator=(const ContainerNode& x);                                      //!< Assignment operator

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
    {
        if ( !elems[i]->isTypeSpec( rlElemType::getClassTypeSpec() ) )
            throw RbException( "Invalid attempt to set container element of type '" + rlElemType::getClassType() + "' with object of type '" + elems[i]->getClassType() + "'" );
        this->push_back( static_cast<rlElemType*>( elems[i] ) );
    }
}


/**
 * Copy constructor. We cannot make a deep copy of the variable elements here.
 * Instead, we have to wait until we get the swapParent call. However, we do
 * need to add ourselves as an extra child of the template container elements,
 * since we share those to start with at least.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::ContainerNode( const ContainerNode<rlElemType, valueType>& n ) :
    RevBayesCore::DynamicNode<valueType>( n ),
    elements( n.elements ),
    touched( true ),
    value()
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    for ( std::vector< RevPtr<Variable> >::iterator it = elements.begin(); it != elements.end(); ++it )
    {
        RevBayesCore::DagNode* theParent = (*it)->getRevObject().getDagNode();
        theParent->addChild( this );
        theParent->incrementReferenceCount();
    }
}


/**
 * Destructor. No need to delete variable elements here because the variable elements are smart pointers.
 * However, we need to remove us as a child of the variable element DAG nodes (our parents). The variable
 * elements still reference our parents, so we do not have to delete them here. By decrementing their
 * reference count, we ensure they die with our elements, if they need to.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>::~ContainerNode( void )
{
    for ( std::vector< RevPtr<Variable> >::iterator it = elements.begin(); it != elements.end(); ++it )
    {
        RevBayesCore::DagNode* theParent = (*it)->getRevObject().getDagNode();
        theParent->removeChild( this );
        theParent->decrementReferenceCount();
    }
}


/**
 * Assignment operator. We cannot make a deep copy of the elements here.
 * Instead we have to wait for the swapParent call.
 */
template<typename rlElemType, typename valueType>
ContainerNode<rlElemType, valueType>& ContainerNode<rlElemType, valueType>::operator=( const ContainerNode<rlElemType, valueType>& x )
{
    if (this != &x)
    {
        RevBayesCore::DynamicNode<valueType>::operator=( x );

        this->type = RevBayesCore::DagNode::DETERMINISTIC;
        
        // Release the old parents
        for ( std::vector< RevPtr<Variable> >::iterator it = elements.begin(); it != elements.end(); ++it )
        {
            RevBayesCore::DagNode* theParent = (*it)->getRevObject().getDagNode();
            theParent->removeChild( this );
            theParent->decrementReferenceCount();
        }

        // Get the new parents
        elements = x.elements;

        // Adopt the new parents
        for ( std::vector< RevPtr<Variable> >::iterator it = elements.begin(); it != elements.end(); ++it )
        {
            RevBayesCore::DagNode* theParent = (*it)->getRevObject().getDagNode();
            theParent->addChild( this );
            theParent->incrementReferenceCount();
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
 * The user can even replace the RevObject inside the Variable instance.
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


/**
 * Get the parents of this node. We simply walk through the elements and return
 * a set with their DAG nodes.
 */
template<typename rlElemType, typename valueType>
std::set<const RevBayesCore::DagNode*> ContainerNode<rlElemType, valueType>::getParents( void ) const
{
    std::set<const RevBayesCore::DagNode*> parents;

    for ( std::vector< RevPtr<Variable> >::const_iterator it = this->elements.begin(); it != this->elements.end(); ++it )
        parents.insert( (*it)->getRevObject().getDagNode() );
    
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
 * but noone asked for our value, we just leave our touched flag
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
    theElementNode->removeChild( this );
    theElementNode->decrementReferenceCount();      // Still referenced by variable/RevObject so we need not delete it

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
    theElementNode->removeChild( this );
    theElementNode->decrementReferenceCount();      // Still referenced by variable/RevObject so we need not delete it
    
    // Now get rid of it
    elements.erase( elements.begin() );

    // Tell everybody we have been changed
    this->touch();
}


/**
 * Print struct for user. Because of lazy evaluation we need to make a
 * const cast, but this should be perfectly safe.
 */
template< typename rlElemType, typename valueType >
void ContainerNode< rlElemType, valueType >::printStructureInfo( std::ostream& o ) const
{
    o << "_address      = " << this->name << " <" << this << ">" << std::endl;
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


/** Push an element onto the back */
template<typename rlElemType, typename valueType>
void ContainerNode<rlElemType, valueType>::push_back( rlElemType* x )
{
    // Connect the new element
    RevBayesCore::DagNode* theParent = x->getDagNode();
    theParent->addChild( this );
    theParent->incrementReferenceCount();
    
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
    // Connect the new element
    RevBayesCore::DagNode* theParent = x->getDagNode();
    theParent->addChild( this );
    theParent->incrementReferenceCount();
    
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
        value.push_back( static_cast<rlElemType&>( (*it)->getRevObject() ).getValue() );
    
    // We are clean!
    touched = false;
}


#endif
