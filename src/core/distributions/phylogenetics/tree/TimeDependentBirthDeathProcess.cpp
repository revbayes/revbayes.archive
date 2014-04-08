
#include "BinaryMultiplication.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "ExponentialFunction.h"
#include "LinearStepFunction.h"
#include "GeneralUnivariateFunction.h"
#include "TimeDependentBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TrapezoidalRule.h"
#include "Topology.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

TimeDependentBirthDeathProcess::TimeDependentBirthDeathProcess(const TypedDagNode<UnivariateFunction> *b, const TypedDagNode<UnivariateFunction> *d, const TypedDagNode<double> *r, unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c) : TypedDistribution<TimeTree>( new TimeTree() ), constraints( c ), lambda( b ), mu( d ), rho( r ), numTaxa( nTaxa ), taxonNames( tn ), approximate( true ), init( false ) {
    
    // NULL settings
    diversificationRateIntegral = NULL;
    survivalProbIntegral        = NULL;
    
    addParameter( lambda );
    addParameter( mu );
    addParameter( rho );
    
    initialize();
    
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++) {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
}



TimeDependentBirthDeathProcess::TimeDependentBirthDeathProcess(const TimeDependentBirthDeathProcess &v) : TypedDistribution<TimeTree>( v ), lambda( v.lambda ), mu( v.mu ), rho( v.rho ), numTaxa( v.numTaxa ), taxonNames( v.taxonNames ), logTreeTopologyProb( v.logTreeTopologyProb ), approximate( v.approximate ) {
    // parameters are automatically copied

    init = false;
    
    // NULL settings
    diversificationRateIntegral = NULL;
    survivalProbIntegral        = NULL;
    
    initialize();
}


TimeDependentBirthDeathProcess::~TimeDependentBirthDeathProcess() {
    
    delete diversificationRateIntegral;
    delete survivalProbIntegral;
}



void TimeDependentBirthDeathProcess::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> times, double T) {
    
    if (index < numTaxa-1) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->setAge( parent->getIndex(), T - times[index] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);
        
        // add a left child
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() ) {
            tips.push_back(leftChild);
        }
        
        // add a right child
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() ) {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(psi, tips, index+1, times, T);
    }
}


void TimeDependentBirthDeathProcess::buildRandomBinaryTree(std::vector<TopologyNode*> &tips) {
    
    if (tips.size() < numTaxa) {
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


TimeDependentBirthDeathProcess* TimeDependentBirthDeathProcess::clone( void ) const {
    return new TimeDependentBirthDeathProcess( *this );
}


double TimeDependentBirthDeathProcess::computeLnProbability( void ) {
    
    
    if ( !matchesConstraints() ) {
        return RbConstants::Double::neginf;
    }
    
    int BLOCKS = 10000;
    if ( approximate ) {
            
        if ( !init ) {
            
            std::vector<double> x;
            // present time 
            double T = double(value->getTipNode(0).getTime());
            for (int i = 0; i <= BLOCKS; ++i) {
                x.push_back( i );
            }
            
            delete diversificationRateIntegral;
            diversificationRateIntegral = new LinearStepFunction( new ConstantNode<double>("", new double(0)), x, 0.0, T );
            
            delete survivalProbIntegral;
            survivalProbIntegral = new LinearStepFunction( new ConstantNode<double>("", new double(0)), x, 0.0, T );
            
            init = true;
        }
    }
    
    // retrieved the speciation times
    std::vector<double> times;
    for (size_t i = 0; i <= value->getNumberOfInteriorNodes(); ++i) {
        double t = value->getInteriorNode( i ).getTime();
        times.push_back(t);
    }
    // sort the vector of times in ascending order
    std::sort(times.begin(), times.end());
    
    // present time 
    double T = value->getTipNode(0).getTime();
    
    if ( approximate ) {
            
        // compute the rate integral
        std::vector<double> &area = diversificationRateIntegral->getPathValues();
        area[0] = 0.0;
        double prev = 0.0;
        double y;
        double window = T / BLOCKS;
        double x = window;
        double a = 0.0;
        TrapezoidalRule integrator = TrapezoidalRule();
        for (int i = 1; i <= BLOCKS; ++i, prev = x, x+=window) {
            y = mu->getValue().integrate(prev, x, integrator) - lambda->getValue().integrate(prev, x, integrator);
            a += y;
            area[i] = a;
        }
        
        // compute the survival probability integral ( int(mu(x)*exp(int(mu(s)-lambda(s)ds) dx))
        std::vector<double> &s_area = survivalProbIntegral->getPathValues();
        TypedDagNode<double> *param = new ConstantNode<double>("", new double(0.0));
        UnivariateFunction *dri = diversificationRateIntegral->clone();
        dri->swapParameter(dri->getParameter(), param);
        TypedDagNode<double> *integral = new DeterministicNode<double>("", dri);
        TypedDagNode<double> *e = new DeterministicNode<double>("", new ExponentialFunction(integral) );
        UnivariateFunction *extinction = mu->getValue().clone();
        extinction->swapParameter(extinction->getParameter(), param);
        TypedDagNode<double> *m = new DeterministicNode<double>("", extinction);
        TypedDagNode<double> *result = new DeterministicNode<double>("", new BinaryMultiplication<double, double, double>(m, e));
        std::set<DagNode*> nodes;
        nodes.insert(integral);
        nodes.insert(e);
        nodes.insert(m);
        nodes.insert(result);
        GeneralUnivariateFunction f = GeneralUnivariateFunction(new double(0.0), param, result, nodes);
        s_area[0] = 0.0;
        prev = 0.0;
        x = window;
        a = 0.0;
        for (int i = 1; i <= BLOCKS; ++i, prev = x, x+=window) {
            y = f.integrate(prev, x, integrator);
            a += y;
            s_area[i] = a;
        }
    }
    
    size_t numInitialSpecies = 1;
    double lnProbTimes = 0;
    // check if we condition on the root or origin
    if ( times[0] == 0.0 ) {
        //        lnProbTimes = - 2 * log( pSurvival(0,T) );
        ++numInitialSpecies;
    } else {
        lnProbTimes = log(pSurvival(0,T,T));
    }

    // for every speciation event
    for (size_t i=(numInitialSpecies-1); i < numTaxa-1; ++i) {
        // Multiply with the probability of no speciation between t_i and t_(i+1) if i species are alive in this interval and
        //multiply the probability density of observing a speciation event exactly at time t_(i+1)
        lnProbTimes += pWaiting(times[i-1],times[i],T,i+numInitialSpecies-1) + log(pSurvival(times[i],T,T)) + log(lambda->getValue().evaluate(times[i]));
    }
    
    
    // Multiply with the probability that we didn't observe a speciation event between the last speciation event t_(N-1) and the present t_N
    TrapezoidalRule integrator = TrapezoidalRule();
    lnProbTimes += log(pSurvival(times[numTaxa-numInitialSpecies],T,T)) * numTaxa + (diversificationRateIntegral->evaluate(T) - diversificationRateIntegral->evaluate(times[numTaxa-numInitialSpecies]) - log(rho->getValue())) * numTaxa;
    
    return lnProbTimes; // + logTreeTopologyProb;

}


void TimeDependentBirthDeathProcess::initialize(void) {
    
    // free old memory
    delete diversificationRateIntegral;
    delete survivalProbIntegral;
    
}


bool TimeDependentBirthDeathProcess::matchesConstraints( void ) {
    
    const TopologyNode &root = value->getRoot();
    
    for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it) {
        if ( !root.containsClade( *it, true ) ) {
            return false;
        }
    }
    
    return true;
}


double TimeDependentBirthDeathProcess::p1(double t, double T) const {
    
    double a = log(pSurvival(t,T,T));
    double b = diversificationRateIntegral->evaluate(T) - diversificationRateIntegral->evaluate(t);
    
    // remove the probability that we might not have sampled this species (or any descendant)
    double samplingProbability = rho->getValue();
    b -= log(samplingProbability);
    
    double p = 2.0 * a + b;
    
    return p;
}


double TimeDependentBirthDeathProcess::pSurvival(double start, double end, double T) const {
    
    double den = 1.0 + ( survivalProbIntegral->evaluate(end) - survivalProbIntegral->evaluate(start) ) / exp(diversificationRateIntegral->evaluate(start));
    
    // remove the probability that we might not have sampled this species (or any descendant)
    if ( start < T && end >= T ) {
        double samplingProbability = rho->getValue();
        den -= (samplingProbability - 1.0) * exp( diversificationRateIntegral->evaluate(T) - diversificationRateIntegral->evaluate(start) - log(samplingProbability) );
    }
    
    if ( den < 0 ) {
        // TODO: throw an error?!
        return 0;
    }
    
    return (1.0 / den);
}





double TimeDependentBirthDeathProcess::pWaiting(double start, double end, double T, size_t n) const {
    
    double samplingProb = 1.0;
    if (start < T && end >= T ) {
        samplingProb = rho->getValue();
    }

    double u = 1.0 - pSurvival(start,end,T) * exp( diversificationRateIntegral->evaluate(end)-diversificationRateIntegral->evaluate(start) - log(samplingProb) );
    double tmp = u * pSurvival(start,T,T) / pSurvival(start,end,T);
    // test if the waiting time for a single species resulted in a finite value
    double p = RbConstants::Double::neginf;
    if (tmp <= 1.0) {
        p = log(1.0 - tmp ) * n;
    }
    
    return p;
}


double TimeDependentBirthDeathProcess::simOrigin( void ) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // random draw
    double u = rng->uniform01();
    
    // get the parameters
    //    double b = lambda->getValue().evaluate(0);
    //    double d = mu->getValue().evaluate(0);
    //    
    //	double origin = std::log( (-b + d * std::pow(u, (1.0/numTaxa) )) / (b * (-1 + std::pow(u,(1.0/numTaxa))))) / (b-d);
    //	
    //    return origin;
    
    return u;
}


double TimeDependentBirthDeathProcess::simSpeciation(double origin) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
        
    double u = rng->uniform01();
    //    
    //    // get the parameters
    //    double b = lambda->getValue().evaluate(0);
    //    double d = mu->getValue().evaluate(0);
    //    
    //    double t = 1.0/(b-d) * log((b - d * exp((d-b)*origin) - d * (1.0 - exp((d-b) * origin)) * u )/(b - d * exp((d-b) * origin) - b * (1.0 - exp(( d-b ) * origin)) * u ) );  
    //	
    //    return t;
    
    return u;
}


void TimeDependentBirthDeathProcess::redrawValue( void ) {
    simulateTree();
}


void TimeDependentBirthDeathProcess::simulateTree( void ) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // The time tree object (topology + times)
    TimeTree psi = TimeTree();
    
    // Draw a random topology
    Topology *tau = new Topology();
    
    // internally we treat unrooted topologies the same as rooted
    tau->setRooted( true );
    
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
    nodes.push_back(root);
    // recursively build the tree
    buildRandomBinaryTree(nodes);
    
    // set tip names
    for (size_t i=0; i<numTaxa; i++) {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+index);
        
        // set name
        std::string& name = taxonNames[i];
        node->setName(name);
    }
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // set the topology of the tree
    psi.setTopology(tau,true);
    
    // now simulate the speciation times
    // draw a sample for the origin
	double t_or = simOrigin();
    
    // allocate the vector for the times
    std::vector<double> t = std::vector<double>(numTaxa,0.0);
	t[0] = 0.0;
    
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 1; i < numTaxa; ++i) {
		t[i] = t_or - simSpeciation(t_or);
	}
    
    std::sort(t.begin(),t.end());
    
    nodes.clear();
    nodes.push_back( root );
    attachTimes(&psi, nodes, 0, t, t_or);
    for (size_t i = 0; i < numTaxa; ++i) {
        TopologyNode& n = tau->getTipNode(i);
        psi.setAge(n.getIndex(),0.0);
    }
    
    // finally store the new value
    *value = psi;
    
}



void TimeDependentBirthDeathProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<UnivariateFunction>* >( newP );
    }
    else if (oldP == mu) {
        mu = static_cast<const TypedDagNode<UnivariateFunction>* >( newP );
    }
    else if (oldP == rho) {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    
    initialize();
}
