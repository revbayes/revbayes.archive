#include "Clade.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbOptions.h"
#include "RbSettings.h"
#include "RbUtil.h"
#include "Taxon.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "Tree.h"

#include <algorithm>
#include <stdio.h>

using namespace RevBayesCore;

/** Default constructor (interior node, no name). Give the node an optional index ID */
TopologyNode::TopologyNode(size_t indx) :
    parent( NULL ),
    tree( NULL ),
    taxon(""),
    index(indx),
    interiorNode( false ),
    rootNode( true ),
    tipNode( true ),
    fossil( false ),
    sampledAncestor( false ),
    newick(""),
    newickNeedsRefreshing( true )
{
    
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const Taxon& t, size_t indx) :
    parent( NULL ),
    tree( NULL ),
    taxon(t),
    index(indx),
    interiorNode( false ),
    rootNode( true ),
    tipNode( true ),
    fossil( false ),
    sampledAncestor( false ),
    newick(""),
    newickNeedsRefreshing( true )
{
    
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, size_t indx) :
    parent( NULL ),
    tree( NULL ),
    taxon(n),
    index(indx),
    interiorNode( false ),
    rootNode( true ),
    tipNode( true ),
    fossil( false ),
    sampledAncestor( false ),
    newick(""),
    newickNeedsRefreshing( true )
{
    
}

/** Copy constructor. We use a shallow copy. */
TopologyNode::TopologyNode(const TopologyNode &n) :
    tree( NULL )
{
    
    // copy the members
    taxon                   = n.taxon;
    index                   = n.index;
    interiorNode            = n.interiorNode;
    tipNode                 = n.tipNode;
    fossil                  = n.fossil;
    sampledAncestor         = n.sampledAncestor;
    rootNode                = n.rootNode;
    newick                  = n.newick;
    newickNeedsRefreshing   = n.newickNeedsRefreshing;
    parent                  = n.parent;
    nodeComments            = n.nodeComments;
    branchComments          = n.branchComments;
    
    // copy the children
    for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++)
    {
        TopologyNode* theNode = *it;
        TopologyNode* theClone = theNode->clone();
        children.push_back( theClone );
        theClone->setParent(this);
    }
    
    
}


/** Destructor */
TopologyNode::~TopologyNode(void)
{
    // we do not own the parent so we do not delete it
    
    // free memory of children
    removeAllChildren();
    
    // make sure that I was removed from my parent
    if (parent != NULL)
    {
        parent->removeChild(this);
    }
    
}


TopologyNode& TopologyNode::operator=(const TopologyNode &n)
{
    
    if (this == &n)
    {
        
        removeAllChildren();
        
        // copy the members
        taxon                   = n.taxon;
        index                   = n.index;
        interiorNode            = n.interiorNode;
        tipNode                 = n.tipNode;
        fossil                  = n.fossil;
        sampledAncestor         = n.sampledAncestor;
        rootNode                = n.rootNode;
        newick                  = n.newick;
        newickNeedsRefreshing   = n.newickNeedsRefreshing;
        nodeComments            = n.nodeComments;
        branchComments          = n.branchComments;
        
        // copy the members
        parent          = n.parent;
        
        // copy the children
        for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++)
        {
            children.push_back( (*it)->clone() );
        }
        
        // add myself as a new child to the parent node
        parent->addChild(this);
        
    }
    
    return *this;
}


void TopologyNode::addBranchParameter(const std::string &n, double p)
{
    
    if ( n == "index" || n == "species" )
    {
        std::cerr << "Illegal branch parameter with name '" << n << "'.\n";
    }
    
    std::stringstream o;
    char s[32];
    snprintf(s, sizeof(s), "%f",p);
    o << n << "=" << s;
    std::string comment = o.str();
    branchComments.push_back( comment );
    
    flagNewickRecomputation();
    
}


void TopologyNode::addBranchParameter(const std::string &n, const std::string &p)
{
    
    if ( n == "index" || n == "species" )
    {
        std::cerr << "Illegal branch parameter with name '" << n << "'.\n";
    }
    
    std::string comment = n + "=" + p;
    branchComments.push_back( comment );
    
//    nodeFields[n] = p;
    
    flagNewickRecomputation();
    
}



void TopologyNode::addBranchParameters(std::string const &n, const std::vector<double> &p, bool internalOnly) {
    
    if ( !internalOnly || !isTip()  )
    {
        std::stringstream o;
        o << n << "=" << p[index];
        std::string comment = o.str();
        branchComments.push_back( comment );
        
        flagNewickRecomputation();
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->addBranchParameters(n, p, internalOnly);
        }
    }
}

void TopologyNode::addBranchParameters(std::string const &n, const std::vector<std::string> &p, bool internalOnly) {
    
    if ( !internalOnly || !isTip()  )
    {
        std::string comment = n + "=" + p[index];
        branchComments.push_back( comment );
        
        flagNewickRecomputation();
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->addBranchParameters(n, p, internalOnly);
        }
    }
}


/** Add a child node. We own it from here on. */
void TopologyNode::addChild(TopologyNode* c, bool forceNewickRecomp)
{
    
    // add the child to our internal vector
    children.push_back(c);
    
    // mark for newick recomputation
    if ( forceNewickRecomp )
    {
        flagNewickRecomputation();
    }
    
    // fire tree change event
    if ( tree != NULL )
    {
        tree->getTreeChangeEventHandler().fire( *c );
    }
    
    tipNode = false;
    interiorNode = true;
}


void TopologyNode::addNodeParameter(const std::string &n, double p)
{
    
    if ( n == "index" || n == "species" )
    {
        std::cerr << "Illegal node parameter with name '" << n << "'.\n";
    }
    
    std::stringstream o;
    char s[32];
    snprintf(s, sizeof(s), "%f",p);
    o << n << "=" << s; //SK
    std::string comment = o.str();
    nodeComments.push_back( comment );
    
    flagNewickRecomputation();
    
}


void TopologyNode::addNodeParameter(const std::string &n, const std::string &p)
{
    
    if ( n == "index" || n == "species" )
    {
        std::cerr << "Illegal node parameter with name '" << n << "'.\n";
    }
    
    std::string comment = n + "=" + p;
    nodeComments.push_back( comment );
    
//    nodeFields[n] = p;
    
    flagNewickRecomputation();
    
}

//std::string TopologyNode::getNodeField(std::string key) const
//{
//
//    std::map<std::string,std::string>::const_iterator i = nodeFields.find(key);
//
//    if (i == nodeFields.end())
//    {
//        std::cerr << "no node field with key : " << key << '\n';
//        throw(0);
//    }
//    
//    return i->second;
//    
//}


void TopologyNode::addNodeParameters(std::string const &n, const std::vector<double> &p, bool internalOnly)
{
    
    if ( !internalOnly || !isTip()  )
    {
        std::stringstream o;
        char s[32];
        snprintf(s, sizeof(s), "%f",p[index]);
        o << n << "=" << s; //SK
        std::string comment = o.str();
        nodeComments.push_back( comment );
        
        flagNewickRecomputation();
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->addNodeParameters(n, p, internalOnly);
        }
    }
}

void TopologyNode::addNodeParameters(std::string const &n, const std::vector<std::string*> &p, bool internalOnly)
{
    
    if ( !internalOnly || !isTip()  )
    {
        std::stringstream o;
        o << n << "=" << *p[index];
        std::string comment = o.str();
        nodeComments.push_back( comment );
        
        flagNewickRecomputation();
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->addNodeParameters(n, p, internalOnly);
        }
    }
}


/* Build newick string */
std::string TopologyNode::buildNewickString( void )
{
    
    // create the newick string
    std::stringstream o;
    
    std::fixed(o);
    o.precision( 6 );
    
    // test whether this is a internal or external node
    if ( tipNode == true )
    {
        // this is a tip so we just return the name of the node
        o << taxon.getName();
        
    }
    else
    {
        o << "(";
        for (size_t i=0; i<(getNumberOfChildren()-1); i++)
        {
            o << getChild(i).computeNewick() << ",";
        }
        o << getChild(getNumberOfChildren()-1).computeNewick() << ")";
    }
    
    if ( nodeComments.size() > 0 || RbSettings::userSettings().getPrintNodeIndex() == true )
    {
        o << "[&";
        
        bool needsComma = false;
        
        // first let us print the node index
        if ( RbSettings::userSettings().getPrintNodeIndex() == true )
        {
            o << "index=" << index;
            needsComma = true;
        }
            
        for (size_t i = 0; i < nodeComments.size(); ++i)
        {
            if ( needsComma == true )
            {
                o << ",";
            }
            o << nodeComments[i];
            needsComma = true;
        }
            
        //Finally let's print the species name (always)
//        if ( needsComma == true )
//        {
//            o << ",";
//        }
//        o << "&species=" << getSpeciesName();
        
        o << "]";
    }
        
    if ( tree != NULL )
    {
        o << ":" << getBranchLength();
    }
    if ( branchComments.size() > 0 )
    {
        o << "[&";
        for (size_t i = 0; i < branchComments.size(); ++i)
        {
            if ( i > 0 )
            {
                o << ",";
            }
            o << branchComments[i];
        }
        o << "]";
    }
    
    if (rootNode)
    {
        o << ";";
    }
    
    return o.str();
}



void TopologyNode::clearParameters(void)
{
    clearBranchParameters();
    clearNodeParameters();
}


void TopologyNode::clearBranchParameters( void )
{
    
    branchComments.clear();
    if ( !isTip()  )
    {
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->clearBranchParameters();
        }
    }
}


void TopologyNode::clearNodeParameters( void )
{
    
    nodeComments.clear();
    if ( !isTip()  )
    {
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->clearNodeParameters();
        }
    }
}


/** Clone function */
TopologyNode* TopologyNode::clone(void) const
{
    
    return new TopologyNode(*this);
}



const std::string& TopologyNode::computeNewick(void)
{
    
    // check if we need to recompute
    if ( newickNeedsRefreshing )
    {
        newick = buildNewickString();
        newickNeedsRefreshing = false;
    }
    
    return newick;
}


/* Build newick string */
std::string TopologyNode::computePlainNewick( void ) const
{
    
    // test whether this is a internal or external node
    if (tipNode)
    {
        // this is a tip so we just return the name of the node
        return taxon.getName();
    }
    else {
        std::string left = getChild(0).computePlainNewick();
        std::string right = getChild(1).computePlainNewick();
        if ( left < right )
        {
            return "(" + left + "," + right + ")";
        }
        else
        {
            return "(" + right + "," + left + ")";
        }
    }
    
}


bool TopologyNode::containsClade(const TopologyNode *c, bool strict) const
{
    
    std::vector<std::string> myTaxa;
    std::vector<std::string> yourTaxa;
    getTaxaStringVector( myTaxa );
    c->getTaxaStringVector( yourTaxa );
    
    if ( myTaxa.size() < yourTaxa.size() )
    {
        return false;
    }
    
    // check that every taxon of the clade is present in this subtree
    for (std::vector<std::string>::const_iterator y_it = yourTaxa.begin(); y_it != yourTaxa.end(); ++y_it)
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it)
        {
            if ( *y_it == *it )
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            return false;
        }
    }
    
    if ( !strict || myTaxa.size() == yourTaxa.size() )
    {
        return true;
    }
    else
    {
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
            contains |= (*it)->containsClade(c,strict);
            if ( contains )
            {
                break;
            }
        }
        return contains;
    }
}


bool TopologyNode::containsClade(const Clade &c, bool strict) const
{
    
    std::vector<std::string> myTaxa;
    getTaxaStringVector( myTaxa );
    
    if ( myTaxa.size() < c.size() )
    {
        return false;
    }
    
    // check that every taxon of the clade is in this subtree
    for (std::vector<std::string>::const_iterator y_it = c.begin(); y_it != c.end(); ++y_it)
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it)
        {
            if ( *y_it == *it )
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            return false;
        }
    }
    
    if ( !strict || myTaxa.size() == c.size() )
    {
        return true;
    }
    else
    {
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
            contains |= (*it)->containsClade(c,strict);
            if ( contains )
            {
                break;
            }
        }
        return contains;
    }
}



bool TopologyNode::equals(const TopologyNode& node) const
{
    
    if (this == &node)
    {
        return true;
    }
    
    // test if the name is the same
    if (taxon != node.taxon)
    {
        return false;
    }
    
    // test if the index is the same
    if (index != node.index)
    {
        return false;
    }
    
    // test if the parent is the same
    if (parent != node.parent)
    {
        return false;
    }
    
    // test if the size of the children differs
    if (children.size() != node.children.size())
    {
        return false;
    }
    
    // test if all children are the same
    for (size_t i=0; i<children.size(); i++)
    {
        if (children[i]->equals(*node.children[i]))
        {
            return false;
        }
    }
    
    return true;
}


void TopologyNode::flagNewickRecomputation( void )
{
    
    if ( parent != NULL && !newickNeedsRefreshing )
    {
        parent->flagNewickRecomputation();
    }
    
    newickNeedsRefreshing = true;
    
}


/*
 * Get the Age.
 * We internally store the age so can return it. However, if we invalidated the age ( age = Inf ),
 * then we need to compute the age from the time.
 */
double TopologyNode::getAge( void ) const
{

    return tree->getAge(index);
}


/*
 * Get the branch length.
 * We compute the difference of my time and my parents time.
 */
double TopologyNode::getBranchLength( void ) const
{

    return tree->getBranchLength(index);
}


/*
 * Get the branch parameters.
 */
const std::vector<std::string>& TopologyNode::getBranchParameters( void ) const
{
    
    return branchComments;
}


/**
 * Get the index of a clade
 */
size_t TopologyNode::getCladeIndex(const TopologyNode *c) const
{
    
    std::vector<std::string> myTaxa;
    std::vector<std::string> yourTaxa;
    getTaxaStringVector( myTaxa );
    c->getTaxaStringVector( yourTaxa );
    
    if ( myTaxa.size() < yourTaxa.size() )
    {
        return RbConstants::Size_t::nan;
    }
    
    // check that every taxon of the clade is present in this subtree
    for (std::vector<std::string>::const_iterator y_it = yourTaxa.begin(); y_it != yourTaxa.end(); ++y_it)
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it)
        {
            if ( *y_it == *it )
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            return RbConstants::Size_t::nan;
        }
    }
	
	if ( myTaxa.size() == yourTaxa.size() )
    {
		
		// this may be the correct node for the clade, but check to see if there
		// is a child node that contains the clade first
		for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
			
			if ( (*it)->containsClade(c,true) && !(*it)->isTip() )
            {
				return (*it)->getCladeIndex( c );
            }
		}
		
        return index;
		
    }
    else
    {
		
		for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
			
			if ( (*it)->containsClade(c,true) )
            {
				return (*it)->getCladeIndex( c );
            }
		}
		
        return RbConstants::Size_t::nan;
		
	}
}


/** Get child at index i */
const TopologyNode& TopologyNode::getChild(size_t i) const
{
    
    return *children[i];
}


/** Get child at index i */
TopologyNode& TopologyNode::getChild(size_t i)
{
    
    return *children[i];
}


const std::vector<TopologyNode*>& TopologyNode::getChildren( void ) const
{
    return children;
}


/** Loop over children and get their indices */
std::vector<int> TopologyNode::getChildrenIndices() const
{
    
    std::vector<int> temp;
    
    for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
    {
        temp.push_back( int( (*i)->getIndex() ) );
    }
    
    return temp;
}


size_t TopologyNode::getIndex( void ) const
{
    
    return index;
}


/**
 * Get the maximal depth starting from this node.
 * The depth here mean the maximal path length along the branches until a terminal node (tip) is reached.
 * For ultrametric trees all path lengths are equivalent, but for serial sampled trees not.
 * Hence, we compute the maximal depths by recursively exploring each path along the branches to the children.
 *
 * \return    The maximal depth (path length) from this node to the most recent tip.
 */
double TopologyNode::getMaxDepth( void ) const
{
    
    // iterate over the childen
    double max = 0.0;
    for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        double m = 0.0;
        TopologyNode& node = *(*it);
        if ( node.isTip() )
        {
            m = node.getBranchLength();
        }
        else
        {
            m = node.getBranchLength() + node.getMaxDepth();
        }
        
        if ( m > max )
        {
            max = m;
        }
    }
    
    return max;
}


const std::string& TopologyNode::getName( void ) const
{
    
    return taxon.getName();
}


/*
 * Get the node parameters.
 */
const std::vector<std::string>& TopologyNode::getNodeParameters( void ) const
{
    
    return nodeComments;
}


size_t TopologyNode::getNumberOfChildren( void ) const
{
    
    return children.size();
}


/**
 * Get the number of nodes contained in the subtree starting with this node as the root.
 * This either returns 1 if this is a tip node (or 0 if we do not count tipes)
 * or computes recursively the number of nodes in both children plus one for this node.
 *
 * \param[in]   tips       Shall we count tips?
 * \return                 Subtree size.
 */
size_t TopologyNode::getNumberOfNodesInSubtree( bool countTips ) const
{
    
    if ( tipNode )
    {
        return (countTips ? 1 : 0);
    }
    else
    {
        return children[0]->getNumberOfNodesInSubtree(countTips) + children[1]->getNumberOfNodesInSubtree(countTips) + 1;
    }
    
}

TopologyNode& TopologyNode::getParent(void)
{
    
    return *parent;
    
}

const TopologyNode& TopologyNode::getParent(void) const
{
    
    return *parent;
}


std::string TopologyNode::getSpeciesName() const
{
    std::string name = taxon.getSpeciesName();
    return name;
}



void TopologyNode::getTaxaStringVector( std::vector<std::string> &taxa ) const
{
    
    if ( isTip() )
    {
        taxa.push_back( taxon.getName() );
    }
    else
    {
        for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
        {
            (*i)->getTaxaStringVector( taxa );
        }
    }
}


void TopologyNode::getTaxa(std::vector<Taxon> &taxa) const
{
    if ( isTip() )
    {
        taxa.push_back( taxon );
    }
    else
    {
        for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
        {
            (*i)->getTaxa( taxa );
        }
    }

    
}


Taxon TopologyNode::getTaxon() const
{
    return taxon;
    
}


double TopologyNode::getTime( void ) const
{
    
    return tree->getTime( index );
}


double TopologyNode::getTmrca(const TopologyNode &n) const
{
    
    std::vector<std::string> myTaxa;
    std::vector<std::string> yourTaxa;
    getTaxaStringVector( myTaxa );
    n.getTaxaStringVector( yourTaxa );
    
    if ( myTaxa.size() < yourTaxa.size() )
    {
        return -1;
    }
    
    for (std::vector<std::string>::const_iterator y_it = yourTaxa.begin(); y_it != yourTaxa.end(); ++y_it)
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it)
        {
            if ( *y_it == *it )
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            return -1;
        }
    }
    
    if ( myTaxa.size() == yourTaxa.size() )
    {
        return getAge();
    }
    else
    {
        double tmrca = -1;
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
            tmrca = (*it)->getTmrca(n);
            contains |= ( tmrca >= 0 );
            if ( contains )
            {
                break;
            }
        }
        return tmrca;
    }
}


void TopologyNode::initiateFlaggingForNewickRecomputation( void )
{
    
    for (size_t i = 0; i < getNumberOfChildren(); ++i)
    {
        getChild( i ).flagNewickRecomputation();
    }
    
}


bool TopologyNode::isFossil( void ) const
{
    
    return fossil;
}


bool TopologyNode::isInternal( void ) const
{
    
    return interiorNode;
}


bool TopologyNode::isRoot( void ) const
{
    
    return parent == NULL;
}


bool TopologyNode::isSampledAncestor( void ) const
{
    
    return sampledAncestor;
}


bool TopologyNode::isTip( void ) const
{
    
    return tipNode;
}


/** Remove all children. We need to call intelligently the destructor here. */
void TopologyNode::removeAllChildren(void)
{
    
    // empty the children vector
    while (children.size() > 0)
    {
        TopologyNode* theNode = children[0];
        // free the memory
        delete theNode;
    }
    
    taxon = Taxon();
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
    tipNode = true;
    interiorNode = false;
}




/** Remove a child from the vector of children */
void TopologyNode::removeChild(TopologyNode* c, bool forceNewickRecomp)
{
    
    std::vector<TopologyNode* >::iterator it = find(children.begin(), children.end(), c);
    if ( it != children.end() )
    {
        children.erase(it);
    }
    else
    {
        throw(RbException("Cannot find node in list of children nodes"));
    }
    
    // update the flags
    tipNode = children.size() == 0;
    interiorNode = children.size() > 0;
    
    // fire tree change event
    if ( tree != NULL )
    {
        tree->getTreeChangeEventHandler().fire( *c );
        tree->getTreeChangeEventHandler().fire( *this );
    }
    
    // mark for newick recomputation
    if (forceNewickRecomp)
    {
        flagNewickRecomputation();
    }
    
}


void TopologyNode::removeTree(Tree *t)
{
    
    // only remove the tree if we had a pointer stored to it
    if ( tree == t )
    {
        tree = NULL;
    }
    
    for (std::vector<TopologyNode *>::iterator i = children.begin(); i != children.end(); ++i)
    {
        (*i)->removeTree( t );
    }
    
}


void TopologyNode::setFossil(bool tf)
{
    
    fossil = tf;
    
}


void TopologyNode::setIndex( size_t idx)
{
    
    index = idx;
    
}

void TopologyNode::setName(std::string const &n)
{
    
    taxon.setName( n );
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
}

//SK
void TopologyNode::setNodeType(bool tip, bool root, bool interior)
{
	
	tipNode = tip;
	rootNode = root;
	interiorNode = interior;
    
}

void TopologyNode::setSpeciesName(std::string const &n)
{
    
    taxon.setSpeciesName( n );
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
}


void TopologyNode::setTaxon(Taxon const &t) {
    
    taxon = t;
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
}


void TopologyNode::setParent(TopologyNode* p, bool forceNewickRecomp)
{
    
    // we only do something if this isn't already our parent
    if (p != parent)
    {
        // we do not own the parent so we do not have to delete it
        parent = p;
        
        // mark for newick recomputation
        if (forceNewickRecomp)
            flagNewickRecomputation();
        
        // fire tree change event
        if ( tree != NULL )
        {
            tree->getTreeChangeEventHandler().fire( *this );
        }
        
    }
    
    rootNode = parent == NULL;
}


void TopologyNode::setSampledAncestor(bool tf)
{
    
    sampledAncestor = tf;
    
}


void TopologyNode::setTree(Tree *t)
{
    
    tree = t;
    for (std::vector<TopologyNode *>::iterator i = children.begin(); i != children.end(); ++i)
    {
        (*i)->setTree( t );
    }
    
}

