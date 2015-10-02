#include "TmrcaStatistic.h"
#include "RbConstants.h"
#include "RbException.h"

using namespace RevBayesCore;

TmrcaStatistic::TmrcaStatistic(const TypedDagNode<Tree> *t, const Clade &c, const bool s) : TypedFunction<double>( new double(0.0) ),
    tree( t ),
    clade( c ),
    stemAge( s ),
    index( RbConstants::Size_t::nan )
{

    // add the tree parameter as a parent
    addParameter( tree );
    
    initialize();
    update();
}


TmrcaStatistic::~TmrcaStatistic( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}



TmrcaStatistic* TmrcaStatistic::clone( void ) const
{
    
    return new TmrcaStatistic( *this );
}


void TmrcaStatistic::initialize( void )
{
    
    taxaCount = clade.size();
    index = RbConstants::Size_t::nan;
    
}


void TmrcaStatistic::update( void )
{
    
    const std::vector<TopologyNode*> &n = tree->getValue().getNodes();
    size_t minCaldeSize = n.size() + 2;
    
    bool found = false;
    if ( index != RbConstants::Size_t::nan )
    {
        
        TopologyNode *node = n[index];
        std::vector<Taxon> taxa;
        node->getTaxa( taxa );
        size_t cladeSize = taxa.size();
        if ( node->containsClade( clade, false ) && taxaCount == cladeSize )
        {
            found = true;
        }
        else
        {
            minCaldeSize = cladeSize;
        }
        
    }
    
    
    if ( found == false )
    {
        
        for (size_t i = tree->getValue().getNumberOfTips(); i < n.size(); ++i)
        {
            
            TopologyNode *node = n[i];
            std::vector<Taxon> taxa;
            node->getTaxa( taxa );
            size_t cladeSize = taxa.size();
            if ( cladeSize < minCaldeSize && cladeSize >= taxaCount && node->containsClade( clade, false ) )
            {
                
                index = node->getIndex();
                minCaldeSize = cladeSize;
                if ( taxaCount == cladeSize )
                {
                    break;
                }
                
            }
            
        }
        
    }
    
    if ( index == RbConstants::Size_t::nan )
    {
        throw RbException("TMRCA-Statistics can only be applied if clade is present.");
    }
    
    if ( stemAge )
    {
        size_t parentIndex = tree->getValue().getNode(index).getParent().getIndex();
        double tmrca = tree->getValue().getNode( parentIndex ).getAge();
        *value = tmrca;
    }
    else
    {
        double tmrca = tree->getValue().getNode( index ).getAge();
        *value = tmrca;
    }
    
}



void TmrcaStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tree) 
    {
        tree = static_cast<const TypedDagNode<Tree>* >( newP );
        index = RbConstants::Size_t::nan;
    }
    
}


