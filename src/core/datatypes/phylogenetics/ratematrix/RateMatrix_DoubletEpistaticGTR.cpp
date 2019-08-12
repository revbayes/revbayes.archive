#include "CodonState.h"
#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_DoubletEpistaticGTR.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_DoubletEpistaticGTR::RateMatrix_DoubletEpistaticGTR( void ) : TimeReversibleRateMatrix( 16 ),
    nucleotide_exchange_rates( 6,0.1666667 ),
    d( 0.0 ),
    doublet_freqs( 16,0.0625 )
{

    eigen_system       = new EigenSystem(the_rate_matrix);
    c_ijk.resize(num_states * num_states * num_states);
    cc_ijk.resize(num_states * num_states * num_states);

    update();
}


/** Copy constructor */
RateMatrix_DoubletEpistaticGTR::RateMatrix_DoubletEpistaticGTR(const RateMatrix_DoubletEpistaticGTR& m) : TimeReversibleRateMatrix( m ),
    d( m.d ),
    nucleotide_exchange_rates( m.nucleotide_exchange_rates ),
    doublet_freqs( m.doublet_freqs )
{

    eigen_system        = new EigenSystem( *m.eigen_system );
    c_ijk               = m.c_ijk;
    cc_ijk              = m.cc_ijk;

    eigen_system->setRateMatrixPtr(the_rate_matrix);
}


/** Destructor */
RateMatrix_DoubletEpistaticGTR::~RateMatrix_DoubletEpistaticGTR(void)
{

    delete eigen_system;
}


RateMatrix_DoubletEpistaticGTR& RateMatrix_DoubletEpistaticGTR::operator=(const RateMatrix_DoubletEpistaticGTR &r)
{

    if (this != &r)
    {
        TimeReversibleRateMatrix::operator=( r );

        delete eigen_system;

        eigen_system        = new EigenSystem( *r.eigen_system );
        c_ijk               = r.c_ijk;
        cc_ijk              = r.cc_ijk;
        nucleotide_exchange_rates               = r.nucleotide_exchange_rates;
        d               = r.d;
        doublet_freqs    = r.doublet_freqs;

        eigen_system->setRateMatrixPtr(the_rate_matrix);
    }

    return *this;
}


RateMatrix_DoubletEpistaticGTR& RateMatrix_DoubletEpistaticGTR::assign(const Assignable &m)
{

    const RateMatrix_DoubletEpistaticGTR *rm = dynamic_cast<const RateMatrix_DoubletEpistaticGTR*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }

}

//TODO: should make a doublet data type that includes this and related index matchers
size_t RateMatrix_DoubletEpistaticGTR::N1 [] = {
    0,  0,  0,  0,
    1,  1,  1,  1,
    2,  2,  2,  2,
    3,  3,  3,  3
};

size_t RateMatrix_DoubletEpistaticGTR::N2 [] = {
    0,  1,  2,  3,
    0,  1,  2,  3,
    0,  1,  2,  3,
    0,  1,  2,  3
};

size_t RateMatrix_DoubletEpistaticGTR::is_canonical [] = {
    0,  0,  0,  1, //AA-AT
    0,  0,  1,  0, //CA-CT
    0,  1,  0,  0, //GA-GT
    1,  0,  0,  0  //TA-TT
};

/** Do precalculations on eigenvectors */
void RateMatrix_DoubletEpistaticGTR::calculateCijk(void)
{

    if ( eigen_system->isComplex() == false )
    {
        // real case
        const MatrixReal& ev  = eigen_system->getEigenvectors();
        const MatrixReal& iev = eigen_system->getInverseEigenvectors();
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
        const MatrixComplex& cev  = eigen_system->getComplexEigenvectors();
        const MatrixComplex& ciev = eigen_system->getComplexInverseEigenvectors();
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
void RateMatrix_DoubletEpistaticGTR::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    double t = rate * (startAge - endAge);
    if ( eigen_system->isComplex() == false )
    {
        tiProbsEigens(t, P);
    }
    else
    {
        tiProbsComplexEigens(t, P);
    }
}


RateMatrix_DoubletEpistaticGTR* RateMatrix_DoubletEpistaticGTR::clone( void ) const
{
    return new RateMatrix_DoubletEpistaticGTR( *this );
}

void RateMatrix_DoubletEpistaticGTR::computeOffDiagonal( void )
{

    MatrixReal& m = *the_rate_matrix;

    // This model has substitutions that affect varying numbers of sites
    // This makes normalization difficult by Rev's usual means, so we handle it here now
    double mu = 0.0;

    // set the off-diagonal portions of the rate matrix
    for (size_t i=0; i<15; ++i)
    {
      size_t x1 = N1[i];
      size_t x2 = N2[i];
// std::cout << "i = " << i << "; x1 = " << x1 << ", x2 = " << x2 << std::endl;

      // Nucleotides at each site in doublet "from"
      for (size_t j=i+1; j<16; ++j)
      {
        // Nucleotides at each site in doublet "to"
        size_t y1 = N1[j];
        size_t y2 = N2[j];

// std::cout << "  j = " << j << "; y1 = " << y1 << ", y2 = " << y2 << std::endl;
        // Single change at position 1
        if (x1 == y1)
        {
// std::cout << "    x1 == y1" << std::endl;
// std::cout << "    " << x1 << " == " << y1 << std::endl;

          // The underlying rate from the GTR model of x1 to y1
          double r_ij;
          if (x1 == 0)
          {
            r_ij = nucleotide_exchange_rates[x1 + y1 - 1];
          }
          else
          {
            r_ij = nucleotide_exchange_rates[x1 + y1];
          }
// std::cout << "r_ij = " << r_ij << std::endl;

          m[i][j] = r_ij * doublet_freqs[j];
          m[j][i] = r_ij * doublet_freqs[i];

          mu += 0.5 * m[i][j];
          mu += 0.5 * m[j][i];
        }
        // Single change at position 2
        else if (x2 == y2)
        {
// std::cout << "    x2 == y2" << std::endl;
// std::cout << "    " << x2 << " == " << y2 << std::endl;
          // The underlying rate from the GTR model of x2 to y2
          double r_ij;
          if (x2 == 0)
          {
            r_ij = nucleotide_exchange_rates[x2 + y2 - 1];
          }
          else
          {
            r_ij = nucleotide_exchange_rates[x2 + y2];
          }
          m[i][j] = r_ij * doublet_freqs[j];
          m[j][i] = r_ij * doublet_freqs[i];

          mu += 0.5 * m[i][j];
          mu += 0.5 * m[j][i];
        }

        // Allowable doublet change
        else if (is_canonical[i] && is_canonical[j])
        {
// std::cout << "    canonical to canonical" << std::endl;
          // The underlying rate from the GTR model of x1 to y1
          double r_x1_y1;
          if (x1 == 0)
          {
            r_x1_y1 = nucleotide_exchange_rates[x2 + y2 - 1];
          }
          else
          {
            r_x1_y1 = nucleotide_exchange_rates[x2 + y2];
          }

          // The underlying rate from the GTR model of x2 to y2
          double r_x2_y2;
          if (x2 == 0)
          {
            r_x2_y2 = nucleotide_exchange_rates[x2 + y2 - 1];
          }
          else
          {
            r_x2_y2 = nucleotide_exchange_rates[x2 + y2];
          }

          m[i][j] = d * r_x1_y1 * r_x2_y2 * doublet_freqs[j];
          m[j][i] = d * r_x1_y1 * r_x2_y2 * doublet_freqs[i];

          mu += m[i][j];
          mu += m[j][i];
        }
        // Unallowed doublet change
        else
        {
          m[i][j] = 0.0;
          m[j][i] = 0.0;
        }
        // std::cout << "m[" << i << "][" << j << "] = " << m[i][j] << std::endl;
      }
    }

    double mu_inverse = 1.0 / mu;
    for (size_t i=0; i<16; ++i)
    {
      for (size_t j=0; j<16; ++j)
      {
        m[i][j] *= mu_inverse;
      }
    }

    // set flags
    needs_update = true;
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_DoubletEpistaticGTR::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const
{

    // get a reference to the eigenvalues
    const std::vector<double>& eigenValue = eigen_system->getRealEigenvalues();

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
            for (size_t s=0; s<num_states; s++)
            {
                sum += (*ptr++) * eigValExp[s];
            }

            //            P[i][j] = (sum < 0.0) ? 0.0 : sum;
            (*p) = (sum < 0.0) ? 0.0 : sum;
        }
    }
}


/** Calculate the transition probabilities for the complex case */
void RateMatrix_DoubletEpistaticGTR::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const
{

    // get a reference to the eigenvalues
    const std::vector<double>& eigenValueReal = eigen_system->getRealEigenvalues();
    const std::vector<double>& eigenValueComp = eigen_system->getImagEigenvalues();

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
            for (size_t s=0; s<num_states; s++)
                sum += (*ptr++) * ceigValExp[s];
            P[i][j] = (sum.real() < 0.0) ? 0.0 : sum.real();
        }
    }
}


void RateMatrix_DoubletEpistaticGTR::setNucleotideExchangeRates(const std::vector<double> &er)
{

    nucleotide_exchange_rates = er;

    // set flags
    needs_update = true;

}


void RateMatrix_DoubletEpistaticGTR::setD(double x)
{

    d = x;

    // set flags
    needs_update = true;

}


void RateMatrix_DoubletEpistaticGTR::setDoubletFrequencies( const std::vector<double> &f )
{
    doublet_freqs = f;

    // set flags
    needs_update = true;
}


/** Update the eigen system */
void RateMatrix_DoubletEpistaticGTR::updateEigenSystem(void)
{

    eigen_system->update();
    calculateCijk();

}


void RateMatrix_DoubletEpistaticGTR::update( void )
{

    if ( needs_update )
    {
        // compute the off-diagonal values
        computeOffDiagonal();

        // set the diagonal values
        setDiagonal();

        // Already scaled to 2.0 as appropriate
        // // rescale
        // rescaleToAverageRate( 3.0 );

        // now update the eigensystem
        updateEigenSystem();

        // clean flags
        needs_update = false;
    }
}
