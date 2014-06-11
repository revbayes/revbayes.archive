#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateAgeBetaShift.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

RateAgeBetaShift::RateAgeBetaShift(StochasticNode<TimeTree> *tr, std::vector<StochasticNode<double> *> v, double d, bool t, double w) : AbstractMove( w, t),
    tree( tr ),
    rates( v ),
    delta( d ),
    storedNode( NULL ),
    storedAge( 0.0 ),
    storedRates( 0.0, rates.size() ),
    numAccepted( 0 )
{
    
}



/* Clone object */
RateAgeBetaShift* RateAgeBetaShift::clone( void ) const
{
    
    return new RateAgeBetaShift( *this );
}



const std::string& RateAgeBetaShift::getMoveName( void ) const {
    
    static std::string name = "RateAgeBetaShift";
    
    return name;
}


/** Perform the move */
void RateAgeBetaShift::performMove( void )
{
    
    // clear rates map
    storedRates.clear();
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = tree->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->isTip() ); 
    
    TopologyNode& parent = node->getParent();
    
    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    double child_Age   = node->getChild( 0 ).getAge();
    if ( child_Age < node->getChild( 1 ).getAge())
    {
        child_Age = node->getChild( 1 ).getAge();
    }
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    size_t nodeIdx = node->getIndex();
    storedRates[nodeIdx] = rates[nodeIdx]->getValue();
    for (size_t i = 0; i < node->getNumberOfChildren(); i++)
    {
        size_t childIdx = node->getChild(i).getIndex();
        storedRates[childIdx] = rates[childIdx]->getValue();
    }
    
    
    // draw new ages and compute the hastings ratio at the same time
    double m = (my_age-child_Age) / (parent_age-child_Age);
    double a = delta * m + 1.0;
    double b = delta * (1.0-m) + 1.0;
    double new_m = RbStatistics::Beta::rv(a, b, *rng);
    double my_new_age = (parent_age-child_Age) * new_m + child_Age;
    
    // compute the Hastings ratio
    double forward = RbStatistics::Beta::lnPdf(a, b, new_m);
    double new_a = delta * new_m + 1.0;
    double new_b = delta * (1.0-new_m) + 1.0;
    double backward = RbStatistics::Beta::lnPdf(new_a, new_b, m);
    
    // set the age
    tau.setAge( node->getIndex(), my_new_age );
    
    double priorRatio = tree->getLnProbabilityRatio();
    
    // set the rates
    rates[nodeIdx]->setValue( new double((node->getParent().getAge() - my_age) * storedRates[nodeIdx] / (node->getParent().getAge() - my_new_age)));
    for (size_t i = 0; i < node->getNumberOfChildren(); i++)
    {
        size_t childIdx = node->getChild(i).getIndex();
        rates[childIdx]->setValue( new double((my_age - node->getChild(i).getAge()) * storedRates[childIdx] / (my_new_age - node->getChild(i).getAge())));
        priorRatio += rates[childIdx]->getLnProbabilityRatio();
    }
    
    double hastingsRatio = backward - forward;
    double lnAcceptanceRatio = priorRatio + hastingsRatio;
    
    if (lnAcceptanceRatio >= 0.0)
    {
        numAccepted++;
        
        tree->keep();
        for (size_t i = 0; i < node->getNumberOfChildren(); i++)
        {
            size_t childIdx = node->getChild(i).getIndex();
            rates[childIdx]->keep();
        }
    }
    else if (lnAcceptanceRatio < -300.0)
    {
        reject();
    }
    else
    {
        double r = exp(lnAcceptanceRatio);
        // Accept or reject the move
        double u = GLOBAL_RNG->uniform01();
        if (u < r)
        {
            numAccepted++;
            
            //keep
            tree->keep();
            for (size_t i = 0; i < node->getNumberOfChildren(); i++)
            {
                size_t childIdx = node->getChild(i).getIndex();
                rates[childIdx]->keep();
            }
        }
        else
        {
            reject();
        }
    }

}


void RateAgeBetaShift::printSummary(std::ostream &o) const {
    o << "delta = " << delta;
}


void RateAgeBetaShift::reject( void )
{
    
    // undo the proposal
    tree->getValue().setAge( storedNode->getIndex(), storedAge );
    
    // undo the rates
    size_t nodeIdx = storedNode->getIndex();
    rates[nodeIdx]->setValue(new double(storedRates[nodeIdx]));
    for (size_t i = 0; i < storedNode->getNumberOfChildren(); i++)
    {
        size_t childIdx = storedNode->getChild(i).getIndex();
        rates[childIdx]->setValue(new double(storedRates[childIdx]));
    }

    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting RateAgeBetaShift proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void RateAgeBetaShift::swapNode(DagNode *oldN, DagNode *newN) {
    
    if (oldN == tree)
    {
        tree = static_cast<StochasticNode<TimeTree>* >(newN) ;
    }
    else
    {
        for (size_t i = 0; i < rates.size(); i++)
        {
            if (oldN == rates[i])
            {
                rates[i] = static_cast<StochasticNode<double>* >(newN);
                break;
            }
        }
    }
}


void RateAgeBetaShift::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 )
    {
        delta /= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        delta *= (2.0 - rate/0.44 );
    }
}


