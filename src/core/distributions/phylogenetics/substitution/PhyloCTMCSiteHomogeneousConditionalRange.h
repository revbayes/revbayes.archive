#ifndef PhyloCTMCSiteHomogeneousConditionalRange_H
#define PhyloCTMCSiteHomogeneousConditionalRange_H

#include "PhyloCTMCSiteHomogeneousConditional.h"

namespace RevBayesCore {

    template<class charType>
    class PhyloCTMCSiteHomogeneousConditionalRange : public PhyloCTMCSiteHomogeneousConditional<charType> {

    public:
        PhyloCTMCSiteHomogeneousConditionalRange(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, AscertainmentBias::Coding cod = AscertainmentBias::ALL, bool internal = false, bool gapmatch = true);
        PhyloCTMCSiteHomogeneousConditionalRange(const PhyloCTMCSiteHomogeneousConditionalRange &n);
        virtual                                            ~PhyloCTMCSiteHomogeneousConditionalRange(void);                                                                   //!< Virtual destructor

        // public member functions
        PhyloCTMCSiteHomogeneousConditionalRange*           clone(void) const;                                                                        //!< Create an independent clone

        double                                              computeLnProbability(void);

    protected:

        Taxon                                               updateNodeSpecies(const TopologyNode& node);

        virtual void                                        updateTransitionProbabilities(size_t node_idx);

    private:

        std::vector<Taxon>                                  node_taxa;
    };

}

#include <climits>

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::PhyloCTMCSiteHomogeneousConditionalRange(const TypedDagNode<Tree> *t, size_t nChars, bool c, size_t nSites, bool amb, AscertainmentBias::Coding ty, bool internal, bool gapmatch) :
    PhyloCTMCSiteHomogeneousConditional<charType>(  t, nChars, c, nSites, amb, ty, internal, gapmatch )
{
    node_taxa = std::vector<Taxon>( this->tau->getValue().getNumberOfNodes(), Taxon() );
}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::~PhyloCTMCSiteHomogeneousConditionalRange( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}


template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>* RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::clone( void ) const {

    return new PhyloCTMCSiteHomogeneousConditionalRange<charType>( *this );
}

template<class charType>
RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::PhyloCTMCSiteHomogeneousConditionalRange(const PhyloCTMCSiteHomogeneousConditionalRange &n) :
PhyloCTMCSiteHomogeneousConditional< charType>( n ), node_taxa( n.node_taxa )
{

}

template<class charType>
double RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::computeLnProbability()
{
    updateNodeSpecies(this->tau->getValue().getRoot());

    return PhyloCTMCSiteHomogeneousConditional<charType>::computeLnProbability();
}

template<class charType>
RevBayesCore::Taxon RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::updateNodeSpecies(const TopologyNode& node)
{
    if ( node.isTip() )
    {
        node_taxa[node.getIndex()] = node.getTaxon();
    }

    std::vector<TopologyNode* > children = node.getChildren();

    bool sa = node.isSampledAncestor(true);

    for(size_t c = 0; c < children.size(); c++)
    {
        const TopologyNode& child = *children[c];

        Taxon species = updateNodeSpecies(child);

        // is the child the same species?
        if( ( sa == false && c == 0 ) || ( sa == true && child.isSampledAncestor() == true ) )
        {
            node_taxa[node.getIndex()] = species;
        }
    }

    return node_taxa[node.getIndex()];
}

template<class charType>
void RevBayesCore::PhyloCTMCSiteHomogeneousConditionalRange<charType>::updateTransitionProbabilities(size_t node_idx)
{
    const TopologyNode& node = this->tau->getValue().getNode(node_idx);

    if (node.isRoot()) throw RbException("dnPhyloCTMC called updateTransitionProbabilities for the root node\n");

    double start_age = node.getParent().getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(start_age) == false )
    {
        // we assume by default that the end is at time 0
        throw(RbException("Morphospeciation model not implemented for branch length trees"));
    }

    double end_age = std::max(node.getAge(), node_taxa[node_idx].getAgeRange().getMax());

    if( end_age >= start_age )
    {
        end_age = start_age;
    }

    double range_len = end_age - node.getAge();

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

                rm->calculateTransitionProbabilities( start_age, end_age,  rate * r, this->transition_prob_matrices[j*this->num_matrices + matrix] );

                for(size_t from = 0; from < rm->getNumberOfStates(); from++)
                {
                    for(size_t to = 0; to < rm->getNumberOfStates(); to++)
                    {
                        this->transition_prob_matrices[j*this->num_matrices + matrix][from][to] *= exp( rm->getRate(to, to, end_age, rate * r) * range_len );
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

            rm->calculateTransitionProbabilities( start_age, end_age,  rate * r, this->transition_prob_matrices[j] );

            for(size_t from = 0; from < rm->getNumberOfStates(); from++)
            {
                for(size_t to = 0; to < rm->getNumberOfStates(); to++)
                {
                    double a = exp( rm->getRate(to, to, end_age, rate * r) * range_len );
                    double b = this->transition_prob_matrices[j][from][to];
                    this->transition_prob_matrices[j][from][to] *= exp( rm->getRate(to, to, end_age, rate * r) * range_len );
                }
            }
        }
    }
}

#endif
