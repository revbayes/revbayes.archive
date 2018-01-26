#include "CodonState.h"
#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_CodonSynonymousNonsynonymousHKY.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_CodonSynonymousNonsynonymousHKY::RateMatrix_CodonSynonymousNonsynonymousHKY( void ) : TimeReversibleRateMatrix( 61 ),
    nucleotide_freqs(4,0.25),
    exclude_stop_codons( true ),
    num_states( 61 )
{
    
    eigen_system       = new EigenSystem(the_rate_matrix);
    c_ijk.resize(num_states * num_states * num_states);
    cc_ijk.resize(num_states * num_states * num_states);
    
    update();
}


/** Copy constructor */
RateMatrix_CodonSynonymousNonsynonymousHKY::RateMatrix_CodonSynonymousNonsynonymousHKY(const RateMatrix_CodonSynonymousNonsynonymousHKY& m) : TimeReversibleRateMatrix( m ),
    nucleotide_freqs( m.nucleotide_freqs )
{
    
    eigen_system        = new EigenSystem( *m.eigen_system );
    c_ijk               = m.c_ijk;
    cc_ijk              = m.cc_ijk;
    
    exclude_stop_codons = m.exclude_stop_codons;
    num_states          = m.num_states;
    
    eigen_system->setRateMatrixPtr(the_rate_matrix);
}


/** Destructor */
RateMatrix_CodonSynonymousNonsynonymousHKY::~RateMatrix_CodonSynonymousNonsynonymousHKY(void)
{
    
    delete eigen_system;
}


RateMatrix_CodonSynonymousNonsynonymousHKY& RateMatrix_CodonSynonymousNonsynonymousHKY::operator=(const RateMatrix_CodonSynonymousNonsynonymousHKY &r)
{
    
    if (this != &r)
    {
        TimeReversibleRateMatrix::operator=( r );
        
        delete eigen_system;
        
        eigen_system        = new EigenSystem( *r.eigen_system );
        c_ijk               = r.c_ijk;
        cc_ijk              = r.cc_ijk;
        nucleotide_freqs    = r.nucleotide_freqs;
        
        exclude_stop_codons = r.exclude_stop_codons;
        num_states          = r.num_states;
        
        eigen_system->setRateMatrixPtr(the_rate_matrix);
    }
    
    return *this;
}


RateMatrix_CodonSynonymousNonsynonymousHKY& RateMatrix_CodonSynonymousNonsynonymousHKY::assign(const Assignable &m)
{
    
    const RateMatrix_CodonSynonymousNonsynonymousHKY *rm = dynamic_cast<const RateMatrix_CodonSynonymousNonsynonymousHKY*>(&m);
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
void RateMatrix_CodonSynonymousNonsynonymousHKY::calculateCijk(void)
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
void RateMatrix_CodonSynonymousNonsynonymousHKY::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
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


RateMatrix_CodonSynonymousNonsynonymousHKY* RateMatrix_CodonSynonymousNonsynonymousHKY::clone( void ) const
{
    return new RateMatrix_CodonSynonymousNonsynonymousHKY( *this );
}

/**
 *
 *        0: codon changes in more than one codon position (or stop codons)
 *        1: synonymous transition
 *        2: synonymous transversion
 *        3: non-synonymous transition
 *        4: non-synonymous transversion
 */
void RateMatrix_CodonSynonymousNonsynonymousHKY::computeOffDiagonal( void )
{
    
    MatrixReal& m = *the_rate_matrix;
    
    std::vector<double> rate = std::vector<double>(5,0);
    rate[0] = 0.0;
    rate[1] = kappa;
    rate[2] = 1;
    rate[3] = kappa*omega;
    rate[4] = omega;
    
    int rateClass = 0;
    std::string v_codons_without_stop [] = {
        "AAA", "AAC", "AAG", "AAT",
        "ACA", "ACC", "ACG", "ACT",
        "AGA", "AGC", "AGG", "AGT",
        "ATA", "ATC", "ATG", "ATT",
        "CAA", "CAC", "CAG", "CAT",
        "CCA", "CCC", "CCG", "CCT",
        "CGA", "CGC", "CGG", "CGT",
        "CTA", "CTC", "CTG", "CTT",
        "GAA", "GAC", "GAG", "GAT",
        "GCA", "GCC", "GCG", "GCT",
        "GGA", "GGC", "GGG", "GGT",
        "GTA", "GTC", "GTG", "GTT",
        "TAC", "TAT",
        "TCA", "TCC", "TCG", "TCT",
        "TGC", "TGG", "TGT",
        "TTA", "TTC", "TTG", "TTT",
    };
    std::vector<std::string> codons_without_stop(v_codons_without_stop, v_codons_without_stop + sizeof(v_codons_without_stop) / sizeof(std::string) );
    
    std::string v_codons_with_stop [] = {
        "AAA", "AAC", "AAG", "AAT",
        "ACA", "ACC", "ACG", "ACT",
        "AGA", "AGC", "AGG", "AGT",
        "ATA", "ATC", "ATG", "ATT",
        "CAA", "CAC", "CAG", "CAT",
        "CCA", "CCC", "CCG", "CCT",
        "CGA", "CGC", "CGG", "CGT",
        "CTA", "CTC", "CTG", "CTT",
        "GAA", "GAC", "GAG", "GAT",
        "GCA", "GCC", "GCG", "GCT",
        "GGA", "GGC", "GGG", "GGT",
        "GTA", "GTC", "GTG", "GTT",
        "TAA", "TAC", "TAG", "TAT",
        "TCA", "TCC", "TCG", "TCT",
        "TGA", "TGC", "TGG", "TGT",
        "TTA", "TTC", "TTG", "TTT",
    };
    std::vector<std::string> codons_with_stop(v_codons_with_stop, v_codons_with_stop + sizeof(v_codons_with_stop) / sizeof(std::string) );
    
    std::vector<std::string> codons = codons_without_stop;
    
    
    // set the off-diagonal portions of the rate matrix
    for (size_t i=0; i<num_states; ++i)
    {
        
        CodonState c1 = CodonState( codons[i] );
        std::vector<unsigned int> codon_from = c1.getTripletStates();
        unsigned int codon_from_pos_1 = codon_from[0];
        unsigned int codon_from_pos_2 = codon_from[1];
        unsigned int codon_from_pos_3 = codon_from[2];
        
        AminoAcidState aa_from = c1.getAminoAcidState();
        
        for (size_t j=i+1; j<num_states; ++j)
        {
            
            size_t position_of_change = 0;
            
            CodonState c2 = CodonState( codons[j] );
            
            std::vector<unsigned int> codon_to = c2.getTripletStates();
            unsigned int codon_to_pos_1 = codon_to[0];
            unsigned int codon_to_pos_2 = codon_to[1];
            unsigned int codon_to_pos_3 = codon_to[2];
            
            AminoAcidState aa_to = c2.getAminoAcidState();
            
            rateClass = -1;
            if (codon_from_pos_1 != codon_to_pos_1)
            {
                // change at position 0
                position_of_change = 0;
                
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
                
                // change at position 1
                position_of_change = 1;
                
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
                
                // change at position 2
                position_of_change = 2;
                
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
            
            double s = 1.0;
            if ( exclude_stop_codons == true && ( c1.isStopCodon() == true || c2.isStopCodon() == true ) )
            {
                s = 0.0;
            }
            
            size_t index_nuc_from   = codon_from[position_of_change];
            size_t index_nuc_to     = codon_to[position_of_change];
            m[i][j] = rate[rateClass] * nucleotide_freqs[index_nuc_to] * s;
            m[j][i] = rate[rateClass] * nucleotide_freqs[index_nuc_from] * s;
            
        }
    }
    
    // set flags
    needs_update = true;
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_CodonSynonymousNonsynonymousHKY::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const
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
void RateMatrix_CodonSynonymousNonsynonymousHKY::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const
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


void RateMatrix_CodonSynonymousNonsynonymousHKY::setKappa(double k)
{
    
    kappa = k;
    
    // set flags
    needs_update = true;
    
}


void RateMatrix_CodonSynonymousNonsynonymousHKY::setOmega(double o)
{
    
    omega = o;
    
    // set flags
    needs_update = true;
    
}


void RateMatrix_CodonSynonymousNonsynonymousHKY::setNucleotideFrequencies( const std::vector<double> &f )
{
    nucleotide_freqs = f;
    
    // set flags
    needs_update = true;
}


/** Update the eigen system */
void RateMatrix_CodonSynonymousNonsynonymousHKY::updateEigenSystem(void)
{
    
    eigen_system->update();
    calculateCijk();
    
}


void RateMatrix_CodonSynonymousNonsynonymousHKY::update( void )
{
    
    if ( needs_update )
    {
        // compute the off-diagonal values
        computeOffDiagonal();
        
        // we also need to update the stationary frequencies
        //        this->stationary_freqs = this->calculateStationaryFrequencies();
        this->stationary_freqs = std::vector<double>(num_states,0.0);
        //        this->stationary_freqs = std::vector<double>(61,1.0/61.0);
        size_t index_curr  = 0;
        size_t index_total = 0;
        double sum = 0.0;
        for (size_t i=0; i<4; ++i)
        {
            for (size_t j=0; j<4; ++j)
            {
                for (size_t k=0; k<4; ++k)
                {
                    if ( exclude_stop_codons == false || ( index_total != 48 && index_total != 50 && index_total != 56 ) )
                    {
                        this->stationary_freqs[ index_curr ] = nucleotide_freqs[i] * nucleotide_freqs[j] * nucleotide_freqs[k];
                        sum += this->stationary_freqs[ index_curr ];
                        index_curr++;
                    }
                    index_total++;
                }
            }
        }
        
        // normalize frequencies
        for (size_t i=0; i<num_states; ++i)
        {
            this->stationary_freqs[ i ] /= sum;
        }
        
        // set the diagonal values
        setDiagonal();
        
        // rescale
        rescaleToAverageRate( 1.0 );
        
        // now update the eigensystem
        updateEigenSystem();
        
        // clean flags
        needs_update = false;
    }
}



