#include "Clade.h"
#include "AbstractBirthDeathProcess.h"
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
AbstractBirthDeathProcess::AbstractBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const std::string &cdt,
                                                     const std::vector<Taxon> &tn, const std::vector<Clade> &c) : TypedDistribution<Tree>( new Tree() ),
    condition( cdt ),
    constraints( c ),
    origin( o ),
    rootAge( ra ),
    numTaxa( tn.size() ),
    taxa( tn ),
    startsAtRoot( origin == NULL )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( origin );
    addParameter( rootAge );
    
    if ( (origin == NULL && rootAge == NULL) || (origin != NULL && rootAge != NULL) )
    {
        throw RbException("You have to condition either on the origin or on the root age.");
    }
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= numTaxa; i++)
    {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - lnFact ;
    //    logTreeTopologyProb = 0.0; // TAH: this is for checking likelihoods to BEAST2
    
}



/**
 * Randomly attach the times to a tree topology.
 * This function works by randomly picking a node from the set of tips,
 * setting its time to times[index], increment the index,
 * adding the two children (if they are not actual tips) to the set of tips,
 * and recursively calling this function again.
 *
 * \param[in]     psi        The tree topology (needed to call setAge).
 * \param[in]     tips       The vector of tips
 * \param[in]     index
 * \param[in]     times
 * \param[in]     T
 */
void AbstractBirthDeathProcess::attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> *times, double T)
{
    
    if (index < times->size() )
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = size_t( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->getNode( parent->getIndex() ).setAge( T - (*times)[index] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+ long(tip_index) );
        
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
        attachTimes(psi, tips, index+1, times, T);
    }
    
    // no return value
}


void AbstractBirthDeathProcess::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
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


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double AbstractBirthDeathProcess::computeLnProbability( void )
{
    
    // prepare probability computation
    prepareProbComputation();
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // first check if the current tree matches the clade constraints
    if ( !matchesConstraints() )
    {
        return RbConstants::Double::neginf;
    }
    
    // present time
    double ra = value->getRoot().getAge();
    double presentTime = 0.0;
    
    // test that the time of the process is larger or equal to the present time
    if ( startsAtRoot == false )
    {
        double org = origin->getValue();
        
        // test that the time of the process is larger or equal to the present time
        if ( ra > org )
        {
            return RbConstants::Double::neginf;
        }
        
        presentTime = org;
        
    }
    else
    {
        presentTime = ra;
        
        if ( ra != rootAge->getValue() )
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
    
    const std::vector<TopologyNode*>& nodes = value->getNodes();
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isRoot() == false )
        {
            if ( the_node.getAge() >= (*it)->getParent().getAge() && the_node.isSampledAncestor() == false )
            {
                return RbConstants::Double::neginf;
            }
        }
    }
    
    
    // what do we condition on?
    // did we condition on survival?
    if ( condition == "survival" )
    {
        lnProbTimes = - log( pSurvival(0,presentTime) );
    }
    
    // if we started at the root then we square the survival prob
    if ( startsAtRoot == true )
    {
        lnProbTimes *= 2.0;
    }
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeLnProbabilityTimes();
    
    return lnProbTimes + logTreeTopologyProb;
    
}


/**
 * Get the divergence times meassured since the time of the origin.
 * We get the ages from the nodes and simply subtruct these from the time of the origin.
 *
 * \return     A vector of times. The caller needs to deallocate this vector.
 */
std::vector<double>* AbstractBirthDeathProcess::divergenceTimesSinceOrigin( void ) const
{
    
    // get the time of the process
    double org = 0.0;
    if ( startsAtRoot )
    {
        org = rootAge->getValue();
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
int AbstractBirthDeathProcess::diversity(double t) const
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
void AbstractBirthDeathProcess::getAffected(std::set<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == rootAge)
    {
        dagNode->getAffectedNodes( affected );
    }
    
}


/**
 * Get the age of the internal nodes meassured since the time of the most recent tip.
 * We get the ages from the nodes and simply subtruct these from the age of the most recent tip.
 *
 * \return     A vector of ages. The caller needs to deallocate this vector.
 */
std::vector<double>* AbstractBirthDeathProcess::getAgesOfInternalNodesFromMostRecentSample( void ) const
{
    
    // get the time of the process
    
    double minTipAge = 0.0;
    for (size_t i = 0; i < numTaxa; ++i)
    {
        double tipAge = value->getNode( i ).getAge();
        if ( tipAge < minTipAge)
        {
            minTipAge = tipAge;
        }
    }
    
    // retrieved the speciation times
    std::vector<double> *ages = new std::vector<double>();
    for (size_t i = numTaxa; i < 2*numTaxa-1; ++i)
    {
        const TopologyNode& n = value->getNode( i );
        double t = n.getAge() - minTipAge;
        ages->push_back(t);
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
std::vector<double>* AbstractBirthDeathProcess::getAgesOfTipsFromMostRecentSample( void ) const
{
    
    // get the time of the process
    
    double minTipAge = 0.0;
    for (size_t i = 0; i < numTaxa; ++i)
    {
        double tipAge = value->getNode( i ).getAge();
        if ( tipAge < minTipAge)
        {
            minTipAge = tipAge;
        }
    }
    
    // retrieved the speciation times
    std::vector<double> *ages = new std::vector<double>();
    for (size_t i = 0; i < numTaxa; ++i)
    {
        const TopologyNode& n = value->getNode( i );
        double t = n.getAge() - minTipAge;
        ages->push_back(t);
    }
    // sort the vector of times in ascending order
    std::sort(ages->begin(), ages->end());
    
    return ages;
}



/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void AbstractBirthDeathProcess::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == rootAge )
    {
        dagNode->keepAffected();
    }
    
}



/**
 * We check here if all the constraints are satisfied.
 * These are hard constraints, that is, the clades must be monophyletic.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool AbstractBirthDeathProcess::matchesConstraints( void )
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
 * Prepare the probability computation. Here we can pre-calculate some values for more
 * efficient probability calculation. The derived classes may want to do something ...
 */
void AbstractBirthDeathProcess::prepareProbComputation( void )
{
    
}


/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void AbstractBirthDeathProcess::redrawValue( void )
{
    
    simulateTree();
    
}


/**
 *
 */
void AbstractBirthDeathProcess::simulateTree( void )
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // the time tree object (topology + times)
    Tree *psi = new Tree();
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( true );
    
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
    nodes.push_back(root);
    
    // recursively build the tree
    buildRandomBinaryTree(nodes);
    
    // set tip names
    for (size_t i=0; i<numTaxa; i++)
    {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // remove the randomly drawn node from the list
        nodes.erase( nodes.begin()+long(index) );
        
        // set name
        const std::string& name = taxa[i].getName();
        node->setName(name);
        node->setSpeciesName(taxa[i].getSpeciesName());
    }
    
    // initialize the topology by setting the root
    psi->setRoot(root);
    
    // now simulate the speciation times
    // first, get the time of the origin
    double t_or = 0.0;
    size_t numInitialSpecies = 1;
    if ( startsAtRoot == true )
    {
        t_or = rootAge->getValue();
        psi->getNode( root->getIndex() ).setAge( t_or );
        numInitialSpecies = 2;
    }
    else
    {
        t_or = origin->getValue();
    }
    
    nodes.clear();
    
    if ( numInitialSpecies < numTaxa)
    {
        // draw a time for each speciation event condition on the time of the process
        std::vector<double> *times = simSpeciations(numTaxa-numInitialSpecies, t_or);
        
        if ( startsAtRoot )
        {
            // add a left child
            TopologyNode* leftChild = &root->getChild(0);
            if ( !leftChild->isTip() )
            {
                nodes.push_back(leftChild);
            }
            
            // add a right child
            TopologyNode* rightChild = &root->getChild(1);
            if ( !rightChild->isTip() )
            {
                nodes.push_back(rightChild);
            }
            attachTimes(psi, nodes, 0, times, t_or);
        }
        else
        {
            nodes.push_back( root );
            attachTimes(psi, nodes, 0, times, t_or);
        }
        
        
        delete times;
    }
    
    // \todo Why are we doing this? (Sebastian)
    for (size_t i = 0; i < numTaxa; ++i)
    {
        TopologyNode& node = psi->getTipNode(i);
        psi->getNode( node.getIndex() ).setAge( 0.0 );
    }
    
    // finally store the new value
    delete value;
    value = psi;
    
}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void AbstractBirthDeathProcess::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == rootAge )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( rootAge->getValue() );
        dagNode->restoreAffected();
    }
    
}


/**
 * Set the current value.
 */
void AbstractBirthDeathProcess::setValue(Tree *v, bool f )
{
    
    // delegate to super class
    TypedDistribution<Tree>::setValue(v, f);
    
    
    if ( rootAge != NULL )
    {
        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(rootAge);
        if ( stoch_root_age != NULL )
        {
            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
        }
        else
        {
//            double factor = rootAge->getValue() / value->getRoot().getAge();
//            TreeUtilities::rescaleTree( value, &value->getRoot(), factor);

            value->getRoot().setAge( rootAge->getValue() );
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
void AbstractBirthDeathProcess::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    if ( oldP == origin )
    {
        origin = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if ( oldP == rootAge )
    {
        rootAge = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void AbstractBirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == rootAge )
    {
        value->getRoot().setAge( rootAge->getValue() );
        dagNode->touchAffected();
    }
    
}