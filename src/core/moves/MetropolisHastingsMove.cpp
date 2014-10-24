#include "AbstractProposal.h"
#include "DagNode.h"
#include "MetropolisHastingsMove.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbMathLogic.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace RevBayesCore;


/** 
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
MetropolisHastingsMove::MetropolisHastingsMove( AbstractProposal *p, double w, bool t ) : AbstractMove(w,t),
    affectedNodes(),
    auxillaryNodes(),
    nodes(),
    numAccepted( 0 ),
    proposal( p )
{
    nodes = proposal->getNodes();
    
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        (*it)->getAffectedNodes( affectedNodes );
    }
    
    // remove all "core" nodes from affectedNodes so their probabilities are not double-counted
    for (std::set<DagNode*>::iterator it = affectedNodes.begin(); it != affectedNodes.end(); it++)
    {
        if ( nodes.find(*it) != nodes.end() )
        {
            affectedNodes.erase(*it);
        }
    }
}


/**
 * Copy constructor.
 * We need to create a deep copy of the proposal here.
 *
 * \param[in]   m   The object to copy.
 *
 */
MetropolisHastingsMove::MetropolisHastingsMove(const MetropolisHastingsMove &m) : AbstractMove(m),
    affectedNodes( m.affectedNodes ),
    nodes( m.nodes ),
    numAccepted( m.numAccepted ),
    proposal( m.proposal->clone() )
{
    
}


/**
 * Basic destructor doing nothing.
 */
MetropolisHastingsMove::~MetropolisHastingsMove( void )
{
    delete proposal;
}


/** 
 * Overloaded assignment operator.
 * We need a deep copy of the operator.
 */
MetropolisHastingsMove& MetropolisHastingsMove::operator=(const RevBayesCore::MetropolisHastingsMove &m)
{
    
    if ( this != &m )
    {
        // free memory
        delete proposal;
        
        affectedNodes = m.affectedNodes;
        nodes = m.nodes;
        numAccepted = m.numAccepted;
        proposal = m.proposal->clone();
    }
    
    return *this;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the MetropolisHastingsMove. 
 */
MetropolisHastingsMove* MetropolisHastingsMove::clone( void ) const 
{
    
    return new MetropolisHastingsMove( *this );
}


/**
 * Get the set of nodes on which this move is working on.
 *
 * \return The set of nodes.
 */
const std::set<DagNode*>& MetropolisHastingsMove::getDagNodes( void ) const
{
    
    return nodes;
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& MetropolisHastingsMove::getMoveName( void ) const 
{
    
    return proposal->getProposalName();
}



void MetropolisHastingsMove::performMove( double heat, bool raiseLikelihoodOnly )
{
    // Propose a new value
    proposal->prepareProposal();
    double lnHastingsRatio = proposal->doProposal();
    
    // first we touch all the nodes
    // that will set the flags for recomputation
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        (*it)->touch();
    }
    
    double lnPriorRatio = 0.0;
    double lnLikelihoodRatio = 0.0;
    
    // compute the probability of the current value for each node
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if ( (*it)->isClamped() )
        {
            
            lnLikelihoodRatio += (*it)->getLnProbabilityRatio();
        }
        else
        {
            lnPriorRatio += (*it)->getLnProbabilityRatio();
        }
    }
    
    // then we recompute the probability for all the affected nodes
    for (std::set<DagNode*>::iterator it = affectedNodes.begin(); it != affectedNodes.end(); ++it) 
    {
        if ( (*it)->isClamped() )
        {
            lnLikelihoodRatio += (*it)->getLnProbabilityRatio();
        }
        else
        {
            lnPriorRatio += (*it)->getLnProbabilityRatio();
        }
    }
    
    // exponentiate with the chain heat
    double lnPosteriorRatio;
    if ( raiseLikelihoodOnly )
    {
        lnPosteriorRatio = heat * lnLikelihoodRatio + lnPriorRatio;
    }
    else
    {
        lnPosteriorRatio = heat * (lnLikelihoodRatio + lnPriorRatio);
    }
	
	if ( !RbMath::isAComputableNumber(lnPosteriorRatio) ) {
		
            proposal->undoProposal();
            
            // call restore for each node
            for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i)->restore();
            }
	}
    else
    {
    
        // finally add the Hastings ratio
        double lnAcceptanceRatio = lnPosteriorRatio + lnHastingsRatio;
    
        if (lnAcceptanceRatio >= 0.0)
        {
            numAccepted++;
        
            // call accept for each node
            for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i)->keep();
            }
        
        }
        else if (lnAcceptanceRatio < -300.0)
        {
            proposal->undoProposal();
        
            // call restore for each node
            for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
            {
                (*i)->restore();
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
            
                // call accept for each node
                for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    (*i)->keep();
                }
            
                proposal->cleanProposal();
            }
            else
            {
                proposal->undoProposal();
            
                // call restore for each node
                for (std::set<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
                {
                    (*i)->restore();
                }
            }
        }

    }

}


/**
 * Print the summary of the move.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void MetropolisHastingsMove::printSummary(std::ostream &o) const 
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    const std::string &n = getMoveName();
    size_t spaces = 40 - (n.length() > 40 ? 40 : n.length());
    o << n;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
    
    // print the DagNode name
    const std::string &dn_name = (*nodes.begin())->getName();
    spaces = 20 - (dn_name.length() > 20 ? 20 : dn_name.length());
    o << dn_name;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight);
    for (int i = 0; i < w_length; ++i) {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried);
    for (int i = 0; i < t_length; ++i) {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numAccepted > 0) a_length -= (int)log10(numAccepted);
    
    for (int i = 0; i < a_length; ++i) {
        o << " ";
    }
    o << numAccepted;
    o << " ";
    
    // print the acceptance ratio
    double ratio = numAccepted / (double)numTried;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i) {
        o << " ";
    }
    o << ratio;
    o << " ";
    
    proposal->printParameterSummary( o );
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}

/**
 * Reset the move counters. Here we only reset the counter for the number of accepted moves.
 *
 */
void MetropolisHastingsMove::resetMoveCounters( void )
{
    numAccepted = 0;
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new variable.
 */
void MetropolisHastingsMove::swapNode(DagNode *oldN, DagNode *newN) 
{
    
    // find the old node
    std::set<DagNode*>::iterator pos = nodes.find( oldN );
    // remove it from the set if it was contained
    if ( pos != nodes.end() )
    {
        nodes.erase( pos );
    }
    // insert the new node
    nodes.insert( newN );
        
    affectedNodes.clear();
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        (*it)->getAffectedNodes( affectedNodes );
    }
    for (std::set<DagNode*>::iterator it = affectedNodes.begin(); it != affectedNodes.end(); it++)
    {
        if ( nodes.find(*it) != nodes.end() )
        {
            affectedNodes.erase(*it);
        }
    }
    
    proposal->swapNode(oldN, newN);
    
}


/**
 * Tune the move to accept the desired acceptance ratio.
 * We only compute the acceptance ratio here and delegate the call to the proposal.
 */
void MetropolisHastingsMove::tune( void ) {
    
    double rate = numAccepted / double(numTried);
    
    proposal->tune( rate );
    
}

