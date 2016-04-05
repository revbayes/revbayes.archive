#include "PhyloOrnsteinUhlenbeckProcessMVN.h"
#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "DistributionMultivariateNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <cmath>


using namespace RevBayesCore;

PhyloOrnsteinUhlenbeckProcessMVN::PhyloOrnsteinUhlenbeckProcessMVN(const TypedDagNode<Tree> *t, size_t ns) :
    AbstractPhyloContinuousCharacterProcess( t, ns ),
    numTips( t->getValue().getNumberOfTips() ),
    obs( std::vector<std::vector<double> >(this->num_sites, std::vector<double>(numTips, 0.0) ) ),
    phylogeneticCovarianceMatrix( new MatrixReal(numTips, numTips) ),
    storedPhylogeneticCovarianceMatrix( new MatrixReal(numTips, numTips) ),
    inversePhylogeneticCovarianceMatrix( numTips, numTips ),
    changedCovariance(false),
    needsCovarianceRecomputation( true ),
    needsScaleRecomputation( true )
{
    // initialize default parameters
    homogeneous_root_state      = new ConstantNode<double>("", new double(0.0) );
    heterogeneous_root_state    = NULL;

    // add parameters
    addParameter( homogeneous_root_state );
    
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
PhyloOrnsteinUhlenbeckProcessMVN::~PhyloOrnsteinUhlenbeckProcessMVN( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
}



PhyloOrnsteinUhlenbeckProcessMVN* PhyloOrnsteinUhlenbeckProcessMVN::clone( void ) const
{
    
    return new PhyloOrnsteinUhlenbeckProcessMVN( *this );
}


double PhyloOrnsteinUhlenbeckProcessMVN::computeLnProbability( void )
{
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    if ( needsCovarianceRecomputation == true )
    {
        // perhaps there is a more efficient way to reset the matrix to 0.
        phylogeneticCovarianceMatrix = new MatrixReal(numTips, numTips);
        recursiveComputeDistanceMatrix(*phylogeneticCovarianceMatrix, root, rootIndex);
        needsCovarianceRecomputation = false;
        inversePhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix->computeInverse();
    }
    
    // sum the partials up
    this->lnProb = sumRootLikelihood();
    
    return this->lnProb;
}


double PhyloOrnsteinUhlenbeckProcessMVN::computeBranchAlpha(size_t branch_idx) const
{
    
    // get the selection rate for the branch
    double a;
    if ( this->heterogeneous_alpha != NULL )
    {
        a = this->heterogeneous_alpha->getValue()[branch_idx];
    }
    else
    {
        a = this->homogeneous_alpha->getValue();
    }
    
    return a;
}


double PhyloOrnsteinUhlenbeckProcessMVN::computeBranchSigma(size_t branch_idx) const
{
    
    // get the selection rate for the branch
    double s;
    if ( this->heterogeneous_sigma != NULL )
    {
        s = this->heterogeneous_sigma->getValue()[branch_idx];
    }
    else
    {
        s = this->homogeneous_sigma->getValue();
    }
    
    return s;
}


double PhyloOrnsteinUhlenbeckProcessMVN::computeBranchTheta(size_t branch_idx) const
{
    
    // get the selection rate for the branch
    double t;
    if ( this->heterogeneous_theta != NULL )
    {
        t = this->heterogeneous_theta->getValue()[branch_idx];
    }
    else
    {
        t = this->homogeneous_theta->getValue();
    }
    
    return t;
}


double PhyloOrnsteinUhlenbeckProcessMVN::computeRootState(size_t siteIdx) const
{
    
    // second, get the clock rate for the branch
    double root_state;
    if ( this->heterogeneous_root_state != NULL )
    {
        root_state = this->heterogeneous_root_state->getValue()[siteIdx];
    }
    else
    {
        root_state = this->homogeneous_root_state->getValue();
    }
    
    return root_state;
}




void PhyloOrnsteinUhlenbeckProcessMVN::keepSpecialization( DagNode* affecter )
{
    
    // reset the flags
    changedCovariance = false;
    needsCovarianceRecomputation = false;
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::resetValue( void )
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
    
}


std::set<size_t> PhyloOrnsteinUhlenbeckProcessMVN::recursiveComputeDistanceMatrix(MatrixReal &m, const TopologyNode &node, size_t nodeIndex)
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
            children = recursiveComputeDistanceMatrix(m, left, leftIndex );
            
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            std::set<size_t> childrenRight = recursiveComputeDistanceMatrix(m, right, rightIndex );
            
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
            std::set<size_t> childrenRight = recursiveComputeDistanceMatrix(m, child, childIndex );
        }
        
    }
    
    return children;
    
}



void PhyloOrnsteinUhlenbeckProcessMVN::restoreSpecialization( DagNode* affecter )
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


void PhyloOrnsteinUhlenbeckProcessMVN::setAlpha(const TypedDagNode<double> *a)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_alpha );
    this->removeParameter( heterogeneous_alpha );
    homogeneous_alpha      = NULL;
    heterogeneous_alpha    = NULL;
    
    
    // set the value
    homogeneous_alpha = a;
    
    // add the new parameter
    this->addParameter( homogeneous_alpha );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::setAlpha(const TypedDagNode<RbVector<double> > *a)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_alpha );
    this->removeParameter( heterogeneous_alpha );
    homogeneous_alpha      = NULL;
    heterogeneous_alpha    = NULL;
    
    
    // set the value
    heterogeneous_alpha = a;
    
    // add the new parameter
    this->addParameter( heterogeneous_alpha );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::setRootState(const TypedDagNode<double> *s)
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


void PhyloOrnsteinUhlenbeckProcessMVN::setRootState(const TypedDagNode<RbVector<double> > *s)
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


void PhyloOrnsteinUhlenbeckProcessMVN::setSigma(const TypedDagNode<double> *s)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_sigma );
    this->removeParameter( heterogeneous_sigma );
    homogeneous_sigma      = NULL;
    heterogeneous_sigma    = NULL;
    
    
    // set the value
    homogeneous_sigma = s;
    
    // add the new parameter
    this->addParameter( homogeneous_sigma );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::setSigma(const TypedDagNode<RbVector<double> > *s)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_sigma );
    this->removeParameter( heterogeneous_sigma );
    homogeneous_sigma      = NULL;
    heterogeneous_sigma    = NULL;
    
    
    // set the value
    heterogeneous_sigma = s;
    
    // add the new parameter
    this->addParameter( heterogeneous_sigma );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::setTheta(const TypedDagNode<double> *t)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_theta );
    this->removeParameter( heterogeneous_theta );
    homogeneous_theta      = NULL;
    heterogeneous_theta    = NULL;
    
    
    // set the value
    homogeneous_theta = t;
    
    // add the new parameter
    this->addParameter( homogeneous_theta );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::setTheta(const TypedDagNode<RbVector<double> > *t)
{
    
    // remove the old parameter first
    this->removeParameter( homogeneous_theta );
    this->removeParameter( heterogeneous_theta );
    homogeneous_theta      = NULL;
    heterogeneous_theta    = NULL;
    
    
    // set the value
    heterogeneous_theta = t;
    
    // add the new parameter
    this->addParameter( heterogeneous_theta );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessMVN::simulateRecursively( const TopologyNode &node, std::vector< ContinuousTaxonData > &taxa)
{
    
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();
    
    // get the sequence of this node
    size_t node_index = node.getIndex();
    const ContinuousTaxonData &parent = taxa[ node_index ];
    
    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);
        
        // get the branch length for this child
        double branch_length = child.getBranchLength();
        
        // get the branch specific rate
        double branch_time = sqrt( computeBranchTime( child.getIndex(), branch_length ) );
        
        // get the branch specific rate
        double branch_sigma = computeBranchSigma( child.getIndex() );
        
        // get the branch specific optimum (theta)
        double branch_theta = computeBranchTheta( child.getIndex() );
        
        // get the branch specific optimum (theta)
        double branch_alpha = computeBranchAlpha( child.getIndex() );
        
        ContinuousTaxonData &taxon = taxa[ child.getIndex() ];
        for ( size_t i = 0; i < num_sites; ++i )
        {
            // get the ancestral character for this site
            double parent_state = parent.getCharacter( i );
            
            // compute the standard deviation for this site
            double branch_rate = branch_time * computeSiteRate( i );

            double e = exp(-branch_alpha * branch_rate);
            double e2 = exp(-2 * branch_alpha * branch_rate);
            double m = e * parent_state + (1 - e) * branch_theta;
            double standDev = branch_sigma * sqrt((1 - e2) / 2 / branch_alpha);
            
            // create the character
            double c = RbStatistics::Normal::rv( m, standDev, *rng);
            
            // add the character to the sequence
            taxon.addCharacter( c );
        }
        
        if ( child.isTip() )
        {
            taxon.setTaxon( child.getTaxon() );
        }
        else
        {
            // recursively simulate the sequences
            simulateRecursively( child, taxa );
        }
        
    }
    
}


std::vector<double> PhyloOrnsteinUhlenbeckProcessMVN::simulateRootCharacters(size_t n)
{
    
    std::vector<double> chars = std::vector<double>(num_sites, 0);
    for (size_t i=0; i<num_sites; ++i)
    {
        chars[i] = computeRootState(i);
    }
    
    return chars;
}


double PhyloOrnsteinUhlenbeckProcessMVN::sumRootLikelihood( void )
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


void PhyloOrnsteinUhlenbeckProcessMVN::touchSpecialization( DagNode* affecter, bool touchAll )
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
void PhyloOrnsteinUhlenbeckProcessMVN::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
        AbstractPhyloContinuousCharacterProcess:swapParameterInternal(oldP, newP);
    }
}


