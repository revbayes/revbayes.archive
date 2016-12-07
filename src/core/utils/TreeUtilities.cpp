#include "MatrixReal.h"
#include "RbException.h"
#include "StringUtilities.h"
#include "Tree.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <iostream>

using namespace RevBayesCore;

void RevBayesCore::TreeUtilities::constructTimeTreeRecursively(TopologyNode *tn, const TopologyNode &n, std::vector<TopologyNode*> &nodes, std::vector<double> &ages, double depth)
{
    
    // set the name
    tn->setName( n.getName() );
    
    // copy the index
    tn->setIndex( n.getIndex() );
    
    // copy the branch "comments"
    const std::vector<std::string> &branchComments = n.getBranchParameters();
    for (size_t i = 0; i < branchComments.size(); ++i)
    {
        std::string tmp = branchComments[i];
        if ( tmp[0] == '&')
        {
            tmp = tmp.substr(1,tmp.size());
        }
        std::vector<std::string> pair;
        StringUtilities::stringSplit(tmp, "=", pair);
        tn->addBranchParameter(pair[0], pair[1]);
    }
    // copy the node "comments"
    const std::vector<std::string> &nodeComments = n.getNodeParameters();
    for (size_t i = 0; i < nodeComments.size(); ++i)
    {
        std::string tmp = nodeComments[i];
        if ( tmp[0] == '&')
        {
            tmp = tmp.substr(1,tmp.size());
        }
        std::vector<std::string> pair;
        StringUtilities::stringSplit(tmp, "=", pair);
        tn->addNodeParameter(pair[0], pair[1]);
    }
    
    // set the node flags
    tn->setFossil( n.isFossil() );
    tn->setSampledAncestor( n.isSampledAncestor() );
    
    // remember the node
    nodes.push_back( tn );
    
    // set the age
    double a = depth - n.getBranchLength();
    if ( a < 1E-4 )
    {
        a = 0.0;
    }
    ages.push_back( a );
    
    // create children
    for (size_t i = 0; i < n.getNumberOfChildren(); ++i)
    {
        const TopologyNode& child = n.getChild( i );
        TopologyNode* newChild = new TopologyNode();
        
        // set parent child relationship
        newChild->setParent( tn );
        tn->addChild( newChild );
        
        // start recursive call
        constructTimeTreeRecursively(newChild, child, nodes, ages, a);
    }
    
    if ( tn->getNumberOfChildren() == 1 )
    {
        tn->setFossil( true );
    }
    
}


RevBayesCore::Tree* RevBayesCore::TreeUtilities::convertTree(const Tree &t, bool resetIndex)
{
    // create time tree object (topology + times)
    Tree *tt = new Tree();
    
    // clock trees should always be rooted
    tt->setRooted( true );
    
    // get the root of the original tree
    const TopologyNode& bln = t.getRoot();
    
    TopologyNode* root = new TopologyNode();
    
    // copy the root index
    root->setIndex( bln.getIndex() );
    
    std::vector<double> ages;
    std::vector<TopologyNode*> nodes;
    
    double maxDepth = bln.getMaxDepth() + bln.getBranchLength();
    
    // recursive creation of the tree
    constructTimeTreeRecursively(root, bln, nodes, ages, maxDepth);
    
    // add the root which creates the topology
    tt->setRoot( root, resetIndex );
    
    // set the ages
    for (size_t i = 0; i < nodes.size(); ++i) 
    {
        nodes[i]->setAge( ages[i] );
        if ( nodes[i]->isTip() && ages[i] > 0.05)
        {
            nodes[i]->setFossil( true );
        }
        
    }
    
    return tt;
}


RevBayesCore::DistanceMatrix* RevBayesCore::TreeUtilities::getDistanceMatrix(const Tree& tree)
{
    
    RevBayesCore::MatrixReal* matrix = new MatrixReal( tree.getNumberOfTips() );
    
    std::vector<std::string> names = tree.getTipNames( ) ;
    
    std::map< std::string, int > namesToId;
    
    for(size_t i = 0; i < names.size(); ++i)
    {
        namesToId[ names[i] ] = int(i);
    }
    
    std::vector< std::pair<std::string, double> > distsToRoot;
    
    processDistsInSubtree( tree.getRoot() , *matrix, distsToRoot, namesToId);
    
    DistanceMatrix* distMat = new DistanceMatrix(*matrix, names);
    
    // free memory
    delete matrix;
    
    return distMat;
}


void RevBayesCore::TreeUtilities::getOldestTip(Tree* t, TopologyNode *n, double& oldest)
{
    
    // we only rescale internal nodes
    if ( !n->isTip() )
    {
        
        // assertion that we have binary trees
#ifdef ASSERTIONS_TREE
        if ( n->getNumberOfChildren() != 2 )
        {
            throw RbException("NNI is only implemented for binary trees!");
        }
#endif
        
        // rescale both children
        getOldestTip( t, &n->getChild(0), oldest);
        getOldestTip( t, &n->getChild(1), oldest);
    }
    else
    {
        if (n->getAge() > oldest)
        {
            oldest = n->getAge();
        }
    }
}


void RevBayesCore::TreeUtilities::getTaxaInSubtree(TopologyNode *n, std::vector<TopologyNode*> &taxa )
{
    
    if ( n->isTip() )
    {
        taxa.push_back( n );
    }
    else
    {
        
        // recursively add children to the list of nodes in this subtree
        for (size_t i = 0; i < n->getNumberOfChildren(); ++i)
        {
            TopologyNode& child = n->getChild( i );
            
            getTaxaInSubtree( &child, taxa );
        }
    }
    
}



void RevBayesCore::TreeUtilities::rescaleSubtree(Tree *t, TopologyNode *n, double factor, bool verbose)
{
    // we only rescale internal nodes
    if ( n->isTip() == false )
    {
        // rescale the age of the node
        double newAge = n->getAge() * factor;
        t->getNode( n->getIndex() ).setAge(newAge);
        
        // assertion that we have binary trees
        if ( verbose == true )
        {
            if ( n->getNumberOfChildren() != 2 )
            {
                throw RbException("Subtree scaling is only implemented for binary trees!");
            }
        }
        
        // rescale both children
        rescaleSubtree( t, &n->getChild(0), factor, verbose);
        rescaleSubtree( t, &n->getChild(1), factor, verbose);
    }
    
}

void RevBayesCore::TreeUtilities::setAges(Tree *t, TopologyNode *n, std::vector<double>& ages)
{
    // we only rescale internal nodes
    if ( n->isTip() == false )
    {
        // rescale the age of the node
        t->getNode( n->getIndex() ).setAge( ages[n->getIndex()] );

        // rescale both children
        std::vector<TopologyNode*> children = n->getChildren();
        for(size_t i = 0; i < children.size(); i++)
            setAges( t, children[i], ages);
    }

}

void RevBayesCore::TreeUtilities::getAges(Tree *t, TopologyNode *n, std::vector<double>& ages)
{
    // we only rescale internal nodes
    if ( n->isTip() == false )
    {
        // rescale the age of the node
        ages[n->getIndex()] = n->getAge();

        // rescale both children
        std::vector<TopologyNode*> children = n->getChildren();
        for(size_t i = 0; i < children.size(); i++)
            getAges( t, children[i], ages);
    }

}

void RevBayesCore::TreeUtilities::rescaleTree(Tree *t, TopologyNode *n, double factor)
{
    // rescale the time of the node
    double newAge = n->getAge() * factor;
    t->getNode(n->getIndex()).setAge( newAge);
    
    // recursive call for internal nodes
    if ( n->isTip() == false )
    {
        
        // assertion that we have binary trees
#ifdef ASSERTIONS_TREE
        if ( n->getNumberOfChildren() != 2 )
        {
            throw RbException("Tree scaling  is only implemented for binary trees!");
        }
#endif
        
        // rescale both children
        rescaleTree( t, &n->getChild(0), factor);
        rescaleTree( t, &n->getChild(1), factor);
    }
    
}



std::string RevBayesCore::TreeUtilities::uniqueNewickTopology(const Tree &t) 
{
    return uniqueNewickTopologyRecursive( t.getRoot() );
}


std::string RevBayesCore::TreeUtilities::uniqueNewickTopologyRecursive(const TopologyNode &n) 
{
    // check whether this is an internal node
    if ( n.isTip() ) 
    {
        return n.getName();
    } 
    else 
    {
        std::string newick = "(";
        std::vector<std::string> children;
        for (size_t i = 0; i < n.getNumberOfChildren(); ++i) 
        {
            children.push_back( uniqueNewickTopologyRecursive(n.getChild( i ) ) );
        }
        sort(children.begin(), children.end());
        for (std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) 
        {
            if ( it != children.begin() ) 
            {
                newick += ",";
            }
            newick += *it;
        }
        newick += ")";
        newick += n.getName();
        
        return newick;
    }
    
}



void RevBayesCore::TreeUtilities::processDistsInSubtree(const RevBayesCore::TopologyNode& node, RevBayesCore::MatrixReal& matrix, std::vector< std::pair<std::string, double> >& distsToNodeFather, const std::map< std::string, int >& namesToId)
{
	distsToNodeFather.clear();
	
	// node-is-leaf case
	if ( node.isTip() )
	{
		distsToNodeFather.push_back(make_pair ( node.getName(), node.getBranchLength() ) );
		return;
	}
	
	// For all leaves in node's subtree, get leaf-to-node distances.
	// Leaves are ordered according to the children of node.
	std::map< size_t, std::vector< std::pair<std::string, double> > > leavesDists;
	for (size_t i = 0; i < node.getNumberOfChildren(); ++i)
	{
		const RevBayesCore::TopologyNode* child = &( node.getChild(i) );
		processDistsInSubtree(*child, matrix, leavesDists[child->getIndex()], namesToId); // recursivity
	}
	
	// Write leaf-leaf distances to the distance matrix.
	// Only pairs in which the two leaves belong to different
	// children are considered.
	for (size_t son1_loc = 0; son1_loc < node.getNumberOfChildren(); ++son1_loc)
	{
		for (size_t son2_loc = 0; son2_loc < son1_loc; ++son2_loc)
		{
			const RevBayesCore::TopologyNode* son1 = &(node.getChild(son1_loc) );
			const RevBayesCore::TopologyNode* son2 = &(node.getChild(son2_loc) );
			
			for (std::vector< std::pair<std::string, double> >::iterator son1_leaf = leavesDists[son1->getIndex()].begin();
				 son1_leaf != leavesDists[son1->getIndex()].end();
				 ++son1_leaf)
			{
				for (std::vector< std::pair<std::string, double> >::iterator son2_leaf = leavesDists[son2->getIndex()].begin();
					 son2_leaf != leavesDists[son2->getIndex()].end();
					 ++son2_leaf)
				{
					int son1_leaf_id = namesToId.at( son1_leaf->first );
					int son2_leaf_id = namesToId.at( son2_leaf->first );
					matrix[son1_leaf_id] [son2_leaf_id] =
					matrix[son2_leaf_id] [son1_leaf_id] =
					( son1_leaf->second + son2_leaf->second );
				}
			}
		}
	}
	
	// node-is-root case
	if (node.isRoot())
	{
		// node-is-root-and-leaf case
		if (node.isTip() )
		{
			std::string root_name = node.getName();
			for (std::vector< std::pair<std::string, double> >::iterator other_leaf = leavesDists[node.getChild(0).getIndex()].begin();
				 other_leaf != leavesDists[node.getChild(0).getIndex()].end();
				 ++other_leaf)
			{
				matrix [ namesToId.at(root_name) ] [ namesToId.at(other_leaf->first) ]= matrix[ namesToId.at(other_leaf->first) ] [ namesToId.at(root_name) ] = other_leaf->second;
			}
		}
		
		return;
	}
	
	
	// Get distances from node's parent to considered leaves
	distsToNodeFather.clear();
	double nodeToFather = node.getBranchLength();
	
	for (std::map<size_t, std::vector<std::pair<std::string, double> > >::iterator son = leavesDists.begin(); son != leavesDists.end(); ++son)
	{
		for (std::vector< std::pair<std::string, double> >::iterator leaf = (son->second).begin(); leaf != (son->second).end(); ++leaf)
		{
			distsToNodeFather.push_back(make_pair(leaf->first, (leaf->second + nodeToFather)));
		}
	}
	
}



void RevBayesCore::TreeUtilities::climbUpTheTree(const TopologyNode& node, boost::unordered_set <const TopologyNode* >& pathFromNodeToRoot) {
    try {
        if (! (node.isRoot( ) ) ) {
            pathFromNodeToRoot.insert(&node);
            climbUpTheTree(node.getParent(), pathFromNodeToRoot);
        }
    }
    catch (RbException e)
    {
        throw e;
    }
    
    return;
}


double RevBayesCore::TreeUtilities::getAgeOfMRCARecursive(const TopologyNode& node, boost::unordered_set <const TopologyNode* >& pathFromOtherNodeToRoot) {
    try {
        
        if ( node.isRoot() || pathFromOtherNodeToRoot.find(&node) != pathFromOtherNodeToRoot.end() ) {
            return node.getAge();
        }
        else {
            return getAgeOfMRCARecursive( node.getParent(), pathFromOtherNodeToRoot );
        }
    }
    catch (RbException e)
    {
        throw e;
    }
    
    
}



double RevBayesCore::TreeUtilities::getAgeOfMRCA(const Tree &t, std::string first, std::string second) {
    
    const TopologyNode &node1 = t.getTipNodeWithName(first) ;
    
    const TopologyNode &node2 = t.getTipNodeWithName(second) ;
    
    if (! (node2.equals( node1 ) ) )
    {
        boost::unordered_set <const TopologyNode* > pathFromNode1ToRoot;
        climbUpTheTree(node1, pathFromNode1ToRoot);
        
        double age = getAgeOfMRCARecursive(node2, pathFromNode1ToRoot);
        return age;
    }
    else {
        return node1.getAge();
    }
    
}


