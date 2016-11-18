#include "Clade.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbOptions.h"
#include "RbSettings.h"
#include "RbUtil.h"
#include "Taxon.h"
#include "TopologyNode.h"
#include "Tree.h"

#include <algorithm>
#include <stdio.h>

using namespace RevBayesCore;

/** Default constructor (interior node, no name). Give the node an optional index ID */
TopologyNode::TopologyNode(size_t indx) :
    age( RbConstants::Double::nan ),
    branch_length( -1 ),
    children(),
    parent( NULL ),
    tree( NULL ),
    taxon(""),
    index(indx),
    interior_node( false ),
    root_node( true ),
    tip_node( true ),
    fossil( false ),
    sampled_ancestor( false )
{
    
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const Taxon& t, size_t indx) :
    age( RbConstants::Double::nan ),
    branch_length( -1 ),
    children(),
    parent( NULL ),
    tree( NULL ),
    taxon(t),
    index(indx),
    interior_node( false ),
    root_node( true ),
    tip_node( true ),
    fossil( false ),
    sampled_ancestor( false )
{
    
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, size_t indx) :
    age( RbConstants::Double::nan ),
    branch_length( -1 ),
    children(),
    parent( NULL ),
    tree( NULL ),
    taxon(n),
    index(indx),
    interior_node( false ),
    root_node( true ),
    tip_node( true ),
    fossil( false ),
    sampled_ancestor( false )
{
    
}

/** Copy constructor. We use a shallow copy. */
TopologyNode::TopologyNode(const TopologyNode &n) :
    age( n.age ),
    branch_length( n.branch_length ),
    parent( n.parent ),
    tree( NULL ),
    taxon( n.taxon ),
    index( n.index ),
    interior_node( n.interior_node ),
    root_node( n.root_node ),
    tip_node( n.tip_node ),
    fossil( n.fossil ),
    sampled_ancestor( n.sampled_ancestor ),
    node_comments( n.node_comments ),
    branch_comments( n.branch_comments )
{
    
    // copy the children
    for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++)
    {
        TopologyNode* the_node = *it;
        TopologyNode* theClone = the_node->clone();
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
        age                     = n.age;
        branch_length           = n.branch_length;
        taxon                   = n.taxon;
        index                   = n.index;
        interior_node           = n.interior_node;
        tip_node                = n.tip_node;
        fossil                  = n.fossil;
        sampled_ancestor        = n.sampled_ancestor;
        root_node               = n.root_node;
        node_comments           = n.node_comments;
        branch_comments         = n.branch_comments;
        
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
    branch_comments.push_back( comment );
    
}


void TopologyNode::addBranchParameter(const std::string &n, const std::string &p)
{
    
    if ( n == "index" || n == "species" )
    {
        std::cerr << "Illegal branch parameter with name '" << n << "'.\n";
    }
    
    std::string comment = n + "=" + p;
    branch_comments.push_back( comment );
    
}



void TopologyNode::addBranchParameters(std::string const &n, const std::vector<double> &p, bool internalOnly) {
    
    if ( !internalOnly || !isTip()  )
    {
        std::stringstream o;
        o << n << "=" << p[index];
        std::string comment = o.str();
        branch_comments.push_back( comment );
        
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
        branch_comments.push_back( comment );
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it)
        {
            (*it)->addBranchParameters(n, p, internalOnly);
        }
        
    }
    
}


/** Add a child node. We own it from here on. */
void TopologyNode::addChild(TopologyNode* c)
{
    
    // add the child to our internal vector
    children.push_back(c);
    
    // fire tree change event
    if ( tree != NULL )
    {
        tree->getTreeChangeEventHandler().fire( *c );
    }
    
    tip_node = false;
    interior_node = true;
    
//    bool child_sampled_ancestor = false;
//    for (size_t i = 0; i < children.size(); i++)
//    {
//        child_sampled_ancestor |= ( children[i]->getAge() == age );
//    }
//    c->setSampledAncestor( child_sampled_ancestor && c->getAge() > 0.0 );
//    c->setFossil( c->getAge() > 0.0 && c->isTip() );
//    fossil          = a < 0.0;

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
    node_comments.push_back( comment );
    
}


void TopologyNode::addNodeParameter(const std::string &n, const std::string &p)
{
    
    if ( n == "index" || n == "species" )
    {
        std::cerr << "Illegal node parameter with name '" << n << "'.\n";
    }
    
    std::string comment = n + "=" + p;
    node_comments.push_back( comment );
    
}


void TopologyNode::addNodeParameters(std::string const &n, const std::vector<double> &p, bool internalOnly)
{
    
    if ( internalOnly == false || isTip() == false  )
    {
        std::stringstream o;
        char s[32];
        size_t num_tip_nodes = 0;
        if ( internalOnly == true && tree != NULL )
        {
            num_tip_nodes = tree->getNumberOfTips();
        }
        snprintf(s, sizeof(s), "%f",p[index - num_tip_nodes]);
        o << n << "=" << s; //SK
        std::string comment = o.str();
        node_comments.push_back( comment );
        
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
        node_comments.push_back( comment );
        
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
    if ( tip_node == true )
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
        
        if ( fossil == true )
        {
            o << taxon.getName();
        }
        
    }
    
    if ( node_comments.size() > 0 || RbSettings::userSettings().getPrintNodeIndex() == true )
    {
        o << "[&";
        
        bool needsComma = false;
        
        // first let us print the node index, we must increment by 1 to match RevLanguage indexing
        if ( RbSettings::userSettings().getPrintNodeIndex() == true )
        {
            o << "index=" << index+1;
            needsComma = true;
        }
            
        for (size_t i = 0; i < node_comments.size(); ++i)
        {
            if ( needsComma == true )
            {
                o << ",";
            }
            o << node_comments[i];
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
        
    o << ":" << getBranchLength();
    
    if ( branch_comments.size() > 0 )
    {
        o << "[&";
        for (size_t i = 0; i < branch_comments.size(); ++i)
        {
            if ( i > 0 )
            {
                o << ",";
            }
            o << branch_comments[i];
        }
        o << "]";
    }
    
    if ( root_node == true )
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
    
    branch_comments.clear();
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
    
    node_comments.clear();
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



std::string TopologyNode::computeNewick(void)
{
    
    return buildNewickString();
}


/* Build newick string */
std::string TopologyNode::computePlainNewick( void ) const
{
    
    // test whether this is a internal or external node
    if ( tip_node == true )
    {
        // this is a tip so we just return the name of the node
        return taxon.getName();
    }
    else
    {
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


/**
 * Is the argument clade contained in the clade descending from this node?
 * By strict we mean that the contained clade has to be monophyletic in the containing clade.
 */
bool TopologyNode::containsClade(const TopologyNode *c, bool strict) const
{
    RbBitSet your_taxa = RbBitSet( tree->getNumberOfTips() );
    c->getTaxa( your_taxa );
    
    return containsClade( your_taxa, strict );
}


/**
 * Is the argument clade contained in the clade descending from this node?
 * By strict we mean that the contained clade has to be monophyletic in the containing clade.
 */
bool TopologyNode::containsClade(const Clade &c, bool strict) const
{
    
    return containsClade( c.getBitRepresentation(), strict );
}


/**
 * Is the argument clade contained in the clade descending from this node?
 * By strict we mean that the contained clade has to be monophyletic in the containing clade.
 */
bool TopologyNode::containsClade(const RbBitSet &your_taxa, bool strict) const
{
    size_t n = tree->getNumberOfTips();
    RbBitSet my_taxa   = RbBitSet( n );
    getTaxa( my_taxa );
    
    if ( your_taxa.size() != my_taxa.size() )
    {
        throw RbException("Problem in bit representation of clades.");
    }
    
    // this node needs to have at least as many taxa to contain the other clade
    if ( your_taxa.getNumberSetBits() > my_taxa.getNumberSetBits() )
    {
        // quick negative abort to safe computational time
        return false;
    }
    
    // check that every taxon of the clade is in this subtree
    for (size_t i=0; i<n; ++i)
    {
        
        // if I don't have any of your taxa then I cannot contain you.
        if ( your_taxa.isSet(i) == true && my_taxa.isSet(i) == false )
        {
            return false;
        }
        
    }
    
    // now check, if required, that the contained clade is monophyletic in the containing clade.
    if ( strict == true )
    {
        // we already know from our check above that all taxa from the contained clade are present in this clade.
        // so we just need to check if there are additional taxa in this clade
        // and if so, then we need to check that the contained clade is contained in one of my children.
        if ( your_taxa.getNumberSetBits() < my_taxa.getNumberSetBits() )
        {
            
            // loop over all children
            for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
            {
                // check if the clade is contained in this child
                bool is_contained_in_child = (*it)->containsClade( your_taxa, strict );
                if ( is_contained_in_child == true )
                {
                    // yeah, so we can abort and return true
                    return true;
                }
            }
            
            return false;
        }
        
    }
    
    return true;
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


/*
 * Get the Age.
 * We internally store the age so can return it. However, if we invalidated the age ( age = Inf ),
 * then we need to compute the age from the time.
 */
double TopologyNode::getAge( void ) const
{

    return age;
}


/*
 * Get the branch length.
 * We compute the difference of my time and my parents time.
 */
double TopologyNode::getBranchLength( void ) const
{

    return branch_length;
}


/*
 * Get the branch parameters.
 */
const std::vector<std::string>& TopologyNode::getBranchParameters( void ) const
{
    
    return branch_comments;
}


/**
 * Get the index of a clade
 */
size_t TopologyNode::getCladeIndex(const TopologyNode *c) const
{
    
    size_t n = tree->getNumberOfTips();
    RbBitSet my_taxa   = RbBitSet( n );
    RbBitSet your_taxa = RbBitSet( n );
    getTaxa( my_taxa );
    c->getTaxa( your_taxa );
    
    // sanity check
    if ( your_taxa.size() != my_taxa.size() )
    {
        throw RbException("Problem in bit representation of clades.");
    }
    
    // this node needs to have at least as many taxa to contain the other clade
    if ( your_taxa.getNumberSetBits() > my_taxa.getNumberSetBits() )
    {
        // quick negative abort to safe computational time
        return RbConstants::Size_t::inf;
    }
    
    // check that every taxon of the clade is in this subtree
    for (size_t i=0; i<n; ++i)
    {
        
        // if I don't have any of your taxa then I cannot contain you.
        if ( your_taxa.isSet(i) == true && my_taxa.isSet(i) == false )
        {
            return RbConstants::Size_t::inf;
        }
        
    }
    
    // we already know from our check above that all taxa from the contained clade are present in this clade.
    // so we just need to check if there are additional taxa in this clade
    // and if so, then we need to check that the contained clade is contained in one of my children.
    if ( your_taxa.getNumberSetBits() < my_taxa.getNumberSetBits() )
    {
        
        // loop over all children
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
            
            // check if the clade is contained in this child
            size_t child_index = (*it)->getCladeIndex( c );
            if ( RbMath::isFinite( child_index ) == true )
            {
                // yeah, so we can abort and return the child index
                return child_index;
            }
            
        }
        
    }
    
    // so the clade must be contained in my clade
    // just return my index
    return index;
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


Clade TopologyNode::getClade( void ) const
{
    
    std::vector<Taxon> taxa;
    getTaxa( taxa );
    
    Clade c = Clade( taxa );
    c.setAge( getAge() );
    
    return c;
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
    
    return getTaxon().getName();
}


/*
 * Get the node parameters.
 */
const std::vector<std::string>& TopologyNode::getNodeParameters( void ) const
{
    
    return node_comments;
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
    
    if ( tip_node )
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


void TopologyNode::getTaxa(RbBitSet &taxa) const
{
    
    if ( isTip() == true )
    {
//        taxa.set( index );
        // We can't use indices for tree comparison because different trees may
        // have different indices for the same taxon.
        // Instead make the BitSet ordered by taxon names.
        // Eventually this should be refactored with the TaxonMap class.
        std::map<std::string, size_t> taxon_bitset_map = tree->getTaxonBitSetMap();
        taxa.set( taxon_bitset_map[taxon.getName()] );
    }
    else
    {
        for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
        {
            (*i)->getTaxa( taxa );
        }
    }
    
    
}


const Taxon& TopologyNode::getTaxon() const
{
    return taxon;
}


double TopologyNode::getTmrca(const Clade &c) const
{
    const std::vector<Taxon>& yourTaxa = c.getTaxa();
    
    return getTmrca( yourTaxa );
}


double TopologyNode::getTmrca(const TopologyNode &n) const
{
    std::vector<Taxon> yourTaxa;
    n.getTaxa( yourTaxa );
    
    return getTmrca( yourTaxa );
}

double TopologyNode::getTmrca(const std::vector<Taxon> &yourTaxa) const
{
    
    std::vector<Taxon> myTaxa;
    getTaxa( myTaxa );
    
    if ( myTaxa.size() < yourTaxa.size() )
    {
        return -1;
    }
    
    for (std::vector<Taxon>::const_iterator y_it = yourTaxa.begin(); y_it != yourTaxa.end(); ++y_it)
    {
        bool found = false;
        for (std::vector<Taxon>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it)
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
        double tmrca = getAge();
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
        {
            double child_tmrca = (*it)->getTmrca( yourTaxa );
            contains |= ( child_tmrca >= 0.0 );
            if ( contains == true )
            {
                tmrca = child_tmrca;
                break;
            }
        }
        return tmrca;
    }
}


bool TopologyNode::isFossil( void ) const
{
    
    return fossil;
}


bool TopologyNode::isInternal( void ) const
{
    
    return interior_node;
}


bool TopologyNode::isRoot( void ) const
{
    
    return parent == NULL;
}


bool TopologyNode::isSampledAncestor( void ) const
{
    
    return sampled_ancestor;
}


bool TopologyNode::isTip( void ) const
{
    
    return tip_node;
}


/**
 * Make this node an all its children bifurcating.
 * The root will not be changed. We throw an error if this node
 * has more than 2 children. If this node has only one child,
 * then we insert a dummy child.
 * This function is called recursively.
 */
void TopologyNode::makeBifurcating( void )
{
    
    if ( isTip() == false )
    {
        
        // we only modify non root nodes
        if ( isRoot() == false )
        {
            
            if ( getNumberOfChildren() > 2 )
            {
                throw RbException("Cannot make this node bifurcating because it has more than 2 children.");
            }
            else if ( getNumberOfChildren() == 1 )
            {
                
                TopologyNode *new_fossil = new TopologyNode( getTaxon() );
                taxon = Taxon("");
                
                // connect to the old fossil
                addChild( new_fossil );
                new_fossil->setParent( this );
                
                // set the fossil flags
                setFossil( false );
                setSampledAncestor( false );
                new_fossil->setFossil( true );
                new_fossil->setSampledAncestor( true );
                
                // set the age and branch-length of the fossil
                new_fossil->setAge( age );
                new_fossil->setBranchLength( 0.0 );
                
            }
            
        }
        
        // call this function recursively for all its children
        for (size_t i=0; i<getNumberOfChildren(); ++i)
        {
            getChild( i ).makeBifurcating();
        }
        
    }
    
}


void TopologyNode::recomputeBranchLength( void )
{
    
    if ( parent == NULL )
    {
        branch_length = 0.0;
    }
    else if ( RbMath::isFinite( age ) == false )
    {
        // don't reset the branch length if this isn't a time tree (WP)
        //branch_length = -1;
    }
    else
    {
        branch_length = parent->getAge() - age;
    }
    
}


/** Remove all children. We need to call intelligently the destructor here. */
void TopologyNode::removeAllChildren(void)
{
    
    // empty the children vector
    while (children.size() > 0)
    {
        TopologyNode* the_node = children[0];
        // free the memory
        delete the_node;
    }
    
    taxon = Taxon("");
    
    tip_node = true;
    interior_node = false;
}




/** Remove a child from the vector of children */
void TopologyNode::removeChild(TopologyNode* c)
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
    tip_node      = (children.size() == 0);
    interior_node = (children.size()  > 0);
    
    // fire tree change event
    if ( tree != NULL )
    {
        tree->getTreeChangeEventHandler().fire( *c    );
        tree->getTreeChangeEventHandler().fire( *this );
    }
    
    /*bool child_sampled_ancestor = false;
    for (size_t i = 0; i < children.size(); i++)
    {
        child_sampled_ancestor |= ( children[i]->getAge() == age );
    }
    c->setSampledAncestor( child_sampled_ancestor && c->getAge() > 0.0 );*/


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


void TopologyNode::setAge(double a)
{
    
    age = a;
    
    // we need to recompute my branch-length
    recomputeBranchLength();
    
//    sampled_ancestor = ( !isRoot() && a == parent->getAge() & a > 0.0 );
//    fossil          = a > 0.0;
    
    //
//    // set the fossil flags
//    setFossil( false );
//    setSampledAncestor( false );
//    new_fossil->setFossil( true );
//    new_fossil->setSampledAncestor( true );
//    
//    // set the age and branch-length of the fossil
//    new_fossil->setAge( age );
//    new_fossil->setBranchLength( 0.0 );
    
    
    
    // we also need to recompute the branch lengths of my children
    for (std::vector<TopologyNode *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        TopologyNode *child = *it;
        child->recomputeBranchLength();
        
        // fire tree change event
        if ( tree != NULL )
        {
            tree->getTreeChangeEventHandler().fire( *child );
        }
    }
    
    
    
}


void TopologyNode::setBranchLength(double b)
{
    
    branch_length = b;
    
    
    // fire tree change event
    if ( tree != NULL )
    {
        tree->getTreeChangeEventHandler().fire( *this );
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
    taxon.setSpeciesName( n );
    
}

//SK
void TopologyNode::setNodeType(bool tip, bool root, bool interior)
{
	
	tip_node = tip;
	root_node = root;
	interior_node = interior;
    
}

void TopologyNode::setSpeciesName(std::string const &n)
{
    
    taxon.setSpeciesName( n );
    
}


void TopologyNode::setTaxon(Taxon const &t)
{
    
    taxon = t;
    
}


//!< Set the indices of the taxa from the taxon map
void TopologyNode::setTaxonIndices(const TaxonMap &tm)
{
    
    if ( isTip() == true )
    {
        size_t idx = tm.getTaxonIndex( taxon );
        index = idx;
    }
    else
    {
        for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
        {
            (*i)->setTaxonIndices( tm );
        }
    }
    
    
}


void TopologyNode::setParent(TopologyNode* p)
{
    
    // we only do something if this isn't already our parent
    if (p != parent)
    {
        // we do not own the parent so we do not have to delete it
        parent = p;
        
        // we need to recompute our branch length
        recomputeBranchLength();
        
        // fire tree change event
        if ( tree != NULL )
        {
            tree->getTreeChangeEventHandler().fire( *this );
        }
        
    }
    
    root_node = parent == NULL;
}


void TopologyNode::setSampledAncestor(bool tf)
{
    
    sampled_ancestor = tf;
    
}


void TopologyNode::setTree(Tree *t)
{
    
    tree = t;
    for (std::vector<TopologyNode *>::iterator i = children.begin(); i != children.end(); ++i)
    {
        (*i)->setTree( t );
    }
    
}

