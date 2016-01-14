#include "GeneralRateMatrix.h"

#include "RbMathMatrix.h"

using namespace RevBayesCore;


GeneralRateMatrix::GeneralRateMatrix(size_t n, bool rto) : AbstractRateMatrix(n),
//    stationaryFreqs( std::vector<double>(numStates,1.0/n) ),
    transitionRates( std::vector<double>(numStates*numStates-numStates, 1.0/n) ),
    rescaleToOne(rto)
{
    
}


GeneralRateMatrix::~GeneralRateMatrix(void)
{
    // nothing to do
}

double GeneralRateMatrix::averageRate(void) const
{
    std::vector<double> stationaryFreqs = getStationaryFrequencies();
    
    double ave = 0.0;
    for (size_t i=0; i<numStates; i++)
    {
        ave += -stationaryFreqs[i] * (*theRateMatrix)[i][i];
    }
    
    return ave;
}



/** Set the exchangeability rates directly. We assume that we know
 what the exchangeability rates are when this function is called. */
void GeneralRateMatrix::setTransitionRates(const std::vector<double>& tr)
{
    
    transitionRates = tr;
    
    // set flags
    needsUpdate = true;
}


std::vector<double> GeneralRateMatrix::getStationaryFrequencies( void ) const
{
    return calculateStationaryFrequencies();
}


/** This function calculates the stationary frequencies of the rate matrix. The
 rate matrix, Q, is the infinitesimal generator of the Markov chain. It is an
 n X n matrix whose off-diagonal elements are q_ij >= 0 and whose diagonal elements
 are specified such that each row sums to zero. The rate matrix is finite (has
 a fixed number of states) and we assume that the input matrix is irreducible, as
 is the usual case for substitution models. Because Q is irreducible and finite,
 it has a stationary distribution, pi, which is a row vector of n probabilities.
 The stationary probabilities can be calculated by solving the homogeneous system
 of equations, pi*Q = 0, where 0 is a vector of zeros.
 
 We do the following to calculate the stationary frequencies. 
 
 1. We perform an LU decomposition of the transpose of the matrix Q.
 
 Q' = LU
 
 2. Now we set Ux = z (x will eventually hold the stationary probabilities). 
 Because L is nonsingular, we have z = 0. We proceed to back substitute on
 Ux = z = 0. When u_nn = 0, we can put in any solution for x. Here, we put
 in x_n = 1. We then solve the other values of x through back substitution.
 
 3. The solution obtained in 2 is not a probability vector. We normalize the
 vector such that the sum of the elements is 1.
 
 Note that the only time we need to use this function is when we don't
 know the stationary frequencies of the rate matrix beforehand. For most
 substitution models used in molecular evolution, the stationary frequencies
 are built into the rate matrix itself. These models are time-reversible.
 This function is useful for the non-reversible models.
 
 For more information on the fascinating topic of calculating the stationary
 probabilities of a rate matrix, see:
 
 Stewart, W. J. 1999. Numerical methods for computing stationary distributions of 
 finite irreducible Markov chains. In "Advances in Computational
 Probability", W. Grassmann, ed. Kluwer Academic Publishers. */
std::vector<double> GeneralRateMatrix::calculateStationaryFrequencies(void) const
{
    
	// transpose the rate matrix and put into QT
    MatrixReal QT(numStates, numStates);
    for (size_t i=0; i<numStates; i++)
    {
        for (size_t j=0; j<numStates; j++)
        {
            QT[i][j] = (*theRateMatrix)[j][i];
        }
    }
    
	// compute the LU decomposition of the transposed rate matrix
    MatrixReal L(numStates, numStates);
    MatrixReal U(numStates, numStates);
	RbMath::computeLandU(QT, L, U);
	
	// back substitute into z = 0 to find un-normalized stationary frequencies, starting with x_n = 1.0
    std::vector<double> pi(numStates, 0.0);
	pi[numStates-1] = 1.0;
    size_t i=numStates-1;
    while ( i > 0 )
    {
        i--;
		double dotProduct = 0.0;
		for (size_t j=i+1; j<numStates; j++)
        {
            dotProduct += U[i][j] * pi[j];
        }
        pi[i] = (0.0 - dotProduct) / U[i][i];
    }
    
	// normalize the solution vector
	double sum = 0.0;
	for (size_t i=0; i<numStates; i++)
    {
		sum += pi[i];
    }
    
    for (size_t i=0; i<numStates; i++)
    {
        pi[i] /= sum;
    }
    
    // return the stationary frequencies
    return pi;
}


void GeneralRateMatrix::update( void )
{
    
    if ( needsUpdate ) 
    {
        
        // rescale
        if (rescaleToOne) {
            rescaleToAverageRate( 1.0 );
        }
        
        // now update the eigensystem
//        updateEigenSystem();
        
        // clean flags
        needsUpdate = false;
    }
}

