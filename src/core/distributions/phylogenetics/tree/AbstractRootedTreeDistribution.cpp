#include "Clade.h"
#include "AbstractRootedTreeDistribution.h"
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
 * \param[in]    o         Origin or time of the process.
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
AbstractRootedTreeDistribution::AbstractRootedTreeDistribution(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const std::vector<Taxon> &tn) : TypedDistribution<Tree>( new Tree() ),
    origin( o ),
    root_age( ra ),
    num_taxa( tn.size() ),
    taxa( tn ),
    starts_at_root( origin == NULL )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( origin );
    addParameter( root_age );
    
    if ( (origin == NULL && root_age == NULL) || (origin != NULL && root_age != NULL) )
    {
        throw RbException("You have to condition either on the origin or on the root age.");
    }
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= num_taxa; i++)
    {
        lnFact += std::log(i);
    }
    
    log_tree_topology_prob = (num_taxa - 1) * RbConstants::LN2 - lnFact ;
    
}


AbstractRootedTreeDistribution::~AbstractRootedTreeDistribution(void)
{
    
}


void AbstractRootedTreeDistribution::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
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


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double AbstractRootedTreeDistribution::computeLnProbability( void )
{
    
    
    // check that the ages are in correct chronological order
    // i.e., no child is older than its parent
    const std::vector<TopologyNode*>& nodes = value->getNodes();
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isRoot() == false )
        {
            
            if ( (the_node.getAge() - (*it)->getParent().getAge()) > 0 && the_node.isSampledAncestor() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( (the_node.getAge() - (*it)->getParent().getAge()) > 1E-6 && the_node.isSampledAncestor() == true )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // check that the sampled ancestor nodes have a zero branch length
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isSampledAncestor() == true )
        {
            
            if ( the_node.isFossil() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( the_node.getBranchLength() > 1E-6 )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // present time
    double ra = value->getRoot().getAge();
    double present_time = 0.0;
    
    // test that the time of the process is larger or equal to the present time
    if ( starts_at_root == false )
    {
        double org = origin->getValue();
        
        // test that the time of the process is larger or equal to the present time
        if ( ra > org )
        {
            return RbConstants::Double::neginf;
        }
        
        present_time = org;
        
    }
    else
    {
        present_time = ra;
        
        if ( ra != root_age->getValue() )
        {
            return RbConstants::Double::neginf;
        }
        
        const std::vector<TopologyNode*> &c = value->getRoot().getChildren();
        
        for (std::vector<TopologyNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
        {
            if ( ra < (*it)->getAge() )
            {
                return RbConstants::Double::neginf;
            }
        }
    }
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeLnProbabilityDivergenceTimes();
    
    return lnProbTimes + log_tree_topology_prob;
}


/**
 * Get the divergence times meassured since the time of the origin.
 * We get the ages from the nodes and simply subtruct these from the time of the origin.
 *
 * \return     A vector of times. The caller needs to deallocate this vector.
 */
std::vector<double>* AbstractRootedTreeDistribution::divergenceTimesSinceOrigin( void ) const
{
    
    // get the time of the process
    double org = 0.0;
    if ( starts_at_root )
    {
        org = root_age->getValue();
    }
    else
    {
        org = origin->getValue();
    }
    
    // retrieved the speciation times
    std::vector<double> *times = new std::vector<double>();
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i)
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double t = org - n.getAge();
        times->push_back(t);
    }
    // sort the vector of times in ascending order
    std::sort(times->begin(), times->end());
    
    return times;
}


/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int AbstractRootedTreeDistribution::diversity(double t) const
{
    std::vector<double>* times = divergenceTimesSinceOrigin();
    
    for (size_t i = 0; i < times->size(); ++i)
    {
        if ( (*times)[i] > t )
        {
            delete times;
            return int( i + 2 );
        }
    }
    
    int rv = int(times->size() + 2);
    delete times;
    
    return rv;
}


/**
 * Get the affected nodes by a change of this node.
 * If the root age has changed than we need to call get affected again.
 */
void AbstractRootedTreeDistribution::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == root_age)
    {
        dagNode->getAffectedNodes( affected );
    }
    
}


/**
 * Get the age of the internal nodes meassured since the time of the most recent tip.
 * We get the ages from the nodes and simply subtract these from the age of the most recent tip.
 *
 * \return     A vector of ages. The caller needs to deallocate this vector.
 */
std::vector<double>* AbstractRootedTreeDistribution::getAgesOfInternalNodesFromMostRecentSample( void ) const
{
    
    double minTipAge = 0.0;
    const std::vector<TopologyNode*> &nodes = value->getNodes();
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        
        const TopologyNode& n = *(nodes[i]);
        if ( n.isTip() == true )
        {
            double tipAge = value->getNode( i ).getAge();
            if ( tipAge < minTipAge)
            {
                minTipAge = tipAge;
            }
            
        }
        
    }
    
    // retrieved the speciation times
    std::vector<double> *ages = new std::vector<double>();
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        
        const TopologyNode& n = *(nodes[i]);
        if ( n.isInternal() == true )
        {
            double t = n.getAge() - minTipAge;
            ages->push_back(t);
        }
        
    }
    // sort the vector of times in ascending order
    std::sort(ages->begin(), ages->end());
    
    return ages;
}


/**
 * Get the age of all tip nodes meassured since the time of the most recent tip.
 * We get the ages from the nodes and simply subtruct these from the age of the most recent tip.
 *
 * \return     A vector of ages. The caller needs to deallocate this vector.
 */
std::vector<double>* AbstractRootedTreeDistribution::getAgesOfTipsFromMostRecentSample( void ) const
{
    
    double minTipAge = 0.0;
    const std::vector<TopologyNode*> &nodes = value->getNodes();
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        
        const TopologyNode& n = *(nodes[i]);
        if ( n.isTip() == true )
        {
            double tipAge = value->getNode( i ).getAge();
            if ( tipAge < minTipAge)
            {
                minTipAge = tipAge;
            }
            
        }
        
    }
    
    // retrieved the speciation times
    std::vector<double> *ages = new std::vector<double>();
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        
        const TopologyNode& n = *(nodes[i]);
        if ( n.isTip() == true && n.isSampledAncestor() == false )
        {
            double t = n.getAge() - minTipAge;
            ages->push_back(t);
            
        }
        
    }
    
    // sort the vector of times in ascending order
    std::sort(ages->begin(), ages->end());
    
    return ages;
}



/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void AbstractRootedTreeDistribution::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        dagNode->keepAffected();
    }
    
}


/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void AbstractRootedTreeDistribution::redrawValue( void )
{
    
    simulateTree();
    
}


/**
 *
 */
void AbstractRootedTreeDistribution::simulateClade(std::vector<TopologyNode *> &n, double age, double present)
{

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // get the minimum age
    double current_age = n[0]->getAge();
    for (size_t i = 1; i < n.size(); ++i)
    {

        if ( current_age > n[i]->getAge() )
        {
            current_age = n[i]->getAge();
        }

    }
    

    while ( n.size() > 2 && current_age < age )
    {

        // get all the nodes before the current age
        std::vector<TopologyNode*> active_nodes;
        for (size_t i = 0; i < n.size(); ++i)
        {

            if ( current_age >= n[i]->getAge() )
            {
                active_nodes.push_back( n[i] );
            }

        }

        // we need to get next age of a node larger than the current age
        double next_node_age = age;
        for (size_t i = 0; i < n.size(); ++i)
        {

            if ( current_age < n[i]->getAge() && n[i]->getAge() < next_node_age )
            {
                next_node_age = n[i]->getAge();
            }

        }

        // only simulate if there are at least to valid/active nodes
        if ( active_nodes.size() < 2 )
        {
            current_age = next_node_age;
        }
        else
        {

            // now we simulate new ages
            double next_sim_age = simulateNextAge(active_nodes.size()-1, present-age, present-current_age, present);

            if ( next_sim_age < next_node_age )
            {

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
                parent->setAge( next_sim_age );

                // insert the parent to our list
                n.push_back( parent );
                
                current_age = next_sim_age;
            }
            else
            {
                current_age = next_node_age;
            }

        }
        
    }


    if ( n.size() == 2 )
    {

        // pick two nodes
        TopologyNode* left_child = n[0];
        TopologyNode* right_right = n[1];

        // erase the nodes also from the origin nodes vector
        n.clear();

        // create a parent for the two
        TopologyNode *parent = new TopologyNode();
        parent->addChild( left_child );
        parent->addChild( right_right );
        left_child->setParent( parent );
        right_right->setParent( parent );
        parent->setAge( age );

        // insert the parent to our list
        n.push_back( parent );
    }
    else
    {
        throw RbException("Unexpected number of taxa in constrained tree simulation");
    }


}



double AbstractRootedTreeDistribution::simulateNextAge(size_t n, double start, double end, double present) const
{

    std::vector<double> *ages = simulateDivergenceTimes(n, start, end);

    double next_age = (*ages)[0];

    delete ages;

    return next_age + present - end;

}


/**
 *
 */
void AbstractRootedTreeDistribution::simulateTree( void )
{

    // the time tree object (topology & times)
    Tree *psi = new Tree();

    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( true );

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


    double ra = 1.0;
    if ( root_age != NULL )
    {
        ra = root_age->getValue();
    }
    else
    {
        ra = origin->getValue();
    }
    
    double present = ra;

    // we need a sorted vector of constraints, starting with the smallest
    std::vector<Clade> sorted_clades;

    std::vector<Clade> constraints;

    for (size_t i = 0; i < constraints.size(); ++i)
    {
        if (constraints[i].getAge() > ra)
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

            clade_age = rng->uniform01() * ( ra - max_node_age ) + max_node_age;
        }

        simulateClade(nodes_in_clade, clade_age, present);
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

    // finally store the new value
    delete value;
    value = psi;

}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void AbstractRootedTreeDistribution::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dagNode->restoreAffected();
    }
    
}


/**
 * Set the current value.
 */
void AbstractRootedTreeDistribution::setValue(Tree *v, bool f )
{
    
    // delegate to super class
    TypedDistribution<Tree>::setValue(v, f);
    
    
    if ( root_age != NULL )
    {
        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(root_age);
        if ( stoch_root_age != NULL )
        {
            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
        }
        else
        {
            //            double factor = root_age->getValue() / value->getRoot().getAge();
            //            TreeUtilities::rescaleTree( value, &value->getRoot(), factor);
            
            value->getRoot().setAge( root_age->getValue() );
        }
        
    }
    
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void AbstractRootedTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    if ( oldP == origin )
    {
        origin = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if ( oldP == root_age )
    {
        root_age = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void AbstractRootedTreeDistribution::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == root_age )
    {
        value->getRoot().setAge( root_age->getValue() );
        dagNode->touchAffected();
    }
    
}