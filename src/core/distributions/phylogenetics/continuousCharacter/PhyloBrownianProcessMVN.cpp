#include "ConstantNode.h"
#include "PhyloBrownianProcessMVN.h"
#include "DistributionMultivariateNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <cmath>


using namespace RevBayesCore;

PhyloBrownianProcessMVN::PhyloBrownianProcessMVN(const TypedDagNode<Tree> *t, size_t ns) :
    AbstractPhyloBrownianProcess( t, ns ),
    numTips( t->getValue().getNumberOfTips() ),
    obs( std::vector<std::vector<double> >(this->num_sites, std::vector<double>(numTips, 0.0) ) ),
    phylogeneticCovarianceMatrix( new MatrixReal(numTips, numTips) ),
    storedPhylogeneticCovarianceMatrix( new MatrixReal(numTips, numTips) ),
    inversePhylogeneticCovarianceMatrix( numTips, numTips ),
    changedCovariance(false),
    needsCovarianceRecomputation( true ),
    needsScaleRecomputation( true )
{
    homogeneous_root_state      = new ConstantNode<double>("", new double(0.0) );
    heterogeneous_root_state    = NULL;

    addParameter( homogeneous_root_state );
    
    
    // now we need to reset the value
    this->redrawValue();
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
PhyloBrownianProcessMVN::~PhyloBrownianProcessMVN( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
}



PhyloBrownianProcessMVN* PhyloBrownianProcessMVN::clone( void ) const
{
    
    return new PhyloBrownianProcessMVN( *this );
}


double PhyloBrownianProcessMVN::computeLnProbability( void )
{
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    if ( needsCovarianceRecomputation == true )
    {
        // perhaps there is a more efficient way to reset the matrix to 0.
        phylogeneticCovarianceMatrix = new MatrixReal(numTips, numTips);
        recursiveComputeCovarianceMatrix(*phylogeneticCovarianceMatrix, root, rootIndex);
        needsCovarianceRecomputation = false;
        inversePhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix->computeInverse();
    }
    
    // sum the partials up
    this->lnProb = sumRootLikelihood();
    
    return this->lnProb;
}


double PhyloBrownianProcessMVN::computeRootState(size_t siteIdx)
{
    
    // second, get the clock rate for the branch
    double rootState;
    if ( this->heterogeneous_root_state != NULL )
    {
        rootState = this->heterogeneous_root_state->getValue()[siteIdx];
    }
    else
    {
        rootState = this->homogeneous_root_state->getValue();
    }
    
    return rootState;
}




void PhyloBrownianProcessMVN::keepSpecialization( DagNode* affecter )
{
    
    // reset the flags
    changedCovariance = false;
    needsCovarianceRecomputation = false;
    
}


void PhyloBrownianProcessMVN::resetValue( void )
{
    
    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices = std::vector<size_t>(this->num_sites,0);
    size_t siteIndex = 0;
    for (size_t i = 0; i < this->num_sites; ++i)
    {
        while ( this->value->isCharacterExcluded(siteIndex) )
        {
            siteIndex++;
            if ( siteIndex >= this->value->getNumberOfCharacters()  )
            {
                throw RbException( "The character matrix cannot set to this variable because it does not have enough included characters." );
            }
        }
        siteIndices[i] = siteIndex;
        siteIndex++;
    }
    
    obs = std::vector<std::vector<double> >(this->num_sites, std::vector<double>(numTips, 0.0) );
    
    std::vector<TopologyNode*> nodes = this->tau->getValue().getNodes();
    for (size_t site = 0; site < this->num_sites; ++site)
    {
        
        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            if ( (*it)->isTip() )
            {
                ContinuousTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );
                double &c = taxon.getCharacter(siteIndices[site]);
                obs[site][(*it)->getIndex()] = c;
            }
        }
    }
    
    
    // finally we set all the flags for recomputation
    needsCovarianceRecomputation = true;
    needsScaleRecomputation = true;
    
    //    for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
    //    {
    //        (*it) = true;
    //    }
    //
    //    // flip the active likelihood pointers
    //    for (size_t index = 0; index < changed_nodes.size(); ++index)
    //    {
    //        activeLikelihood[index] = 0;
    //        changed_nodes[index] = true;
    //    }
}


std::set<size_t> PhyloBrownianProcessMVN::recursiveComputeCovarianceMatrix(MatrixReal &m, const TopologyNode &node, size_t nodeIndex)
{
    
    // I need to know all my children
    std::set<size_t> children;
    
    if ( node.isRoot() == false )
    {
        // get my scaled branch length
        double v = this->computeBranchTime(nodeIndex, node.getBranchLength() );
        
        if ( node.isTip() )
        {
            children.insert( nodeIndex );
            m[nodeIndex][nodeIndex] += v;
        }
        else
        {
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            children = recursiveComputeCovarianceMatrix(m, left, leftIndex );
            
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            std::set<size_t> childrenRight = recursiveComputeCovarianceMatrix(m, right, rightIndex );
            
            children.insert(childrenRight.begin(), childrenRight.end());
            
            // now we loop over all combination of the children pairs to add their variance terms
            for (std::set<size_t>::iterator i_itr = children.begin(); i_itr != children.end(); ++i_itr)
            {
                for (std::set<size_t>::iterator j_itr = children.begin(); j_itr != children.end(); ++j_itr)
                {
                    m[*i_itr][*j_itr] += v;
                }
            }
            
        }
        
    }
    else
    {
        
        for (size_t i = 0; i < node.getNumberOfChildren(); ++i)
        {
            const TopologyNode &child = node.getChild(i);
            size_t childIndex = child.getIndex();
            std::set<size_t> childrenRight = recursiveComputeCovarianceMatrix(m, child, childIndex );
        }
        
    }
    
    return children;
    
}



void PhyloBrownianProcessMVN::restoreSpecialization( DagNode* affecter )
{
    
    // reset the flags
    if ( changedCovariance == true )
    {
        changedCovariance = false;
        needsCovarianceRecomputation = false;
        
        MatrixReal *tmp = phylogeneticCovarianceMatrix;
        phylogeneticCovarianceMatrix = storedPhylogeneticCovarianceMatrix;
        storedPhylogeneticCovarianceMatrix = tmp;
        
    }
    
}


void PhyloBrownianProcessMVN::setRootState(const TypedDagNode<double> *s)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_root_state );
    this->removeParameter( heterogeneous_root_state );
    homogeneous_root_state      = NULL;
    heterogeneous_root_state    = NULL;
    
    
    // set the value
    homogeneous_root_state = s;
    
    // add the new parameter
    this->addParameter( homogeneous_root_state );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloBrownianProcessMVN::setRootState(const TypedDagNode<RbVector<double> > *s)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_root_state );
    this->removeParameter( heterogeneous_root_state );
    homogeneous_root_state      = NULL;
    heterogeneous_root_state    = NULL;
    
    
    // set the value
    heterogeneous_root_state = s;
    
    // add the new parameter
    this->addParameter( heterogeneous_root_state );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


std::vector<double> PhyloBrownianProcessMVN::simulateRootCharacters(size_t n)
{
    
    std::vector<double> chars = std::vector<double>(num_sites, 0);
    for (size_t i=0; i<num_sites; ++i)
    {
        chars[i] = computeRootState(i);
    }
    
    return chars;
}


double PhyloBrownianProcessMVN::sumRootLikelihood( void )
{
    
    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    for (size_t site = 0; site < this->num_sites; ++site)
    {
        std::vector<double> m = std::vector<double>(numTips, computeRootState(site) );
        
        double sigma = this->computeSiteRate(site);
        //        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfCovariance(m, *phylogeneticCovarianceMatrix, obs[site], sigma*sigma);
        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfPrecision(m, inversePhylogeneticCovarianceMatrix, obs[site], sigma*sigma);
    }
    
    return sumPartialProbs;
}


void PhyloBrownianProcessMVN::touchSpecialization( DagNode* affecter, bool touchAll )
{
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == homogeneous_root_state )
    {
        
        
    }
    else if ( affecter == heterogeneous_root_state )
    {
        
        
    }
    else if ( affecter == this->homogeneous_clock_rate )
    {
        needsCovarianceRecomputation = true;
        if ( changedCovariance == false )
        {
            MatrixReal *tmp = phylogeneticCovarianceMatrix;
            phylogeneticCovarianceMatrix = storedPhylogeneticCovarianceMatrix;
            storedPhylogeneticCovarianceMatrix = tmp;
        }
        changedCovariance = true;
        
    }
    else if ( affecter == this->heterogeneous_clock_rates )
    {
        needsCovarianceRecomputation = true;
        if ( changedCovariance == false )
        {
            storedPhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix;
        }
        changedCovariance = true;
        
    }
    else if ( affecter == this->tau )
    {
        needsCovarianceRecomputation = true;
        if ( changedCovariance == false )
        {
            storedPhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix;
        }
        changedCovariance = true;
        
    }
    else if ( affecter != this->tau ) // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    {
        touchAll = true;
    }
    
}


/** Swap a parameter of the distribution */
void PhyloBrownianProcessMVN::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == homogeneous_root_state)
    {
        homogeneous_root_state = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneous_root_state)
    {
        heterogeneous_root_state = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else
    {
        AbstractPhyloBrownianProcess::swapParameterInternal(oldP, newP);
    }
    
}


