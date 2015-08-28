#include "CodonState.h"
#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_CodonSynonymousNonsynonymous.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_CodonSynonymousNonsynonymous::RateMatrix_CodonSynonymousNonsynonymous(size_t n) : TimeReversibleRateMatrix( n )
{
    
    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates * numStates * numStates);
    cc_ijk.resize(numStates * numStates * numStates);
    
    update();
}


/** Copy constructor */
RateMatrix_CodonSynonymousNonsynonymous::RateMatrix_CodonSynonymousNonsynonymous(const RateMatrix_CodonSynonymousNonsynonymous& m) : TimeReversibleRateMatrix( m )
{
    
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix_CodonSynonymousNonsynonymous::~RateMatrix_CodonSynonymousNonsynonymous(void)
{
    
    delete theEigenSystem;
}


RateMatrix_CodonSynonymousNonsynonymous& RateMatrix_CodonSynonymousNonsynonymous::operator=(const RateMatrix_CodonSynonymousNonsynonymous &r)
{
    
    if (this != &r)
    {
        TimeReversibleRateMatrix::operator=( r );
        
        delete theEigenSystem;
        
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
    }
    
    return *this;
}


RateMatrix_CodonSynonymousNonsynonymous& RateMatrix_CodonSynonymousNonsynonymous::assign(const Assignable &m)
{
    
    const RateMatrix_CodonSynonymousNonsynonymous *rm = dynamic_cast<const RateMatrix_CodonSynonymousNonsynonymous*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
    
}



/** Do precalculations on eigenvectors */
void RateMatrix_CodonSynonymousNonsynonymous::calculateCijk(void)
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
void RateMatrix_CodonSynonymousNonsynonymous::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    double t = rate * (startAge - endAge);
    if ( theEigenSystem->isComplex() == false )
    {
        tiProbsEigens(t, P);
    }
    else
    {
        tiProbsComplexEigens(t, P);
    }
}


RateMatrix_CodonSynonymousNonsynonymous* RateMatrix_CodonSynonymousNonsynonymous::clone( void ) const
{
    return new RateMatrix_CodonSynonymousNonsynonymous( *this );
}

/**
 *
 *		0: codon changes in more than one codon position (or stop codons)
 *		1: synonymous transition
 *		2: synonymous transversion
 *		3: non-synonymous transition
 *		4: non-synonymous transversion
 */
void RateMatrix_CodonSynonymousNonsynonymous::computeOffDiagonal( void )
{
    
    MatrixReal& m = *theRateMatrix;
    
    std::vector<double> rate = std::vector<double>(5,0);
    rate[0] = 0;
    rate[1] = kappa;
    rate[2] = 1;
    rate[3] = kappa*omega;
    rate[4] = omega;

    size_t rateClass = 0;
    
    // set the off-diagonal portions of the rate matrix
    for (size_t i=0; i<numStates; ++i)
    {
        CodonState c1 = CodonState(i);
        std::vector<unsigned int> codon_from = c1.getTripletStates();
        unsigned int codon_from_pos_1 = codon_from[0];
        unsigned int codon_from_pos_2 = codon_from[1];
        unsigned int codon_from_pos_3 = codon_from[2];
        
        AminoAcidState aa_from = c1.getAminoAcidState();
        
        for (size_t j=i+1; j<numStates; j++)
        {
            CodonState c2 = CodonState(2);
            
            std::vector<unsigned int> codon_to = c2.getTripletStates();
            unsigned int codon_to_pos_1 = codon_to[0];
            unsigned int codon_to_pos_2 = codon_to[1];
            unsigned int codon_to_pos_3 = codon_to[2];
            
            AminoAcidState aa_to = c2.getAminoAcidState();
            
            rateClass = 0;
            if (codon_from_pos_1 != codon_to_pos_1)
            {
                if ( (codon_from_pos_1 == 0 && codon_to_pos_1 == 2) || (codon_from_pos_1 == 2 && codon_to_pos_1 == 0) || // A <-> G
                     (codon_from_pos_1 == 1 && codon_to_pos_1 == 3) || (codon_from_pos_1 == 3 && codon_to_pos_1 == 1) )  // C <-> T
                {
                    rateClass = 1; // Transition at position 1
                }
                else
                {
                    rateClass = 2; // Transversion at position 1
                }
            }
            
            if (codon_from_pos_2 != codon_to_pos_2)
            {
                
                if (rateClass == -1)
                {
                    if ( (codon_from_pos_2 == 0 && codon_to_pos_2 == 2) || (codon_from_pos_2 == 2 && codon_to_pos_2 == 0) || // A <-> G
                         (codon_from_pos_2 == 1 && codon_to_pos_2 == 3) || (codon_from_pos_2 == 3 && codon_to_pos_2 == 1) )  // C <-> T
                    {
                        rateClass = 1; // Transition
                    }
                    else
                    {
                        rateClass = 2; // Transversion
                    }
                }
                else
                {
                    rateClass = 0; // Codon changes at more than one position
                }
                
            }
            
            if (codon_from_pos_3 != codon_to_pos_3)
            {
                
                if (rateClass == -1)
                {
                    if ( (codon_from_pos_3 == 0 && codon_to_pos_3 == 2) || (codon_from_pos_3 == 2 && codon_to_pos_3 == 0) || // A <-> G
                         (codon_from_pos_3 == 1 && codon_to_pos_3 == 3) || (codon_from_pos_3 == 3 && codon_to_pos_3 == 1) )  // C <-> T
                    {
                        rateClass = 1; // Transition
                    }
                    else
                    {
                        rateClass = 2; // Transversion
                    }
                    
                }
                else
                {
                    rateClass = 0; // Codon changes at more than one position
                }
                
            }
            
            if (rateClass != 0)
            {
                
                if (aa_from != aa_to)
                {
                    rateClass += 2; // Is a non-synonymous change
                }
                
            }
            
            m[i][j] = rate[rateClass] * stationaryFreqs[j];
            m[j][i] = rate[rateClass] * stationaryFreqs[i];
            
        }
    }
    
    // set flags
    needsUpdate = true;
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_CodonSynonymousNonsynonymous::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const
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
void RateMatrix_CodonSynonymousNonsynonymous::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const {
    
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
void RateMatrix_CodonSynonymousNonsynonymous::updateEigenSystem(void)
{
    
    theEigenSystem->update();
    calculateCijk();
    
}


void RateMatrix_CodonSynonymousNonsynonymous::update( void )
{
    
    if ( needsUpdate )
    {
        // compute the off-diagonal values
        computeOffDiagonal();
        
        // set the diagonal values
        setDiagonal();
        
        // rescale
        rescaleToAverageRate( 1.0 );
        
        // now update the eigensystem
        updateEigenSystem();
        
        // clean flags
        needsUpdate = false;
    }
}


