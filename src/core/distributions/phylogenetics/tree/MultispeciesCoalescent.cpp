#include "Clade.h"
#include "ConstantNode.h"
#include "MultispeciesCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

MultispeciesCoalescent::MultispeciesCoalescent(const TypedDagNode<Tree> *sp,
                                               const std::vector<Taxon> &t) : TypedDistribution<Tree>( NULL ),
    taxa(t),
    species_tree( sp ),
    Nes( NULL ),
    Ne( new ConstantNode<double>("Ne", new double(1.0) ) ),
    num_taxa( taxa.size() ),
    log_tree_topology_prob (0.0)
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( species_tree );
    addParameter( Ne );
    
    std::set<std::string> species_names;
    for (std::vector<Taxon>::const_iterator it=taxa.begin(); it!=taxa.end(); ++it)
    {
        species_names.insert( it->getSpeciesName() );
    }
    
//    if ( sp->getValue().getNumberOfTips() != species_names.size() )
//    {
//        throw RbException("Unequal number of species between species tree and gene tree.");
//    }
    
    double ln_fact = RbMath::lnFactorial((int)(num_taxa));
    
    log_tree_topology_prob = (num_taxa - 1) * RbConstants::LN2 - 2.0 * ln_fact - std::log( num_taxa ) ;
    
    redrawValue();
    
    

}


MultispeciesCoalescent::~MultispeciesCoalescent()
{
    
}



void MultispeciesCoalescent::attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times) {
    
    if (index < num_taxa-1)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->getNode( parent->getIndex() ).setAge( times[num_taxa - index - 2] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);
        
        // add a left child
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() )
        {
            tips.push_back(leftChild);
        }
        
        // add a right child
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() )
        {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(psi, tips, index+1, times);
    }
}


void MultispeciesCoalescent::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
{
    
    if (tips.size() < num_taxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
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


MultispeciesCoalescent* MultispeciesCoalescent::clone( void ) const
{
    
    return new MultispeciesCoalescent( *this );
}


double MultispeciesCoalescent::computeLnProbability( void )
{
    
    // variable declarations and initialization
    double lnProbCoal = 0;
    
    const Tree &sp = species_tree->getValue();
    
    const std::vector< TopologyNode* > &speciesTreeNodes = sp.getNodes();
    // first let's create a map from species names to the nodes of the species tree
    std::map<std::string, TopologyNode * > speciesNames2speciesNodes;
    for (std::vector< TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            const std::string &name = (*it)->getName();
            speciesNames2speciesNodes[name] = *it;
        }
    }
    
    
    // create a map from individual name to the actual tip node for convenience
    std::map< std::string, TopologyNode*> individualNames2geneTreeTips;
    for ( size_t i = 0; i < num_taxa; ++i)
    {
        TopologyNode *tip = &value->getTipNode( i );
        individualNames2geneTreeTips[ tip->getName() ] = tip;
    }
    
    
    std::map< const TopologyNode *, std::set< TopologyNode* > > individualsPerBranch;
    
    for (std::vector< Taxon >::iterator it = taxa.begin(); it != taxa.end(); ++it)
    {
        const std::string &tipName = it->getName();
        TopologyNode *n = individualNames2geneTreeTips[tipName];
        const std::string &speciesName = it->getSpeciesName();
        TopologyNode *speciesNode = speciesNames2speciesNodes[speciesName];
        std::set< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.insert( n );
    }

    /*
    for (std::map<std::string,std::string>::iterator it = gene2species.begin(); it != gene2species.end(); ++it)
    {
        const std::string &tipName = it->first;
        TopologyNode *n = individualNames2geneTreeTips[tipName];
        const std::string &speciesName = it->second;
        TopologyNode *speciesNode = speciesNames2speciesNodes[speciesName];
        std::set< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.insert( n );
    }
    */
    std::map<double, TopologyNode*> speciesAge2speciesNodes;
    double i = -0.01;
    for (std::vector<TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            speciesAge2speciesNodes[ i ] = (*it);
            i -= 0.01;
        }
        else
        {
            speciesAge2speciesNodes[ (*it)->getAge() ] = (*it);
        }
    }
    
    // we loop over the nodes of the species tree in phylogenetic traversal
    for (std::map<double, TopologyNode *>::const_iterator it = speciesAge2speciesNodes.begin(); it != speciesAge2speciesNodes.end(); ++it)
    {
        TopologyNode *spNode = it->second;
        const TopologyNode *spParentNode = NULL;
        double speciesAge = spNode->getAge();
        double parentSpeciesAge = RbConstants::Double::inf;
//        double branchLength = RbConstants::Double::inf;
        if ( !spNode->isRoot() )
        {
            spParentNode = &spNode->getParent();
            parentSpeciesAge = spParentNode->getAge();
//            branchLength = parentSpeciesAge - speciesAge;
        }
        
        // create a local copy of the individuals per branch
        std::set<TopologyNode*> remainingIndividuals = individualsPerBranch[spNode];
        
        // get all coalescent events among the individuals
        std::set<double> coalTimes;
        
        
        std::map<double, TopologyNode *> coalTimes2coalNodes;
        for ( std::set<TopologyNode*>::iterator remNode = remainingIndividuals.begin(); remNode != remainingIndividuals.end(); ++remNode)
        {
            TopologyNode *rn = (*remNode);
            if ( !rn->isRoot() )
            {
                TopologyNode &parent = rn->getParent();
                double parentAge = parent.getAge();
                coalTimes2coalNodes[ parentAge ] = &parent;
            }
        }
        
        double currentTime = speciesAge;
        while ( currentTime < parentSpeciesAge && coalTimes2coalNodes.size() > 0 )
        {
            
            TopologyNode *parent = coalTimes2coalNodes.begin()->second;
            double parentAge = parent->getAge();
			double branchNe = getNe(spNode->getIndex());
			double theta = 1.0 / branchNe;
            
            if ( parentAge < parentSpeciesAge )
            { //Coalescence in the species tree branch
                
                // get the left and right child of the parent
                TopologyNode *left = &parent->getChild( 0 );
                TopologyNode *right = &parent->getChild( 1 );
                if ( remainingIndividuals.find( left ) == remainingIndividuals.end() || remainingIndividuals.find( right ) == remainingIndividuals.end() )
                {
                    // one of the children does not belong to this species tree branch
                    return RbConstants::Double::neginf;
                }
                
				//We remove the coalescent event and the coalesced lineages
                coalTimes2coalNodes.erase( coalTimes2coalNodes.begin() );
                remainingIndividuals.erase( remainingIndividuals.find( left ) );
                remainingIndividuals.erase( remainingIndividuals.find( right ) );
                
				//We insert the parent in the vector of lineages in this branch
                remainingIndividuals.insert( parent );
                if ( !parent->isRoot() )
                {
                    TopologyNode *grandParent = &parent->getParent();
                    coalTimes2coalNodes[ grandParent->getAge() ] = grandParent;
                }
                
				//a is the time between the previous and the current coalescences
                double a = parentAge - currentTime;
                currentTime = parentAge;
                
                // now we do the computation
                
                // get the number j of individuals we had before the current coalescence
                size_t j = remainingIndividuals.size() + 1;
                // compute the number of pairs: pairs = C(j choose 2) = j * (j-1.0) / 2.0
                double nPairs = j * (j-1.0) / 2.0;
                double lambda = nPairs * theta;
                
                // add the density for this coalescent event
                //lnProbCoal += log( lambda ) - lambda * a;
				//Corrected version:
                lnProbCoal += log( theta ) - lambda * a;
				
                
                
            } //End if coalescence in the species tree branch
            else
            { //No more coalescences in this species tree branch
                
                // compute the probability of no coalescent event in the final part of the branch
                // only do this if the branch is not the root branch
                if ( spParentNode != NULL )
                {
                    double finalInterval = parentSpeciesAge - currentTime;
                    size_t k = remainingIndividuals.size();
                    lnProbCoal -= k*(k-1.0)/2.0 * finalInterval * theta;
                }
                
                // jump out of the while loop
//                currentTime = speciesAge;
                break;
            }
            
            
        } // end of while loop
        
        
        // merge the two sets of individuals that go into the next species
        if ( spParentNode != NULL )
        {
            std::set<TopologyNode *> &incomingLineages = individualsPerBranch[spParentNode];
            incomingLineages.insert( remainingIndividuals.begin(), remainingIndividuals.end());
        }
        
        
    } // end loop over all nodes in the species tree (in phylogenetic ordering)
    
    
    return lnProbCoal; // + logTreeTopologyProb;
    
}


double  MultispeciesCoalescent::getNe(size_t index) const
{
    
    if (Ne != NULL)
    {
        return Ne->getValue();
    }
    else if (Nes != NULL)
    {
        return Nes->getValue()[index];
    }
    else
    {
        std::cerr << "Error: Null Pointers for Ne and Nes." << std::endl;
        exit(-1);
    }
}



void MultispeciesCoalescent::redrawValue( void )
{
    
    simulateTree();
    
}

void MultispeciesCoalescent::setNes(TypedDagNode< RbVector<double> >* input_nes)
{

    removeParameter( Nes );
    removeParameter( Ne );
    
    Nes = input_nes;
    Ne  = NULL;
    
    addParameter( Nes );

}


void MultispeciesCoalescent::setNe(TypedDagNode<double>* input_ne)
{

    removeParameter( Ne );
    removeParameter( Nes );
    
    Ne  = input_ne;
    Nes = NULL;
    
    addParameter( Ne );
}




void MultispeciesCoalescent::simulateTree( void )
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    const Tree &sp = species_tree->getValue();
    const std::vector< TopologyNode* > &species_tree_nodes = sp.getNodes();
    // first let's create a map from species names to the nodes of the species tree
    std::map<std::string, TopologyNode * > species_names_2_nodes;
    for (std::vector< TopologyNode *>::const_iterator it = species_tree_nodes.begin(); it != species_tree_nodes.end(); ++it)
    {
        if ( (*it)->isTip() == true )
        {
            const std::string &name = (*it)->getName();
            species_names_2_nodes[name] = *it;
        }
    }
    
    
    std::map< const TopologyNode *, std::vector< TopologyNode* > > individuals_per_branch;
    
    for (std::vector< Taxon >::iterator it = taxa.begin(); it != taxa.end(); ++it)
    {
        TopologyNode *n = new TopologyNode( *it );
        const std::string &species_name = n->getSpeciesName();
        
        if ( species_name == "" )
        {
            throw RbException("Cannot match a taxon without species to a tip in the species tree. The taxon map is probably wrong.");
        }
        
        TopologyNode *species_node = species_names_2_nodes[species_name];
        n->setAge( 0.0 );
        std::vector< TopologyNode * > &nodes_at_node = individuals_per_branch[ species_node ];
        nodes_at_node.push_back( n );
    }

    
 /*   for (std::map<std::string,std::string>::iterator it = gene2species.begin(); it != gene2species.end(); ++it)
    {
        const std::string &tipName = it->first;
        TopologyNode *n = new TopologyNode( tipName );
        const std::string &speciesName = it->second;
        TopologyNode *speciesNode = speciesNames2Nodes[speciesName];
        std::vector< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.push_back( n );
    }
    */
    
    std::map<TopologyNode *, double> nodes_2_ages;
    TopologyNode *root = NULL;
    // we loop over the nodes of the species tree in phylogenetic traversal
    for (std::vector<TopologyNode *>::const_iterator it = species_tree_nodes.begin(); it != species_tree_nodes.end(); ++it)
    {
        TopologyNode *sp_node = *it;
        const TopologyNode *sp_parent_node = NULL;
        double branch_length = RbConstants::Double::inf;
        if ( sp_node->isRoot() == false )
        {
            sp_parent_node = &sp_node->getParent();
            branch_length = sp_parent_node->getAge() - sp_node->getAge();
        }
        
        std::vector<TopologyNode*> initial_individuals_at_branch = individuals_per_branch[sp_node];
        double branch_ne = getNe(sp_node->getIndex() );

        double theta = 1.0 / branch_ne;
        
        double prev_coalescent_time = 0.0;
        
        size_t j = initial_individuals_at_branch.size();
        double n_pairs = j * (j-1) / 2.0;
        double lambda = n_pairs * theta;
        double u = RbStatistics::Exponential::rv( lambda, *rng);
        double next_coalescent_time = prev_coalescent_time + u;
        
        while ( next_coalescent_time < branch_length && j > 1 )
        {
            // randomly coalesce two lineages
            size_t index = static_cast<size_t>( floor(rng->uniform01()*initial_individuals_at_branch.size()) );
            TopologyNode *left = initial_individuals_at_branch[index];
            initial_individuals_at_branch.erase( initial_individuals_at_branch.begin() + index);
            
            index = static_cast<size_t>( floor(rng->uniform01()*initial_individuals_at_branch.size()) );
            TopologyNode *right = initial_individuals_at_branch[index];
            initial_individuals_at_branch.erase( initial_individuals_at_branch.begin() + index);
            
            TopologyNode *new_parent = new TopologyNode();
            new_parent->addChild(left);
            left->setParent(new_parent);
            new_parent->addChild(right);
            right->setParent(new_parent);
            
            root = new_parent;
            
            if ( root == NULL )
            {
                std::cerr << "Oh, the root is NULL :(" << std::endl;
            }
            
            initial_individuals_at_branch.push_back( new_parent );
            
            nodes_2_ages[new_parent] = next_coalescent_time + sp_node->getAge();
            
            
            prev_coalescent_time = next_coalescent_time;
            j--;
            n_pairs = j * (j-1) / 2.0;
            lambda = n_pairs * theta ;
            u = RbStatistics::Exponential::rv( lambda, *rng);
            next_coalescent_time = prev_coalescent_time + u;
        }
        
        if ( sp_parent_node != NULL )
        {
            std::vector<TopologyNode *> &incoming_lineages = individuals_per_branch[sp_parent_node];
            incoming_lineages.insert(incoming_lineages.end(), initial_individuals_at_branch.begin(), initial_individuals_at_branch.end());
        }
        
        
    }
    
    // the time tree object (topology + times)
    Tree *psi = new Tree();
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( true );
    
    // initialize the topology by setting the root
    psi->setRoot(root, true);
    
    for ( std::map<TopologyNode*, double>::iterator it = nodes_2_ages.begin(); it != nodes_2_ages.end(); ++it)
    {
        TopologyNode *node = it->first;
        node->setAge( it->second );
    }
    
    // finally store the new value
    value = psi;
    
}


/** Swap a parameter of the distribution */
void MultispeciesCoalescent::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == Nes )
    {
        Nes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
    if ( oldP == Ne )
    {
        Ne = static_cast<const TypedDagNode< double >* >( newP );
    }
    
    if ( oldP == species_tree )
    {
        species_tree = static_cast<const TypedDagNode< Tree >* >( newP );
    }
    
}
