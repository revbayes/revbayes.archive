#include "PhyloOrnsteinUhlenbeckProcessEVE.h"
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

PhyloOrnsteinUhlenbeckProcessEVE::PhyloOrnsteinUhlenbeckProcessEVE(const TypedDagNode<Tree> *t, size_t ns) :
AbstractPhyloContinuousCharacterProcess( t, ns ),
num_tips( t->getValue().getNumberOfTips() ),
obs( std::vector<std::vector<double> >(this->num_sites, std::vector<double>(num_tips, 0.0) ) ),
means( new std::vector<double>(num_tips, 0.0) ),
//stored_means( new std::vector<std::vector<double> >(this->num_sites, std::vector<double>(num_tips, 0.0) ) ),
phylogenetic_covariance_matrix( new MatrixReal(num_tips, num_tips) ),
//stored_phylogenetic_covariance_matrix( new MatrixReal(num_tips, num_tips) ),
inverse_phylogenetic_covariance_matrix( num_tips, num_tips ),
changed_covariance(false),
needs_covariance_recomputation( true ),
needs_scale_recomputation( true )
{
    // initialize default parameters
    root_state                  = new ConstantNode<double>("", new double(0.0) );
    homogeneous_alpha           = new ConstantNode<double>("", new double(1.0) );
    homogeneous_sigma           = new ConstantNode<double>("", new double(1.0) );
    homogeneous_theta           = new ConstantNode<double>("", new double(0.0) );
    heterogeneous_alpha         = NULL;
    heterogeneous_sigma         = NULL;
    heterogeneous_theta         = NULL;
    
    
    // add parameters
    addParameter( homogeneous_alpha );
    addParameter( homogeneous_sigma );
    addParameter( homogeneous_theta );
    
    
    // now we need to reset the value
    this->redrawValue();
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
PhyloOrnsteinUhlenbeckProcessEVE::~PhyloOrnsteinUhlenbeckProcessEVE( void )
{
    
    //    delete phylogenetic_covariance_matrix;
    //    delete stored_phylogenetic_covariance_matrix;
}



PhyloOrnsteinUhlenbeckProcessEVE* PhyloOrnsteinUhlenbeckProcessEVE::clone( void ) const
{
    
    return new PhyloOrnsteinUhlenbeckProcessEVE( *this );
}


void PhyloOrnsteinUhlenbeckProcessEVE::computeCovariance(MatrixReal &covariance)
{
    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t root_index = root.getIndex();
    
    std::vector<double> variance = std::vector<double>(this->tau->getValue().getNumberOfNodes(), 0.0);
    //set rootVar
    if ( computeBranchAlpha(root_index) > 0) //to stationary var if alpha>0
    {
        variance[root_index] = computeBranchSigma( root_index ) / (2.0*computeBranchAlpha(root_index));
    }
    else //to 0 otherwise
    {
        variance[root_index] = 0.0;
    }
    
    //calculate variance on daughter branches
    for (size_t i=0; i<root.getNumberOfChildren(); ++i)
    {
        const TopologyNode &child = root.getChild( i );
        computeVarianceRecursive(child, variance);
    }
    
    //copy Vars to Cov diag
    for (size_t i=0; i<num_tips; ++i)
    {
        covariance[i][i] = variance[i];
    }
    
    //calc cov between all leaf pairs
    for (size_t left_index=0; left_index<num_tips; ++left_index)
    {
        const TopologyNode *left_tip_node = &this->tau->getValue().getTipNode( left_index );
        
        for (size_t right_index=(left_index+1); right_index<num_tips; ++right_index)
        {
            
            const TopologyNode *right_tip_node = &this->tau->getValue().getTipNode( right_index );
            
            //get mrca
            size_t mrca_index = computeMrcaIndex(left_tip_node, right_tip_node);
            
            //get sum of alpha*branchlength for non-shared branches
            double sum_AT = 0.0;
            
            // first the computation for the left subtree
            size_t current_index = left_index;
            const TopologyNode *current_node = left_tip_node;
            while (current_index != mrca_index)
            {
                sum_AT += computeBranchAlpha(current_index) * current_node->getBranchLength();
                current_node = &current_node->getParent();
                current_index = current_node->getIndex();
            }
            
            // first the computation for the left subtree
            current_index = right_index;
            current_node = right_tip_node;
            while (current_index != mrca_index)
            {
                sum_AT += computeBranchAlpha(current_index) * current_node->getBranchLength();
                current_node = &current_node->getParent();
                current_index = current_node->getIndex();
            }
            
            covariance[left_index][right_index] = variance[mrca_index] * exp(-sum_AT);
        }
        
    }
    
    expandCovariance( covariance );
    
    for (size_t i=0; i<num_tips; ++i)
    {
        for (size_t j=0; j<num_tips; ++j)
        {
            covariance[j][i] = covariance[i][j];
        }
        
    }

}



void PhyloOrnsteinUhlenbeckProcessEVE::computeExpectation(std::vector<double> &expectations)
{
    
    const TopologyNode &root = this->tau->getValue().getRoot();

    // init root E to theta (be it longrun theta (in OU)
    // or theta_root (in non-evol, BM)
    double my_expectation = computeRootState();
    
    //run leaf expecation on daughter branches
    for (size_t i=0; i<root.getNumberOfChildren(); ++i)
    {
        const TopologyNode &child = root.getChild( i );
        computeExpectationRecursive(child, my_expectation, expectations);
    }
    
    // expand the expectation to multiple samples per species
    expandExpectation( expectations);
    
}


size_t PhyloOrnsteinUhlenbeckProcessEVE::computeMrcaIndex(const TopologyNode *left, const TopologyNode *right)
{
    
    //printf("mrca of %d and %d", nodei, nodej);
    if ( left == right )  //same
    {
        return left->getIndex();
    }
    else if ( left->getAge() < right->getAge() )
    {
        return computeMrcaIndex( &left->getParent(), right );
    }
    else
    {
        return computeMrcaIndex( left, &right->getParent() );
    }
    
}



void PhyloOrnsteinUhlenbeckProcessEVE::computeExpectationRecursive(const TopologyNode &node, double parent_expectation, std::vector<double> &expectations)
{
    
    // get parameters for this branch
    size_t node_index       = node.getIndex();
    double alpha            = computeBranchAlpha(node_index);
    double theta            = computeBranchTheta(node_index);
    double bl               = node.getBranchLength();
    
    double eAT = exp(-1.0 * alpha * bl);
    double my_expectation = parent_expectation*eAT + theta*(1.0-eAT);
    
    if ( node.isTip() )
    {
        expectations[node_index] = my_expectation;
    }
    else
    {
        //run leaf expecation on daughter branches
        for (size_t i=0; i<node.getNumberOfChildren(); ++i)
        {
            const TopologyNode &child = node.getChild( i );
            computeExpectationRecursive(child, my_expectation, expectations);
        }
    }
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::computeVarianceRecursive(const TopologyNode &node, std::vector<double> &variance)
{
    // get parameters for this branch
    size_t node_index       = node.getIndex();
    size_t parent_index     = node.getParent().getIndex();
    double alpha            = computeBranchAlpha(node_index);
    double sigma_square     = computeBranchSigma(node_index);
    double bl               = node.getBranchLength();

    double e2AT = exp(-2.0 * alpha * bl);
    
    variance[node_index] = (sigma_square / (2.0*alpha))*(1.0 - e2AT) + variance[parent_index]*e2AT;
    
    for (size_t i=0; i<node.getNumberOfChildren(); ++i)
    {
        const TopologyNode &child = node.getChild( i );
        computeVarianceRecursive(child, variance);
    }

}



double PhyloOrnsteinUhlenbeckProcessEVE::computeLnProbability( void )
{
    
    // compute the ln probability by recursively calling the probability calculation for each node
//    const TopologyNode &root = this->tau->getValue().getRoot();
//    
//    // we start with the root and then traverse down the tree
//    size_t root_index = root.getIndex();
//    
//    std::vector<double> expectations = std::vector<double>(num_tips, 0.0);
//    std::vector<std::vector<double> > covariance = std::vector<std::vector<double> >( num_tips, std::vector<double>(num_tips, 0.0) );

    //get exp and cov
    computeExpectation( *means );
    computeCovariance( *phylogenetic_covariance_matrix );
    
//    //check for zero cov
//    if(matrixSum(gCov, totnindiv, totnindiv) <= 0.0) {
//        if(matrixSum(gCov, totnindiv, totnindiv) < 0.0) {
//            if(verbose > 90) { printf("got less than zero covariance matrix with gparmin %le and regime ", gparmin); printDouArr(regimes[0], 4); }
//            if(vectorEquals(expr[findmaxgenei], gEs, totnindiv) == 1)
//                return(0.0);
//            else
//                return(-1000000.0);//-10000000000000000.0);
//        }
//        if(verbose > 90) { printf("got zero covariance matrix with gparmin %le and regime ", gparmin); printDouArr(regimes[0], 4); }
//        if(vectorEquals(expr[findmaxgenei], gEs, totnindiv) == 1)
//            return(0.0);
//        else
//            return(-1000000.0);//-10000000000000000.0);
//    }
    
//    //convert to gsl data types
//    douArr2gslVec(gEs, ggslE, totnindiv);
//    dou2DArr2gslMat(gCov, ggslCov, totnindiv, totnindiv);
//    douArr2gslVec(expr[findmaxgenei], ggslexpr, totnindiv);
    
    //get logL
//    lL = dmvnorm(totnindiv, ggslexpr, ggslE, ggslCov);
    
    inverse_phylogenetic_covariance_matrix = phylogenetic_covariance_matrix->computeInverse();

    
    // sum the partials up
    this->ln_prob = sumRootLikelihood();
    
    return this->ln_prob;
}


double PhyloOrnsteinUhlenbeckProcessEVE::computeBranchAlpha(size_t branch_idx) const
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


double PhyloOrnsteinUhlenbeckProcessEVE::computeBranchSigma(size_t branch_idx) const
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


double PhyloOrnsteinUhlenbeckProcessEVE::computeBranchTheta(size_t branch_idx) const
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


double PhyloOrnsteinUhlenbeckProcessEVE::computeRootState( void ) const
{
    
    // second, get the clock rate for the branch
    double root_state = this->root_state->getValue();
    
    return root_state;
}


void PhyloOrnsteinUhlenbeckProcessEVE::expandExpectation( std::vector<double> &e ) //(double *lE, double *iE) {
{
//    int j, iEi=0;
//    
//    for (size_t i=0; i<num_tips; ++i)
//    {
//        
//        for(size_t j=0; j<(tree[i].nindiv); j++)
//        {
//            iE[iEi] = lE[i];
//            iEi++;
//        }
//    }
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::expandCovariance( MatrixReal &covariance )
{
    
//    int i, j, ii, ij, iCovi, iCovj;
//    double inpopvar;
//    
//    iCovi=0;
//    for (size_t i=0; i<num_tips; ++i)
//    {
//        size_t iCovj=iCovi;
//        
//        for (j=i; j<nspecies; j++)
//        {
//            for(ii=0; ii<(tree[i].nindiv); ii++)
//            {
//                for(ij=0; ij<(tree[j].nindiv); ij++)
//                {
//                    covariance[(iCovi+ii)][(iCovj+ij)] = lCov[i][j];
//                }
//            }
//            iCovj += tree[j].nindiv;
//        }
//        iCovi += tree[i].nindiv;
//    }
//    
//    //add in variance within pops
//    iCovi=0;
//    for(size_t i=0; i<num_tips; ++i)
//    {
//        double inpopvar = 1.0 * computeBranchSigma(i)/(2.0*computeBranchSigma(i)) * regimes[tree[i].regime][3];
//        for(ii=0; ii<tree[i].nindiv; ii++)
//        {
//            covariance[iCovi][iCovi] += inpopvar;
//            iCovi++;
//        }
//    }

    
}




void PhyloOrnsteinUhlenbeckProcessEVE::keepSpecialization( DagNode* affecter )
{
    
    // reset the flags
    changed_covariance = false;
    needs_covariance_recomputation = false;
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::resetValue( void )
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
    
    obs = std::vector<std::vector<double> >(this->num_sites, std::vector<double>(num_tips, 0.0) );
    
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
    
    // reset the means vectors
    delete means;
//    delete stored_means;
    means = new std::vector<double>(num_tips, 0.0);
//    stored_means    = new std::vector<std::vector<double> >(this->num_sites, std::vector<double>(num_tips, 0.0) );
    
    
    
    // finally we set all the flags for recomputation
    needs_covariance_recomputation = true;
    needs_scale_recomputation = true;
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::recursiveComputeRootToTipDistance(std::vector<double> &distances, double distance_from_root, const RevBayesCore::TopologyNode &node, size_t node_index)
{
    
    if ( node.isRoot() == false )
    {
        // get my scaled branch length
        double v = this->computeBranchTime(node_index, node.getBranchLength() );
        
        if ( node.isTip() )
        {
            distances[node_index] = distance_from_root + v;
        }
        else
        {
            const TopologyNode &left = node.getChild(0);
            size_t left_index = left.getIndex();
            recursiveComputeRootToTipDistance(distances, distance_from_root+v, left, left_index );
            
            const TopologyNode &right = node.getChild(1);
            size_t right_index = right.getIndex();
            recursiveComputeRootToTipDistance(distances, distance_from_root+v, right, right_index );
            
        }
        
    }
    else
    {
        
        for (size_t i = 0; i < node.getNumberOfChildren(); ++i)
        {
            const TopologyNode &child = node.getChild(i);
            size_t childIndex = child.getIndex();
            recursiveComputeRootToTipDistance(distances, 0, child, childIndex );
        }
        
    }
    
}


std::set<size_t> PhyloOrnsteinUhlenbeckProcessEVE::recursiveComputeDistanceMatrix(MatrixReal &m, const TopologyNode &node, size_t node_index)
{
    
    // I need to know all my children
    std::set<size_t> children;
    
    if ( node.isRoot() == false )
    {
        // get my scaled branch length
        double v = this->computeBranchTime(node_index, node.getBranchLength() );
        
        if ( node.isTip() )
        {
            children.insert( node_index );
            m[node_index][node_index] += v;
        }
        else
        {
            const TopologyNode &left = node.getChild(0);
            size_t left_index = left.getIndex();
            children = recursiveComputeDistanceMatrix(m, left, left_index );
            
            const TopologyNode &right = node.getChild(1);
            size_t right_index = right.getIndex();
            std::set<size_t> childrenRight = recursiveComputeDistanceMatrix(m, right, right_index );
            
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



void PhyloOrnsteinUhlenbeckProcessEVE::restoreSpecialization( DagNode* affecter )
{
    
    // reset the flags
    if ( changed_covariance == true )
    {
        changed_covariance = false;
        needs_covariance_recomputation = false;
        
//        MatrixReal *tmp = phylogenetic_covariance_matrix;
//        phylogenetic_covariance_matrix = stored_phylogenetic_covariance_matrix;
//        stored_phylogenetic_covariance_matrix = tmp;
        
    }
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::setAlpha(const TypedDagNode<double> *a)
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


void PhyloOrnsteinUhlenbeckProcessEVE::setAlpha(const TypedDagNode<RbVector<double> > *a)
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


void PhyloOrnsteinUhlenbeckProcessEVE::setRootState(const TypedDagNode<double> *s)
{
    
    // remove the old parameter first
    this->removeParameter( root_state );
    root_state = s;
    
    // add the new parameter
    this->addParameter( root_state );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::setSigma(const TypedDagNode<double> *s)
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


void PhyloOrnsteinUhlenbeckProcessEVE::setSigma(const TypedDagNode<RbVector<double> > *s)
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


void PhyloOrnsteinUhlenbeckProcessEVE::setTheta(const TypedDagNode<double> *t)
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


void PhyloOrnsteinUhlenbeckProcessEVE::setTheta(const TypedDagNode<RbVector<double> > *t)
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


void PhyloOrnsteinUhlenbeckProcessEVE::simulateRecursively( const TopologyNode &node, std::vector< ContinuousTaxonData > &taxa)
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


std::vector<double> PhyloOrnsteinUhlenbeckProcessEVE::simulateRootCharacters(size_t n)
{
    
    std::vector<double> chars = std::vector<double>(num_sites, 0);
    for (size_t i=0; i<num_sites; ++i)
    {
        chars[i] = computeRootState();
    }
    
    return chars;
}


double PhyloOrnsteinUhlenbeckProcessEVE::sumRootLikelihood( void )
{
    
    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    for (size_t site = 0; site < this->num_sites; ++site)
    {
        double sr = this->computeSiteRate(site);
        
        //        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfCovariance(m, *phylogeneticCovarianceMatrix, obs[site], sigma*sigma);
        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfPrecision( (*means), inverse_phylogenetic_covariance_matrix, obs[site], sr*sr);
    }
    
    return sumPartialProbs;
}


/** Swap a parameter of the distribution */
void PhyloOrnsteinUhlenbeckProcessEVE::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == root_state)
    {
        root_state = static_cast<const TypedDagNode< double >* >( newP );
    }
    
    if (oldP == homogeneous_alpha)
    {
        homogeneous_alpha = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneous_alpha)
    {
        heterogeneous_alpha = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    
    if (oldP == homogeneous_sigma)
    {
        homogeneous_sigma = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneous_sigma)
    {
        heterogeneous_sigma = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    
    if (oldP == homogeneous_theta)
    {
        homogeneous_theta = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneous_theta)
    {
        heterogeneous_theta = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    
    this->AbstractPhyloContinuousCharacterProcess::swapParameterInternal(oldP, newP);
    
}


void PhyloOrnsteinUhlenbeckProcessEVE::touchSpecialization( DagNode* affecter, bool touchAll )
{
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == root_state )
    {
        
        
    }
    else
    {
        needs_covariance_recomputation = true;
        if ( changed_covariance == false )
        {
//            MatrixReal *tmp = phylogenetic_covariance_matrix;
//            phylogenetic_covariance_matrix = stored_phylogenetic_covariance_matrix;
//            stored_phylogenetic_covariance_matrix = tmp;
        }
        changed_covariance = true;
        
    }
    //    else if ( affecter != this->tau ) // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    //    {
    //        touchAll = true;
    //    }
    
}


