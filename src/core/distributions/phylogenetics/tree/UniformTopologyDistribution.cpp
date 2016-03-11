#include "UniformTopologyDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

UniformTopologyDistribution::UniformTopologyDistribution(const std::vector<Taxon> &tn, const std::vector<Clade> &c) : TypedDistribution<Tree>( new Tree() ),
    numTaxa( tn.size() ),
    taxa( tn ),
	constraints( c ),
    logTreeTopologyProb( RbConstants::Double::nan )
{
    
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++)
    {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
    
}


UniformTopologyDistribution::~UniformTopologyDistribution()
{
    // the tree will be deleted automatically by the base class
    
}


void UniformTopologyDistribution::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
{
    
    if (tips.size() < numTaxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+long(index));
        
        // add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips);
    }
    
}


UniformTopologyDistribution* UniformTopologyDistribution::clone( void ) const
{

    return new UniformTopologyDistribution( *this );
}


double UniformTopologyDistribution::computeLnProbability( void )
{
    
	// first check if the current tree matches the clade constraints
    if ( matchesConstraints() == false )
    {
        return RbConstants::Double::neginf;
    }
    return logTreeTopologyProb;
    
}


void UniformTopologyDistribution::redrawValue( void )
{
    simulateTree();
}


/**
 *
 */
void UniformTopologyDistribution::simulateClade(std::vector<TopologyNode *> &n, bool bifurcating )
{
    
    size_t num_root_childred = 2;
    if ( bifurcating == false )
    {
        num_root_childred = 3;
    }
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    while ( n.size() > num_root_childred )
    {
        
        // get all the nodes before the current age
        std::vector<TopologyNode*> active_nodes = n;
        
        // randomly pick two nodes
        size_t index_left = static_cast<size_t>( floor(rng->uniform01()*active_nodes.size()) );
        TopologyNode* left_child = active_nodes[index_left];
        active_nodes.erase(active_nodes.begin()+long(index_left));
        size_t index_right = static_cast<size_t>( floor(rng->uniform01()*active_nodes.size()) );
        TopologyNode* right_right = active_nodes[index_right];
        active_nodes.erase(active_nodes.begin()+long(index_right));
                
        // erase the nodes also from the origin nodes vector
        n.erase(std::remove(n.begin(), n.end(), left_child), n.end());
        n.erase(std::remove(n.begin(), n.end(), right_right), n.end());
                
                
        // create a parent for the two
        TopologyNode *parent = new TopologyNode();
        parent->addChild( left_child );
        parent->addChild( right_right );
        left_child->setParent( parent );
        right_right->setParent( parent );
                
        // insert the parent to our list
        n.push_back( parent );

        
    }
    
    
    if ( n.size() == num_root_childred )
    {
        
        // create a parent
        TopologyNode *parent = new TopologyNode();

        for ( size_t i=0; i<num_root_childred; ++i)
        {
            TopologyNode* child = n[i];
            
            parent->addChild( child );
            child->setParent( parent );

        }
        
        // erase the nodes also from the origin nodes vector
        n.clear();
        
        // insert the parent to our list
        n.push_back( parent );
    }
    else
    {
        throw RbException("Unexpected number of taxa in constrained tree simulation");
    }
    
    
}


void UniformTopologyDistribution::simulateTree( void )
{
    
    // the time tree object (topology + times)
//    Tree *psi = new Tree();
    Tree *psi = value;
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( false );
    
    // create the tip nodes
    std::vector<TopologyNode*> nodes;
    for (size_t i=0; i<numTaxa; i++)
    {
        
        // create the i-th taxon
        TopologyNode* node = new TopologyNode( taxa[i], i );
        
        // add the new node to the list
        nodes.push_back( node );
        
    }
    
    // we need a sorted vector of constraints, starting with the smallest
    std::vector<Clade> sorted_clades = constraints;
    
    // create a clade that contains all species
    Clade all_species = Clade(taxa);
    sorted_clades.push_back(all_species);
    
    // next sort the clades
    std::sort(sorted_clades.begin(),sorted_clades.end());
    
    // remove duplicates
    std::vector<Clade> tmp;
    tmp.push_back( sorted_clades[0] );
    for (size_t i = 1; i < sorted_clades.size(); ++i)
    {
        Clade &a = tmp[tmp.size()-1];
        Clade &b = sorted_clades[i];
        
        if ( a.size() != b.size() )
        {
            tmp.push_back( sorted_clades[i] );
        }
        else
        {
            bool equal = true;
            for (size_t j = 0; j < a.size(); ++j)
            {
                if ( a.getTaxon(j) != b.getTaxon(j) )
                {
                    equal = false;
                    break;
                }
            }
            if ( equal == false )
            {
                tmp.push_back( sorted_clades[i] );
            }
        }
        
    }
    sorted_clades = tmp;
    
    std::vector<Clade> virtual_taxa;
    for (size_t i = 0; i < sorted_clades.size(); ++i)
    {
        
        Clade &c = sorted_clades[i];
        std::vector<Taxon> taxa = c.getTaxa();
        std::vector<Clade> clades;
        
        for (int j = int(i)-1; j >= 0; --j)
        {
            const Clade &c_nested = sorted_clades[j];
            const std::vector<Taxon> &taxa_nested = c_nested.getTaxa();
            
            bool found_all = true;
            bool found_some = false;
            for (size_t k = 0; k < taxa_nested.size(); ++k)
            {
                std::vector<Taxon>::iterator it = std::find(taxa.begin(), taxa.end(), taxa_nested[k]);
                if ( it != taxa.end() )
                {
                    taxa.erase( it );
                    found_some = true;
                }
                else
                {
                    found_all = false;
                }
                
            }
            
            if ( found_all == true )
            {
                //                c.addTaxon( virtual_taxa[j] );
                //                taxa.push_back( virtual_taxa[j] );
                clades.push_back( virtual_taxa[j] );
            }
            
            if ( found_all == false && found_some == true )
            {
                throw RbException("Cannot simulate tree: conflicting monophyletic clade constraints. Check that all clade constraints are properly nested.");
            }
            
        }
        
        std::vector<TopologyNode*> nodes_in_clade;
        
        
        for (size_t k = 0; k < taxa.size(); ++k)
        {
            clades.push_back( Clade( taxa[k] ) );
        }
        
        for (size_t k = 0; k < clades.size(); ++k)
        {
            for (size_t j = 0; j < nodes.size(); ++j)
            {
                if (nodes[j]->getClade() == clades[k])
                {
                    nodes_in_clade.push_back( nodes[j] );
                    nodes.erase( nodes.begin()+j );
                    break;
                }
                
            }
            
        }
        
        
        simulateClade( nodes_in_clade, i < (sorted_clades.size()-1) );
        nodes.push_back( nodes_in_clade[0] );
        
        std::vector<Taxon> v_taxa;
        nodes_in_clade[0]->getTaxa(v_taxa);
        virtual_taxa.push_back( Clade(v_taxa) );
        
    }
    
    TopologyNode *root = nodes[0];
    
    // initialize the topology by setting the root
    psi->setRoot(root);
    
    // finally store the new value
//    delete value;
//    value = psi;
    
    
    
    
//    
//    
//    // Get the rng
//    RandomNumberGenerator* rng = GLOBAL_RNG;
//    
//    TopologyNode* root = new TopologyNode();
//    std::vector<TopologyNode* > nodes;
////    nodes.push_back(root);
//        
//    
//    // add a left child
//    TopologyNode* leftChild = new TopologyNode(0);
//    root->addChild(leftChild);
//    leftChild->setParent(root);
//    nodes.push_back(leftChild);
//    
//    // add a middle child
//    TopologyNode* middleChild = new TopologyNode(0);
//    root->addChild(middleChild);
//    middleChild->setParent(root);
//    nodes.push_back(middleChild);
//    
//    // add a right child
//    TopologyNode* rightChild = new TopologyNode(0);
//    root->addChild(rightChild);
//    rightChild->setParent(root);
//    nodes.push_back(rightChild);
//    
//    // recursively build the tree
//    buildRandomBinaryTree(nodes);
//    
//    // set tip names
//    for (size_t i=0; i<numTaxa; i++)
//    {
//        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
//        
//        // get the node from the list
//        TopologyNode* node = nodes.at(index);
//        
//        // remove the randomly drawn node from the list
//        nodes.erase(nodes.begin()+long(index));
//        
//        // set name
//        std::string& name = taxonNames[i];
//        node->setName(name);
//    }
//    
//    value->setRoot( root );
//    
//    // re-couple tip names and tip indices (WP)
//    for (size_t i=0; i<numTaxa; i++)
//    {
//        value->getTipNodeWithName(taxonNames[i]).setIndex(i);
//    }
//    
//    value->orderNodesByIndex();
}


/** Swap a parameter of the distribution */
void UniformTopologyDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
}

/**
 * We check here if all the constraints are satisfied.
 * These are hard constraints, that is, the clades must be monophyletic.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool UniformTopologyDistribution::matchesConstraints( void ) 
{
    
    if (constraints.empty())
    {
		return true;
	}
    else
    {
		
		const TopologyNode &root = value->getRoot();
		for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it) 
		{
			if ( !root.containsClade( *it, true ) ) 
			{
				return false;
			}
		}
        
		return true;
	}
    
}
