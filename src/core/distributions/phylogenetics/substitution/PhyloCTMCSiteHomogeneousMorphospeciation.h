#ifndef PhyloCTMCSiteHomogeneousMorphospeciation_H
#define PhyloCTMCSiteHomogeneousMorphospeciation_H

#include "PhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "Morphospeciation_ODE.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

#include <boost/numeric/odeint.hpp>

namespace RevBayesCore {

    template<class charType>
    class PhyloCTMCSiteHomogeneousMorphospeciation : public PhyloCTMCSiteHomogeneous<charType> {

    public:
        PhyloCTMCSiteHomogeneousMorphospeciation( const TypedDagNode<double> *age, const TypedDagNode<double> *lambda, const TypedDagNode<double> *lambda_a, const TypedDagNode<double> *mu, const TypedDagNode<double>* psi, const TypedDagNode<double> *rho, bool uo, const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, bool internal, bool gapmatch);
        virtual                                            ~PhyloCTMCSiteHomogeneousMorphospeciation(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousMorphospeciation*           clone(void) const;                                                                          //!< Create an independent clone


    protected:

        void                                                numericallyIntegrateProcess(double begin_age, double end_age, double rate, state_type &transition_probs) const; //!< Wrapper function for the ODE time stepper function.
        virtual void                                        updateTransitionProbabilities(size_t node_idx);
        virtual void                                        simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &t, const std::vector<bool> &inv, const std::vector<size_t> &perSiteRates);

        const TypedDagNode< double >*                       process_age;

        const TypedDagNode< double >*                       asymmetric_speciation_rate;
        const TypedDagNode< double >*                       anagenetic_speciation_rate;
        const TypedDagNode< double >*                       extinction_rate;
        const TypedDagNode< double >*                       fossilization_rate;
        const TypedDagNode< double >*                       extant_sampling_fraction;

        bool                                                use_origin;

        double                                              NUM_TIME_SLICES;
        double                                              dt;

        std::vector<size_t>                                 speciation_chars;
    };

}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>::PhyloCTMCSiteHomogeneousMorphospeciation( const TypedDagNode<double> *age,
                                                                                                            const TypedDagNode<double> *lambda,
                                                                                                            const TypedDagNode<double> *lambda_a,
                                                                                                            const TypedDagNode<double> *mu,
                                                                                                            const TypedDagNode<double>* psi,
                                                                                                            const TypedDagNode<double> *rho,
                                                                                                            bool uo,
                                                                                                            const TypedDagNode<Tree> *t,
                                                                                                            size_t nChars,
                                                                                                            bool c,
                                                                                                            size_t nSites,
                                                                                                            bool amb,
                                                                                                            bool internal,
                                                                                                            bool gapmatch) :
    PhyloCTMCSiteHomogeneous<charType>(  t, nChars, c, nSites, amb, internal, gapmatch ),
    process_age( age ),
    asymmetric_speciation_rate( lambda ),
    anagenetic_speciation_rate( lambda_a ),
    extinction_rate( mu ),
    fossilization_rate( psi ),
    extant_sampling_fraction( rho ),
    use_origin( uo ),
    NUM_TIME_SLICES( 500 )
{

        // set the length of the time slices used by the ODE for numerical integration
        dt = process_age->getValue() / NUM_TIME_SLICES * 10.0;
}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>::~PhyloCTMCSiteHomogeneousMorphospeciation( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>* RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>::clone( void ) const
{

    return new PhyloCTMCSiteHomogeneousMorphospeciation<charType>( *this );
}

/**
 * Wrapper function for the ODE time stepper function.
 */
template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>::numericallyIntegrateProcess(double begin_age, double end_age, double rate, state_type &transition_probs) const
{
    Morphospeciation_ODE ode = Morphospeciation_ODE( this->num_sites, this->num_chars, asymmetric_speciation_rate->getValue(), anagenetic_speciation_rate->getValue(), extinction_rate->getValue(), fossilization_rate->getValue(), extant_sampling_fraction->getValue(), rate);

//    double dt = root_age->getValue() / NUM_TIME_SLICES * 10;
    typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > stepper_type;
    boost::numeric::odeint::integrate_adaptive( make_controlled( 1E-7, 1E-7, stepper_type() ) , ode , transition_probs, begin_age , end_age , dt );

    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    for (size_t i = 0; i < this->num_chars; ++i)
    {
        // Sebastian: The likelihoods here are probability densities (not log-transformed).
        // These are densities because they are multiplied by the probability density of the speciation event happening.
        transition_probs[i] = ( transition_probs[i] < 0.0 ? 0.0 : transition_probs[i] );

    }

}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>::updateTransitionProbabilities(size_t node_idx)
{
    const TopologyNode* node = this->tau->getValue().getNodes()[node_idx];

    if (node->isRoot()) throw RbException("dnPhyloCTMC called updateTransitionProbabilities for the root node\n");

    // second, get the clock rate for the branch
    double rate = this->homogeneous_clock_rate->getValue();

    double end_age = node->getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(end_age) == false )
    {
        // we assume by default that the end is at time 0
        end_age = 0.0;
    }
    double start_age = end_age + node->getBranchLength();

    // first, get the rate matrix for this branch
    for (size_t j = 0; j < this->num_site_rates; ++j)
    {
        double r = 1.0;
        if ( this->rate_variation_across_sites == true )
        {
            r = this->site_rates->getValue()[j];
        }

        for( size_t from = 0; from < this->num_chars; from++)
        {
            numericallyIntegrateProcess( start_age, end_age, rate*r, this->transition_prob_matrices[j][from]);
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousMorphospeciation<charType>::simulate( const TopologyNode &node, std::vector< DiscreteTaxonData< charType > > &taxa, const std::vector<bool> &invariant, const std::vector<size_t> &perSiteMixtures)
{

    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();

    // get the sequence of this node
    size_t node_index = node.getIndex();
    const DiscreteTaxonData< charType > &parent = taxa[ node_index ];

    bool substituted = false;
    bool sa = node.isSampledAncestor(true);

    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);

        DiscreteTaxonData< charType > &taxon = taxa[ child.getIndex() ];

        if( sa == false && substituted == false )
        {
            substituted = true;

            // update the transition probability matrix
            updateTransitionProbabilities( child.getIndex() );

            std::vector<double> weights;
            double total = 0.0;
            for ( size_t i = 0; i < this->num_sites; ++i )
            {
                if ( invariant[i] == true )
                {
                    // cannot substitute an invariant character
                    weights.push_back(0);
                }
                else
                {
                    // get the ancestral character for this site
                    unsigned long parentState = parent.getCharacter( i ).getStateIndex();

                    double w = this->transition_prob_matrices[ perSiteMixtures[i] ][ parentState ][ parentState ];

                    total += -w;

                    weights.push_back(-w);
                }
            }

            // sample site
            double u = rng->uniform01()*total;

            bool found = false;

            for (size_t i = 0; i < this->num_sites; ++i)
            {
                u -= weights[i];

                if ( u < 0.0 && found == false )
                {
                    found = true;

                    unsigned long parentState = parent.getCharacter( i ).getStateIndex();

                    double * freqs = this->transition_prob_matrices[ perSiteMixtures[i] ][ parentState ];

                    // create the character
                    charType c = charType( this->num_chars );
                    c.setToFirstState();
                    // draw the state
                    double u = rng->uniform01();
                    size_t stateIndex = 0;
                    while ( true )
                    {
                        if( stateIndex != parentState )
                        {
                            u -= *freqs;
                        }
                        ++stateIndex;

                        if ( u > 0.0 && stateIndex < this->num_chars)
                        {
                            ++c;
                            ++freqs;
                        }
                        else
                        {
                            break;
                        }

                    }

                    // add the character to the sequence
                    taxon.addCharacter( c );
                }
                else
                {
                    taxon.addCharacter( parent.getCharacter( i ) );
                }
            }
        }
        else
        {
            for ( size_t i = 0; i < this->num_sites; ++i )
                taxon.addCharacter( parent.getCharacter( i ) );
        }

        if ( child.isTip() )
        {
            taxon.setTaxon( child.getTaxon() );
        }
        else
        {
            // recursively simulate the sequences
            std::stringstream ss;
            ss << "Node" << child.getIndex();
            taxon.setTaxon( Taxon(ss.str()) );
            simulate( child, taxa, invariant, perSiteMixtures );
        }

    }

}

#endif
