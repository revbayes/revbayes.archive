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
    storedRates( rates.size(), 0.0 ),
    nodes(),
    numAccepted( 0 )
{
    
    nodes.insert( tree );
    for (std::vector<StochasticNode<double>* >::iterator it = rates.begin(); it != rates.end(); ++it)
    {
        // get the pointer to the current node
        DagNode* theNode = *it;
        
        // add myself to the set of moves
        theNode->addMove( this );
        
        // increase the DAG node reference count because we also have a pointer to it
        theNode->incrementReferenceCount();
        
        nodes.insert( theNode );
    }

    
}

/**
 * Copy constructor.
 * We need to create a deep copy of the proposal here.
 *
 * \param[in]   m   The object to copy.
 *
 */
RateAgeBetaShift::RateAgeBetaShift(const RateAgeBetaShift &m) : AbstractMove(m),
    tree( m.tree ),
    rates( m.rates ),
    delta( m.delta ),
    storedNode( NULL ),
    storedAge( 0.0 ),
    storedRates( rates.size(), 0.0 ),
    nodes( m.nodes ),
    numAccepted( m.numAccepted )
{
    
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        // get the pointer to the current node
        DagNode* theNode = *it;
        
        // add myself to the set of moves
        theNode->addMove( this );
        
        // increase the DAG node reference count because we also have a pointer to it
        theNode->incrementReferenceCount();
        
    }
    
}


/**
 * Overloaded assignment operator.
 * We need a deep copy of the operator.
 */
RateAgeBetaShift& RateAgeBetaShift::operator=(const RateAgeBetaShift &m)
{
    
    if ( this != &m )
    {
        
        for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            // get the pointer to the current node
            DagNode* theNode = *it;
            
            // add myself to the set of moves
            theNode->removeMove( this );
            
            // decrease the DAG node reference count because we also have a pointer to it
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
            
        }
        
        tree            = m.tree;
        rates           = m.rates;
        delta           = m.delta;
        storedNode      = NULL;
        storedAge       = 0.0;
        storedRates     = std::vector<double>(rates.size(), 0.0 );
        nodes           = m.nodes;
        numAccepted     = m.numAccepted;
        
        
        for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            // get the pointer to the current node
            DagNode* theNode = *it;
            
            // add myself to the set of moves
            theNode->addMove( this );
            
            // increase the DAG node reference count because we also have a pointer to it
            theNode->incrementReferenceCount();
            
        }
    }
    
    return *this;
}


/**
 * Basic destructor doing nothing.
 */
RateAgeBetaShift::~RateAgeBetaShift( void )
{
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        // get the pointer to the current node
        DagNode* theNode = *it;
        
        // add myself to the set of moves
        theNode->removeMove( this );
        
        // decrease the DAG node reference count because we also have a pointer to it
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete theNode;
        }
        
    }
    
}




/* Clone object */
RateAgeBetaShift* RateAgeBetaShift::clone( void ) const
{
    
    return new RateAgeBetaShift( *this );
}


const std::set<DagNode*>& RateAgeBetaShift::getDagNodes( void ) const
{
    
    return nodes;
}



const std::string& RateAgeBetaShift::getMoveName( void ) const
{
    
    static std::string name = "RateAgeBetaShift";
    
    return name;
}


/** Perform the move */
void RateAgeBetaShift::performMove( double heat, bool raiseLikelihoodOnly )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = tree->getValue();
    std::set<DagNode*> affected;
    tree->getAffectedNodes( affected );
    
    double oldLnLike = 0.0;
    for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it)
    {
        (*it)->touch();
        oldLnLike += (*it)->getLnProbability();
    }
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    size_t nodeIdx = 0;
    do {
        double u = rng->uniform01();
        nodeIdx = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(nodeIdx);
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
    tau.setAge( nodeIdx, my_new_age );
    
    // get the probability ratio of the tree
    double treeProbRatio = tree->getLnProbabilityRatio();
    
    
    // set the rates
    double pa = node->getParent().getAge();
    double my_new_rate =(pa - my_age) * storedRates[nodeIdx] / (pa - my_new_age);
    
    // now we set the new value
    // this will automcatically call a touch
    rates[nodeIdx]->setValue( new double( my_new_rate ) );
    
    // get the probability ratio of the new rate
    double ratesProbRatio = rates[nodeIdx]->getLnProbabilityRatio();
    
    for (size_t i = 0; i < node->getNumberOfChildren(); i++)
    {
        size_t childIdx = node->getChild(i).getIndex();
        double a = node->getChild(i).getAge();
        double child_new_rate = (my_age - a) * storedRates[childIdx] / (my_new_age - a);
        
        // now we set the new value
        // this will automcatically call a touch
        rates[childIdx]->setValue( new double( child_new_rate ) );

        // get the probability ratio of the new rate
        ratesProbRatio += rates[childIdx]->getLnProbabilityRatio();
        
        
    }
    
    double lnProbRatio = 0;
    double newLnLike = 0;
    for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it)
    {

        double tmp = (*it)->getLnProbabilityRatio();
        lnProbRatio += tmp;
        newLnLike += (*it)->getLnProbability();
    }
    
    if ( fabs(lnProbRatio) > 1E-8 )
    {
        throw RbException("Likelihood shortcut computation failed in rate-age-proposal.");
    }
    
    double hastingsRatio = backward - forward;
    double lnAcceptanceRatio = treeProbRatio + ratesProbRatio + hastingsRatio;
    
    if (lnAcceptanceRatio >= 0.0)
    {
        numAccepted++;
        
        tree->keep();
        rates[nodeIdx]->keep();
        for (size_t i = 0; i < node->getNumberOfChildren(); i++)
        {
            size_t childIdx = node->getChild(i).getIndex();
            rates[childIdx]->keep();
        }
    }
    else if (lnAcceptanceRatio < -300.0)
    {
        reject();
        tree->restore();
        rates[nodeIdx]->restore();
        for (size_t i = 0; i < node->getNumberOfChildren(); i++)
        {
            size_t childIdx = node->getChild(i).getIndex();
            rates[childIdx]->restore();
        }
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
            rates[nodeIdx]->keep();
            for (size_t i = 0; i < node->getNumberOfChildren(); i++)
            {
                size_t childIdx = node->getChild(i).getIndex();
                rates[childIdx]->keep();
            }
        }
        else
        {
            reject();
            tree->restore();
            rates[nodeIdx]->restore();
            for (size_t i = 0; i < node->getNumberOfChildren(); i++)
            {
                size_t childIdx = node->getChild(i).getIndex();
                rates[childIdx]->restore();
            }
        }
    }

}


void RateAgeBetaShift::printSummary(std::ostream &o) const
{
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
            }
        }
    }
    
    
    // find the old node
    std::set<DagNode*>::iterator pos = nodes.find( oldN );
    // remove it from the set if it was contained
    if ( pos != nodes.end() )
    {
        nodes.erase( pos );
    }
    // insert the new node
    nodes.insert( newN );
    
    // remove myself from the old node and add myself to the new node
    oldN->removeMove( this );
    newN->addMove( this );
    
    // increment and decrement the reference counts
    newN->incrementReferenceCount();
    if ( oldN->decrementReferenceCount() == 0 )
    {
        throw RbException("Memory leak in Metropolis-Hastings move. Please report this bug to Sebastian.");
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


