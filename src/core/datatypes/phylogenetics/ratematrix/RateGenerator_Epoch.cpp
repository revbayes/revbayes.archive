//
//  RateGenerator_Epoch.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "RateGenerator_Epoch.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include "RateMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateGenerator_Epoch::RateGenerator_Epoch(size_t n, size_t ne) : RateGenerator( n ), numEpochs( ne ), needs_update(true)
{
    update();
}

/** Destructor */
RateGenerator_Epoch::~RateGenerator_Epoch(void)
{

}

RateGenerator_Epoch& RateGenerator_Epoch::assign(const Assignable &m)
{
    
    const RateGenerator_Epoch *rm = dynamic_cast<const RateGenerator_Epoch*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}


/** Calculate the transition probabilities */
void RateGenerator_Epoch::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    // what amount of tole
    double precisionError = 1E-6;
    double diffAge = startAge - endAge;
    
    if (diffAge + precisionError < 0)
    {
        throw RbException("RateGenerator_Epoch cannot compute transition probabilities for negative branch lengths");
    }
    
    // P = I
    TransitionProbabilityMatrix tp(num_states);
    for (size_t i = 0; i < num_states; i++)
        tp[i][i] = 1.0;
    
    if (diffAge > 0)
    {
        // get current time on branch
        double currAge = startAge;
        
        // find epoch
        size_t epochIdx = findEpochIndex(currAge);
        
        // multiply transition probs across epochs
        while (currAge > endAge)
        {
            double nextAge = 0.0;
            if (epochIdx < numEpochs)
                nextAge = epochTimes[epochIdx];
            
            // get next time, which is the next epoch or branch end
            if (nextAge < endAge)
                nextAge = endAge;
    //        double dt = currAge - nextAge;
            
            // first, get the rate matrix for this branch
            const RateGenerator& rg = epochRateGenerators[epochIdx];
            
            double r = epochRates[epochIdx];
            
            rg.calculateTransitionProbabilities( currAge, nextAge, r * rate, P );
            
            double eps = 1e-4;

            for (size_t i = 0; i < P.getNumberOfStates(); i++)
            {
                for (size_t j = 0; j < P.getNumberOfStates(); j++)
                {
                    if (P[i][j] > 1.0 + eps)
                    {
                        ;
//                        std::cout << "error!\n";
//                        std::cout << i << " " << j << " " << P[i][j] << "\n";
//                        
//                        std::cout << P << "\n";
//                        
//                        
//                        // A = make_matrix_from_pointer(initialValues);
//                        boost::numeric::ublas::matrix<double> input;
//                        typedef boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix;
//                        boost::numeric::ublas::matrix<double> A(input);
//                        
//                        // create a permutation matrix for the LU-factorization
//                        pmatrix pm(A.size1());
//                        
//                        // perform LU-factorization
//                        int res = (unsigned)boost::numeric::ublas::lu_factorize(A, pm);
//                        if (res != 0)
//                            std::cout << "Error!\n";
//                        
//                        
//                        
//                        rg.calculateTransitionProbabilities( currAge, nextAge, r * rate, P );
//
//                        std::cout << "\n";
                    }
                }
            }
            
            // epochs construct DTMC
            tp *= P;
            
//            std::cout << P << "\n\n";
//            std::cout << tp << "\n";
//            std::cout << "-------\n";
            
            // advance increment
            currAge = nextAge;
            epochIdx++;
        }
    }
    else
    {
        ; // do nothing, i.e. transition probabilty matrix equals the identity matrix
    }
    
    P = TransitionProbabilityMatrix(tp);
//    std::cout << P << "\n";
}


RateGenerator_Epoch* RateGenerator_Epoch::clone( void ) const
{
    
    return new RateGenerator_Epoch( *this );
}


size_t RateGenerator_Epoch::findEpochIndex( double t ) const
{
    int i;
   
    for (i = 0; i < epochTimes.size() && t < epochTimes[i]; i++)
    {
        ; // do nothing
    }
    return i;
}


double RateGenerator_Epoch::getRate(size_t from, size_t to, double age, double rate) const
{
    size_t epochIdx = findEpochIndex(age);
   
    const RateGenerator *rg = &epochRateGenerators[epochIdx];

    double r = rg->getRate(from, to, age, rate * epochRates[epochIdx]);
    
    return r;
}



const RbVector<RateGenerator>& RateGenerator_Epoch::getRateGenerators(void) const
{
    return epochRateGenerators;
}

const RbVector<double>& RateGenerator_Epoch::getEpochTimes(void) const
{
    return epochTimes;
}

const RbVector<double>& RateGenerator_Epoch::getEpochRates(void) const
{
    return epochRates;
}

void RateGenerator_Epoch::setEpochGenerators(const RbVector<RateGenerator>& rg)
{
    epochRateGenerators = rg;
    needs_update = true;
}

void RateGenerator_Epoch::setEpochTimes(const RbVector<double> &t)
{
    epochTimes = t;
    needs_update = true;
}

void RateGenerator_Epoch::setEpochRates(const RbVector<double>& r)
{
    epochRates = r;
    needs_update = true;
}

void RateGenerator_Epoch::update( void ) {
    
    if ( needs_update )
    {
//        for (size_t i = 0; i < epochRateGenerators.size(); i++)
//        {
//            epochRateGenerators[i].update();
//        }
//        // compute the off-diagonal values
//        computeOffDiagonal();
//        
//        // set the diagonal values
//        setDiagonal();
//        
//        // rescale
//        rescaleToAverageRate( 1.0 );
//        
//        // now update the eigensystem
//        updateEigenSystem();
//
        // clean flags
        needs_update = false;
    }
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateGenerator_Epoch& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.size(); i++) {
        if (i == 0)
            o << "[ ";
        else
            o << "  ";
        
        for (size_t j = 0; j < x.size(); ++j)
        {
            if (j != 0)
                o << ", ";
                o << "ok"; //x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}
