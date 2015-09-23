/**
 * @file
 * This file contains the declaration and implementation of the PhyloDiversity function
 * which calculates the sum of all branches on the tree needed to connect the sampled taxa.
 *
 * @brief Declaration and implementation of PhyloDiversity
 *
 * (c) Copyright 2009- under GPL version 3
 * @author Will Freyman
 * @license GPL version 3
 * @version 1.0
 *
 */

#ifndef PhyloDiversityFunction_H
#define PhyloDiversityFunction_H


#include "Clade.h"
#include "RbException.h"
#include "TypedFunction.h"


namespace RevBayesCore {
    
    template <class treeType>
    class PhyloDiversityFunction : public TypedFunction<double> {
        
    public:
        PhyloDiversityFunction(const TypedDagNode<treeType> *t, const Clade &c, const bool i, const TypedDagNode< RbVector< double > > *w);
        virtual                                            ~PhyloDiversityFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        PhyloDiversityFunction*                             clone(void) const;                                                                  //!< Create an independent clone
        void                                                update(void);
        
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        double                                              calculateBranchWeights( size_t j );
        double                                              sumPDforNode(size_t j, std::vector<size_t>* nodesVisited, size_t stopIndex);
        
        // members
        std::vector<double>                                 branchWeights;
        bool                                                includeRoot;
        size_t                                              numTaxa;
        const Clade                                         sample;
        const TypedDagNode<treeType>*                       tau;
        const TypedDagNode< RbVector< double > >*           tipWeights;
        bool                                                weightedPD;
    };
    
}


#include "Clade.h"
#include "TypedFunction.h"


using namespace RevBayesCore;


template <class treeType>
PhyloDiversityFunction<treeType>::PhyloDiversityFunction(const TypedDagNode<treeType> *t, const Clade &c, const bool i, const TypedDagNode< RbVector< double > > *w) : TypedFunction<double>( new double(0.0) ),
includeRoot( i ),
sample( c ),
tau( t ),
tipWeights( w )
{
    addParameter( tau );
    this->numTaxa = sample.size();
}


template <class treeType>
PhyloDiversityFunction<treeType>::~PhyloDiversityFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


template <class treeType>
PhyloDiversityFunction<treeType>* PhyloDiversityFunction<treeType>::clone( void ) const
{
    return new PhyloDiversityFunction<treeType>( *this );
}


template <class treeType>
void PhyloDiversityFunction<treeType>::update( void )
{
    if (tipWeights == NULL || tipWeights->getNumberOfElements() == 0)
    {
        weightedPD = false;
    }
    else
    {
        if (tipWeights->getNumberOfElements() == sample.size())
        {
            weightedPD = true;
            branchWeights = std::vector<double>(tau->getValue().getNodes().size(), 0.0);
            calculateBranchWeights( tau->getValue().getRoot().getIndex() );
        }
        else
        {
            throw RbException("The sample size must equal the number of weights.");
        }
    }
    
    size_t stopIndex = tau->getValue().getRoot().getIndex();
    
    if (!includeRoot)
    {
        stopIndex = RbConstants::Size_t::nan;
        // get the tmrca of all the members of the sample
        const std::vector<TopologyNode*> &nodes = tau->getValue().getNodes();
        size_t minCladeSize = nodes.size() + 2;
        for (size_t i = tau->getValue().getNumberOfTips(); i < nodes.size(); ++i)
        {
            
            TopologyNode *node = nodes[i];
            std::vector<std::string> taxa;
            node->getTaxaStringVector( taxa );
            size_t cladeSize = taxa.size();
            if ( cladeSize < minCladeSize && cladeSize >= numTaxa && node->containsClade( sample, false ) )
            {
                stopIndex = node->getIndex();
                minCladeSize = cladeSize;
                if ( numTaxa == cladeSize )
                {
                    break;
                }
            }
        }
        if ( stopIndex == RbConstants::Size_t::nan )
        {
            *value = 0.0;
            return;
        }
    }

    double pd = 0.0;
    std::vector<size_t> nodesVisited;
    std::vector<std::string> taxa = sample.getTaxonNames();
    
    // loop through each of the sampled taxa
    for (size_t i = 0; i < numTaxa; ++i)
    {
        std::string taxon = taxa[i];
        size_t j = tau->getValue().getTipIndex( taxon );
        if (j != ((size_t)-1))
        {
            pd += sumPDforNode(j, &nodesVisited, stopIndex);
        }
    }
    *value = pd;
}


/**
 * Function that recursively traverses the tree from the given node up to the stopIndex summing PD.
 **/
template <class treeType>
double  PhyloDiversityFunction<treeType>::sumPDforNode(size_t j, std::vector<size_t>* nodesVisited, size_t stopIndex)
{
    double pd = 0.0;
    // check to see if we have visited this node
    if ( std::find(nodesVisited->begin(), nodesVisited->end(), j) == nodesVisited->end() )
    {
        const TopologyNode& n = tau->getValue().getNode( j );
        if (weightedPD)
        {
            pd += n.getBranchLength() * branchWeights[j];
        }
        else
        {
            pd += n.getBranchLength();
        }
        nodesVisited->push_back( j );
        if (includeRoot)
        {
            if (j != tau->getValue().getRoot().getIndex())
            {
                size_t parentIndex = n.getParent().getIndex();
                pd += sumPDforNode(parentIndex, nodesVisited, stopIndex);
            }
        }
        else
        {
            if (n.getParent().getIndex() != stopIndex)
            {
                size_t parentIndex = n.getParent().getIndex();
                pd += sumPDforNode(parentIndex, nodesVisited, stopIndex);
            }
        }
    }
    return pd;
}


/**
 * Function that recursively traverses the tree in preorder calculating branch weights.
 **/
template <class treeType>
double  PhyloDiversityFunction<treeType>::calculateBranchWeights(size_t j)
{
    double weight = 0.0;
    const TopologyNode& n = tau->getValue().getNode( j );
    if (n.isTip())
    {
        std::vector<std::string> taxa = sample.getTaxonNames();
        for (size_t i = 0; i < numTaxa; ++i)
        {
            if (n.getName() == taxa[i])
            {   
                branchWeights[j] = tipWeights->getValue()[i];
                weight = branchWeights[j];
                break;
            }
        }
    }
    else
    {
        std::vector<int> children = n.getChildrenIndices();
        double weightLeft = calculateBranchWeights( children[0] );
        double weightRight = calculateBranchWeights( children[1] );
        branchWeights[j] = weightLeft + weightRight / 2;
        weight = branchWeights[j];
    }
    return weight;
}


template <class treeType>
void PhyloDiversityFunction<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
    }
}


#endif
