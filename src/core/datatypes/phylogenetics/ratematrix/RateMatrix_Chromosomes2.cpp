//
//  RateMatrix_Chromosomes2.cpp
//
// Will Freyman
//

#include "RateMatrix_Chromosomes2.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"
#include "EigenSystem.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Chromosomes2::RateMatrix_Chromosomes2(size_t n) : AbstractRateMatrix( n ), matrixSize( n ){
    setLambda(0.0);
    setRho(0.0);
    setDelta(0.0);
	
	theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates * numStates * numStates);
    cc_ijk.resize(numStates * numStates * numStates);
    
    updateMatrix();
	
	
}


/** Copy constructor */
RateMatrix_Chromosomes2::RateMatrix_Chromosomes2(const RateMatrix_Chromosomes2& m) : AbstractRateMatrix( m ), matrixSize(m.matrixSize), rho(m.rho), delta(m.delta), lambda(m.lambda), precision(m.precision), stationaryFreqs(m.stationaryFreqs) {
	theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix_Chromosomes2::~RateMatrix_Chromosomes2(void) {
    delete theEigenSystem;
}


RateMatrix_Chromosomes2& RateMatrix_Chromosomes2::operator=(const RateMatrix_Chromosomes2 &r) {
    
    if (this != &r)
    {
        AbstractRateMatrix::operator=( r );
        
        delete theEigenSystem;
        
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
    }
    
    return *this;
}

/** Do precalculations on eigenvectors */
void RateMatrix_Chromosomes2::calculateCijk(void)
{
    
    if ( theEigenSystem->isComplex() == false )
    {
        // real case
        const MatrixReal& ev  = theEigenSystem->getEigenvectors();
        const MatrixReal& iev = theEigenSystem->getInverseEigenvectors();
        double* pc = &c_ijk[0];
        for (size_t i=0; i<numStates; i++)
        {
            for (size_t j=0; j<numStates; j++)
            {
                for (size_t k=0; k<numStates; k++)
                {
                    *(pc++) = ev[i][k] * iev[k][j];
                }
            }
        }
    }
    else
    {
        // complex case
        const MatrixComplex& cev  = theEigenSystem->getComplexEigenvectors();
        const MatrixComplex& ciev = theEigenSystem->getComplexInverseEigenvectors();
        std::complex<double>* pc = &cc_ijk[0];
        for (size_t i=0; i<numStates; i++)
        {
            for (size_t j=0; j<numStates; j++)
            {
                for (size_t k=0; k<numStates; k++)
                {
                    *(pc++) = cev[i][k] * ciev[k][j];
                }
            }
        }
    }
}

double RateMatrix_Chromosomes2::averageRate(void) const {
    return 1.0;
}

void RateMatrix_Chromosomes2::setLambda( double l ) {
	
	lambda = l;
	
	// set flags
	needsUpdate = true;
	
}

void RateMatrix_Chromosomes2::setRho( double r ) {
	
	rho = r;
	
	// set flags
	needsUpdate = true;
	
}

void RateMatrix_Chromosomes2::setDelta( double d ) {
	
	delta = d;
	
	// set flags
	needsUpdate = true;
	
}


void RateMatrix_Chromosomes2::buildRateMatrix(void) 
{
    
    for (size_t i=0; i< matrixSize; i++) {
        for (size_t j=0; j< matrixSize; j++) {
            if (j == i+1) {
                (*theRateMatrix)[i][j] = lambda;
            } else if (j == i-1) {
                (*theRateMatrix)[i][j] = delta;
            } else if (j == ((2*i)+1)) {
                (*theRateMatrix)[i][j] = rho;
            } else {
                (*theRateMatrix)[i][j] = 0.0;
            }
        }
    }
    
    // set the diagonal values
    setDiagonal();
    
    // do I need to rescale the rates??
    //rescaleToAverageRate( 1.0 );
}




/** Calculate the transition probabilities */
void RateMatrix_Chromosomes2::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
	
	if ( theEigenSystem->isComplex() == false )
    {
		tiProbsEigens(t, P);
    }
	else
    {
		tiProbsComplexEigens(t, P);
    }
	
	
	
	
	// std::cout << "In calculateTransitionProbabilities: "<< t <<std::endl;
    
    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
	
	// Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003)
    //computeExponentialMatrixByRepeatedSquaring(t, P);
    
    //return;
}

void RateMatrix_Chromosomes2::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const {
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary. 
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    double tOver2s = t/(1024);
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P[i][j] = (*theRateMatrix)[i][j] * tOver2s; 
        }
    }
    //Add the identity matrix:
	for ( size_t i = 0; i < matrixSize; i++ ) {
		P[i][i] += 1;
	}
	//Now we can do the multiplications
	TransitionProbabilityMatrix P2 (matrixSize);
	squareMatrix (P, P2); //P2 at power 2
	squareMatrix (P2, P); //P at power 4
	squareMatrix (P, P2); //P2 at power 8
	squareMatrix (P2, P); //P at power 16
	squareMatrix (P, P2); //P2 at power 32
	squareMatrix (P2, P); //P at power 64
	squareMatrix (P, P2); //P2 at power 128
	squareMatrix (P2, P); //P at power 256
	squareMatrix (P, P2); //P2 at power 512
	squareMatrix (P2, P); //P at power 1024
	
	return;
}

inline void RateMatrix_Chromosomes2::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const {
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < matrixSize; k++ ) {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
			}
		}
	}
}



RateMatrix_Chromosomes2* RateMatrix_Chromosomes2::clone( void ) const {
    return new RateMatrix_Chromosomes2( *this );
}


/** Calculate the transition probabilities for the real case */
void RateMatrix_Chromosomes2::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const {
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValue = theEigenSystem->getRealEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<double> eigValExp(numStates);
	for (size_t s=0; s<numStates; s++)
    {
		eigValExp[s] = exp(eigenValue[s] * t);
    }
    
    // calculate the transition probabilities
	const double* ptr = &c_ijk[0];
    double*         p = P.theMatrix;
	for (size_t i=0; i<numStates; i++) 
    {
		for (size_t j=0; j<numStates; j++, ++p) 
        {
			double sum = 0.0;
			for(size_t s=0; s<numStates; s++)
            {
				sum += (*ptr++) * eigValExp[s];
            }
            
			//			P[i][j] = (sum < 0.0) ? 0.0 : sum;
			(*p) = (sum < 0.0) ? 0.0 : sum;
        }
    }
}


/** Calculate the transition probabilities for the complex case */
void RateMatrix_Chromosomes2::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const {
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValueReal = theEigenSystem->getRealEigenvalues();
    const std::vector<double>& eigenValueComp = theEigenSystem->getImagEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<std::complex<double> > ceigValExp(numStates);
	for (size_t s=0; s<numStates; s++)
    {
        std::complex<double> ev = std::complex<double>(eigenValueReal[s], eigenValueComp[s]);
		ceigValExp[s] = exp(ev * t);
    }
    
    // calculate the transition probabilities
	const std::complex<double>* ptr = &cc_ijk[0];
	for (size_t i=0; i<numStates; i++) 
    {
		for (size_t j=0; j<numStates; j++) 
        {
			std::complex<double> sum = std::complex<double>(0.0, 0.0);
			for(size_t s=0; s<numStates; s++)
				sum += (*ptr++) * ceigValExp[s];
			P[i][j] = (sum.real() < 0.0) ? 0.0 : sum.real();
        }
    }
}


/** Update the eigen system */
void RateMatrix_Chromosomes2::updateEigenSystem(void) {
    
    theEigenSystem->update();
    calculateCijk();
    
}


void RateMatrix_Chromosomes2::updateMatrix( void ) {
    
    if ( needsUpdate ) 
    {
		buildRateMatrix();
		
        // compute the off-diagonal values
        //computeOffDiagonal();
        
        // set the diagonal values
        //setDiagonal();
        
        // rescale 
        //rescaleToAverageRate( 1.0 );
        
        // now update the eigensystem
        updateEigenSystem();
        
        // clean flags
        needsUpdate = false;
    }
}


const std::vector<double>& RateMatrix_Chromosomes2::getStationaryFrequencies( void ) const {
    
    return stationaryFreqs;
}


//void RateMatrix_Chromosomes2::updateMatrix( void ) {
//    
//    if ( needsUpdate )
//    {
//        buildRateMatrix();
//        // clean flags
//        needsUpdate = false;
//    }
//}


