#ifndef PhyloDistanceGamma_H
#define PhyloDistanceGamma_H

#include "MatrixReal.h"
#include "TopologyNode.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class treeType>
    class PhyloDistanceGamma : public TypedDistribution< MatrixReal >, public TreeChangeEventListener {
        
    public:
		PhyloDistanceGamma( const TypedDagNode< treeType > *t );//, std::vector<std::string> names ); //, MatrixReal distMatrix, MatrixReal varMatrix );
		PhyloDistanceGamma( const PhyloDistanceGamma &p);
		
        virtual                                            ~PhyloDistanceGamma(void);                                                                   //!< Virtual destructor
        
        // public member functions
        PhyloDistanceGamma*                           clone(void) const;                                                                          //!< Create an independent clone
		double                                                              computeLnProbability(void);
		void                                                                fireTreeChangeEvent(const TopologyNode &n);                                                 //!< The tree has changed and we want to know which part.
		void                                                                redrawValue(void);
		void                                                                reInitialized(void);
		void                                                                setDistanceMatrix(const TypedDagNode< MatrixReal > *dm);
		void                                                                setVarianceMatrix(const TypedDagNode< MatrixReal > *dm);
		void                                                                setNames(const std::vector< std::string >& n);
		//void 																setValue(const MatrixReal *dm, const MatrixReal *vm); //!< Set the current value, e.g. attach an observation (clamp)
		void 																simulate( );


		
    protected:
		
		// Parameter management functions.
		virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
		
		
		// virtual methods that may be overwritten, but then the derived class should call this methods
		virtual void                                                        keepSpecialization(DagNode* affecter);
		virtual void                                                        touchSpecialization(DagNode *toucher, bool touchAll);
		
		void 																updateAlphaAndBetaMatrices();
		
		// members
		const TypedDagNode<treeType>*                                       tau;
		size_t 																numTips;
		const TypedDagNode<MatrixReal>*                                     distanceMatrix;
		const TypedDagNode<MatrixReal>*                                     varianceMatrix;
		MatrixReal                                                          alphaMatrix;
		MatrixReal                                                          betaMatrix;
		std::vector<std::string> 											matrixNames;
		double 																lnProb;

		// the likelihoods
		std::vector<double>                                                 partialLikelihoods;
		

    private:
		double 																computeLogLikelihood( void );

		
    
    };
    
}


#include "DistributionGamma.h"
#include "RandomNumberFactory.h"
#include "TreePairwiseDistances.h"

#include <cmath>
#include <cstring>

template<class treeType>
RevBayesCore::PhyloDistanceGamma<treeType>::PhyloDistanceGamma( const TypedDagNode< treeType > *t ) : TypedDistribution<MatrixReal>( new MatrixReal() ), tau( t ), numTips(t->getValue().getNumberOfTips() ) /*, distanceMatrix(distMatrix), varianceMatrix(varMatrix), matrixNames (names)*/
{
	lnProb = 0.0 ;
    //fill alpha and beta matrices
	updateAlphaAndBetaMatrices( );
}


template<class treeType>
RevBayesCore::PhyloDistanceGamma<treeType>::PhyloDistanceGamma( const PhyloDistanceGamma &p): TypedDistribution<MatrixReal>( new MatrixReal( p.getValue()) ), tau(p.tau), numTips(p.numTips), distanceMatrix (p.distanceMatrix), varianceMatrix(p.varianceMatrix), alphaMatrix(p.alphaMatrix), betaMatrix(p.betaMatrix), matrixNames(p.matrixNames)

{
	lnProb = p.lnProb;
	
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class treeType>
RevBayesCore::PhyloDistanceGamma<treeType>::~PhyloDistanceGamma( void )
{
	// We don't delete the params, because they might be used somewhere else too. The model needs to do that!
	
	// remove myself from the tree listeners
	if ( tau != NULL )
	{
		tau->getValue().getTreeChangeEventHandler().removeListener( this );
		if ( tau->decrementReferenceCount() == 0 )
		{
			delete tau;
		}
		
	}
	
}



template<class treeType>
RevBayesCore::PhyloDistanceGamma<treeType>* RevBayesCore::PhyloDistanceGamma< treeType>::clone( void ) const {
    
    return new PhyloDistanceGamma<treeType>( *this );
}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma< treeType>::keepSpecialization(DagNode* affecter) {
	
	
}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma< treeType>::touchSpecialization(DagNode *toucher, bool touchAll) {
	
	
}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma< treeType>::reInitialized(void) {
	
	
}




template<class treeType>
double RevBayesCore::PhyloDistanceGamma<treeType>::computeLogLikelihood( void )
{
	//First, compute the distance matrix from the current tree
	MatrixReal* mat  = RevBayesCore::TreeUtilities::getDistanceMatrix<treeType> ( tau->getValue() );
	// Now we need to know the order in which the distances have been put in the matrix.
	// We know they are output in the same order the tipnames are given.
	std::vector< std::string > namesFromTree = tau->getValue().getTipNames();
	//We build a map linking names to their id in the tree-based matrix.
	std::map<std::string, size_t > namesToId;
	for (size_t i = 0; i < namesFromTree.size() ; ++i) {
		namesToId [ namesFromTree[i] ] = i;
	}
	//Second, for each pairwise distance, compute its logprobability according to a Gamma distribution with parameters alpha and beta.
	double alpha, beta;
	double logL = 0.0;
	std::string nameI;
	std::string nameJ;
	for (size_t i = 0; i < numTips - 1; ++i) {
		nameI =matrixNames[i];
		for (size_t j = i+1; j < numTips; ++j) {
			nameJ =matrixNames[j];
			alpha = alphaMatrix[i][j];
			beta = betaMatrix[i][j];
			logL += RbStatistics::Gamma::lnPdf( alpha, beta, (*mat)[namesToId[nameI]][namesToId[nameJ]] );
		}
	}
	
	return logL;
	
}


template<class treeType>
double RevBayesCore::PhyloDistanceGamma<treeType>::computeLnProbability( void )
{

	// sum the partials up
	this->lnProb = computeLogLikelihood();
	
	return this->lnProb;

}



template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n )
{
	
	// call a recursive flagging of all node above (closer to the root) and including this node
	// For the moment we do nothing...
}



template< class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::redrawValue( void )
{
	
	// delete the old value first
	delete this->value;
	
	// create a new MatrixReal object
	this->value = distanceMatrix->getValue().clone();
//TreePairwiseDistances<treeType> ( tau );
	
	
}

/*
template<class treeType>
void RevBayesCore::PhyloDistanceGamma< treeType>::setValue(const MatrixReal *dm, const MatrixReal *vm)
{
	
	setDistanceMatrix(dm);
	setVarianceMatrix(vm);

}
*/

template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::simulate( )
{
	this->redrawValue();
	
}

template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::setDistanceMatrix(const TypedDagNode< MatrixReal > *dm) {
	
	// remove the old parameter first
	if ( distanceMatrix != NULL )
	{
		this->removeParameter( distanceMatrix );
		distanceMatrix = NULL;
	}
	
	// set the value
	distanceMatrix = dm;
	
	// add the new parameter
	this->addParameter( distanceMatrix );
	
	updateAlphaAndBetaMatrices( );
	
	// redraw the current value
	if ( this->dagNode == NULL || !this->dagNode->isClamped() )
	{
		this->redrawValue();
	}
	
}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::setVarianceMatrix(const TypedDagNode< MatrixReal > *vm) {
	
	// remove the old parameter first
	if ( varianceMatrix != NULL )
	{
		this->removeParameter( varianceMatrix );
		varianceMatrix = NULL;
	}
	
	// set the value
	varianceMatrix = vm;
	
	// add the new parameter
	this->addParameter( varianceMatrix );
	
	updateAlphaAndBetaMatrices( );
	
	// redraw the current value
	if ( this->dagNode == NULL || !this->dagNode->isClamped() )
	{
		this->redrawValue();
	}
	
}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::setNames(const std::vector< std::string >& n) {
	
	matrixNames = n;
	
}


/** Swap a parameter of the distribution */
template< class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
	
if (oldP == tau)
	{
		tau->getValue().getTreeChangeEventHandler().removeListener( this );
		tau->decrementReferenceCount();
		tau = static_cast<const TypedDagNode<treeType>* >( newP );
		tau->getValue().getTreeChangeEventHandler().addListener( this );
		tau->incrementReferenceCount();
	}
	
}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::updateAlphaAndBetaMatrices( )
{
	//fill alpha and beta matrices
	// We assume the branch length is distributed according to a Gamma distribution.
	// We know the mean and the variance, we can compute the two parameters alpha and beta of the Gamma distribution.
	// mean = alpha/beta
	// variance = alpha /beta^2
	// Therefore:
	// alpha = mean^2 / variance
	// beta = mean / variance
	alphaMatrix = *( distanceMatrix->getValue().clone() );
	betaMatrix = *( varianceMatrix->getValue().clone() );
	double d, v;
	for (size_t i = 0; i < numTips - 1; ++i) {
		for (size_t j = i+1; j < numTips; ++j) {
			d = distanceMatrix->getValue()[i][j];
			v = varianceMatrix->getValue()[i][j];
			alphaMatrix[i][j] = alphaMatrix[j][i] =  pow(d, 2.0) / v;
			betaMatrix[i][j] = betaMatrix[j][i] = d / v ;
		}
	}
 
}

#endif
