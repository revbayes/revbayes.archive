#include "AbstractRootedTreeDistribution.h"
#include "Clade.h"
#include "TopologyConstrainedTreeDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "Taxon.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    c         Clade constraints.
 */
TopologyConstrainedTreeDistribution::TopologyConstrainedTreeDistribution(TypedDistribution<Tree> *base_dist, const std::vector<Clade> &c) : TypedDistribution<Tree>( NULL ),
    base_distribution( base_dist ),
    constraints( c ),
    owns_tree( false )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    
    // add the parameters of the distribution
    const std::vector<const DagNode*>& pars = base_distribution->getParameters();
    for (std::vector<const DagNode*>::const_iterator it = pars.begin(); it != pars.end(); ++it)
    {
        this->addParameter( *it );
    }
    
    if ( owns_tree == true )
    {
        value = base_distribution->getValue().clone();
    }
    else
    {
        value = &base_distribution->getValue();
    }
    
    redrawValue();
}


/**
 * Copy Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    c         Clade constraints.
 */
TopologyConstrainedTreeDistribution::TopologyConstrainedTreeDistribution(const TopologyConstrainedTreeDistribution &d) : TypedDistribution<Tree>( d ),
    base_distribution( d.base_distribution->clone() ),
    constraints( d.constraints )
{
    // the copy constructor of the TypedDistribution creates a new copy of the value
    // however, here we want to hold exactly the same value as the base-distribution
    // thus, we delete the newly created value
    delete value;
    
    // and then set it to the value of the base distribution
    if ( owns_tree == true )
    {
        value = base_distribution->getValue().clone();
    }
    else
    {
        value = &base_distribution->getValue();
    }
    
}



TopologyConstrainedTreeDistribution::~TopologyConstrainedTreeDistribution()
{
    
    delete base_distribution;
    
    // DO NOT DELETE THE VALUE
    // the base distribution is the actual owner of the value!!!
    // we simply avoid the deletion of the value by setting its pointer to NULL
    // our base class, the TypedDistribution thinks that it owns the value and thus deletes it
    if ( owns_tree == false )
    {
        value = NULL;
    }
    
}


TopologyConstrainedTreeDistribution* TopologyConstrainedTreeDistribution::clone( void ) const
{
    
    return new TopologyConstrainedTreeDistribution( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double TopologyConstrainedTreeDistribution::computeLnProbability( void )
{
    
    // first check if the current tree matches the clade constraints
    if ( !matchesConstraints() )
    {
        return RbConstants::Double::neginf;
    }
    
    
    // since we and the base distribution own the same value,
    // we do not need to set the value of the base distribution
    if ( owns_tree == true )
    {
        base_distribution->setValue( value->clone() );
    }
    
    double lnProb = base_distribution->computeLnProbability();
    
    return lnProb;
}


/**
 * We check here if all the constraints are satisfied.
 * These are hard constraints, that is, the clades must be monophyletic.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool TopologyConstrainedTreeDistribution::matchesConstraints( void )
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


/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void TopologyConstrainedTreeDistribution::redrawValue( void )
{
    
    Tree* new_value = simulateTree();
    // base_distribution->redrawValue();
    
    // if we own the tree, then we need to free the memory before we create a new random variable
    if ( owns_tree == true )
    {
        delete value;
//        value = base_distribution->getValue().clone();
        value = new_value;
        base_distribution->setValue( value->clone() );
    }
    else
    {
        // if we don't own the tree, then we just replace the current pointer with the pointer
        // to the new value of the base distribution
//        value = &base_distribution->getValue();
        value = new_value;
        base_distribution->setValue( value );
    }
    
}


///**
// *
// */
//void TopologyConstrainedTreeDistribution::simulateClade(std::vector<TopologyNode *> &n, double age)
//{
//    
//    // Get the rng
//    RandomNumberGenerator* rng = GLOBAL_RNG;
//    
//    // get the minimum age
//    double current_age = n[0]->getAge();
//    for (size_t i = 1; i < n.size(); ++i)
//    {
//        
//        if ( current_age > n[i]->getAge() )
//        {
//            current_age = n[i]->getAge();
//        }
//        
//    }
//    
//    while ( n.size() > 2 && current_age < age )
//    {
//        
//        // get all the nodes before the current age
//        std::vector<TopologyNode*> active_nodes;
//        for (size_t i = 0; i < n.size(); ++i)
//        {
//            
//            if ( current_age >= n[i]->getAge() )
//            {
//                active_nodes.push_back( n[i] );
//            }
//            
//        }
//        
//        // we need to get next age of a node larger than the current age
//        double next_node_age = age;
//        for (size_t i = 0; i < n.size(); ++i)
//        {
//            
//            if ( current_age < n[i]->getAge() && n[i]->getAge() < next_node_age )
//            {
//                next_node_age = n[i]->getAge();
//            }
//            
//        }
//        
//        // only simulate if there are at least to valid/active nodes
//        if ( active_nodes.size() < 2 )
//        {
//            current_age = next_node_age;
//        }
//        else
//        {
//            
//            // now we simulate new ages
//            double next_sim_age = simNextAge(active_nodes.size()-1, current_age, age);
//            
//            if ( next_sim_age < next_node_age )
//            {
//                
//                // randomly pick two nodes
//                size_t index_left = static_cast<size_t>( floor(rng->uniform01()*active_nodes.size()) );
//                TopologyNode* left_child = active_nodes[index_left];
//                active_nodes.erase(active_nodes.begin()+long(index_left));
//                size_t index_right = static_cast<size_t>( floor(rng->uniform01()*active_nodes.size()) );
//                TopologyNode* right_right = active_nodes[index_right];
//                active_nodes.erase(active_nodes.begin()+long(index_right));
//                
//                // erase the nodes also from the origin nodes vector
//                n.erase(std::remove(n.begin(), n.end(), left_child), n.end());
//                n.erase(std::remove(n.begin(), n.end(), right_right), n.end());
//                
//                
//                // create a parent for the two
//                TopologyNode *parent = new TopologyNode();
//                parent->addChild( left_child );
//                parent->addChild( right_right );
//                left_child->setParent( parent );
//                right_right->setParent( parent );
//                parent->setAge( next_sim_age );
//                
//                // insert the parent to our list
//                n.push_back( parent );
//                
//                current_age = next_sim_age;
//            }
//            else
//            {
//                current_age = next_node_age;
//            }
//            
//        }
//        
//    }
//    
//    
//    if ( n.size() == 2 )
//    {
//        
//        // pick two nodes
//        TopologyNode* left_child = n[0];
//        TopologyNode* right_right = n[1];
//        
//        // erase the nodes also from the origin nodes vector
//        n.clear();
//        
//        // create a parent for the two
//        TopologyNode *parent = new TopologyNode();
//        parent->addChild( left_child );
//        parent->addChild( right_right );
//        left_child->setParent( parent );
//        right_right->setParent( parent );
//        parent->setAge( age );
//        
//        // insert the parent to our list
//        n.push_back( parent );
//    }
//    else
//    {
//        throw RbException("Unexpected number of taxa in constrained tree simulation");
//    }
//    
//    
//}


//double TopologyConstrainedTreeDistribution::simNextAge(size_t n, double start, double end) const
//{
//    
//    std::vector<double> *ages = simSpeciations(n, end-start);
//    
//    double next_age = (*ages)[ages->size()-1];
//    
//    delete ages;
//    
//    return end - next_age;
//    
//}

///**
// *
// */
//void TopologyConstrainedTreeDistribution::simulateTree( void )
//{

/**
 *
 */
Tree* TopologyConstrainedTreeDistribution::simulateTree( void )
{
    
    // the time tree object (topology & times)
    Tree *psi = new Tree();
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( true );
    
    AbstractRootedTreeDistribution* tree_base_distribution = dynamic_cast<AbstractRootedTreeDistribution*>( base_distribution );
    size_t num_taxa = tree_base_distribution->getNumberOfTaxa();
    const std::vector<Taxon> &taxa = tree_base_distribution->getTaxa();
    
    // create the tip nodes
    std::vector<TopologyNode*> nodes;
    for (size_t i=0; i<num_taxa; ++i)
    {
        
        // create the i-th taxon
        TopologyNode* node = new TopologyNode( taxa[i], i );
        
        // set the age of this tip node
        node->setAge( taxa[i].getAge() );
        
        if (node->getAge() > 0)
        {
            node->setFossil(true);
        }
        
        // add the new node to the list
        nodes.push_back( node );
        
    }
    
    
    double ra = tree_base_distribution->getRootAge();
    double present = tree_base_distribution->getOriginTime();
    double max_age = std::max(ra,present);
    
    // we need a sorted vector of constraints, starting with the smallest
    std::vector<Clade> sorted_clades;
    
    for (size_t i = 0; i < constraints.size(); ++i)
    {
        if (constraints[i].getAge() > max_age)
        {
            throw RbException("Cannot simulate tree: clade constraints are older than the root age.");
        }
        
        // set the ages of each of the taxa in the constraint
        for (size_t j = 0; j < constraints[i].size(); ++j)
        {
            for (size_t k = 0; k < num_taxa; k++)
            {
                if ( taxa[k].getName() == constraints[i].getTaxonName(j) )
                {
                    constraints[i].setTaxonAge(j, taxa[k].getAge());
                    break;
                }
            }
        }
        
        if ( constraints[i].size() > 1 && constraints[i].size() < num_taxa )
        {
            sorted_clades.push_back( constraints[i] );
        }
    }
    
    // create a clade that contains all species
    Clade all_species = Clade(taxa);
    all_species.setAge( ra );
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
            Clade c = Clade( taxa[k] );
            c.setAge( taxa[k].getAge() );
            clades.push_back( c );
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
        
        double clade_age = c.getAge();
        
        double max_node_age = 0;
        for (size_t j = 0; j < nodes_in_clade.size(); ++j)
        {
            if ( nodes_in_clade[j]->getAge() > max_node_age )
            {
                max_node_age = nodes_in_clade[j]->getAge();
            }
        }
        if ( clade_age <= max_node_age )
        {
            // Get the rng
            RandomNumberGenerator* rng = GLOBAL_RNG;
            
            clade_age = rng->uniform01() * ( max_age - max_node_age ) + max_node_age;
        }

        tree_base_distribution->simulateClade(nodes_in_clade, clade_age, max_age);
        nodes.push_back( nodes_in_clade[0] );
        
        std::vector<Taxon> v_taxa;
        nodes_in_clade[0]->getTaxa(v_taxa);
        Clade new_clade = Clade(v_taxa);
        new_clade.setAge( nodes_in_clade[0]->getAge() );
        virtual_taxa.push_back( new_clade );
        
    }
    
    TopologyNode *root = nodes[0];
    
    // initialize the topology by setting the root
    psi->setRoot(root);
    
    return psi;
}


//    
//    // the time tree object (topology & times)
//    Tree *psi = new Tree();
//    
//    // internally we treat unrooted topologies the same as rooted
//    psi->setRooted( true );
//    
//    // create the tip nodes
//    std::vector<TopologyNode*> nodes;
//    for (size_t i=0; i<num_taxa; i++)
//    {
//        
//        // create the i-th taxon
//        TopologyNode* node = new TopologyNode( taxa[i], i );
//        
//        // set the age of this tip node
//        node->setAge( taxa[i].getAge() );
//        
//        if (node->getAge() > 0)
//        {
//            node->setFossil(true);
//        }
//        
//        // add the new node to the list
//        nodes.push_back( node );
//        
//    }
//    
//    
//    double root_age = 1.0;
//    if ( rootAge != NULL )
//    {
//        root_age = rootAge->getValue();
//    }
//    else
//    {
//        root_age = origin->getValue();
//    }
//    
//    
//    // we need a sorted vector of constraints, starting with the smallest
//    std::vector<Clade> sorted_clades;
//    
//    for (size_t i = 0; i < constraints.size(); ++i)
//    {
//        if (constraints[i].getAge() > root_age)
//        {
//            throw RbException("Cannot simulate tree: clade constraints are older than the root age.");
//        }
//        
//        // set the ages of each of the taxa in the constraint
//        for (size_t j = 0; j < constraints[i].size(); ++j)
//        {
//            for (size_t k = 0; k < num_taxa; k++)
//            {
//                if ( taxa[k].getName() == constraints[i].getTaxonName(j) )
//                {
//                    constraints[i].setTaxonAge(j, taxa[k].getAge());
//                    break;
//                }
//            }
//        }
//        
//        if ( constraints[i].size() > 1 && constraints[i].size() < num_taxa )
//        {
//            sorted_clades.push_back( constraints[i] );
//        }
//    }
//    
//    // create a clade that contains all species
//    Clade all_species = Clade(taxa, root_age);
//    sorted_clades.push_back(all_species);
//    
//    // next sort the clades
//    std::sort(sorted_clades.begin(),sorted_clades.end());
//    
//    // remove duplicates
//    std::vector<Clade> tmp;
//    tmp.push_back( sorted_clades[0] );
//    for (size_t i = 1; i < sorted_clades.size(); ++i)
//    {
//        Clade &a = tmp[tmp.size()-1];
//        Clade &b = sorted_clades[i];
//        
//        if ( a.size() != b.size() )
//        {
//            tmp.push_back( sorted_clades[i] );
//        }
//        else
//        {
//            bool equal = true;
//            for (size_t i = 0; i < a.size(); ++i)
//            {
//                if ( a.getTaxon(i) != b.getTaxon(i) )
//                {
//                    equal = false;
//                    break;
//                }
//            }
//            if ( equal == false )
//            {
//                tmp.push_back( sorted_clades[i] );
//            }
//        }
//        
//    }
//    sorted_clades = tmp;
//    
//    std::vector<Clade> virtual_taxa;
//    for (size_t i = 0; i < sorted_clades.size(); ++i)
//    {
//        
//        Clade &c = sorted_clades[i];
//        std::vector<Taxon> taxa = c.getTaxa();
//        std::vector<Clade> clades;
//        
//        for (int j = int(i)-1; j >= 0; --j)
//        {
//            const Clade &c_nested = sorted_clades[j];
//            const std::vector<Taxon> &taxa_nested = c_nested.getTaxa();
//            
//            bool found_all = true;
//            bool found_some = false;
//            for (size_t k = 0; k < taxa_nested.size(); ++k)
//            {
//                std::vector<Taxon>::iterator it = std::find(taxa.begin(), taxa.end(), taxa_nested[k]);
//                if ( it != taxa.end() )
//                {
//                    taxa.erase( it );
//                    found_some = true;
//                }
//                else
//                {
//                    found_all = false;
//                }
//                
//            }
//            
//            if ( found_all == true )
//            {
//                //                c.addTaxon( virtual_taxa[j] );
//                //                taxa.push_back( virtual_taxa[j] );
//                clades.push_back( virtual_taxa[j] );
//            }
//            
//            if ( found_all == false && found_some == true )
//            {
//                throw RbException("Cannot simulate tree: conflicting monophyletic clade constraints. Check that all clade constraints are properly nested.");
//            }
//            
//        }
//        
//        
//        std::vector<TopologyNode*> nodes_in_clade;
//        
//        
//        for (size_t k = 0; k < taxa.size(); ++k)
//        {
//            clades.push_back( Clade(taxa[k], taxa[k].getAge()) );
//        }
//        
//        for (size_t k = 0; k < clades.size(); ++k)
//        {
//            for (size_t j = 0; j < nodes.size(); ++j)
//            {
//                if (nodes[j]->getClade() == clades[k])
//                {
//                    nodes_in_clade.push_back( nodes[j] );
//                    nodes.erase( nodes.begin()+j );
//                    break;
//                }
//                
//            }
//            
//        }
//        
//        double clade_age = c.getAge();
//        
//        double max_node_age = 0;
//        for (size_t j = 0; j < nodes_in_clade.size(); ++j)
//        {
//            if ( nodes_in_clade[j]->getAge() > max_node_age )
//            {
//                max_node_age = nodes_in_clade[j]->getAge();
//            }
//        }
//        if ( clade_age <= max_node_age )
//        {
//            // Get the rng
//            RandomNumberGenerator* rng = GLOBAL_RNG;
//            
//            clade_age = rng->uniform01() * ( root_age - max_node_age ) + max_node_age;
//        }
//        
//        simulateClade(nodes_in_clade, clade_age);
//        nodes.push_back( nodes_in_clade[0] );
//        
//        std::vector<Taxon> v_taxa;
//        nodes_in_clade[0]->getTaxa(v_taxa);
//        virtual_taxa.push_back( Clade(v_taxa,nodes_in_clade[0]->getAge()) );
//        
//    }
//    
//    TopologyNode *root = nodes[0];
//    
//    // initialize the topology by setting the root
//    psi->setRoot(root);
//    
//    // finally store the new value
//    delete value;
//    value = psi;
//    
//}


/**
 * Set the DAG node.
 */
void TopologyConstrainedTreeDistribution::setStochasticNode( StochasticNode<Tree> *n )
{
    
    // delegate to base class first
    TypedDistribution<Tree>::setStochasticNode( n );
    
    if ( base_distribution != NULL )
    {
        base_distribution->setStochasticNode( n );
    }
    
}


/**
 * Set the current value.
 */
void TopologyConstrainedTreeDistribution::setValue(Tree *v, bool f )
{
    
    if ( owns_tree == true )
    {
        TypedDistribution<Tree>::setValue(v, f);
        
        // if we own the tree then we simply initialize the base distribution with a clone
        base_distribution->setValue(v->clone(), f);
    }
    else
    {
        // otherwise we set our value to the same value as the base distribution
        // but first we need to make sure that our base class doesn't delete the value
        value = NULL;
        
        // and the we can set it for both ourselves and the base distribution
        TypedDistribution<Tree>::setValue(v, f);
        base_distribution->setValue(v, f);
    }
    
//    if ( rootAge != NULL )
//    {
//        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(rootAge);
//        if ( stoch_root_age != NULL )
//        {
//            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
//        }
//        else
//        {
//            //            double factor = rootAge->getValue() / value->getRoot().getAge();
//            //            TreeUtilities::rescaleTree( value, &value->getRoot(), factor);
//            
//            value->getRoot().setAge( rootAge->getValue() );
//        }
//        
//    }
    
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void TopologyConstrainedTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    base_distribution->swapParameter(oldP,newP);
    
}

/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void TopologyConstrainedTreeDistribution::touchSpecialization(DagNode *affecter, bool touchAll)
{
    base_distribution->touch(affecter, touchAll);
    double a = base_distribution->getValue().getRoot().getAge();
    value->getRoot().setAge( a );
}

void TopologyConstrainedTreeDistribution::keepSpecialization(DagNode *affecter)
{
    base_distribution->keep(affecter);
    double a = base_distribution->getValue().getRoot().getAge();
    value->getRoot().setAge( a );
}

void TopologyConstrainedTreeDistribution::restoreSpecialization(DagNode *restorer)
{
    base_distribution->restore(restorer);
    double a = base_distribution->getValue().getRoot().getAge();
    value->getRoot().setAge( a );

}
