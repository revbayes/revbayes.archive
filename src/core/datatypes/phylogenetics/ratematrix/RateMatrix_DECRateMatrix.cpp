//
//  RateMatrix_DECRateMatrix.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/16/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_DECRateMatrix.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "RbVector.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_DECRateMatrix::RateMatrix_DECRateMatrix(size_t n) : GeneralRateMatrix( n ),
    numStates(n),
    numCharacters(round(log2(n))),
    dispersalRates( RbVector<RbVector<double > >( numCharacters, RbVector<double>(numCharacters, 0.0) ) ),
    extirpationRates( std::vector<double>(numCharacters, 1.0/n) ),
    rangeSize( std::vector<double>(numCharacters+1, 1.0/n) )
{

    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates * numStates * numStates);
    cc_ijk.resize(numStates * numStates * numStates);
    
    makeBits();
    makeTransitions();
    
    for (size_t i = 0; i < numStates; ++i)
    {
        for (size_t j = 0; j < numStates; ++j)
        {
            (*theRateMatrix)[i][j] = 0.0;
        }
    }
    
    update();
}


/** Copy constructor */
RateMatrix_DECRateMatrix::RateMatrix_DECRateMatrix(const RateMatrix_DECRateMatrix& m) : GeneralRateMatrix( m ) {
    
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    bits                 = m.bits;
    inverseBits          = m.inverseBits;
    transitions          = m.transitions;
    lossOrGain           = m.lossOrGain;
    transitionAreas      = m.transitionAreas;
    numCharacters        = m.numCharacters;
    numStates            = m.numStates;
    dispersalRates       = m.dispersalRates;
    extirpationRates     = m.extirpationRates;
    rangeSize            = m.rangeSize;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix_DECRateMatrix::~RateMatrix_DECRateMatrix(void) {
    
    delete theEigenSystem;
}


RateMatrix_DECRateMatrix& RateMatrix_DECRateMatrix::operator=(const RateMatrix_DECRateMatrix &r) {
    
    if (this != &r)
    {
        GeneralRateMatrix::operator=( r );
        
        delete theEigenSystem;
        
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        dispersalRates       = r.dispersalRates;
        extirpationRates     = r.extirpationRates;
        bits                 = r.bits;
        inverseBits          = r.inverseBits;
        transitions          = r.transitions;
        lossOrGain           = r.lossOrGain;
        transitionAreas      = r.transitionAreas;
        numCharacters        = r.numCharacters;
        numStates            = r.numStates;
        rangeSize            = r.rangeSize;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
        
    }
    
    return *this;
}

double RateMatrix_DECRateMatrix::averageRate(void) const
{
    double ave = 0.0;
    for (size_t i=0; i<numStates; i++)
        ave -= (*theRateMatrix)[i][i];
    return ave / numStates;
}



void RateMatrix_DECRateMatrix::fillRateMatrix( void )
{
        
    MatrixReal& m = *theRateMatrix;
    
    std::vector<std::vector<unsigned> >::iterator it;
    std::vector<unsigned>::iterator jt;
    
    // get normalized range sizes, s.t. largest range size value = 1
    double maxRangeSize = rangeSize[0];
    for (size_t i = 1; i < rangeSize.size(); i++)
    {
        if (rangeSize[i] > maxRangeSize)
        {
            maxRangeSize = rangeSize[i];
        }
    }
    std::vector<double> normalizedRangeSize = rangeSize;
    for (size_t i = 0; i < rangeSize.size(); i++)
    {
        normalizedRangeSize[i] = rangeSize[i] / maxRangeSize;
    }
    
    for (size_t i = 1; i < transitions.size(); i++)
    {
        int n = 0;
        for (size_t j = 0; j < bits[i].size(); j++)
            n += bits[i][j];
        double p = normalizedRangeSize[n+1];

        //        bool maxSize = n >= maxRangeSize;
//        for (size_t j = 0; j < bits[i].size(); j++)
//            std::cout << bits[i][j];
//        std::cout << " : ";
        
        double sum = 0.0;
        for (size_t j = 0; j < transitions[i].size(); j++)
        {
            double v = 0.0;
            // extinction
            if (lossOrGain[i][j] == 0)
            {
                v = extirpationRates[ transitionAreas[i][j][0] ];
            }
            // dispersal
            else if (lossOrGain[i][j] == 1) // && !maxSize)
            {
                for (size_t k = 0; k < transitionAreas[i][j].size(); k++)
                {
                    v += dispersalRates[ transitionAreas[i][j][k] ][ j ];
                }
                v *= p;
            }
            
            // store value
            m[i][transitions[i][j]] = v;
          
            sum += v;
            
//            std::cout << "  ";
//            for (size_t k = 0; k < bits[i].size(); k++)
//                std::cout << bits[transitions[i][j]][k];
//            std::cout << ":" << v << " ";
        }
        
        m[i][i] = -sum;
//        std::cout << "\n";
    }
    
    // set flags
    needsUpdate = true;
}

/** Do precalculations on eigenvectors */
void RateMatrix_DECRateMatrix::calculateCijk(void)
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
void RateMatrix_DECRateMatrix::calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double startAge, double endAge, double rate) const
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


RateMatrix_DECRateMatrix* RateMatrix_DECRateMatrix::clone( void ) const
{
    return new RateMatrix_DECRateMatrix( *this );
}

const RbVector<RbVector<double> >& RateMatrix_DECRateMatrix::getDispersalRates(void) const
{
    return dispersalRates;
}

const std::vector<double>& RateMatrix_DECRateMatrix::getExtirpationRates(void) const
{
    return extirpationRates;
}

const std::vector<double>& RateMatrix_DECRateMatrix::getRangeSize(void) const
{
    return rangeSize;
}


void RateMatrix_DECRateMatrix::makeBits(void)
{
    bits = std::vector<std::vector<unsigned> >(numStates, std::vector<unsigned>(numCharacters, 0));
    for (size_t i = 1; i < numStates; i++)
    {
        size_t n = i;
        for (size_t j = 0; j < numCharacters; j++)
        {
            bits[i][j] = n % 2;
            n /= 2;
            if (n == 0)
                break;
        }
    }
    for (size_t i = 0; i < numStates; i++)
    {
        inverseBits[ bits[i] ] = (unsigned)i;
    }
}

void RateMatrix_DECRateMatrix::makeTransitions(void)
{
    transitions.resize(numStates);
    lossOrGain.resize(numStates);
    transitionAreas.resize(numStates);
    
    // populate integer-valued transitions between states
    for (size_t i = 1; i < numStates; i++)
    {
        std::vector<unsigned> b = bits[i];
        
        // each row has b.size() events (excluding i==0)
        for (size_t j = 0; j < b.size(); j++)
        {
            std::vector<unsigned> tmp = b;
            
            // change the range cfg at area j
            tmp[j] = (b[j] == 0 ? 1 : 0);
            
            // store integer-valued event
            transitions[i].push_back(inverseBits[tmp]);
            
            // is event a gain or a loss?
            lossOrGain[i].push_back(tmp[j]);
            
            std::vector<unsigned> a;
            // store dispersal event source areas
            if (tmp[j]==1)
            {
                for (size_t k = 0; k < b.size(); k++)
                {
                    if (b[k]==1)
                    {
                        a.push_back((unsigned)k);
                    }
                }
            }
            // extinction events pushes only the lost area
            else
            {
                a.push_back((unsigned)j);
            }
            transitionAreas[i].push_back(a);
        }
    }
    
    
}

void RateMatrix_DECRateMatrix::setDispersalRates(const RbVector<RbVector<double> >& dr)
{
    dispersalRates = dr;
    needsUpdate = true;
}

void RateMatrix_DECRateMatrix::setExtirpationRates(const std::vector<double>& er)
{
    extirpationRates = er;
    needsUpdate = true;
}

void RateMatrix_DECRateMatrix::setRangeSize(const std::vector<double>& rs)
{
    rangeSize = rs;
    needsUpdate = true;
}

/** Calculate the transition probabilities for the real case */
void RateMatrix_DECRateMatrix::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const {
    
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
void RateMatrix_DECRateMatrix::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const {
    
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
void RateMatrix_DECRateMatrix::updateEigenSystem(void) {
    
    theEigenSystem->update();
    calculateCijk();
    
}


void RateMatrix_DECRateMatrix::update( void ) {
    
    if ( needsUpdate )
    {
        // assign all rate matrix elements
        fillRateMatrix();
        
        // rescale
        rescaleToAverageRate( 1.0 );
        
        // now update the eigensystem
        updateEigenSystem();
        
        // clean flags
        needsUpdate = false;
    }
}

