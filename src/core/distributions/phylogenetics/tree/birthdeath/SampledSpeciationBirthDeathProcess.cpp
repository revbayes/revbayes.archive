#include "Clade.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include "SampledSpeciationBirthDeathProcess.h"

#include <algorithm>
#include <cmath>
#include <boost/numeric/odeint.hpp>

using namespace RevBayesCore;

SampledSpeciationBirthDeathProcess::SampledSpeciationBirthDeathProcess( const TypedDagNode<double> *a, const TypedDagNode<double> *s, const TypedDagNode<double> *e, const TypedDagNode< double > *r, const std::vector<Taxon> &n) : AbstractCharacterHistoryBirthDeathProcess(),
root_age( a ),
speciation( s ),
extinction( e ),
rho( r ),
branch_histories( NULL, 1, 1),
taxa( n ),
activeLikelihood( std::vector<size_t>(2*n.size()-1, 0) ),
storedLikelihood( std::vector<std::vector<double> >(2*n.size()-1, std::vector<double>(2, 0.0))),
changedNodes( std::vector<bool>(2*n.size()-1, false) ),
dirtyNodes( std::vector<bool>(2*n.size()-1, true) ),
scalingFactors( std::vector<std::vector<double> >(2*n.size()-1, std::vector<double>(2,0.0) ) ),
totalScaling( 0.0 )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( root_age );
    addParameter( speciation );
    addParameter( extinction );
    addParameter( rho );
    
    num_taxa = taxa.size();
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= num_taxa; i++)
    {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (num_taxa - 1) * RbConstants::LN2 - lnFact ;
    
    simulateTree();
    
}


SampledSpeciationBirthDeathProcess::~SampledSpeciationBirthDeathProcess()
{
    
}


/**
 * Recursive call to attach ordered interior node times to the time tree psi. Call it initially with the
 * root of the tree.
 */
void SampledSpeciationBirthDeathProcess::attachTimes(Tree* psi, std::vector<TopologyNode *> &nodes, size_t index, const std::vector<double> &interiorNodeTimes, double originTime )
{
    
    if (index < num_taxa-1)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // Randomly draw one node from the list of nodes
        size_t node_index = static_cast<size_t>( floor(rng->uniform01()*nodes.size()) );
        
        // Get the node from the list
        TopologyNode* parent = nodes.at(node_index);
        psi->getNode( parent->getIndex() ).setAge( originTime - interiorNodeTimes[index] );
        
        // Remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(node_index));
        
        // Add the left child if an interior node
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() )
        {
            nodes.push_back(leftChild);
        }
        
        // Add the right child if an interior node
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() )
        {
            nodes.push_back(rightChild);
        }
        
        // Recursive call to this function
        attachTimes(psi, nodes, index+1, interiorNodeTimes, originTime);
    }
    
}


/** Build random binary tree to size numTaxa. The result is a draw from the uniform distribution on histories. */
void SampledSpeciationBirthDeathProcess::buildRandomBinaryHistory(std::vector<TopologyNode*> &tips)
{
    
    if (tips.size() < num_taxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // Randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // Get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // Remove the randomly drawn node from the list
        tips.erase(tips.begin()+long(index));
        
        // Add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // Add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // Recursive call to this function
        buildRandomBinaryHistory(tips);
    }
}


/* Clone function */
SampledSpeciationBirthDeathProcess* SampledSpeciationBirthDeathProcess::clone( void ) const
{
    
    return new SampledSpeciationBirthDeathProcess( *this );
}


/* Compute probability */
double SampledSpeciationBirthDeathProcess::computeLnProbability( void )
{
    // for now
    totalScaling = 0;
    
    // Variable declarations and initialization
    double lnProb = 0.0;
    double age = root_age->getValue();
    
    // we need to check that the root age matches
    if ( age != value->getRoot().getAge() )
    {
        return RbConstants::Double::neginf;
    }
    
    
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
    
    
    // add the survival of a second species if we condition on the MRCA
    lnProb += computeRootLikelihood();
    
    return lnProb + logTreeTopologyProb;
}


void SampledSpeciationBirthDeathProcess::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t node_index)
{
    
    // check for recomputation
    bool bypassDirtyFlag = !false;
    if ( dirtyNodes[node_index] || bypassDirtyFlag )
    {
        // mark as computed
        dirtyNodes[node_index] = false;
        
        const BranchHistory& bh = branch_histories[ node_index ];
        const std::multiset<CharacterEvent*,CharacterEventCompare>& hist = bh.getHistory();
        
        double lnProb = 0.0;
        if ( node.isTip() )
        {
            // this is a tip node
            lnProb = log(rho->getValue());
        }
        else
        {
            // this is an internal node
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            computeNodeProbability( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            computeNodeProbability( right, rightIndex );
        }
        
        // process parameters
        const double &birth = speciation->getValue();
        const double &death = extinction->getValue();
        const double birthPlusDeath = birth + death;
        const double logBirth = log(birth);

        // branch/tree variables
        double branch_length = node.getBranchLength();
        double prev_age      = (node.isRoot() ? node.getAge() : node.getParent().getAge() );
        double end_age       = 0.0; // NB: assumes the process ends at the present, T==0
        double prev_time     = 0.0;

//        std::cout << "---\n";
//        std::cout << "branch  \t" << node.getParent().getAge() << " " << node.getAge() << "\n";
//        std::cout << "prev_age\t" << prev_age << "\n";
        
        // compute probability for the observed and sampled speciation events on the branch
        for (std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it=hist.begin(); it!=hist.end(); ++it)
        {
            CharacterEvent* event = *it;
            double curr_time = event->getTime();
            double time_interval = (curr_time - prev_time) * branch_length;
            double curr_age = prev_age - time_interval;

            // compute the probability that the next event was a birth event
            lnProb += logBirth - birthPlusDeath * time_interval;
   
            // compute probability one lineage goes extinct by the present
            lnProb += computeLnProbExtinctByPresent(-curr_age, -end_age);
            
//            std::cout << "time_int\t" << time_interval << "\n";
//            std::cout << "(prev_age, curr_age)\t(" << prev_age << ", " << curr_age << ")\n";
//            std::cout << exp(computeLnProbExtinctByPresent(-curr_age, -end_age)) << " = e^computeLnProbExtinctByPresent(" << -curr_age << ", " << -end_age << ")\n";

            // advance time
            prev_time = curr_time;
            prev_age  = curr_age;
        }
        
        double time_interval = ( 1.0 - prev_time ) * branch_length;
        if ( node.isTip() ) {
            // if node is a tip, no further events occurred
            lnProb += -birthPlusDeath * time_interval;
        }
        else {
            // if node is not a tip, the next event is a speciation event
            lnProb += logBirth - birthPlusDeath * time_interval;
        }
        
//        std::cout << "time_int\t" << time_interval << "\n";
//        std::cout << "curr_age\t" << prev_age - time_interval << "\n";
        
        storedLikelihood[node_index][activeLikelihood[node_index]] = lnProb;
    }
}

double SampledSpeciationBirthDeathProcess::computeLnProbExtinctByPresent(double t_low, double t_high)
{
    
    // We want the probability that a lineage leaves no descendants after time (t_high-t_low)
    double birthRate = speciation->getValue();
    double deathRate = extinction->getValue();
    double samplingProb = rho->getValue();
    double prob = 1.0 - (samplingProb * (birthRate - deathRate)) /
                        (samplingProb * birthRate + ( (1.0 - samplingProb) * birthRate - deathRate) * exp(-(birthRate - deathRate) * (t_high - t_low)));
    
    double lnProb = log(prob);

    return lnProb;
}


// r == 1.0: sampling prob
//double SampledSpeciationBirthDeathProcess::lnP1(double t, double T, double r) const
//{
//    
//    // get the survival probability
//    double a = log( pSurvival(t,T,r) );
//    double b = rateIntegral(t, T) - log(r);
//    
//    // compute the probability of observing/sampling exactly one lineage
//    double p = 2.0 * a + b;  // exp(2*a + b)
//    
//    return p;
//    
//}

//double ConstantRateBirthDeathProcess::rateIntegral(double t_low, double t_high) const
//{
//    
//    double rate = (speciation->getValue() - extinction->getValue()) * (t_low - t_high);
//    
//    return rate;
//}


//double BirthDeathProcess::pSurvival(double start, double end, double r) const
//{
//    double rate = rateIntegral(start, end);
//    double ps = 1.0 / pSurvival(start, end);
//    
//    return 1.0 / (ps - (r-1.0)/r * exp(rate) );
//}


//double ConstantRateBirthDeathProcess::pSurvival(double start, double end) const
//{
//    
//    // compute the rate
//    double mu = extinction->getValue();
//    double lambda = speciation->getValue();
//    double rate = mu - lambda;
//    
//    // do the integration of int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds )
//    // where rate(t,s) = int_{t}^{s} ( mu(x)-lambda(x) dx )
//    
//    //    double den = 1.0 + ( exp(-rate*start) * mu / rate ) * ( exp(rate*end) - exp(rate*start) );
//    double den = 1.0 + mu / rate * ( exp(rate*(end-start)) - 1 );
//    
//    return (1.0 / den);
//}



double SampledSpeciationBirthDeathProcess::computeRootLikelihood( void )
{
    
    const TopologyNode &root = value->getRoot();

    // fill the likelihoods
    const TopologyNode &left = root.getChild(0);
    size_t leftIndex = left.getIndex();
    computeNodeProbability( left, leftIndex );
    const TopologyNode &right = root.getChild(1);
    size_t rightIndex = right.getIndex();
    computeNodeProbability( right, rightIndex );
    
    // sum lnProbs across all nodes
    double lnProb = 0.0;
    for (size_t i = 0; i < storedLikelihood.size(); ++i)
    {
        
        lnProb += storedLikelihood[i][activeLikelihood[i]];
    }
    
    return lnProb;
}


void SampledSpeciationBirthDeathProcess::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<int> &rv) const
{
    
    if ( n == "numberEvents" )
    {
        size_t num_branches = branch_histories.getNumberBranches();
        rv.clear();
        rv.resize( num_branches );
        
        for (size_t i = 0; i < num_branches; ++i)
        {
            rv[i] = int(branch_histories[i].getNumberEvents());
        }
        
    }
    else
    {
        throw RbException("The heterogeneous rate birth-death process does not have a member method called '" + n + "'.");
    }
    
}

/**
 * Get the character history object.
 */
CharacterHistory& SampledSpeciationBirthDeathProcess::getCharacterHistory( void )
{
    
    return branch_histories;
}




void SampledSpeciationBirthDeathProcess::redrawValue( void )
{
    simulateTree();
}


void SampledSpeciationBirthDeathProcess::setValue(Tree *v, bool force)
{
    
    // delegate to the parent class
    TypedDistribution< Tree >::setValue(v, force);
    
    branch_histories.setTree( value );
    
    simulateEvents();
}

/** Simulate events for the given tree */
void SampledSpeciationBirthDeathProcess::simulateEvents( void )
{
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // reset histories
    branch_histories.setTree( value );
    
    for (size_t i = 0; i < value->getNumberOfNodes(); i++)
    {
        TopologyNode& node = value->getNode(i);
        if (!node.isRoot())
        {
            double startAge = node.getParent().getAge();
            double endAge = node.getAge();
            double currAge = startAge;
            while (currAge > endAge)
            {
                double t = RbStatistics::Exponential::rv(speciation->getValue(), *rng);
                currAge -= t;
                
                if (currAge > endAge)
                {
                    double p = exp(computeLnProbExtinctByPresent(-currAge, 0.0));
                    double u = rng->uniform01();
                    if (u < p)
                    {
                        double pos = (startAge - currAge) / (startAge - endAge);
                        CharacterEvent* evt = new CharacterEvent(0, 0, pos);
                        
                        branch_histories.addEvent(evt, i);
                    }
                }
            }
        }
    }
    
//    std::cout << branch_histories.getNumberEvents() << "\n";
//    std::cout << "done!\n";
}

/** Simulate the tree conditioned on the time of origin */
void SampledSpeciationBirthDeathProcess::simulateTree( void )
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Create the time tree object (topology + times)
    Tree *psi = new Tree();
    
    // Root the topology by setting the appropriate flag
    psi->setRooted( true );
    
    // Create the root node and a vector of nodes
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
    nodes.push_back(root);
    
    // Draw a random tree history
    buildRandomBinaryHistory(nodes);
    
    // Set the tip names
    for (size_t i=0; i<num_taxa; i++)
    {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // Get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // Remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(index) );
        
        // Set taxon
        node->setTaxon( taxa[i] );
    }
    
    // Initialize the topology by setting the root
    psi->setRoot(root);
    
    // Now simulate the speciation times counted from originTime
    std::vector<double> intNodeTimes;
    double              t_or = root_age->getValue();
    intNodeTimes.push_back( 0.0 );  // For time of mrca
    for ( size_t i=0; i<num_taxa-2; i++ )
    {
        double t = rng->uniform01() * t_or;
        intNodeTimes.push_back( t );
    }
    
    // Sort the speciation times from 0.0 (root node) to the largest value
    std::sort( intNodeTimes.begin(), intNodeTimes.end() );
    
    // Attach times
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, intNodeTimes, t_or);
    for (size_t i = 0; i < num_taxa; ++i)
    {
        TopologyNode& node = psi->getTipNode(i);
        psi->getNode( node.getIndex() ).setAge( 0.0 );
    }
    
    // Finally store the new value
    value = psi;
    
    // set branch histories to the tree (this clears branch_histories)
    branch_histories.setTree( psi );
    
    // simulate and add events
    simulateEvents();
    
}

void SampledSpeciationBirthDeathProcess::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == root_age)
    {
        dagNode->getAffectedNodes( affected );
    }
    
}

/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void SampledSpeciationBirthDeathProcess::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        dagNode->keepAffected();
    }
    
}

/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void SampledSpeciationBirthDeathProcess::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dagNode->restoreAffected();
    }
    
}


/** Swap a parameter of the distribution */
void SampledSpeciationBirthDeathProcess::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == root_age)
    {
        root_age = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == speciation)
    {
        speciation = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == extinction)
    {
        extinction = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho)
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void SampledSpeciationBirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dagNode->touchAffected();
    }
    
}
