#ifndef ConverterNode_H
#define ConverterNode_H

#include "DynamicNode.h"
#include "RevPtr.h"

#include <vector>

namespace RevLanguage {
    
    /**
     * @brief DAG node for type conversions
     *
     * This DAG node class is used for dynamic Rev type conversions. Such
     * dynamic type conversions are created by statements like:
     *
     *    a := foo( b )
     *
     * where b needs type conversion to fit as an argument to foo.
     *
     * The parent node of the ConverterNode is the argument to the function. It is
     * kept as a variable inside the ConverterNode, and it is used to update the
     * converted value. The rlType template parameter is the Rev type of the converted
     * object.
     */
    template<typename rlType>
    class ConverterNode : public RevBayesCore::DynamicNode<typename rlType::valueType> {
        
    public:
        ConverterNode(const std::string& n, const RevPtr<Variable>& arg, const TypeSpec& ts );                          //!< Basic constructor
        ConverterNode(const ConverterNode<rlType>& n);                                                                  //!< Copy constructor
        
        virtual                                 ~ConverterNode(void);                                                   //!< Virtual destructor
        
        // Assignment operator
        ConverterNode&                          operator=(const ConverterNode& x);                                      //!< Assignment operator
        
        // Public methods
        ConverterNode<rlType>*                  clone(void) const;                                                      //!< Type-safe clone
        RevBayesCore::DagNode*                  cloneDAG(std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*> &nodesMap) const;   //!< Clone the entire DAG connected to this node
        typename rlType::valueType&             getValue(void);                                                         //!< Get the value
        const typename rlType::valueType&       getValue(void) const;                                                   //!< Get the value (const)
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
        RevPtr<Variable>                        argument;                                           //!< The argument variable
        rlType*                                 convertedObject;                                    //!< The converted object
        const TypeSpec&                         typeSpec;                                           //!< Type specification to convert to
        
    };
    
}


#include "Variable.h"

using namespace RevLanguage;


/**
 * Basic constructor of dynamic conversion node. We need to add us as a child
 * of the argument.
 */
template<typename rlType>
ConverterNode<rlType>::ConverterNode( const std::string& n, const RevPtr<Variable>& arg, const TypeSpec& ts ) :
    RevBayesCore::DynamicNode<typename rlType::valueType>( n ),
    argument( arg ),
    convertedObject( NULL ),
    typeSpec( ts )
{
    this->hidden = true;
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Add us as a child to the argument DAG node
    RevBayesCore::DagNode* argNode = argument->getRevObject().getDagNode();
    argNode->addChild( this);
    argNode->incrementReferenceCount();
}


/**
 * Copy constructor. The two copies will point to the same
 * argument variable.
 */
template<typename rlType>
ConverterNode<rlType>::ConverterNode( const ConverterNode<rlType>& n ) :
    RevBayesCore::DynamicNode<typename rlType::valueType>( n ),
    argument( n.argument ),
    convertedObject( NULL ),
    typeSpec( n.typeSpec )
{
    this->hidden = true;
    this->type = RevBayesCore::DagNode::DETERMINISTIC;
    
    // Add us as a child to the argument DAG node
    RevBayesCore::DagNode* argNode = argument->getRevObject().getDagNode();
    argNode->addChild( this);
    argNode->incrementReferenceCount();
}


/**
 * Destructor. We need to delete the value. There are also
 * some DAG node management tasks to be performed here.
 */
template<typename rlType>
ConverterNode<rlType>::~ConverterNode( void )
{
    // Detach our bond to the argument DAG node
    RevBayesCore::DagNode* argNode = argument->getRevObject().getDagNode();
    argNode->removeChild( this );
    
    // We adhere to standard even though reference count should not be 0 here
    if ( argNode->decrementReferenceCount() == 0 )
        delete argNode;
    
    // Delete the value. The argument will delete itself (smart pointer)
    delete convertedObject;
}


/**
 * Assignment operator. Important parent DAG node management issues
 * to be dealt with here. We need to make sure we detach ourselves
 * from the old argument attach ourselves to the new argument.
 *
 * There is no need to delete or update our value, as long as we mark
 * ourselves as dirty.
 */
template<typename rlType>
ConverterNode<rlType>& ConverterNode<rlType>::operator=( const ConverterNode<rlType>& x )
{
    if (this != &x)
    {
        if ( typeSpec != x.typeSpec )
            throw RbException( "Invalid assignment involving converter nodes with different types" );
        
        RevBayesCore::DynamicNode<typename rlType::valueType>::operator=( x );
        
        // Detach our bond to the old argument DAG node
        RevBayesCore::DagNode* oldArgNode = argument->getRevObject().getDagNode();
        oldArgNode->removeChild( this );
        
        // We adhere to standard even though reference count should not be 0 here
        if ( oldArgNode->decrementReferenceCount() == 0 )
            delete oldArgNode;
        
        // Now copy the argument variable
        argument = x.argument;
        
        // Add us as a child to the new argument DAG node
        RevBayesCore::DagNode* newArgNode = argument->getRevObject().getDagNode();
        newArgNode->addChild( this);
        newArgNode->incrementReferenceCount();
        
        // Tell everybody we have been changed (and mark ourselves as dirty)
        this->touch();
    }
    
    return *this;
}


/** Type-safe clone function */
template<typename rlType>
ConverterNode<rlType>* ConverterNode<rlType>::clone( void ) const
{
    return new ConverterNode<rlType>( *this );
}


/**
 * Clone the entire graph: clone children, swap parents. Because the argument DAG node is
 * doubly managed by us and by the variable wrapper around the argument object, we need
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
template<typename rlType>
RevBayesCore::DagNode* ConverterNode<rlType>::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode* >& newNodes ) const
{
    // Return our clone if we have already been cloned
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // We avoid the conflict between having to make a deep copy in our clone
    // function and the need to have a shallow copy (identical parents) in
    // this function by replacing the argument variable using special code.
    ConverterNode<rlType>* copy = this->clone();
    
    // Add this node and its copy to the map
    newNodes[ this ] = copy;
    
    // Swap argument variable using non-standard code
    
    // Get our argument DAG node
    const RevBayesCore::DagNode *theArgumentNode = argument->getRevObject().getDagNode();
    
    // Remove the copy as a child to our argument DAG node so the cloning works
    theArgumentNode->removeChild( copy );
    
    // Make sure the copy has its own Rev object argument with its DAG node being the clone of our argument DAG node
    copy->argument = new Variable( argument->getRevObject().cloneDAG( newNodes ), argument->getName() );
    
    // Now swap copy parents: detach the copy node from its old parent and attach it to the new parent
    // If we called swapParent here, the swapParent function would not find the old parent in its RevObject
    // argument variable wrapper because that wrapper has already been exchanged above. Note that we already
    // removed the copy as a child of the argument DAG node above. Just to conform to the pattern we check for
    // the need to delete even though the test should never be true here.
    if ( theArgumentNode->decrementReferenceCount() == 0 )
        delete theArgumentNode;
    RevBayesCore::DagNode* theArgumentNodeClone = copy->argument->getRevObject().getDagNode();
    theArgumentNodeClone->addChild( copy );
    theArgumentNodeClone->incrementReferenceCount();
    
    /* Make sure the children clone themselves */
    for( std::set<RevBayesCore::DagNode*>::const_iterator it = this->children.begin(); it != this->children.end(); ++it )
        (*it)->cloneDAG( newNodes );
    
    return copy;
}


/**
 * Get the parents of this node. We simply return the argument DAG node.
 */
template<typename rlType>
std::set<const RevBayesCore::DagNode*> ConverterNode<rlType>::getParents( void ) const
{
    std::set<const RevBayesCore::DagNode*> parents;
    
    parents.insert( argument->getRevObject().getDagNode() );
    
    return parents;
}


/**
 * Get the value (non-const version). The const cast really should not be
 * needed but clang gets confused otherwise.
 */
template<typename rlType>
typename rlType::valueType& ConverterNode<rlType>::getValue( void )
{
    if ( this->touched )
        update();
    
    return const_cast<typename rlType::valueType&>( convertedObject->getValue() );
}


/**
 * Get the value (const version). Note that we need to type-cast here to get access to the
 * update function in case we have been touched and have not updated before. This is required
 * by the lazy evaluation mechanism.
 */
template<typename rlType>
const typename rlType::valueType& ConverterNode<rlType>::getValue( void ) const
{
    if ( this->touched )
        const_cast<ConverterNode<rlType>*>( this )->update();
    
    return convertedObject->getValue();
}



/**
 * Is this node constant? Only if the argument variable is constant.
 */
template<typename rlType>
bool ConverterNode<rlType>::isConstant( void ) const
{
    // Check the argument
    if ( !argument->getRevObject().getDagNode()->isConstant() )
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
template<typename rlType>
void ConverterNode<rlType>::keepMe( RevBayesCore::DagNode* affecter )
{
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In keepMe of converter node " << this->getName() << " <" << this << ">" << std::endl;
#endif
 
    // Pass the call on
    this->keepAffected();
}


/**
 * Print struct for user. Because of lazy evaluation we need to make a
 * const cast, but this should be perfectly safe.
 */
template<typename rlType>
void ConverterNode<rlType>::printStructureInfo( std::ostream& o, bool verbose ) const
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
    o << "_dagType      = Type conversion DAG node" << std::endl;
    
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
void ConverterNode<rlType>::restoreMe( RevBayesCore::DagNode *restorer )
{

#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In restoreMe of Converter node " << this->getName() << " <" << this << ">" << std::endl;
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
void ConverterNode<rlType>::touchMe( RevBayesCore::DagNode *toucher )
{

#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In touchMe of converter node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // Touch myself
    this->touched = true;
    
    // Dispatch the touch message to downstream nodes
    this->touchAffected();
}


/**
 * Swap parent. We get this call just before the argument object is being replaced by another argument
 * object. In the cloneDAG algorithm, the swapParent function of this class is not (see comments for
 * the swapParent function).
 *
 * We throw an error if the old parent cannot be found.
 */
template<typename rlType>
void ConverterNode<rlType>::swapParent(const RevBayesCore::DagNode* oldParent, const RevBayesCore::DagNode* newParent)
{
    if ( oldParent != argument->getRevObject().getDagNode() )
        throw RbException( "Invalid attempt to swap non-parent" );
    
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
 * Update our value. We assume that the caller knows
 * that an update is really needed.
 */
template<typename rlType>
void ConverterNode<rlType>::update()
{
    // Free memory
    delete convertedObject;
    
    // Get the converted object
    convertedObject = static_cast<rlType*>( argument->getRevObject().convertTo( typeSpec ) );
    
    // We are clean!
    this->touched = false;
}


#endif
