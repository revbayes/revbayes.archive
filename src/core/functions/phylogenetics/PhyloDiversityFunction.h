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
#include "TypedFunction.h"


namespace RevBayesCore {
    
    template <class treeType>
    class PhyloDiversityFunction : public TypedFunction<double> {
        
    public:
        PhyloDiversityFunction(const TypedDagNode<treeType> *t, const Clade &c, const bool i);
        virtual                                            ~PhyloDiversityFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        PhyloDiversityFunction*                             clone(void) const;                                                                  //!< Create an independent clone
        void                                                update(void);
        
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        double                                              sumPDforNode(size_t j, std::vector<size_t>* nodesVisited, size_t stopIndex);
        
        // members
        const TypedDagNode<treeType>*                       tau;
        const Clade                                         sample;
        size_t                                              numTaxa;
        bool                                                includeRoot;
    };
    
}


#include "Clade.h"
#include "TypedFunction.h"


using namespace RevBayesCore;


template <class treeType>
PhyloDiversityFunction<treeType>::PhyloDiversityFunction(const TypedDagNode<treeType> *t, const Clade &c, const bool i) : TypedFunction<double>( new double(0.0) ),
tau( t ),
sample( c ),
includeRoot( i )
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
        pd += n.getBranchLength();
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


template <class treeType>
void PhyloDiversityFunction<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<treeType>* >( newP );
    }
}


#endif
