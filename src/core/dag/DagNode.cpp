#include "DagNode.h"
#include "DynamicNode.h"
#include "RbException.h"
#include "RbOptions.h"

using namespace RevBayesCore;

/**
 * Construct an empty DAG node, potentially
 * with a name (default is "").
 */
DagNode::DagNode( const std::string &n ) :
children(),
name( n ),
touchedElements(),
refCount( 0 )
{
    
}


/**
 * Copy constructor. We delegate the handling
 * of parents to derived classes with parents,
 * and they need to take care of the management
 * of those parents, removing them as a child
 * from those parents and deleting the parents
 * if necessary.
 */
DagNode::DagNode( const DagNode &n ) :
children(),
name( n.name ),
touchedElements( n.touchedElements ),
refCount( 0 )
{
}


/**
 * Destructor. The destructor should never be called when the reference count
 * is larger than 0, or when we have children. All children should increase the
 * reference count, so there should be no children left when the reference count
 * is 0. Other Rev objects may also increase the reference count to protect a
 * DAG node from being deleted before they die, so the reference count can be
 * larger than the number of children, but never smaller.
 *
 * Deletion of parents is delegated to the destructors of derived classes with
 * parents.
 */
DagNode::~DagNode( void )
{
#if defined ( DEBUG_MEMORY )
    // For debugging memory leaks
    if ( getName() != "" )
        std::cerr << "Deleting DAG node '" << name << "' <" << this << ">" << std::endl;
    else
        std::cerr << "Deleting DAG node <" << this << ">" << std::endl;
#endif
    
    // Sanity checks
    if ( refCount != 0 )
    {
        std::stringstream s;
        s << "Deleting DAG Node with " << refCount << " references to it!" << std::endl;
        throw RbException( s.str() );
    }
    if ( children.size() != 0 )
    {
        std::ostringstream o;
        o << "DAG node has " << children.size() << " remaining children!" << std::endl;
        o << "Remaining children are: " << std::endl;
        printChildren( o, 0, 70 );
        o << std::endl;
        throw RbException( o );
    }
}


/**
 * Assignment operator. Note that parent management is delegated to
 * derived classes that actually have parents, so their assignment
 * operators need to deal with parent replacement at assignment.
 *
 * Note that children do not need to replace or remove this node
 * as their parent, because we remain alive and their parent
 * pointer will continue to be relevant.
 */
DagNode& DagNode::operator=(const DagNode &d)
{
    if ( &d != this )
    {
        name = d.name;
        touchedElements = d.touchedElements;
    }
    
    return *this;
}


void DagNode::addChild(DagNode *child) const
{
    children.insert( child );
}


/* Add the index of an element that has been touched */
void DagNode::addTouchedElementIndex(size_t i)
{
    
    touchedElements.insert( i );
    
}


void DagNode::clearTouchedElementIndices( void )
{
    
    touchedElements.clear();
    
}


/** Clone the graph downstream from this node: clone children */
DagNode* DagNode::cloneDownstreamDag( std::map<const DagNode*, DagNode* >& newNodes ) const
{
    
    if ( newNodes.find( this ) != newNodes.end() )
    {
        return ( newNodes[ this ] );
    }
    // Get pristine copy
    DagNode* copy = clone();
    
    // Add this node to the map
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<DagNode*>::const_iterator i = this->children.begin(); i != this->children.end(); i++ )
    {
        DagNode *child = (*i)->cloneDownstreamDag( newNodes );
        child->swapParent(this, copy);
    }
    
    return copy;
}


void DagNode::collectDownstreamGraph(std::set<RevBayesCore::DagNode *> &nodes)
{
    
    // for efficiency we check for multiple calls
    if ( nodes.find( this ) == nodes.end() )
    {
        // first, add myself
        nodes.insert( this );
        
        // now, perform a recursive call
        for (std::set<DagNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->collectDownstreamGraph( nodes );
        }
        
    }
    
}


/**
 * Decrement the reference count and return it.
 */
size_t DagNode::decrementReferenceCount( void ) const
{
    // Sanity checks
    if ( refCount == 0) {
        std::ostringstream o;
        if ( getName() != "" )
            o << "Decrementing reference count of node " << getName() << " <" << this << "> below 0" << std::endl;
        else
            o << "Decrementing reference count of node <" << this << "> below 0" << std::endl;
        throw RbException( o );
    }
    else if ( refCount <= children.size() )
    {
        std::ostringstream o;
        if ( getName() != "" )
            o << "Decrementing reference count of node " << getName() << " below number of children" << std::endl;
        else
            o << "Decrementing reference count of node <" << this << "> below number of children" << std::endl;
        throw RbException( o );
    }
    
    refCount--;
    
    return refCount;
}


/**
 * Get the stochastic nodes affected by an upstream change of a DAG node. The default
 * behavior, which is appropriate for all DAG nodes except stochastic nodes, is to
 * pass on the call to our children through their getAffected() function, updating
 * the affecter when passing the call on, so that the stochastic nodes at the end
 * of the chain will know through which immediate parent the call came.
 */
void DagNode::getAffected( std::set<DagNode*>& affected, DagNode* affecter )
{
    // Dispatch the call to our children
    for ( std::set<DagNode*>::iterator it = children.begin(); it != children.end(); ++it )
    {
        (*it)->getAffected( affected, this );
    }
}


/**
 * Get the downstream stochastic nodes whose probability will be affected by a change of this
 * node. We simply pass on the call to our children through their getAffected() function.
 */
void DagNode::getAffectedNodes( std::set<DagNode*>& affected )
{
    // Dispatch the call to our children
    for ( std::set<DagNode*>::iterator it = children.begin(); it != children.end(); ++it )
    {
        (*it)->getAffected( affected, this );
    }
}


const std::set<DagNode*>& DagNode::getChildren( void ) const
{
    return children;
}


std::string DagNode::getDagNodeType( void ) const
{
    
    if ( type == CONSTANT )
    {
        return "constant";
    }
    else if ( type == DETERMINISTIC )
    {
        return "deterministic";
    }
    else
    {
        return "stochastic";
    }
    
}

DagNode* DagNode::getFirstChild( void ) const
{
    
    return *children.begin();
}


const std::string& DagNode::getName( void ) const
{
    
    return name;
}


size_t DagNode::getNumberOfChildren( void ) const
{
    
    return children.size();
}


/**
 * To allow calls to getParents even on classes without parents,
 * we simply return an empty set by default.
 */
std::set<const DagNode*> DagNode::getParents( void ) const
{
    
    return std::set<const DagNode*>();
}


/**
 * Get the reference count.
 */
size_t DagNode::getReferenceCount( void ) const
{
    
    return refCount;
}


/* Get the indices of all touched elements */
const std::set<size_t>& DagNode::getTouchedElementIndices( void ) const
{
    
    return touchedElements;
}


/**
 * Increment the reference count.
 */
void DagNode::incrementReferenceCount( void ) const
{
    
    refCount++;
    
#if defined ( DEBUG_MEMORY )
    // For debugging memory leaks
    if ( refCount - children.size() > 1 )
    {
        if ( getName() != "" )
        {
            std::cerr << "More than 1 non-DAG references to node " << getName() << std::endl;
        }
        else
        {
            std::cerr << "More than 1 non-DAG references to node <" << this << std::endl;
        }
        
    }
#endif
    
}


/**
 * This function returns true if the DAG node is
 * a node that is modifiable by the user through
 * assignment. This occurs when the node is named
 * or at least one of the parents is assignable.
 *
 * @todo The current code really belongs to the
 *       language layer but it is convenient to
 *       implement it here. See if it is possible
 *       to move it to the language layer.
 */
bool DagNode::isAssignable( void ) const
{
    const std::set<const DagNode*>& parents = getParents();
    
    if ( getName() != "" )
        return true;
    
    for ( std::set<const DagNode*>::const_iterator it = parents.begin(); it != parents.end(); ++it )
        if ( (*it)->isAssignable() )
            return true;
    
    return false;
}


bool DagNode::isClamped( void ) const
{
    
    return false;
}


/**
 * Is this node composite, that is, does it keep track of elements or member variables
 * of a single composite language object? The default is false; composite nodes in the
 * language layer need to override this method and return true.
 */
bool DagNode::isComposite( void ) const
{
    return false;
}


bool DagNode::isConstant( void ) const
{
    
    return false;
}


/** Is this a non-applicable (NA) value? */
bool DagNode::isNAValue( void ) const
{
    return false;
}


/**
 * Is this variable a simple numeric variable?
 * This is asked for example by the model monitor that only wants to monitor simple
 * numeric variable because all others (e.g. trees and vectors/matrices) cannot be read
 * by Tracer.
 */
bool DagNode::isSimpleNumeric( void ) const
{
    return false;
}


bool DagNode::isStochastic( void ) const
{
    return false;
}

/**
 * Keep the value of the node.
 * This function delegates the call to keepMe() and calls keepAffected() too.
 */
void DagNode::keep(void)
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "Keeping DAG node " << getName() << " <" << this << ">" << std::endl;
#endif
    
    // keep myself first
    keepMe( this );
    
    // next, keep all my children
    keepAffected();
}

/**
 * Tell affected variable nodes to keep the current value.
 */
void DagNode::keepAffected()
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "Keeping affected of DAG node " << getName() << " <" << this << ">" << std::endl;
#endif
    
    // keep all my children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
    {
        (*i)->keepMe( this );
    }
    
}


/** Print children. We assume indent is from line 2 (hanging indent). Line length is lineLen. */
void DagNode::printChildren( std::ostream& o, size_t indent, size_t lineLen, bool verbose ) const
{
    std::string pad;
    for ( size_t i = 0; i < indent; ++i )
    {
        pad.push_back( ' ' );
    }
    
    o << "[ ";
    pad += "  ";
    indent += 2;
    
    size_t currentLength = indent + 2;
    std::ostringstream s;
    
    std::set<DagNode*>::const_iterator it;
    size_t i = 0;
    for ( i = 0, it = children.begin(); it != children.end(); ++it, ++i )
    {
        std::ostringstream s;
        if ( (*it)->getName() == "" )
        {
            s << "<" << (*it) << ">";
        }
        else
        {
            if ( verbose == true )
            {
                s << (*it)->getName() << " <" << (*it) << ">";
            }
            else
            {
                s << (*it)->getName();
            }
        }
        
        if ( children.size() - i > 1 )
        {
            s << ", ";
        }
        
        if ( s.str().size() + currentLength > lineLen )
        {
            o << std::endl << pad;
            currentLength = pad.size();
        }
        o << s.str();
        currentLength += s.str().size();
        s.str("");
    }
    
    o << " ]";
}


/**
 * Print parents. This function does not access the parents directly, but
 * through the getParents function. This is to allow derived classes to
 * handle parents in a different way than done in this class.
 *
 * Note the use of a const reference here to potentially avoid one copy
 * operation on the set of DAG nodes.
 */
void DagNode::printParents( std::ostream& o, size_t indent, size_t lineLen, bool verbose ) const
{
    const std::set<const DagNode*>& parents = getParents();
    
    std::string pad;
    for ( size_t i = 0; i < indent; ++i )
    {
        pad.push_back( ' ' );
    }
    
    o << "[ ";
    pad += "  ";
    indent += 2;
    
    size_t currentLength = indent + 2;
    std::ostringstream s;
    
    std::set<const DagNode*>::const_iterator it;
    size_t i = 0;
    
    for ( i = 0, it = parents.begin(); it != parents.end(); ++it, ++i )
    {
        if ( (*it)->getName() == "" )
        {
            s << "<" << (*it) << ">";
        }
        else
        {
            if ( verbose == true )
            {
                s << (*it)->getName() << " <" << (*it) << ">";
            }
            else
            {
                s << (*it)->getName();
            }
        }
        
        if ( parents.size() - i > 1 )
        {
            s << ", ";
        }
        
        if ( s.str().size() + currentLength > lineLen )
        {
            o << std::endl << pad;
            currentLength = pad.size();
        }
        o << s.str();
        currentLength += s.str().size();
        s.str("");
    }
    o << " ]";
}


/**
 * By default we do not need to do anything when re-initializiating.
 */
void DagNode::reInitialized( void )
{
    
    reInitializeMe();
    reInitializeAffected();
    
}


/**
 * By default we do not need to do anything when re-initializiating.
 */
void DagNode::reInitializeAffected( void )
{
    
    // next, reInitialize all my children
    for ( std::set<DagNode *>::iterator i = children.begin(); i != children.end(); i++ )
    {
        (*i)->reInitializeMe();
    }
    
}


/**
 * By default we do not need to do anything when re-initializiating.
 */
void DagNode::reInitializeMe( void )
{
    // nothing to do
}


void DagNode::removeChild(DagNode *child) const
{
    
    // test if we even have this node as a child
    if (children.find(child) != children.end())
    {
        // we do not own our children! See addChildNode for explanation
        
        // remove the child from our list
        children.erase(child);
        
    }
}


/**
 * Replace the DAG node.
 * We call swap parent for all children so that they get a new parent. We do not change the parents.
 */
void DagNode::replace( DagNode *n )
{
    
    // replace myself at all my children
    while ( !children.empty() )
    {
        DagNode *child = *children.begin();
        child->swapParent( this, n);
    }
    
}


/**
 * Restore this DAGNode.
 * This means we call restoreMe() and restoreAffected(). There is a default implementation of restoreAffected()
 * which call restoreMe() of all children of this node. restoreMe() is pure virtual.
 */
void DagNode::restore(void)
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "Restoring DAG node " << getName() << " <" << this << ">" << std::endl;
#endif
    
    // first restore myself
    restoreMe( this );
    
    // next, restore all my children
    restoreAffected();
}


/**
 * Restore all affected nodes this DAGNode.
 * This means we call restoreMe() of all children. restoreMe() is pure virtual.
 */
void DagNode::restoreAffected(void)
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "Restoring affected of DAG node " << getName() << " <" << this << ">" << std::endl;
#endif
    
    // next, restore all my children
    for ( std::set<DagNode *>::iterator i = children.begin(); i != children.end(); i++ )
    {
        (*i)->restoreMe( this );
    }
    
}


void DagNode::setName(std::string const &n)
{
    
    name = n;
    
}


void DagNode::setPriorOnly(bool tf)
{
    
    priorOnly = tf;
    
}


/**
 * Swap parent node. We delegate this task to derived DAG node classes
 * with parents. Here we just throw an error in case a derived class
 * with parents forgets to override this function.
 */
void DagNode::swapParent( const DagNode *oldParent, const DagNode *newParent )
{
    
    throw RbException( "This DAG node does not have any parents" );
    
}


/**
 * Touch the DAG node.
 *
 * This function should be called if the value of the variable has changed or if you want this node to be reevaluated.
 * The function will automatically call the touchMe() which is implemented differently in the different DAG node types.
 *
 * Since the DAG node was touched and possibly changed, we tell affected DAG nodes that they too have been touched
 * and need to update their value.
 */
void DagNode::touch()
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "Touching DAG node " << getName() << " <" << this << ">" << std::endl;
#endif
    
    // first touch myself
    touchMe( this );
    
    // next, touch all my children
    touchAffected();
}


/**
 * Tell affected variable nodes to touch themselves (i.e. that they've been touched).
 */
void DagNode::touchAffected()
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "Touching affected of DAG node " << getName() << " <" << this << ">" << std::endl;
#endif
    
    // touch all my children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchMe( this );
}
