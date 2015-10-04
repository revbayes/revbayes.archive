#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_FreeSymmetric.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_FreeSymmetric::RateMatrix_FreeSymmetric(size_t n, bool r) : GeneralRateMatrix( n ),
    rescaled( r )
{
    
    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates * numStates * numStates);
    cc_ijk.resize(numStates * numStates * numStates);
    
    update();
}


/** Copy constructor */
RateMatrix_FreeSymmetric::RateMatrix_FreeSymmetric(const RateMatrix_FreeSymmetric& m) : GeneralRateMatrix( m )
{
    
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix_FreeSymmetric::~RateMatrix_FreeSymmetric(void)
{
    
    delete theEigenSystem;
}


RateMatrix_FreeSymmetric& RateMatrix_FreeSymmetric::operator=(const RateMatrix_FreeSymmetric &r)
{
    
    if (this != &r)
    {
        GeneralRateMatrix::operator=( r );
        
        delete theEigenSystem;
        
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
        
    }
    
    return *this;
}

//double RateMatrix_FreeSymmetric::averageRate(void) const
//{
//    return 1.0;
//}



void RateMatrix_FreeSymmetric::fillRateMatrix( void )
{
    
    MatrixReal& m = *theRateMatrix;
    
    // fill the rate matrix
    for (size_t i=0, k=0; i<numStates-1; ++i)
    {
        
        // off-diagonal
        for (size_t j=i+1; j<numStates; ++j)
        {
            double r = transitionRates[k];
            m[i][j] = r;
            m[j][i] = r;
            k++;
        }
        
    }
    
    // compute the diagonal values
    setDiagonal();
    
    // set flags
    needsUpdate = true;
}

/** Do precalculations on eigenvectors */
void RateMatrix_FreeSymmetric::calculateCijk(void)
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


/** Calculate the transition probabilities */
void RateMatrix_FreeSymmetric::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    
    
    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    
    // Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003)
    double t = rate * (startAge - endAge);
    computeExponentialMatrixByRepeatedSquaring(t, P);
    
//    double t = rate * (startAge - endAge);
//    if ( theEigenSystem->isComplex() == false )
//    {
//        tiProbsEigens(t, P);
//    }
//    else
//    {
//        tiProbsComplexEigens(t, P);
//    }
    
}


RateMatrix_FreeSymmetric* RateMatrix_FreeSymmetric::clone( void ) const
{
    return new RateMatrix_FreeSymmetric( *this );
}

void RateMatrix_FreeSymmetric::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const
{
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary.
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    
    // add three to s
    size_t s = 10;
    double test = 1024;
    
    double tOver2s = t/test;
    for ( size_t i = 0; i < numStates; i++ )
    {
        for ( size_t j = 0; j < numStates; j++ )
        {
            P[i][j] = (*theRateMatrix)[i][j] * tOver2s;
        }
    }
    //Add the identity matrix:
    for ( size_t i = 0; i < numStates; i++ )
    {
        P[i][i] += 1;
    }
    //Now we can do the multiplications
    TransitionProbabilityMatrix P2 (numStates);
    for (size_t i=0; i<s; i+=2)
    {
        squareMatrix (P, P2); //P2 at power 2
        squareMatrix (P2, P); //P at power 4
    }
    
//    std::cerr << *theRateMatrix << std::endl << std::endl;
//    std::cerr << P << std::endl << std::endl;
//    
    
}

inline void RateMatrix_FreeSymmetric::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const
{
    
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < numStates; i++ )
    {
        for ( size_t j = 0; j < numStates; j++ )
        {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < numStates; k++ )
            {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
            }
            
        }
        
    }
    
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_FreeSymmetric::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const
{
    
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
void RateMatrix_FreeSymmetric::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const
{
    
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
            {
                sum += (*ptr++) * ceigValExp[s];
            }
            
            P[i][j] = (sum.real() < 0.0) ? 0.0 : sum.real();
        }
        
    }
    
}


/** Update the eigen system */
void RateMatrix_FreeSymmetric::updateEigenSystem(void)
{
    
    theEigenSystem->update();
    calculateCijk();
    
}


void RateMatrix_FreeSymmetric::update( void )
{
    
    if ( needsUpdate )
    {
        // assign all rate matrix elements
        fillRateMatrix();
        
        // rescale
        if ( rescaled == true )
        {
            rescaleToAverageRate( 1.0 );
        }
        
        // now update the eigensystem
        updateEigenSystem();
        
        // clean flags
        needsUpdate = false;
    }
    
}

