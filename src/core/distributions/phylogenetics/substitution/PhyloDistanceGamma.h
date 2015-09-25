#ifndef PhyloDistanceGamma_H
#define PhyloDistanceGamma_H

#include "DistanceMatrix.h"
#include "MatrixReal.h"
#include "TopologyNode.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class treeType>
    class PhyloDistanceGamma : public TypedDistribution< DistanceMatrix >, public TreeChangeEventListener {
        
    public:
		PhyloDistanceGamma( const TypedDagNode< treeType > *t );//, std::vector<std::string> names ); //, MatrixReal distMatrix, MatrixReal varMatrix );
		PhyloDistanceGamma( const PhyloDistanceGamma &p);
		
        virtual                                                             ~PhyloDistanceGamma(void);                            //!< Virtual destructor
        
        // public member functions
        PhyloDistanceGamma*                          						clone(void) const;                                    //!< Create an independent clone
		double                                                              computeLnProbability(void);
		void                                                                fireTreeChangeEvent(const TopologyNode &n);                                                 //!< The tree has changed and we want to know which part.
		void                                                                redrawValue(void);
		void                                                                reInitialized(void);
		void                                                                setDistanceMatrix(const TypedDagNode< DistanceMatrix > *dm);
		void                                                                setVarianceMatrix(const TypedDagNode< DistanceMatrix > *dm);
		void                                                                setNames(const std::vector< std::string >& n);
		//void 																setValue(const MatrixReal *dm, bool f=false); //!< Set the current value, e.g. attach an observation (clamp)
		void 																simulate( );
		void 																updateAlphaAndBetaMatrices();


		
    protected:
		
		// Parameter management functions.
		virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
		
		
		// virtual methods that may be overwritten, but then the derived class should call this methods
		virtual void                                                        keepSpecialization(DagNode* affecter);
		virtual void                                                        touchSpecialization(DagNode *toucher, bool touchAll);
		
		
		// members
		const TypedDagNode<treeType>*                                       tau;
		size_t 																numTips;
		const TypedDagNode<DistanceMatrix>*                                 distanceMatrix;
		const TypedDagNode<DistanceMatrix>*                                 varianceMatrix;
		DistanceMatrix                                                      alphaMatrix;
		DistanceMatrix                                                      betaMatrix;
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
RevBayesCore::PhyloDistanceGamma<treeType>::PhyloDistanceGamma( const TypedDagNode< treeType > *t ) : TypedDistribution<DistanceMatrix>( new DistanceMatrix() ),
																tau( t ),
																numTips(t->getValue().getNumberOfTips() ),
																distanceMatrix( new ConstantNode<DistanceMatrix>("distanceMatrix", new DistanceMatrix() ) ),
																varianceMatrix( new ConstantNode<DistanceMatrix>("varianceMatrix", new DistanceMatrix() ) )  /*, distanceMatrix(distMatrix), varianceMatrix(varMatrix), matrixNames (names)*/
{
	// add the parameters to our set (in the base class)
	// in that way other class can easily access the set of our parameters
	// this will also ensure that the parameters are not getting deleted before we do
	this->addParameter( tau );
	this->addParameter( distanceMatrix );
	this->addParameter( varianceMatrix );

	this->value = NULL;

	std::cout << "CORE " <<std::endl;
	lnProb = 0.0 ;
	std::cout << "CORE 2" <<std::endl;
	
	
	
	// We don'e want tau to die before we die, or it can't remove us as listener
	this->tau->getValue().getTreeChangeEventHandler().addListener( this );
	this->tau->incrementReferenceCount();
	
    //fill alpha and beta matrices
	//updateAlphaAndBetaMatrices( );
	std::cout << "CORE 3" <<std::endl;

}


template<class treeType>
RevBayesCore::PhyloDistanceGamma<treeType>::PhyloDistanceGamma( const PhyloDistanceGamma &p): TypedDistribution<DistanceMatrix>( new DistanceMatrix( p.getValue()) ), tau(p.tau), numTips(p.numTips), distanceMatrix (p.distanceMatrix), varianceMatrix(p.varianceMatrix), alphaMatrix(p.alphaMatrix), betaMatrix(p.betaMatrix), matrixNames(p.matrixNames)

{
	lnProb = p.lnProb;
	// We don't want tau to die before we die, or it can't remove us as listener
	this->tau->getValue().getTreeChangeEventHandler().addListener( this );
	this->tau->incrementReferenceCount();

	
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
	DistanceMatrix* mat  = RevBayesCore::TreeUtilities::getDistanceMatrix<treeType> ( tau->getValue() );
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
			std::cout <<"alpha: "<< alpha << " beta: " << beta << "; (*mat)[namesToId[nameI]][namesToId[nameJ]]: " << (*mat)[namesToId[nameI]][namesToId[nameJ]] << " ; lnPdf: " << RbStatistics::Gamma::lnPdf( alpha, beta, (*mat)[namesToId[nameI]][namesToId[nameJ]]) << std::endl;
			logL += RbStatistics::Gamma::lnPdf( alpha, beta, (*mat)[namesToId[nameI]][namesToId[nameJ]] );
		}
	}
	
	return logL;
	
}


template<class treeType>
double RevBayesCore::PhyloDistanceGamma<treeType>::computeLnProbability( void )
{

	std::cout << "computeLnProbability"<<std::endl;
	// sum the partials up
	this->lnProb = computeLogLikelihood();
	
	return this->lnProb;

}



template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n )
{
	/*
	// we need to flag this node and all ancestral nodes for recomputation
	size_t index = n.getIndex();
	// if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
	if ( dirtyNodes[index] == false )
	{
		// the root doesn't have an ancestor
		if ( !n.isRoot() )
		{
			recursivelyFlagNodeDirty( n.getParent() );
		}
		
		// set the flag
		dirtyNodes[index] = true;
		
		// if we previously haven't touched this node, then we need to change the active likelihood pointer
		if ( changedNodes[index] == false )
		{
			activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
			changedNodes[index] = true;
		}
		
	}*/
	this->value = RevBayesCore::TreeUtilities::getDistanceMatrix<treeType> ( tau->getValue() );
}



template< class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::redrawValue( void )
{
	std::cout << "redrawValue " <<std::endl;

	// delete the old value first
	if (this->value != NULL) delete this->value;
	
	// create a new MatrixReal object, not exactly the way it is supposed to be done but...
	if ( distanceMatrix != NULL && varianceMatrix != NULL && distanceMatrix->getValue().size() > 0 && varianceMatrix->getValue().size() > 0  )
	{
		this->value = distanceMatrix->getValue().clone();
		//TreePairwiseDistances<treeType> ( tau );
		
		std::cout << "redrawValue 2 "<< this->value <<std::endl;
		
		double alpha, beta;
		std::string nameI;
		std::string nameJ;
		for (size_t i = 0; i < numTips - 1; ++i) {
			nameI =matrixNames[i];
			std::cout << "redrawValue 3" <<std::endl;

			for (size_t j = i+1; j < numTips; ++j) {
				std::cout << "redrawValue 3bis"<< i << "; "<<j <<std::endl;

				nameJ = matrixNames[j];
				std::cout << "redrawValue 4" <<std::endl;

				alpha = alphaMatrix[i][j];
				beta = betaMatrix[i][j];
				std::cout << "redrawValue 5" <<std::endl;

				this->value->getElement(i,j) = RbStatistics::Gamma::rv( alpha, beta, *GLOBAL_RNG );
				std::cout << "redrawValue 6" <<std::endl;

				//(*(this->value))[i][j] = RbStatistics::Gamma::rv( alpha, beta, *GLOBAL_RNG );
			}
		}
	}
	
}

/*
template<class treeType>
void RevBayesCore::PhyloDistanceGamma< treeType>::setValue( const MatrixReal *dm, bool f )
{
	
	setDistanceMatrix(dm);
//	setVarianceMatrix(vm);

}*/


template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::simulate( )
{
	this->redrawValue();
	
}

template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::setDistanceMatrix(const TypedDagNode< DistanceMatrix > *dm) {
	
	std::cout << "setDistanceMatrix " <<std::endl;

	// remove the old parameter first
	if ( distanceMatrix != NULL )
	{
		this->removeParameter( distanceMatrix );
		distanceMatrix = NULL;
	}
	std::cout << "setDistanceMatrix 2" <<std::endl;
	
	// set the value
	distanceMatrix = dm;
	std::cout << "setDistanceMatrix 3" <<std::endl;

	// add the new parameter
	this->addParameter( distanceMatrix );
	
	alphaMatrix = *( distanceMatrix->getValue().clone() );

	//updateAlphaAndBetaMatrices( );
	std::cout << "setDistanceMatrix 4" <<std::endl;

	this->value = distanceMatrix->getValue().clone();

	// redraw the current value
/*	if ( this->dagNode == NULL || !this->dagNode->isClamped() )
	{
		this->redrawValue();
	}*/
	
	this->addParameter( distanceMatrix );
	
	std::cout << "setDistanceMatrix 5" <<std::endl;

}


template<class treeType>
void RevBayesCore::PhyloDistanceGamma<treeType>::setVarianceMatrix(const TypedDagNode< DistanceMatrix > *vm) {
	
	std::cout << "setVarianceMatrix " <<std::endl;
	// remove the old parameter first
	if ( varianceMatrix != NULL )
	{
		std::cout << "setVarianceMatrix 1" <<std::endl;
	
		this->removeParameter( varianceMatrix );
		std::cout << "setVarianceMatrix 12" <<std::endl;

		varianceMatrix = NULL;
		std::cout << "setVarianceMatrix 13" <<std::endl;

	}
	std::cout << "setVarianceMatrix 2" <<std::endl;
	
	// set the value
	varianceMatrix = vm;
	
	std::cout << "setVarianceMatrix 3" <<std::endl;

	// add the new parameter
	this->addParameter( varianceMatrix );
	std::cout << "setVarianceMatrix 4" <<std::endl;

	//updateAlphaAndBetaMatrices( );
	betaMatrix = *( varianceMatrix->getValue().clone() );

	// redraw the current value
	if ( this->dagNode == NULL || !this->dagNode->isClamped() )
	{
		this->redrawValue();
	}
	
	this->addParameter( varianceMatrix );

	std::cout << "setVarianceMatrix 5" <<std::endl;

	
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
	
	if (oldP == distanceMatrix)
	{
		distanceMatrix = static_cast<const TypedDagNode< DistanceMatrix >* >( newP );
		updateAlphaAndBetaMatrices( );
	}
	
	if (oldP == varianceMatrix)
	{
		varianceMatrix = static_cast<const TypedDagNode< DistanceMatrix >* >( newP );
		updateAlphaAndBetaMatrices( );
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
	std::cout << "CORE 4" <<std::endl;

	alphaMatrix = *( distanceMatrix->getValue().clone() );
	std::cout << "CORE 5" <<std::endl;

	betaMatrix = *( varianceMatrix->getValue().clone() );
	std::cout << "CORE 6" <<std::endl;

	double d, v;
	for (size_t i = 0; i < numTips - 1; ++i) {
		std::cout << "CORE 7" <<std::endl;

		for (size_t j = i+1; j < numTips; ++j) {
			std::cout << "CORE 8" <<std::endl;

			d = distanceMatrix->getValue()[i][j];
			v = varianceMatrix->getValue()[i][j];
			std::cout << "CORE 9" <<std::endl;

			alphaMatrix[i][j] = alphaMatrix[j][i] =  pow(d, 2.0) / v;
			betaMatrix[i][j] = betaMatrix[j][i] = d / v ;
			std::cout << "CORE 10" <<std::endl;

		}
	}
 
}

#endif
