#include "AbstractGibbsMove.h"
#include "DagNode.h"
#include "RbException.h"

#include <cmath>
#include <iomanip>


using namespace RevBayesCore;


/**
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
AbstractGibbsMove::AbstractGibbsMove( double w  ) : AbstractMove( w, false )
{
    
}



/**
 * Basic destructor doing nothing.
 */
AbstractGibbsMove::~AbstractGibbsMove( void )
{
    
}



/**
 * Perform the move.
 * Here we store some info and delegate to performMove.
 */
void AbstractGibbsMove::performMcmcMove( double lHeat, double pHeat )
{
    // check heating values
    if ( lHeat != 1.0 || pHeat != 1.0 )
    {
        throw RbException("Cannot apply Gibbs sampler when the probability is heated.");
    }
    
    // delegate to derived class
    performGibbsMove();
    
}


/**
 * Print the summary of the move.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void AbstractGibbsMove::printSummary(std::ostream &o) const
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    const std::string &n = getMoveName();
    size_t spaces = 40 - (n.length() > 40 ? 40 : n.length());
    o << n;
    for (size_t i = 0; i < spaces; ++i)
    {
        o << " ";
    }
    o << " ";
    
    // print the DagNode name
    const std::vector<DagNode*> nodes = getDagNodes();
    std::string dn_name = "???";
    if ( nodes.size() > 0 )
    {
        dn_name = nodes[0]->getName();
    }
    spaces = 20 - (dn_name.length() > 20 ? 20 : dn_name.length());
    o << dn_name;
    for (size_t i = 0; i < spaces; ++i)
    {
        o << " ";
    }
    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight);
    for (int i = 0; i < w_length; ++i)
    {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried);
    for (int i = 0; i < t_length; ++i)
    {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numTried > 0) a_length -= (int)log10(numTried);
    
    for (int i = 0; i < a_length; ++i)
    {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the acceptance ratio
    double ratio = 1.0;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i)
    {
        o << " ";
    }
    o << ratio;
    o << " ";
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}





/**
 * Tune the move.
 * This is a dummy implementation because Gibbs move cannot be tuned.
 */
void AbstractGibbsMove::tune( void )
{
    
}




