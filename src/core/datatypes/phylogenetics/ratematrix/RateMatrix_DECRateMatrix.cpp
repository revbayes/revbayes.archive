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
RateMatrix_DECRateMatrix::RateMatrix_DECRateMatrix(size_t ns, size_t nc, bool cs, bool ex, bool os, bool uc, size_t mrs) : GeneralRateMatrix( ns ),
    num_states(ns),
    numCharacters(nc),
    dispersalRates( RbVector<RbVector<double > >( numCharacters, RbVector<double>(numCharacters, 0.0) ) ),
    extirpationRates( std::vector<double>(numCharacters, 1.0/ns) ),
    rangeSize( std::vector<double>(numCharacters, 1.0/ns) ),
    birthRate(0.0),
    useSquaring(!true),
    excludeNullRange(ex),
    conditionSurvival(cs),
    orderStatesByNum(os),
    useCladogenesis(uc),
    maxRangeSize(mrs)
{

    theEigenSystem = new EigenSystem(the_rate_matrix);
    c_ijk.resize(num_states * num_states * num_states);
    cc_ijk.resize(num_states * num_states * num_states);
    
    makeBits();
    makeTransitions();
    
    for (size_t i = 0; i < num_states; ++i)
    {
        for (size_t j = 0; j < num_states; ++j)
        {
            (*the_rate_matrix)[i][j] = 0.0;
        }
    }
    
    update();
}


/** Copy constructor */
RateMatrix_DECRateMatrix::RateMatrix_DECRateMatrix(const RateMatrix_DECRateMatrix& m) : GeneralRateMatrix( m ) {
    
    bits                 = m.bits;
    inverseBits          = m.inverseBits;
    statesToBitsByNumOn  = m.statesToBitsByNumOn;
    bitsToStatesByNumOn  = m.bitsToStatesByNumOn;
    transitions          = m.transitions;
    lossOrGain           = m.lossOrGain;
    transitionAreas      = m.transitionAreas;
    numCharacters        = m.numCharacters;
    num_states           = m.num_states;
    useSquaring          = m.useSquaring;
    conditionSurvival    = m.conditionSurvival;
    excludeNullRange     = m.excludeNullRange;
    orderStatesByNum     = m.orderStatesByNum;
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    dispersalRates       = m.dispersalRates;
    extirpationRates     = m.extirpationRates;
    rangeSize            = m.rangeSize;
    birthRate            = m.birthRate;
    cladogeneticMatrix   = m.cladogeneticMatrix;
    useCladogenesis      = m.useCladogenesis;
    maxRangeSize         = m.maxRangeSize;
    
    theEigenSystem->setRateMatrixPtr(the_rate_matrix);

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
        
        bits                 = r.bits;
        inverseBits          = r.inverseBits;
        statesToBitsByNumOn  = r.statesToBitsByNumOn;
        bitsToStatesByNumOn  = r.bitsToStatesByNumOn;
        transitions          = r.transitions;
        lossOrGain           = r.lossOrGain;
        transitionAreas      = r.transitionAreas;
        numCharacters        = r.numCharacters;
        num_states           = r.num_states;
        useSquaring          = r.useSquaring;
        conditionSurvival    = r.conditionSurvival;
        excludeNullRange     = r.excludeNullRange;
        orderStatesByNum     = r.orderStatesByNum;
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        dispersalRates       = r.dispersalRates;
        extirpationRates     = r.extirpationRates;
        rangeSize            = r.rangeSize;
        birthRate            = r.birthRate;
        cladogeneticMatrix   = r.cladogeneticMatrix;
        useCladogenesis      = r.useCladogenesis;
        maxRangeSize         = r.maxRangeSize;

        theEigenSystem->setRateMatrixPtr(the_rate_matrix);
        
    }
    
    return *this;
}

RateMatrix_DECRateMatrix& RateMatrix_DECRateMatrix::assign(const Assignable &m)
{
    const RateMatrix_DECRateMatrix *rm = dynamic_cast<const RateMatrix_DECRateMatrix*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }

}

double RateMatrix_DECRateMatrix::averageRate(void) const
{
    double ave = 0.0;
    for (size_t i=0; i<num_states; i++)
        ave -= (*the_rate_matrix)[i][i];
    return ave / num_states;
}



void RateMatrix_DECRateMatrix::fillRateMatrix( void )
{
        
    MatrixReal& m = *the_rate_matrix;
    
    std::vector<std::vector<unsigned> >::iterator it;
    std::vector<unsigned>::iterator jt;
    
    // get normalize range sizes s.t. expected multiplier == 1
    std::vector<double> normalizedRangeSize = rangeSize;
    for (size_t i = 0; i < rangeSize.size(); i++)
    {
        normalizedRangeSize[i] = rangeSize[i] * rangeSize.size();
    }
    
    for (size_t i = 1; i < transitions.size(); i++)
    {
        int n = 0;
        for (size_t j = 0; j < bits[i].size(); j++)
            n += bits[i][j];
        double p = normalizedRangeSize[n-1];

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
                if (!excludeNullRange || j > 0)
                    v = extirpationRates[ transitionAreas[i][j][0] ];
                else if (excludeNullRange && j == 0)
                    v = 1e-100;
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
    needs_update = true;
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
        for (size_t i=0; i<num_states; i++)
        {
            for (size_t j=0; j<num_states; j++)
            {
                for (size_t k=0; k<num_states; k++)
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
        for (size_t i=0; i<num_states; i++)
        {
            for (size_t j=0; j<num_states; j++)
            {
                for (size_t k=0; k<num_states; k++)
                {
                    *(pc++) = cev[i][k] * ciev[k][j];
                }
            }
        }
    }
}


/** Calculate the transition probabilities */
void RateMatrix_DECRateMatrix::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    double t = rate * (startAge - endAge);
    if (useSquaring) {
        //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
        computeExponentialMatrixByRepeatedSquaring(t, P);
    }
	else if ( theEigenSystem->isComplex() == false )
    {
		tiProbsEigens(t, P);
    }
	else
    {
		tiProbsComplexEigens(t, P);
    }
    
    // condition P_ij on j!=0, P'_ij = P_ij / (1.0 - P_i0)
    if (conditionSurvival)
    {
        for (size_t i = 0; i < num_states; i++) {
            for (size_t j = 1; j < num_states; j++) {
                P[i][j] = P[i][j] / (1.0 - P[i][0]);
            }
            P[i][0] = 0.0;
        }
        for (size_t i = 0; i < num_states; i++) {
            P[0][i] = 0.0;
        }
        
    }
    // std::cout << P << "\n";
    
    return;
}

void RateMatrix_DECRateMatrix::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const {
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary.
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    size_t num_squaring = 2*5;
    size_t d_squaring = (size_t)pow(2,num_squaring);
    double tOver2s = t/(d_squaring);
    
    for ( size_t i = 0; i < num_states; i++ ) {
        for ( size_t j = 0; j < num_states; j++ ) {
            P[i][j] = (*the_rate_matrix)[i][j] * tOver2s;
        }
    }
    //Add the identity matrix:
    for ( size_t i = 0; i < num_states; i++ ) {
        P[i][i] += 1;
    }
    //Now we can do the multiplications
    TransitionProbabilityMatrix P2 (num_states);
    
    for (size_t i = 0; i < num_squaring; i += 2)
    {
        squareMatrix (P, P2); //P2 at power 2^i
        squareMatrix (P2, P); //P at power 2^i+1
    }
    
    if (false) {
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
    }
    
    return;
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
    
    size_t num_all_states = (size_t)pow(2,numCharacters);
    bitsByNumOn.resize(numCharacters+1);
    statesToBitsByNumOn.resize(num_all_states);
    bits = std::vector<std::vector<unsigned> >(num_all_states, std::vector<unsigned>(numCharacters, 0));
    bitsByNumOn[0].push_back(bits[0]);
    for (size_t i = 1; i < num_all_states; i++)
    {
        size_t m = i;
        for (size_t j = 0; j < numCharacters; j++)
        {
            bits[i][j] = m % 2;
            m /= 2;
            if (m == 0)
                break;
        }
        size_t j = numBitsOn(bits[i]);
        bitsByNumOn[j].push_back(bits[i]);
        
    }
    for (size_t i = 0; i < num_all_states; i++)
    {
        inverseBits[ bits[i] ] = (unsigned)i;
    }
    
    // assign state to each bit vector, sorted by numOn
    size_t k = 0;
    for (size_t i = 0; i < bitsByNumOn.size(); i++)
    {
        for (size_t j = 0; j < bitsByNumOn[i].size(); j++)
        {
            statesToBitsByNumOn[k++] = bitsByNumOn[i][j];
        }
    }
    
    for (size_t i = 0; i < statesToBitsByNumOn.size(); i++)
    {
        bitsToStatesByNumOn[ statesToBitsByNumOn[i] ] = (unsigned)i;
    }
    

    
}

size_t RateMatrix_DECRateMatrix::numBitsOn(std::vector<unsigned> v)
{
    size_t n = 0;
    for (size_t i = 0; i < v.size(); i++) {
        n += v[i];
    }
    return n;
}

void RateMatrix_DECRateMatrix::makeTransitions(void)
{
    
    transitions.resize(num_states);
    lossOrGain.resize(num_states);
    transitionAreas.resize(num_states);
    
    // populate integer-valued transitions between states
    for (size_t i = 1; i < num_states; i++)
    {
//        std::vector<unsigned> b = bits[i];
        
        std::vector<unsigned> b;
        if (orderStatesByNum)
            b = statesToBitsByNumOn[i];
        else
            b = bits[i];
        
        // each row has b.size() events (excluding i==0)
        for (size_t j = 0; j < b.size(); j++)
        {
            std::vector<unsigned> tmp = b;
            
            // change the range cfg at area j
            tmp[j] = (b[j] == 0 ? 1 : 0);
            
            // ignore events larger than maxRangeSize
            if (numBitsOn(tmp) > maxRangeSize || numBitsOn(b) > maxRangeSize)
            {
//                std::cout << i << " : ";
//                for (size_t k = 0; k < b.size(); k++)
//                    std::cout << b[k];
//                std::cout << " -> ";
//                for (size_t k = 0; k < b.size(); k++)
//                    std::cout << tmp[k];
//                std::cout << "\n";
                continue;
            }
            
            // store integer-valued event
//            transitions[i].push_back(inverseBits[tmp]);
            if (orderStatesByNum)
                transitions[i].push_back(bitsToStatesByNumOn[tmp]);
            else
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
    needs_update = true;
}

void RateMatrix_DECRateMatrix::setExtirpationRates(const std::vector<double>& er)
{
    extirpationRates = er;
    needs_update = true;
}

void RateMatrix_DECRateMatrix::setRangeSize(const std::vector<double>& rs)
{
    rangeSize = rs;
    needs_update = true;
}

void RateMatrix_DECRateMatrix::setCladogeneticMatrix(const RevBayesCore::MatrixReal &cp)
{
    cladogeneticMatrix = cp;
    needs_update = true;
}

void RateMatrix_DECRateMatrix::setBirthRate(const double &br)
{
    birthRate = br;
    needs_update = true;
}

inline void RateMatrix_DECRateMatrix::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const {
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < num_states; i++ ) {
        for ( size_t j = 0; j < num_states; j++ ) {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < num_states; k++ ) {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
            }
        }
    }
}


/** Calculate the transition probabilities for the real case */
void RateMatrix_DECRateMatrix::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const
{
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValue = theEigenSystem->getRealEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<double> eigValExp(num_states);
	for (size_t s=0; s<num_states; s++)
    {
		eigValExp[s] = exp(eigenValue[s] * t);
    }
    
    // calculate the transition probabilities
	const double* ptr = &c_ijk[0];
    double*         p = P.theMatrix;
	for (size_t i=0; i<num_states; i++)
    {
		for (size_t j=0; j<num_states; j++, ++p)
        {
			double sum = 0.0;
			for(size_t s=0; s<num_states; s++)
            {
				sum += (*ptr++) * eigValExp[s];
            }
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
    std::vector<std::complex<double> > ceigValExp(num_states);
	for (size_t s=0; s<num_states; s++)
    {
        std::complex<double> ev = std::complex<double>(eigenValueReal[s], eigenValueComp[s]);
		ceigValExp[s] = exp(ev * t);
    }
    
    // calculate the transition probabilities
	const std::complex<double>* ptr = &cc_ijk[0];
	for (size_t i=0; i<num_states; i++)
    {
		for (size_t j=0; j<num_states; j++)
        {
			std::complex<double> sum = std::complex<double>(0.0, 0.0);
			for(size_t s=0; s<num_states; s++)
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
    
    if ( needs_update )
    {
        // assign all rate matrix elements
        fillRateMatrix();
        
        // rescale
        rescaleToAverageRate( 1.0 );
        
        // now update the eigensystem
        if (!useSquaring)
            updateEigenSystem();
        

//        std::cout << *the_rate_matrix << "\n";
        
        // clean flags
        needs_update = false;
    }
}

