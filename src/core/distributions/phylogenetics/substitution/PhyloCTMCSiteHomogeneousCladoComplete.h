#ifndef PhyloCTMCSiteHomogeneousCladoComplete_H
#define PhyloCTMCSiteHomogeneousCladoComplete_H

#include "PhyloCTMCSiteHomogeneous.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

    template<class charType>
    class PhyloCTMCSiteHomogeneousCladoComplete : public PhyloCTMCSiteHomogeneous<charType> {

    public:
        PhyloCTMCSiteHomogeneousCladoComplete(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, bool internal, bool gapmatch);
        virtual                                            ~PhyloCTMCSiteHomogeneousCladoComplete(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousCladoComplete*                           clone(void) const;                                                                          //!< Create an independent clone


    protected:

        virtual void                                        updateTransitionProbabilities(size_t node_idx, double brlen);

    private:

        virtual void                                        simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &t, const std::vector<bool> &inv, const std::vector<size_t> &perSiteRates);


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
RevBayesCore::PhyloCTMCSiteHomogeneousCladoComplete<charType>::PhyloCTMCSiteHomogeneousCladoComplete(const TypedDagNode<Tree> *t, size_t nChars, bool c, size_t nSites, bool amb, bool internal, bool gapmatch) : PhyloCTMCSiteHomogeneous<charType>(  t, nChars, c, nSites, amb, internal, gapmatch )
{

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousCladoComplete<charType>::~PhyloCTMCSiteHomogeneousCladoComplete( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousCladoComplete<charType>* RevBayesCore::PhyloCTMCSiteHomogeneousCladoComplete<charType>::clone( void ) const
{

    return new PhyloCTMCSiteHomogeneousCladoComplete<charType>( *this );
}


template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousCladoComplete<charType>::updateTransitionProbabilities(size_t node_idx, double brlen)
{
    const TopologyNode* node = this->tau->getValue().getNodes()[node_idx];

    if (node->isRoot()) throw RbException("dnPhyloCTMC called updateTransitionProbabilities for the root node\n");

    // second, get the clock rate for the branch
    double rate = 1.0;
    if ( this->branch_heterogeneous_clock_rates == true )
    {
        rate = this->heterogeneous_clock_rates->getValue()[node_idx];
    }
    else if (this->homogeneous_clock_rate != NULL)
    {
        rate = this->homogeneous_clock_rate->getValue();
    }

    // we rescale the rate by the inverse of the proportion of invariant sites
    rate /= ( 1.0 - this->getPInv() );

    double end_age = node->getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(end_age) == false )
    {
        // we assume by default that the end is at time 0
        end_age = 0.0;
    }
    double start_age = end_age + node->getBranchLength();

    // first, get the rate matrix for this branch
    RateMatrix_JC jc(this->num_chars);
    const RateGenerator *rm = &jc;

    if (this->branch_heterogeneous_substitution_matrices == false )
    {
        for (size_t matrix = 0; matrix < this->num_matrices; ++matrix)
        {
            if ( this->heterogeneous_rate_matrices != NULL )
            {
                rm = &this->heterogeneous_rate_matrices->getValue()[matrix];
            }
            else if ( this->homogeneous_rate_matrix != NULL )
            {
                rm = &this->homogeneous_rate_matrix->getValue();
            }

            for (size_t j = 0; j < this->num_site_rates; ++j)
            {
                double r = 1.0;
                if ( this->rate_variation_across_sites == true )
                {
                    r = this->site_rates->getValue()[j];
                }

                for( size_t from = 0; from < rm->getNumberOfStates(); from++)
                {
                    double total = 0;
                    for( size_t to = 0; to < rm->getNumberOfStates(); to++)
                    {
                        double rr = rm->getRate(from, to, 0.0, rate * r);
                        this->transition_prob_matrices[j*this->num_matrices + matrix][from][to] = rr;

                        if( to != from )
                            total += rr;
                    }

                    for( size_t to = 0; to < rm->getNumberOfStates(); to++)
                    {
                        if( to != from )
                            this->transition_prob_matrices[j*this->num_matrices + matrix][from][to] /= total;
                    }
                }
            }
        }
    }
    else
    {
        if ( this->heterogeneous_rate_matrices != NULL )
        {
            rm = &this->heterogeneous_rate_matrices->getValue()[node_idx];
        }
        else if ( this->homogeneous_rate_matrix != NULL )
        {
            rm = &this->homogeneous_rate_matrix->getValue();
        }

        for (size_t j = 0; j < this->num_site_rates; ++j)
        {
            double r = 1.0;
            if ( this->rate_variation_across_sites == true )
            {
                r = this->site_rates->getValue()[j];
            }

            for( size_t from = 0; from < rm->getNumberOfStates(); from++)
            {
                double total = 0;
                for( size_t to = 0; to < rm->getNumberOfStates(); to++)
                {
                    double rr = rm->getRate(from, to, 0.0, rate * r);
                    this->transition_prob_matrices[j][from][to] = rr;

                    if( to != from )
                        total += rr;
                }

                for( size_t to = 0; to < rm->getNumberOfStates(); to++)
                {
                    if( to != from )
                        this->transition_prob_matrices[j][from][to] /= total;
                }
            }
        }
    }
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousCladoComplete<charType>::simulate( const TopologyNode &node, std::vector< DiscreteTaxonData< charType > > &taxa, const std::vector<bool> &invariant, const std::vector<size_t> &perSiteMixtures)
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
            updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );

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
