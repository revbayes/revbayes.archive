//
//  RateMatrix_ReversiblePomo.cpp
//
//  Created by Bastien Boussau on 4/4/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//

#include "RateMatrix_ReversiblePomo.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_ReversiblePomo::RateMatrix_ReversiblePomo(size_t n) : AbstractRateMatrix( n ), Q_mut( 4 ), N( 10 ), matrixSize( n )
{

    update();
}

/** Construct rate matrix with n states and a matrix of mutation rates */
RateMatrix_ReversiblePomo::RateMatrix_ReversiblePomo(size_t n,  const RateGenerator &Qmut) : AbstractRateMatrix( n ), Q_mut( static_cast< const RateMatrix_GTR& >( Qmut ) ), N( 10 ), matrixSize( n )
{

    stationary_freqs = Q_mut.getStationaryFrequencies();
    update();
}

/** Construct rate matrix with n states, a matrix of mutation rates, and a virtual population size */
RateMatrix_ReversiblePomo::RateMatrix_ReversiblePomo(const size_t n,  const RateGenerator &Qmut, const size_t vps )  : AbstractRateMatrix( n ), Q_mut( static_cast< const RateMatrix_GTR& > ( Qmut ) ), N( vps ), matrixSize( n )
{
  stationary_freqs = Q_mut.getStationaryFrequencies();
  update();
}


/** Destructor */
RateMatrix_ReversiblePomo::~RateMatrix_ReversiblePomo(void)
{

}


/**
 * Assign the value of m to this instance. This function is our mechanism to call the assignment operator.
 *
 *
 */
RateMatrix_ReversiblePomo& RateMatrix_ReversiblePomo::assign(const Assignable &m)
{

    const RateMatrix_ReversiblePomo *rm = dynamic_cast<const RateMatrix_ReversiblePomo*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}

double RateMatrix_ReversiblePomo::averageRate(void) const
{
    return 1.0;
}


// Code modified from IQ-Tree:
void RateMatrix_ReversiblePomo::decomposeState(int state, int &i, int &nt1, int &nt2) {
    if (state < 4) {
        // Fixed A, C, G or T
        i = N;
        nt1 = state;
        nt2 = -1; // -1 for unknown nt
    } else if (state < 4+(N-1)) {
        // (iA,N-iC)
        i = state-3;
        nt1 = 0; // A
        nt2 = 1; // C
    } else if (state < 4+2*(N-1)) {
        // (iA,N-iG)
        i = state-3-(N-1);
        nt1 = 0; // A
        nt2 = 2; // G
    } else if (state < 4+3*(N-1)) {
        // (iA,N-iT)
        i = state-3-2*(N-1);
        nt1 = 0; // A
        nt2 = 3; // T
    } else if (state < 4+4*(N-1)) {
        // (iC,N-iG)
        i = state-3-3*(N-1);
        nt1 = 1; // C
        nt2 = 2; // G
    } else if (state < 4+5*(N-1)) {
        // (iC,N-iT)
        i = state-3-4*(N-1);
        nt1 = 1; // C
        nt2 = 3; // T
    } else if (state < 4+6*(N-1)) {
        // (iG,N-iT)
        i = state-3-5*(N-1);
        nt1 = 2; // G
        nt2 = 3; // T
    } else {
      throw RbException("State exceeds limit");
    }
}

// Code modified from IQ-Tree:
// Here, we return directly m_xy * \pi_xy, whereas the IQ-Tree code returned only m_xy (I think)
double RateMatrix_ReversiblePomo::mutCoeff(int nt1, int nt2) {
    assert(nt1!=nt2 && nt1<4 && nt2<4);
    // if (nt2 < nt1) {
    //     int tmp=nt1;
    //     nt1=nt2;
    //     nt2=tmp;
    // }
    if (nt1==0) return Q_mut.getRate(nt1, nt2);//mutation_prob[nt2-1];
    //if (nt1==1) return mutation_prob[nt2+1];
    //if (nt1==2) return mutation_prob[5];
    assert(0);
}


// Code modified from IQ-Tree:
double RateMatrix_ReversiblePomo::computeProbBoundaryMutation(int state1, int state2) {
    // The transition rate to the same state will be calculated by
    // (row_sum = 0).
    assert(state1 != state2);

    // Both states are decomposed into the abundance of the first
    // allele as well as the nucleotide of the first and the second
    // allele.
    int i1=0, i2=0, nt1=-1, nt2=-1, nt3=-1, nt4=-1;
    decomposeState(state1, i1, nt1, nt2);
    decomposeState(state2, i2, nt3, nt4);

    // Either the first nucleotides match or the first of state 1 with
    // the second of state 2 or the first of state 2 with the second
    // of state 1.  Additionally, we have to consider fixed states as
    // special cases.
    if (nt1 == nt3 && (nt2==nt4 || nt2==-1 || nt4 == -1)) {
        assert(i1 != i2); // because state1 != state2
        if (i1+1==i2)
            // e.g.: 2A8C -> 3A7C or 9A1C -> 10A
            // Changed Dom Tue Sep 29 13:31:02 CEST 2015
            // return double(i1*(N-i1)) / double(N*N);
            return double(i1*(N-i1)) / double(N);
        else if (i1-1 == i2)
            // e.g.: 3A7C -> 2A8C or 10A -> 9A1C
            if (nt2 == -1)
                // e.g. 10A -> 9A1C
                // return mutCoeff(nt1,nt4) * state_freq[nt4];
                return mutCoeff(nt1,nt4) ;//* freq_fixed_states[nt4];
            else
                // e.g. 9A1C -> 8A2C
                // Changed Dom Tue Sep 29 13:30:43 CEST 2015
                // return double(i1*(N-i1)) / double(N*N);
                return double(i1*(N-i1)) / double(N);
        else
            return 0.0;
    } else if (nt1 == nt4 && nt2 == -1 && i2 == 1)  {
        // e.g.: 10G -> 1A9G
        //return mutCoeff(nt1,nt3) * state_freq[nt3];
        return mutCoeff(nt1,nt3) ;//* freq_fixed_states[nt3];
    } else if (nt2 == nt3  && i1 == 1 && nt4 == -1) {
        // E.g.: 1A9G -> 10G
        // Changed Dom Tue Sep 29 13:30:25 CEST 2015
        // return double(i1*(N-i1)) / double(N*N);
        return double(i1*(N-i1)) / double(N);
    } else
        // 0 for all other transitions
        return 0.0;
}





void RateMatrix_ReversiblePomo::buildRateMatrix(void)
{
    // the destination state is in column, the starting state in rows.
        int state1, state2;

        // Activate this if frequencies of fixed states sum up to 1.0.
        // updateFreqFixedState();

      //  computeStateFreq();

        // Code modified from IQ-Tree
        // Loop over rows (transition starting from state1).
        for (state1 = 0; state1 < num_states; state1++) {
            double row_sum = 0.0;
            // Loop over columns in row state1 (transition to state2).
            for (state2 = 0; state2 < num_states; state2++)
                if (state2 != state1) {
                    row_sum +=
                        ((*the_rate_matrix)[state1][state2] =
                         computeProbBoundaryMutation(state1, state2));
                }
            (*the_rate_matrix)[state1][state1] = -(row_sum);
        }
        // if (verbose_mode >= VB_MAX) {
        //     std::cout << std::setprecision(7)
        //               << "DEBUG: Rate Matrix calculated." << std::endl
        //               << "DEBUG: mu=" << "\t"
        //               << mutation_prob[0] << "\t"
        //               << mutation_prob[1] << "\t"
        //               << mutation_prob[2] << "\t"
        //               << mutation_prob[3] << "\t"
        //               << mutation_prob[4] << "\t"
        //               << mutation_prob[5] << std::endl;
        //     std::cout << "DEBUG: " << std::setprecision(3) << "PIs:\t"
        //               << freq_fixed_states[0] << "\t"
        //               << freq_fixed_states[1] << "\t"
        //               << freq_fixed_states[2] << "\t"
        //               << freq_fixed_states[3] << std::endl;
        // }

}

/** Calculate the transition probabilities */
void RateMatrix_ReversiblePomo::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
   // std::cout << "In calculateTransitionProbabilities: "<< t <<std::endl;

    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    double t = rate * (startAge - endAge);
    computeExponentialMatrixByRepeatedSquaring(t, P);

/*    for (size_t i = 0 ; i<58; ++i) {
      //  for (size_t j = 0 ; j<58; ++j) {
            std::cout << "t: "<< t <<  " Diag "<< i << " : "<< P.getElement(i, i)<<std::endl;
        //}

    }*/

    return;
}

void RateMatrix_ReversiblePomo::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const {
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary.
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    double tOver2s = t/(1024);
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P[i][j] = (*the_rate_matrix)[i][j] * tOver2s;
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

inline void RateMatrix_ReversiblePomo::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const {
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



RateMatrix_ReversiblePomo* RateMatrix_ReversiblePomo::clone( void ) const
{
    return new RateMatrix_ReversiblePomo( *this );
}

std::vector<double> RateMatrix_ReversiblePomo::getStationaryFrequencies( void ) const
{

    return stationary_freqs;
}


void RateMatrix_ReversiblePomo::update( void )
{

    if ( needs_update )
    {
        buildRateMatrix();
        // clean flags
        needs_update = false;
    }
}


void RateMatrix_ReversiblePomo::setMutationRates(const RateMatrix& mm)
{

    Q_mut = RateMatrix_GTR ( static_cast<const RateMatrix_GTR& >( mm ) );
    // double age = 0.0;
    // double rate = 1.0;
    // mu[0][1] = mm.getRate(0,1,age,rate);
    // mu[0][2] = mm.getRate(0,2,age,rate);
    // mu[0][3] = mm.getRate(0,3,age,rate);
    // mu[1][0] = mm.getRate(1,0,age,rate);
    // mu[1][2] = mm.getRate(1,2,age,rate);
    // mu[1][3] = mm.getRate(1,3,age,rate);
    // mu[2][0] = mm.getRate(2,0,age,rate);
    // mu[2][1] = mm.getRate(2,1,age,rate);
    // mu[2][3] = mm.getRate(2,3,age,rate);
    // mu[3][0] = mm.getRate(3,0,age,rate);
    // mu[3][1] = mm.getRate(3,1,age,rate);
    // mu[3][2] = mm.getRate(3,2,age,rate);
}
