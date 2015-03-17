#include "DistributionInverseWishart.h"
#include "DistributionWishart.h"
#include "MultivariateNormalDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ConjugateInverseWishartBrownianMove.h"

#include <cmath>

using namespace RevBayesCore;

ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(StochasticNode<MatrixReal>* s, TypedDagNode<double>* k, TypedDagNode<int>* d, double w) : AbstractMove(w),
    sigma( s ),
    kappa( k ),
    df( d ),
    nodes(),
    numAccepted( 0 )
{
    
    nodes.insert( sigma );
    nodes.insert( kappa );
    nodes.insert( df );
    
}

/**
 * Copy constructor.
 * We need to create a deep copy of the proposal here.
 *
 * \param[in]   m   The object to copy.
 *
 */
ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(const ConjugateInverseWishartBrownianMove &m) : AbstractMove(m),
    sigma( m.sigma ),
    kappa( m.kappa ),
    df( m.df ),
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
ConjugateInverseWishartBrownianMove& ConjugateInverseWishartBrownianMove::operator=(const ConjugateInverseWishartBrownianMove &m)
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
        
        sigma           = m.sigma;
        kappa           = m.kappa;
        df              = m.df;
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
ConjugateInverseWishartBrownianMove::~ConjugateInverseWishartBrownianMove( void )
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
ConjugateInverseWishartBrownianMove* ConjugateInverseWishartBrownianMove::clone( void ) const
{
    
    return new ConjugateInverseWishartBrownianMove( *this );
}


const std::set<DagNode*>& ConjugateInverseWishartBrownianMove::getDagNodes( void ) const
{
    
    return nodes;
}



const std::string& ConjugateInverseWishartBrownianMove::getMoveName( void ) const
{
    
    static std::string name = "ConjugateInverseWishartBrownianMove";
    
    return name;
}


/** Perform the move */
void ConjugateInverseWishartBrownianMove::performMove( double lHeat, double pHeat )
{
    
    // Get random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    size_t dim = sigma->getValue().getDim();
    
//    const MatrixReal& bksigma = sigma->getValue();
    
    std::vector< StochasticNode<RbVector<double> >* > children;
    const std::set<DagNode*>& c = sigma->getChildren();
    for (std::set<DagNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        StochasticNode<RbVector<double> >* tmp = dynamic_cast< StochasticNode<RbVector<double> > *>( *it );
        if ( tmp != NULL )
        {
            children.push_back( tmp );
        }
        
    }
    
    // calculate sufficient statistics based on current process
    MatrixReal A = MatrixReal(dim);
    size_t test = 0;
    for (std::vector<StochasticNode<RbVector<double> > *>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        MultivariateNormalDistribution* dist = dynamic_cast<MultivariateNormalDistribution *>( &(*it)->getDistribution() );
        if ( dist != NULL )
        {
            A += dist->computeContrasts();
            ++test;
        }
        
    }
    for (size_t i=0; i<dim; ++i)
    {
        A[i][i] += kappa->getValue();
    }
    
    size_t nnodes = children.size();
    
    if (nnodes != test) {
        std::cerr << "non matching number of children\n";
        exit(1);
    }
    
    double logp1 = sigma->getLnProbability();
    double logs1 = sigma->getLnProbability();
    for (std::vector<StochasticNode<RbVector<double> > *>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        logs1 += (*it)->getLnProbability();
    }
    
    
    // calculate old posterior for sigma based on current process
    double backward = RbStatistics::InverseWishart::lnPdf(A, nnodes + df->getValue(), sigma->getValue());

//    sigma->getValue().touch();
    
    // resample sigma based on new sufficient statistics
    sigma->setValue( RbStatistics::InverseWishart::rv(A, nnodes + df->getValue(), *rng).clone() );

    sigma->touch();
    sigma->keep();

    
    double logp2 = sigma->getLnProbability();
    double logs2 = sigma->getLnProbability();
    for (std::vector<StochasticNode<RbVector<double> > *>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        logs2 += (*it)->getLnProbability();
    }
    
//    sigma->getValue().update();
    
    // calculate posterior for sigma based on current process
    double forward = RbStatistics::InverseWishart::lnPdf(A, nnodes + df->getValue(), sigma->getValue());

    
    double alpha = logs2 - logs1 + backward - forward;
    
    
    if ( fabs(alpha - 0.0) > 1E-8 )
    {
        std::cerr << "oooohh" << std::endl;
        std::cerr << alpha << '\n';
        std::cerr << backward << '\t' << forward << '\t' << backward - forward << '\n';
        std::cerr << logs1 << '\t' << logs2 << '\t' << logs2 - logs1 << '\n';
        std::cerr << logp1 << '\t' << logp2 << '\n';
        exit(1);
    }
    // log hastings ratio
    // should cancel out the ratio of probabilities of the final and initial configuration
//    return logs1 - logs2;
    
    
}


void ConjugateInverseWishartBrownianMove::printSummary(std::ostream &o) const
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
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
}


void ConjugateInverseWishartBrownianMove::reject( void )
{
    
    // undo the proposal
    
}

/**
 * Reset the move counters. Here we only reset the counter for the number of accepted moves.
 *
 */
void ConjugateInverseWishartBrownianMove::resetMoveCounters( void )
{
    numAccepted = 0;
}


void ConjugateInverseWishartBrownianMove::swapNode(DagNode *oldN, DagNode *newN)
{
    
    if (oldN == sigma)
    {
        sigma = static_cast<StochasticNode<MatrixReal>* >(newN) ;
    }
    else if ( oldN == kappa )
    {
        kappa = static_cast<TypedDagNode<double>* >(newN) ;
    }
    else if ( oldN == df )
    {
        df = static_cast<TypedDagNode<int>* >(newN) ;
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


void ConjugateInverseWishartBrownianMove::tune( void )
{
    
}


