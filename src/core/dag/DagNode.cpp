#include "DagNode.h"
#include "Distribution.h"
#include "DynamicNode.h"
#include "Monitor.h"
#include "Move.h"
#include "RbException.h"
#include "RbOptions.h"

using namespace RevBayesCore;

/**
 * Construct an empty DAG node, potentially
 * with a name (default is "").
 */
DagNode::DagNode( const std::string &n ) :
    children(),
    elementVar( false ),
    hidden( false ),
    monitors(),
    moves(),
    name( n ),
    priorOnly( false ),
    touchedElements(),
    refCount( 0 )
{
    
#if defined ( DEBUG_MEMORY )
    std::cerr << "Creating node <" << this << "> with name '" << name << "'\n";
#endif
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
    elementVar( n.elementVar ),
    hidden( n.hidden ),
    monitors( ),
    moves( ),
    name( n.name ),
    priorOnly( n.priorOnly ),
    touchedElements( n.touchedElements ),
    refCount( 0 )
{
    
#if defined ( DEBUG_MEMORY )
    std::cerr << "Creating node <" << this << "> with name '" << name << "'\n";
#endif
    
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
        std::cerr << o.str();
//        throw RbException( o );
    }
#endif
    
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
        name            = d.name;
        elementVar      = d.elementVar;
        hidden          = d.hidden;
        priorOnly       = d.priorOnly;
        touchedElements = d.touchedElements;
    }
    
    return *this;
}


/**
 * Add a new child node to this node.
 * Since we store the children in a set we don't need to worry about duplicates.
 *
 * Note, the caller also needs to increment the reference count to this node.
 */
void DagNode::addChild(DagNode *child) const
{
    
    // add the child to our set of children
    // the set will take care automaticall of duplicates.
    children.insert( child );
    
}


/**
 * Add a new monitor which monitors this node.
 * We only keep these pointers to notify the monitor if we are replaced.
 *
 *
 * Note, the caller also needs to increment the reference count to this node.
 */
void DagNode::addMonitor(Monitor *m)
{
    // insert into our local set which makes sure that there are no duplicates
    monitors.insert( m );
}


/**
 * Add a new move which update this node.
 * We only keep these pointers to notify the monitor if we are replaced.
 *
 *
 * Note, the caller also needs to increment the reference count to this node.
 */
void DagNode::addMove(Move *m)
{
    // insert into our local set which makes sure that there are no duplicates
    moves.insert( m );
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


/** 
 * Decrement the reference count and return it. 
 */
size_t DagNode::decrementReferenceCount( void ) const 
{

    refCount--;
    
    return refCount;
}


/**
 * Get all affected nodes this DAGNode.
 * This means we call getAffected() of all children. getAffected() is pure virtual.
 */
void DagNode::getAffectedNodes(std::set<DagNode *> &affected)
{
    
    // get all my affected children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
    {
        (*i)->getAffected(affected, this);
    }
    
}


/**
 * Get a const reference to the local set of children of this node.
 */
const std::set<DagNode*>& DagNode::getChildren( void ) const
{
    return children;
}


/**
 * Get the type of the DAG node as a string.
 */
DagNode::DagNodeTypes DagNode::getDagNodeType( void ) const
{
    
    return type;
    
//    if ( type == CONSTANT ) 
//    {
//        return "constant";
//    } 
//    else if ( type == DETERMINISTIC )
//    {
//        return "deterministic";
//    }
//    else if ( type == STOCHASTIC )
//    {
//        return "stochastic";
//    }
//    else
//    {
//        throw RbException("Unknown DAG type.");
//    }
    
}


/**
 * Get the distribution of this node. Only stochastic nodes have a distribution and thus we throw an error here.
 */
const Distribution& DagNode::getDistribution( void ) const
{
    throw RbException("Only stochastic nodes have a distribution.");
}


/**
 * Get the distribution of this node. Only stochastic nodes have a distribution and thus we throw an error here.
 */
Distribution& DagNode::getDistribution( void )
{
    throw RbException("Only stochastic nodes have a distribution.");
}


/**
 * Get the first child of this node.
 * Here we simply return a pointer to the first element stored in the set of children.
 */
DagNode* DagNode::getFirstChild( void ) const
{
    
    return *children.begin();
}


/**
 * Get a const reference to our local monitor set.
 */
const std::set<Monitor*>& DagNode::getMonitors( void ) const
{
    // return the local object
    return monitors;
}


/**
 * Get a const reference to our local moves set.
 */
const std::set<Move*>& DagNode::getMoves( void ) const
{
    // return the local object
    return moves;
}


/**
 * Get a const reference to the variable name.
 */
const std::string& DagNode::getName( void ) const
{

    return name;
}


/**
 * Get the number of children for this DAG node.
 */
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
 * Get the printable children by filling the set of DAG nodes with the printable children.
 * This method will skip hidden variables and instead replace the hidden variables by their children,
 * thus calling this function recursively for the hidden children.
 * Hidden children may be converter nodes or similar.
 */
void DagNode::getPrintableChildren(std::set<DagNode *> &c) const
{
    
    for (std::set<DagNode*>::const_iterator it = children.begin(); it != children.end(); ++it )
    {
        if ( (*it)->isHidden() == false )
        {
            // just insert this child
            c.insert( (*it) );
        }
        else
        {
            // do not add this child but all the children below because we omit this node
            (*it)->getPrintableChildren( c );
        }
    }
    
}



/**
 * Get the printable parents by filling the set of DAG nodes with the printable parents.
 * This method will skip hidden variables and instead replace the hidden variables by their parents,
 * thus calling this function recursively for the hidden parents.
 * Hidden parents may be converter nodes or similar.
 */
void DagNode::getPrintableParents(std::set<const DagNode *> &p) const
{
    
    std::set<const DagNode*> parents = getParents();
    for (std::set<const DagNode*>::const_iterator it = parents.begin(); it != parents.end(); ++it )
    {
        if ( (*it)->isHidden() == false )
        {
            // just insert this child
            p.insert( (*it) );
        }
        else
        {
            // do not add this child but all the children below because we omit this node
            (*it)->getPrintableParents( p );
        }
    }
    
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


bool DagNode::isConstant( void ) const
{
    
    return false;
}

bool DagNode::isElementVariable( void ) const
{
    
    return elementVar;
}

bool DagNode::isHidden( void ) const
{
    
    return hidden;
}


/**
 * Is this variable a simple numeric variable?
 * This is asked for example by the model monitor that only wants to monitor simple numeric variable 
 * because all others (e.g. trees and vectors/matrices) cannot be read by Tracer.
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
    
    // create my own copy of the pointers to the children
    std::set<DagNode*> printableChildren = children;

    // replace the children that should not be printed
    if ( verbose == false )
    {
        
        printableChildren.clear();
        getPrintableChildren( printableChildren );
        
    }

    std::set<DagNode*>::const_iterator it;
    size_t i = 0;
    for ( i = 0, it = printableChildren.begin(); it != printableChildren.end(); ++it, ++i )
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
        
        if ( printableChildren.size() - i > 1 )
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
    
    // create my own copy of the pointers to the children
    std::set<const DagNode*> printableParents = getParents();
    
    // replace the children that should not be printed
    if ( verbose == false )
    {
        
        printableParents.clear();
        getPrintableParents( printableParents );
        
    }

    std::set<const DagNode*>::const_iterator it;
    size_t i = 0;

    for ( i = 0, it = printableParents.begin(); it != printableParents.end(); ++it, ++i )
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
        
        if ( printableParents.size() - i > 1 )
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
 * Print the elements of the value separated by the specified separator.
 * By default we assume that this is a simple value that doesn't have elements
 * and therefore we delegate the call to printValue.
 * DAG nodes holding container/vectors need to overwrite this function.
 */
void DagNode::printValueElements(std::ostream &o, const std::string &sep, int l, bool left, bool flatten) const
{
    
    // just delegate the call
    printValue(o, l, left);
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


/**
 * Remove the DAG node from our set of children.
 *
 * Note, the caller also needs to decrement the reference count to this node.
 */
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
 * Remove the monitor from our set of monitors.
 *
 * Note, the caller also needs to decrement the reference count to this node.
 */
void DagNode::removeMonitor(Monitor *m)
{
    
    // test if we even have this monitor
    if ( monitors.find(m) != monitors.end() )
    {
        // we do not own our monitors! See addMonitor for explanation
        
        // remove the monitor from our list
        monitors.erase( m );
        
    }
}


/**
 * Remove the move from our set of moves.
 *
 * Note, the caller also needs to decrement the reference count to this node.
 */
void DagNode::removeMove(Move *m)
{
    
    // test if we even have this move
    if ( moves.find(m) != moves.end() )
    {
        // we do not own our moves! See addMove for explanation
        
        // remove the move from our list
        moves.erase( m );
        
    }
}


/**
 * Replace the DAG node. 
 * We call swap parent for all children so that they get a new parent. We do not change the parents.
 */
void DagNode::replace( DagNode *n )
{
    
    // replace myself for all my monitor
    while ( !monitors.empty() )
    {
        Monitor *theMonitor = *monitors.begin();
        theMonitor->swapNode( this, n);
    }
    
    // replace myself for all my moves
    while ( !moves.empty() )
    {
        Move *theMove = *moves.begin();
        theMove->swapNode( this, n);
    }
    
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
    
    // next, restore all my children
    for ( std::set<DagNode *>::iterator i = children.begin(); i != children.end(); i++ )
    {
        (*i)->restoreMe( this );
    }
    
}


void DagNode::setElementVariable(bool tf)
{
    elementVar = tf;
}


void DagNode::setHidden(bool tf)
{
    hidden = tf;
}


void DagNode::setName(std::string const &n)
{
    // set the internal value
    name = n;

}



/**
 * Set the number of processes available to this specific DAG node object.
 * If there is more than one process available, then we can use these
 * to compute the likelihood in parallel. Yeah!
 */
void DagNode::setNumberOfProcesses(size_t n, size_t offset)
{
    
}


void DagNode::setParentNamePrefix(const std::string &p)
{
    
    
    std::set<const DagNode*> parents = getParents();
    for (std::set<const DagNode*>::const_iterator it = parents.begin(); it != parents.end(); ++it )
    {
        if ( (*it)->getName().size() > 0 && (*it)->getName()[0] == '.' )
        {
            DagNode *parentNode = const_cast<DagNode *>( (*it) );
            // just insert this child
            parentNode->setName( p + parentNode->getName() );
            parentNode->setParentNamePrefix( parentNode->getName() );
        }
    }
    
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
void DagNode::touch(bool touchAll)
{

    // first touch myself
    touchMe( this, touchAll );
    
    // next, touch all my children
    touchAffected( touchAll );
}


/**
 * Tell affected variable nodes to touch themselves (i.e. that they've been touched).
 */
void DagNode::touchAffected(bool touchAll)
{

    // touch all my children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
    {
        (*i)->touchMe( this, touchAll );
    }
    
}
