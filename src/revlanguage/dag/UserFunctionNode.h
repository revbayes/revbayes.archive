#ifndef UserFunctionNode_H
#define UserFunctionNode_H

#include "DynamicNode.h"
#include "RevPtr.h"

#include <vector>

namespace RevLanguage {
    
    /**
     * @brief DAG node for user-defined Rev functions
     *
     * This DAG node class is used for deterministic nodes, where the value is
     * controlled by a user-defined function written in Rev. Unlike other
     * deterministic nodes used in the program
     *
     *    a := b
     *
     * where a is supposed to change value dynamically as the value of b changes.
     *
     * The single parent node of the UserFunctionNode is simply the DAG node
     * inside variable b in the statement above.
     */
    template<typename rlType>
    class UserFunctionNode : public RevBayesCore::DynamicNode<typename rlType::valueType> {
        
    public:
        UserFunctionNode(const std::string& n, RevBayesCore::TypedDagNode<typename rlType::valueType>* var);       //!< Basic constructor
        UserFunctionNode(const UserFunctionNode<rlType>& n);                                                  //!< Copy constructor
        
        virtual                                 ~UserFunctionNode(void);                                           //!< Virtual destructor
        
        // Assignment operator
        UserFunctionNode&                  operator=(const UserFunctionNode& x);                              //!< Assignment operator
        
        // Public methods
        UserFunctionNode<rlType>*          clone(void) const;                                                      //!< Type-safe clone
        double                                  getLnProbability(void) { return 0.0; }                                  //!< Get ln prob
        double                                  getLnProbabilityRatio(void) { return 0.0; }                             //!< Get ln prob ratio
        typename rlType::valueType&             getValue(void);                                                         //!< Get the value
        const typename rlType::valueType&       getValue(void) const;                                                   //!< Get the value (const)
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
        const RevBayesCore::TypedDagNode<typename rlType::valueType>*   referencedNode;                                 //!< DAG node of referenced variable
        typename rlType::valueType*                                     value;                                          //!< Current value
    };
    
}


#include "Cloner.h"
#include "Cloneable.h"
#include "IsDerivedFrom.h"
#include "RevPtr.h"
#include "Variable.h"

using namespace RevLanguage;


/**
 * Basic constructor of indirect reference. We need to add us as a child
 * of the referenced variable.
 */
template<typename rlType>
UserFunctionNode<rlType>::UserFunctionNode( const std::string& n, RevBayesCore::TypedDagNode<typename rlType::valueType>* var ) :
RevBayesCore::DynamicNode<typename rlType::valueType>( n ),
referencedNode( var ),
value( NULL )
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Adopt our parent
    referencedNode->addChild( this );
    referencedNode->incrementReferenceCount();
}


/**
 * Copy constructor. Make sure to adopt our parent,
 * otherwise not much to do.
 */
template<typename rlType>
UserFunctionNode<rlType>::UserFunctionNode( const UserFunctionNode<rlType>& n ) :
RevBayesCore::DynamicNode<typename rlType::valueType>( n ),
referencedNode( n.referencedNode ),
value( NULL )
{
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Attach ourselves to the referenced node
    referencedNode->addChild( this );
    referencedNode->incrementReferenceCount();
}


/**
 * Destructor. We need to delete the value here and there are
 * also some DAG node management tasks to be performed.
 */
template<typename rlType>
UserFunctionNode<rlType>::~UserFunctionNode( void )
{
    // Detach our bond to the referenced node
    referencedNode->removeChild( this );
    if ( referencedNode->decrementReferenceCount() == 0 )
        delete referencedNode;
    
    // Delete our internal value
    delete value;
}


/**
 * Assignment operator. We make sure we detach ourselves from
 * the old referenced node and attach ourselves to the new one.
 * There is no need to update the value, as long as we mark
 * ourselves as dirty.
 */
template<typename rlType>
UserFunctionNode<rlType>& UserFunctionNode<rlType>::operator=( const UserFunctionNode<rlType>& x )
{
    if (this != &x)
    {
        RevBayesCore::DynamicNode<typename rlType::valueType>::operator=( x );
        
        // Detach our bond to the new referenced node
        referencedNode->removeChild( this );
        if ( referencedNode->decrementReferenceCount() == 0 )
            delete referencedNode;
        
        // Copy the referenced node
        referencedNode = x.referencedNode;
        
        // Attach ourselves to the new referenced node
        referencedNode->addChild( this );
        referencedNode->incrementReferenceCount();
        
        // Tell everybody we have been changed
        this->touch();
    }
    
    return *this;
}


/** Type-safe clone function */
template<typename rlType>
UserFunctionNode<rlType>* UserFunctionNode<rlType>::clone( void ) const
{
    return new UserFunctionNode<rlType>( *this );
}


/**
 * Get the affected nodes.
 * This call is started by the parent. We need to delegate this call to all our children.
 */
template<typename rlType>
void UserFunctionNode<rlType>::getAffected( std::set<RevBayesCore::DagNode *>& affected, RevBayesCore::DagNode* affecter )
{
    this->getAffectedNodes( affected );
}


/**
 * Get the parents of this node. We simply return the referenced node
 * in a set with one element.
 */
template<typename rlType>
std::set<const RevBayesCore::DagNode*> UserFunctionNode<rlType>::getParents( void ) const
{
    std::set<const RevBayesCore::DagNode*> parents;
    
    parents.insert( referencedNode );
    
    return parents;
}


/** Get the value (non-const version) */
template<typename rlType>
typename rlType::valueType& UserFunctionNode<rlType>::getValue( void )
{
    if ( this->touched )
        update();
    
    return *value;
}


/**
 * Get the value (const version). Note that we need to type-cast here to get access to the
 * update function in case we have been touched and have not updated before. This is required
 * by the lazy evaluation mechanism.
 */
template<typename rlType>
const typename rlType::valueType& UserFunctionNode<rlType>::getValue( void ) const {
    
    if ( this->touched )
        const_cast<UserFunctionNode<rlType>*>( this )->update();
    
    return *value;
}



/**
 * Is this node constant? We just ask the referenced
 * node if it is constant.
 */
template<typename rlType>
bool UserFunctionNode<rlType>::isConstant( void ) const
{
    return referencedNode->isConstant();
}


/**
 * Keep the current value of the node.
 *
 * @note We cannot change the touched flag, because if we have been
 *       touched and not updated, we need to stay touched until
 *       someone asks for our value.
 */
template<typename rlType>
void UserFunctionNode<rlType>::keepMe( RevBayesCore::DagNode* affecter )
{
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In keepMe of indirect reference node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // Pass the call on
    this->keepAffected();
}


/**
 * Print struct for user. Because of lazy evaluation we need to make a
 * const cast, but this should be perfectly safe.
 */
template< typename rlType >
void UserFunctionNode<rlType>::printStructureInfo( std::ostream& o, bool verbose ) const
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
    
    o << "_dagType      = Indirect reference DAG node" << std::endl;
    
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
template<typename rlType>
void UserFunctionNode<rlType>::restoreMe( RevBayesCore::DagNode *restorer )
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In restoreMe of indirect reference node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // we probably need to recompute our value; this will clear any touched flags
    this->update();
    
    // dispatch call to others
    this->restoreAffected();
}


/**
 * Touch this node for recalculation.
 *
 * @todo Can we test here for being touched and only pass the call
 *       on if we are not touched? It is not safe in DeterministicNode
 *       so we always pass the call on here, to be safe.
 */
template<typename rlType>
void UserFunctionNode<rlType>::touchMe( RevBayesCore::DagNode *toucher )
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In touchMe of container node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // Touch myself
    this->touched = true;
    
    // Dispatch the touch message to downstream nodes
    this->touchAffected();
}


/**
 * Swap parent. We get this call just before the base variable is being replaced by another variable.
 * We need to make sure we detach ourselves as a child of the old node and add ourselves as
 * a child to the new node.
 *
 * We throw an error if the old parent cannot be found.
 */
template <typename rlType>
void UserFunctionNode<rlType>::swapParent(const RevBayesCore::DagNode* oldParent, const RevBayesCore::DagNode* newParent)
{
    // Check whether we have the old parent node
    if ( oldParent != referencedNode )
        throw RbException( "Invalid attempt to swap non-parent" );
    
    // Replace the referenced node with new parent
    referencedNode = static_cast< const RevBayesCore::TypedDagNode<typename rlType::valueType>* >( newParent );
    
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
 * that an update is really needed. We need to work
 * with pointers to be able to hande abstract value
 * types.
 */
template<typename rlType>
void UserFunctionNode<rlType>::update()
{
    delete value;
    value = RevBayesCore::Cloner<typename rlType::valueType, IsDerivedFrom<typename rlType::valueType, RevBayesCore::Cloneable>::Is >::createClone( referencedNode->getValue() );
    
    // We are clean!
    this->touched = false;
}


#endif
