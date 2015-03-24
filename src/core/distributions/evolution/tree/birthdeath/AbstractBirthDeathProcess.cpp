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
#include "Topology.h"

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
                                                     const std::vector<Taxon> &tn, const std::vector<Clade> &c) : TypedDistribution<TimeTree>( new TimeTree() ),
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



///**
// * Randomly attach the times to a tree topology.
// * This function works by randomly picking a node from the set of tips,
// * setting its time to times[index], increment the index,
// * adding the two children (if they are not actual tips) to the set of tips,
// * and recursively calling this function again.
// *
// * \param[in]     psi        The tree topology (needed to call setAge).
// * \param[in]     tips       The vector of tips
// * \param[in]     index   
// * \param[in]     times         
// * \param[in]     T      
// */
//void AbstractBirthDeathProcess::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> *times, double T)
//{
//    
//    if (index < times->size() )
//    {
//        // Get the rng
//        RandomNumberGenerator* rng = GLOBAL_RNG;
//        
//        // randomly draw one node from the list of tips
//        size_t tip_index = size_t( floor(rng->uniform01()*tips.size()) );
//        
//        // get the node from the list
//        TopologyNode* parent = tips.at(tip_index);
//        psi->setAge( parent->getIndex(), T - (*times)[index] );
//        
//        // remove the randomly drawn node from the list
//        tips.erase(tips.begin()+ long(tip_index) );
//        
//        // add a left child
//        TopologyNode* leftChild = &parent->getChild(0);
//        if ( !leftChild->isTip() ) 
//        {
//            tips.push_back(leftChild);
//        }
//        
//        // add a right child
//        TopologyNode* rightChild = &parent->getChild(1);
//        if ( !rightChild->isTip() ) 
//        {
//            tips.push_back(rightChild);
//        }
//        
//        // recursive call to this function
//        attachTimes(psi, tips, index+1, times, T);
//    }
//    
//    // no return value
//}


std::vector<double> AbstractBirthDeathProcess::buildConstraintBinaryTree(std::vector<TopologyNode*> &nodes)
{
    // create a vector for the node ages
    std::vector<double> ages;
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    size_t nodeIndex = nodes.size();
    size_t remainingNodes = nodes.size();
    
    // we need a sorted vector of constraints, starting with the smallest
    std::vector<Clade> sortedClades = constraints;

    // create a clade that contains all species
//    Clade allSpecies = Clade(taxa);
//    sortedClades.push_back(allSpecies);
    
    // next sort the clades
    std::sort(sortedClades.begin(),sortedClades.end());
    
    // create the size of number of exclusive nodes per clade
    std::vector<size_t> taxaPerClade;
    
    // to do so we need all the taxon names first
    std::vector<std::vector<std::string> > taxonNamesPerClade;
    for (size_t i = 0; i < sortedClades.size(); ++i)
    {
        const Clade &c = sortedClades[i];
        taxonNamesPerClade.push_back( c.getTaxonNames() );
    }
    
    
    for (size_t i = 0; i < sortedClades.size(); ++i)
    {
        std::vector<std::string> &names_i = taxonNamesPerClade[i];
        for (size_t j = i+1; j < sortedClades.size(); ++j)
        {
            
            std::vector<std::string> &names_j = taxonNamesPerClade[j];
            
            for (size_t k = 0; k < names_i.size(); ++k)
            {
                
                std::vector<std::string>::iterator it = std::find(names_j.begin(), names_j.end(), names_i[k]);
                if ( it != names_j.end() )
                {
                    names_j.erase( it );
                }
                
            }
        }
    }
    
    
    while (remainingNodes > 1)
    {
        
        // randomly draw one node from the list of nodes
        size_t indexLeft = static_cast<size_t>( floor(rng->uniform01()*nodes.size()) );
        
        // get the node from the list
        TopologyNode* left = nodes.at(indexLeft);
        
        // remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(indexLeft));
        
        // randomly draw a second node from the list of nodes
        size_t indexRight = static_cast<size_t>( floor(rng->uniform01()*nodes.size()) );
        
        // get the node from the list
        TopologyNode* right = nodes.at(indexRight);
        
        // remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(indexRight));
        
        // create the parent node
        TopologyNode* parent = new TopologyNode(nodeIndex);
        
        // add both children
        parent->addChild(left);
        left->setParent(parent);
        parent->addChild(right);
        right->setParent(parent);
        
        // add the new parent to the list of nodes
        nodes.push_back(parent);
        
        // increase the node index counter
        ++nodeIndex;
        
        // decrement the remaining nodes counter
        --remainingNodes;
    }
    
    return ages;
}


//void AbstractBirthDeathProcess::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
//{
//    
//    if (tips.size() < numTaxa) 
//    {
//        // Get the rng
//        RandomNumberGenerator* rng = GLOBAL_RNG;
//        
//        // randomly draw one node from the list of tips
//        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
//        
//        // get the node from the list
//        TopologyNode* parent = tips.at(index);
//        
//        // remove the randomly drawn node from the list
//        tips.erase(tips.begin()+long(index));
//        
//        // add a left child
//        TopologyNode* leftChild = new TopologyNode(0);
//        parent->addChild(leftChild);
//        leftChild->setParent(parent);
//        tips.push_back(leftChild);
//        
//        // add a right child
//        TopologyNode* rightChild = new TopologyNode(0);
//        parent->addChild(rightChild);
//        rightChild->setParent(parent);
//        tips.push_back(rightChild);
//        
//        // recursive call to this function
//        buildRandomBinaryTree(tips);
//    }
//}


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
        if ( !(*it)->isRoot() )
        {
            if ( (*it)->getAge() >= (*it)->getParent().getAge() )
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
    
    double tipTime = value->getTipNode(0).getTime();
    double offset = org - tipTime;
    
    // retrieved the speciation times
    std::vector<double> *times = new std::vector<double>();
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i) 
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double t = n.getTime() + offset;
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
        double tipAge = value->getAge( i );
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
        double tipAge = value->getAge( i );
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
    
    // the time tree object (topology + times)
    TimeTree *psi = new TimeTree();
    
    // Draw a random topology
    Topology *tau = new Topology();
    
    // internally we treat unrooted topologies the same as rooted
    tau->setRooted( true );
    
    std::vector<TopologyNode* > nodes;
    
    // set tip names
    for (size_t i=0; i<numTaxa; i++) 
    {
        size_t index = i;
        
        // create the node from the list
        TopologyNode* node = new TopologyNode(index);
        
        // set name
        const std::string& name = taxa[i].getName();
        node->setName(name);
        node->setSpeciesName(taxa[i].getSpeciesName());
        
        // add the node to the list
        nodes.push_back( node );
        
    }
    
    // now simulate the speciation times
    // first, get the time of the origin
	double t_or = 0.0;
    size_t numInitialSpecies = 1;
    if ( startsAtRoot == true )
    {
        t_or = rootAge->getValue();
        numInitialSpecies = 2;
    }
    else
    {
        t_or = origin->getValue();
    }
    
    
    // draw a time for each speciation event condition on the time of the process
    std::vector<double> *times = simSpeciations(numTaxa-numInitialSpecies, t_or);
    
    // recursively build the tree
    buildConstraintBinaryTree(nodes);
    
    TopologyNode *root = nodes[0];
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // connect the tree with the topology
    psi->setTopology( tau, true );
    
    nodes.clear();
    
    
    // We need to tell the tree the ages because the nodes do not store this information
    for (size_t i = 0; i < (2*numTaxa-numInitialSpecies); ++i)
    {
        TopologyNode& node = tau->getNode(i);
        if ( node.isTip() )
        {
            // we should use here the provided ages instead
            psi->setAge( i, 0.0 );
        }
        else
        {
            psi->setAge( i, (*times)[i-numTaxa] );
        }
    }
    
    // we may also need to set the root age (if it wasn't randomly drawn)
    if ( startsAtRoot == true )
    {
        psi->setAge(root->getIndex(), t_or);
    }
    
    // free the memory for the times.
    delete times;
    
    // reset the listeners
    const std::set<TreeChangeEventListener*> l = value->getTreeChangeEventHandler().getListeners();
    
    for (std::set<TreeChangeEventListener*>::const_iterator it = l.begin(); it != l.end(); ++it)
    {
        value->getTreeChangeEventHandler().removeListener( *it );
        psi->getTreeChangeEventHandler().addListener( *it );
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
        value->setAge(value->getRoot().getIndex(), rootAge->getValue() );
        dagNode->restoreAffected();
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
void AbstractBirthDeathProcess::touchSpecialization(DagNode *affecter)
{
    
    if ( affecter == rootAge )
    {
        value->setAge(value->getRoot().getIndex(), rootAge->getValue() );
        dagNode->touchAffected();
    }
    
}
