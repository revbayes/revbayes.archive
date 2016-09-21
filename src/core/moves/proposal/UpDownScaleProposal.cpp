#include "UpDownScaleProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
UpDownScaleProposal::UpDownScaleProposal( double l ) : Proposal(),
    upScalarVariables(  ),
    upVectorVariables(  ),
    upTreeVariables(  ),
    downScalarVariables(  ),
    downVectorVariables(  ),
    downTreeVariables(  ),
    lambda( l )
{
    
}


/**
 * Add a variable.
 */
void UpDownScaleProposal::addVariable( StochasticNode<double> *v, bool up)
{
    
    
    // add it to the nodes vector
    addNode( v );
    
    // add it to my internal vector
    if ( up == true )
    {
        upScalarVariables.push_back( v );
    }
    else
    {
        downScalarVariables.push_back( v );
    }
    
}


/**
 * Add a variable.
 */
void UpDownScaleProposal::addVariable( StochasticNode<RbVector<double> > *v, bool up)
{
    
    
    // add it to the nodes vector
    addNode( v );
    
    // add it to my internal vector
    if ( up == true )
    {
        upVectorVariables.push_back( v );
    }
    else
    {
        downVectorVariables.push_back( v );
    }
    
}


/**
 * Add a variable.
 */
void UpDownScaleProposal::addVariable( StochasticNode<Tree> *v, bool up )
{
    
    
    // add it to the nodes vector
    addNode( v );
    
    // add it to my internal vector
    if ( up == true )
    {
        upTreeVariables.push_back( v );
    }
    else
    {
        downTreeVariables.push_back( v );
    }
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void UpDownScaleProposal::cleanProposal( void )
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
UpDownScaleProposal* UpDownScaleProposal::clone( void ) const
{
    
    return new UpDownScaleProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& UpDownScaleProposal::getProposalName( void ) const
{
    static std::string name = "UpDownScaling";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and scales the current vale by a scaling factor
 * sf = exp( lambda * u )
 * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double UpDownScaleProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scaling_factor = std::exp( lambda * ( u - 0.5 ) );
    
    double scalingCount = 0.0;
    
    storedUpScalarValues = std::vector<double>(upScalarVariables.size(), 0.0);
    // scale all the single variable up
    for (size_t i=0; i<upScalarVariables.size(); ++i)
    {
        storedUpScalarValues[i] = upScalarVariables[i]->getValue();
        upScalarVariables[i]->getValue() *= scaling_factor;
        ++scalingCount;
    }
    
    storedUpVectorValues = std::vector<std::vector<double> >(upVectorVariables.size(), std::vector<double>());
    // scale all the vector variables up
    for (size_t i=0; i<upVectorVariables.size(); ++i)
    {
        RbVector<double> &v = upVectorVariables[i]->getValue();

        storedUpVectorValues[i] = std::vector<double>(v.size(), 0.0);
        for (size_t j=0; j<v.size(); ++j)
        {
            storedUpVectorValues[i][j] = v[j];
            v[j] *= scaling_factor;
        }
        scalingCount += v.size();
    }
    
    storedUpTreeValues = std::vector<std::vector<double> >(upTreeVariables.size(), std::vector<double>());
    // scale all the tree variables up
    for (size_t i=0; i<upTreeVariables.size(); ++i)
    {
        
        Tree& tau = upTreeVariables[i]->getValue();
        
        TopologyNode& node = tau.getRoot();

        storedUpTreeValues[i] = std::vector<double>(tau.getNumberOfNodes(), 0.0);
        TreeUtilities::getAges(&tau, &node, storedUpTreeValues[i]);
        
        // rescale the subtrees
        TreeUtilities::rescaleSubtree(&tau, &node, scaling_factor );
        
        scalingCount += tau.getNumberOfInteriorNodes();
    }
    
    storedDownScalarValues = std::vector<double>(downScalarVariables.size(), 0.0);
    // scale all the single variable down
    for (size_t i=0; i<downScalarVariables.size(); ++i)
    {
        storedDownScalarValues[i] = downScalarVariables[i]->getValue();
        downScalarVariables[i]->getValue() /= scaling_factor;
        --scalingCount;
    }
    
    storedDownVectorValues = std::vector<std::vector<double> >(downVectorVariables.size(), std::vector<double>());
    // scale all the vector variables down
    for (size_t i=0; i<downVectorVariables.size(); ++i)
    {
        RbVector<double> &v = downVectorVariables[i]->getValue();

        storedDownVectorValues[i] = std::vector<double>(v.size(), 0.0);
        for (size_t j=0; j<v.size(); ++j)
        {
            storedDownVectorValues[i][j] = v[j];
            v[j] /= scaling_factor;
        }
        scalingCount -= v.size();
    }
    
    storedDownTreeValues = std::vector<std::vector<double> >(downTreeVariables.size(), std::vector<double>());
    // scale all the tree variables down
    for (size_t i=0; i<downTreeVariables.size(); ++i)
    {
        
        Tree& tau = downTreeVariables[i]->getValue();
        
        TopologyNode& node = tau.getRoot();
        
        storedDownTreeValues[i] = std::vector<double>(tau.getNumberOfNodes(), 0.0);
        TreeUtilities::getAges(&tau, &node, storedDownTreeValues[i]);

        // rescale the subtrees
        TreeUtilities::rescaleSubtree(&tau, &node, 1.0/scaling_factor );
        
        scalingCount -= tau.getNumberOfInteriorNodes();
    }
    
    // compute the Hastings ratio
    double lnHastingsratio = scalingCount * log( scaling_factor );
    
    return lnHastingsratio;
}


/**
 * Prepare the proposal, e.g., pick the UpDown that we want to change.
 * Here we do not need to do any preparation.
 */
void UpDownScaleProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void UpDownScaleProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "lambda = " << lambda;
    
}


/**
 * Remove a variable.
 */
void UpDownScaleProposal::removeVariable( StochasticNode<double> *v, bool up)
{
    
    
    // add it to the nodes vector
    removeNode( v );
    
    // add it to my internal vector
    if ( up == true )
    {
        upScalarVariables.erase(std::remove(upScalarVariables.begin(), upScalarVariables.end(), v), upScalarVariables.end());
    }
    else
    {
        downScalarVariables.erase(std::remove(downScalarVariables.begin(), downScalarVariables.end(), v), downScalarVariables.end());
    }
    
}


/**
 * Remove a variable.
 */
void UpDownScaleProposal::removeVariable( StochasticNode<RbVector<double> > *v, bool up)
{
    
    
    // add it to the nodes vector
    removeNode( v );
    
    // add it to my internal vector
    if ( up == true )
    {
        upVectorVariables.erase(std::remove(upVectorVariables.begin(), upVectorVariables.end(), v), upVectorVariables.end());
    }
    else
    {
        downVectorVariables.erase(std::remove(downVectorVariables.begin(), downVectorVariables.end(), v), downVectorVariables.end());
    }
    
}


/**
 * Remove a variable.
 */
void UpDownScaleProposal::removeVariable( StochasticNode<Tree> *v, bool up )
{
    
    
    // add it to the nodes vector
    removeNode( v );
    
    // add it to my internal vector
    if ( up == true )
    {
        upTreeVariables.erase(std::remove(upTreeVariables.begin(), upTreeVariables.end(), v), upTreeVariables.end());
    }
    else
    {
        downTreeVariables.erase(std::remove(downTreeVariables.begin(), downTreeVariables.end(), v), downTreeVariables.end());
    }
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void UpDownScaleProposal::undoProposal( void )
{
    
    // scale all the single variable up
    for (size_t i=0; i<upScalarVariables.size(); ++i)
    {
        upScalarVariables[i]->getValue() = storedUpScalarValues[i];
    }
    
    // scale all the vector variables up
    for (size_t i=0; i<upVectorVariables.size(); ++i)
    {
        RbVector<double> &v = upVectorVariables[i]->getValue();
        for (size_t j=0; j<v.size(); ++j)
        {
            v[j] = storedUpVectorValues[i][j];
        }
    }
    
    // scale all the tree variables up
    for (size_t i=0; i<upTreeVariables.size(); ++i)
    {
        
        Tree& tau = upTreeVariables[i]->getValue();
        
        TopologyNode& node = tau.getRoot();
        
        // rescale the subtrees
        TreeUtilities::setAges(&tau, &node, storedUpTreeValues[i] );
    }
    
    // scale all the single variable down
    for (size_t i=0; i<downScalarVariables.size(); ++i)
    {
        downScalarVariables[i]->getValue() = storedDownScalarValues[i];
    }
    
    // scale all the vector variables down
    for (size_t i=0; i<downVectorVariables.size(); ++i)
    {
        RbVector<double> &v = downVectorVariables[i]->getValue();
        for (size_t j=0; j<v.size(); ++j)
        {
            v[j] = storedDownVectorValues[i][j];
        }
    }
    
    // scale all the tree variables down
    for (size_t i=0; i<downTreeVariables.size(); ++i)
    {
        
        Tree& tau = downTreeVariables[i]->getValue();
        
        TopologyNode& node = tau.getRoot();
        
        // rescale the subtrees
        TreeUtilities::setAges(&tau, &node, storedDownTreeValues[i] );
    }
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void UpDownScaleProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    for (size_t i = 0; i < upScalarVariables.size(); ++i)
    {
        if ( upScalarVariables[i] == oldN )
        {
            upScalarVariables[i] = static_cast<StochasticNode<double> *>(newN);
        }
    }
    
    for (size_t i = 0; i < upVectorVariables.size(); ++i)
    {
        if ( upVectorVariables[i] == oldN )
        {
            upVectorVariables[i] = static_cast<StochasticNode<RbVector<double> > *>(newN);
        }
    }
    
    for (size_t i = 0; i < upTreeVariables.size(); ++i)
    {
        if ( upTreeVariables[i] == oldN )
        {
            upTreeVariables[i] = static_cast<StochasticNode<Tree> *>(newN);
        }
    }
    
    for (size_t i = 0; i < downScalarVariables.size(); ++i)
    {
        if ( downScalarVariables[i] == oldN )
        {
            downScalarVariables[i] = static_cast<StochasticNode<double> *>(newN);
        }
    }
    
    for (size_t i = 0; i < downVectorVariables.size(); ++i)
    {
        if ( downVectorVariables[i] == oldN )
        {
            downVectorVariables[i] = static_cast<StochasticNode<RbVector<double> > *>(newN);
        }
    }
    
    for (size_t i = 0; i < downTreeVariables.size(); ++i)
    {
        if ( downTreeVariables[i] == oldN )
        {
            downTreeVariables[i] = static_cast<StochasticNode<Tree> *>(newN);
        }
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void UpDownScaleProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
    
}

