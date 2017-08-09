#include "DistributionInverseWishart.h"
#include "DistributionWishart.h"
#include "MultivariateNormalDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ConjugateInverseWishartBrownianMove.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

ConjugateInverseWishartBrownianMove::ConjugateInverseWishartBrownianMove(StochasticNode<MatrixReal>* s, TypedDagNode<double>* k, TypedDagNode<int>* d, double w) : AbstractGibbsMove(w),
    sigma( s ),
    kappa( k ),
    df( d )
{
    
    addNode( sigma );
    addNode( kappa );
    addNode( df );
    
}


/**
 * Basic destructor doing nothing.
 */
ConjugateInverseWishartBrownianMove::~ConjugateInverseWishartBrownianMove( void )
{
    
    // nothing to do
    // the base class does the work
    
}




/* Clone object */
ConjugateInverseWishartBrownianMove* ConjugateInverseWishartBrownianMove::clone( void ) const
{
    
    return new ConjugateInverseWishartBrownianMove( *this );
}



const std::string& ConjugateInverseWishartBrownianMove::getMoveName( void ) const
{
    
    static std::string name = "ConjugateInverseWishartBrownianMove";
    
    return name;
}


/** Perform the move */
void ConjugateInverseWishartBrownianMove::performGibbsMove( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    size_t dim = sigma->getValue().getDim();
    
//    const MatrixReal& bksigma = sigma->getValue();
    
    std::vector< StochasticNode<RbVector<double> >* > children;
    const std::vector<DagNode*>& c = sigma->getChildren();
    for (std::vector<DagNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
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
    
    if (nnodes != test)
    {
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


void ConjugateInverseWishartBrownianMove::swapNodeInternal(DagNode *oldN, DagNode *newN)
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
    
}


