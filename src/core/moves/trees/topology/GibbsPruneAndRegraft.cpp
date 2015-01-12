#include "GibbsPruneAndRegraft.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

GibbsPruneAndRegraft::GibbsPruneAndRegraft(StochasticNode<TimeTree> *v, double w) : SimpleMove( v, w),
    variable( v )
{
    
}



/* Clone object */
GibbsPruneAndRegraft* GibbsPruneAndRegraft::clone( void ) const
{
    
    return new GibbsPruneAndRegraft( *this );
}


void GibbsPruneAndRegraft::findNewBrothers(std::vector<TopologyNode *> &b, TopologyNode &p, TopologyNode *n)
{
    
    // security check that I'm not a tip
    if (!n->isTip() && &p != n) 
    {
        // check the first child
        TopologyNode* child = &n->getChild( 0 );
        if ( child->getAge() < p.getAge() ) 
        {
            b.push_back( child );
        } 
        else 
        {
            findNewBrothers(b, p, child);
        }
        
        // check the second child
        child = &n->getChild( 1 );
        if ( child->getAge() < p.getAge() ) 
        {
            b.push_back( child );
        } 
        else 
        {
            findNewBrothers(b, p, child);
        }
    }
}



const std::string& GibbsPruneAndRegraft::getMoveName( void ) const 
{
    static std::string name = "GPR";
    
    return name;
}


/** Perform the move */
double GibbsPruneAndRegraft::performSimpleMove( void ) 
{
    // reset flags
    failed = false;
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // potential affected nodes for likelihood computation
    std::set<DagNode *> affected;
    variable->getAffectedNodes( affected );
    
    double backwardLikelihood = variable->getLnProbability();
    for (std::set<DagNode*>::const_iterator it = affected.begin(); it != affected.end(); ++it) 
    {
        backwardLikelihood += (*it)->getLnProbability();
    }
    int offset = (int) -backwardLikelihood;
    double backward = exp(backwardLikelihood + offset);
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getParent().isRoot() );
    
    TopologyNode* parent        = &node->getParent(); 
    TopologyNode& grandparent   = parent->getParent();
    TopologyNode& brother       = parent->getChild( 0 );
    // check if we got the correct child
    if ( &brother == node ) 
    {
        brother = parent->getChild( 1 );
    }
    
    // collect the possible reattachement points
    std::vector<TopologyNode*> new_brothers;
    findNewBrothers(new_brothers, *parent, &tau.getRoot());
    
    // we only need to propose a new tree if there are any other re-attachement points
    if ( new_brothers.size() < 1)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    std::vector<double> weights = std::vector<double>(new_brothers.size(), 0.0);
    double sumOfWeights = 0.0;
    for (size_t i = 0; i<new_brothers.size(); ++i) 
    {
        // get the new brother
        TopologyNode* newBro = new_brothers[i];
        
        // do the proposal
        TopologyNode *newGrandparent = pruneAndRegraft(&brother, newBro, parent, grandparent);
        
        // flag for likelihood recomputation
        variable->touch();
        
        // compute the likelihood of the new value
        double priorRatio = variable->getLnProbability();
        double likelihoodRatio = 0.0;
        for (std::set<DagNode*>::const_iterator it = affected.begin(); it != affected.end(); ++it) 
        {
            likelihoodRatio += (*it)->getLnProbability();
        }
        weights[i] = exp(priorRatio + likelihoodRatio + offset);
        sumOfWeights += weights[i];
        
        // undo proposal
        pruneAndRegraft(newBro, &brother, parent, *newGrandparent);
        
        // restore the previous likelihoods;
        variable->restore();
    }
    
    if (sumOfWeights <= 1E-100)
    {
        // hack
        // the proposals have such a small likelihood that they can be neglected
//        throw new OperatorFailedException("Couldn't find another proposal with a decent likelihood.");
        failed = true;
        
        return 0.0;
    }
    
    double ran = rng->uniform01() * sumOfWeights;
    size_t index = 0;
    while (ran > 0.0)
    {
        ran -= weights[index];
        index++;
    }
    index--;
    
    TopologyNode* newBro = new_brothers[index];
    
    // now we store all necessary values
    storedBrother       = &brother;
    storedNewBrother    = newBro;
    
    pruneAndRegraft(&brother, newBro, parent, grandparent);
    
    double forward = weights[index];
    
    double forwardProb = (forward / sumOfWeights);
    double backwardProb = (backward / (sumOfWeights - forward + backward));
    double hastingsRatio = log(backwardProb / forwardProb);
    
    return hastingsRatio;
}


TopologyNode* GibbsPruneAndRegraft::pruneAndRegraft(TopologyNode *brother, TopologyNode *newBrother, TopologyNode *parent, TopologyNode &grandparent)
{
    
    // prune
    grandparent.removeChild( parent );
    parent->removeChild( brother );
    grandparent.addChild( brother );
    brother->setParent( &grandparent );
    
    // regraft
    TopologyNode* newGrandParent = &newBrother->getParent();
    newGrandParent->removeChild( newBrother );
    newGrandParent->addChild( parent );
    parent->setParent( newGrandParent );
    parent->addChild( newBrother );
    newBrother->setParent( parent );
    
    return newGrandParent;
}


void GibbsPruneAndRegraft::rejectSimpleMove( void )
{
    // only undo if we actually proposed something
    if ( failed == false )
    {
        // undo the proposal
        TopologyNode& parent = storedNewBrother->getParent();
        TopologyNode& newGrandparent = parent.getParent();
        TopologyNode& grandparent = storedBrother->getParent();
    
        // prune
        newGrandparent.removeChild( &parent );
        parent.removeChild( storedNewBrother );
        newGrandparent.addChild( storedNewBrother );
        storedNewBrother->setParent( &newGrandparent );
        
        // regraft
        grandparent.removeChild( storedBrother );
        parent.addChild( storedBrother );
        storedBrother->setParent( &parent );
        grandparent.addChild( &parent );
        parent.setParent( &grandparent );
    
    }
    
}


void GibbsPruneAndRegraft::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

