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
    num_taxa( tn.size() ),
    taxa( tn ),
	constraints( c ),
    logTreeTopologyProb( RbConstants::Double::nan )
{
    
    double lnFact = 0.0;
    for (size_t i = 2; i < num_taxa; i++)
    {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (num_taxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( num_taxa ) ;
    
    simulateTree();
    
}


UniformTopologyDistribution::~UniformTopologyDistribution()
{
    // the tree will be deleted automatically by the base class
    
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
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    while ( n.size() >= 2 )
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
    
    
}


void UniformTopologyDistribution::simulateTree( void )
{
    
    // the tree object
//    Tree *psi = new Tree();
    Tree *psi = value;
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( false );
    
    // create the tip nodes
    std::vector<TopologyNode*> ingroup_nodes;
    std::vector<TopologyNode*> outgroup_nodes;
    for (size_t i=0; i<num_taxa; ++i)
    {
        
        // create the i-th taxon
        TopologyNode* node = new TopologyNode( taxa[i], i );
        
        bool is_outgroup = false;
        for (size_t j = 0; j < a.size(); ++j)
        {
            if ( outgroup.getTaxon(j) != taxa[i]) )
            {
                equal = true;
                break;
            }
        }
        
        // add the new node to the list
        if ( is_outgroup == true )
        {
            outgroup_nodes.push_back( node );
        }
        else
        {
            ingroup_nodes.push_back( node );
        }
        
    }
    
    // we need a sorted vector of constraints, starting with the smallest
    std::vector<Clade> sorted_clades = constraints;
    
    // create a clade that contains all species
    Clade all_ingroup_species = Clade(ingroup_taxa);
    sorted_clades.push_back(all_ingroup_species);
    
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
        
        
        simulateClade( nodes_in_clade );
        nodes.push_back( nodes_in_clade[0] );
        
        std::vector<Taxon> v_taxa;
        nodes_in_clade[0]->getTaxa(v_taxa);
        virtual_taxa.push_back( Clade(v_taxa) );
        
    }
    
    TopologyNode *root = nodes[0];
    
    // initialize the topology by setting the root
    psi->setRoot(root);
    
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
    
    if ( constraints.empty() == true )
    {
		return true;
	}
    else
    {
		
		const TopologyNode &root = value->getRoot();
		for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it) 
		{
			if ( root.containsClade( *it, true ) == false )
			{
				return false;
			}
		}
        
		return true;
	}
    
}
