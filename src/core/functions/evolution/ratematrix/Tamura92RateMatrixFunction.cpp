#include "Tamura92RateMatrixFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

Tamura92RateMatrixFunction::Tamura92RateMatrixFunction(const TypedDagNode< double > *eqGc, const TypedDagNode< double > *tstv) : TypedFunction<RateMatrix>( new RateMatrix_GTR( 4 ) ), equilibriumGc( eqGc ), transitionTransversionRate( tstv ) {
    // add the lambda parameter as a parent
    addParameter( equilibriumGc );
    addParameter( transitionTransversionRate );
    
    update();
}


Tamura92RateMatrixFunction::Tamura92RateMatrixFunction(const Tamura92RateMatrixFunction &n) : TypedFunction<RateMatrix>( n ), equilibriumGc( n.equilibriumGc ), transitionTransversionRate( n.transitionTransversionRate ) {
    // no need to add parameters, happens automatically
}


Tamura92RateMatrixFunction::~Tamura92RateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



Tamura92RateMatrixFunction* Tamura92RateMatrixFunction::clone( void ) const {
    return new Tamura92RateMatrixFunction( *this );
}


void Tamura92RateMatrixFunction::update( void ) {
    // get the information from the arguments for reading the file
    double gc = equilibriumGc->getValue();
    double tstv = transitionTransversionRate->getValue();

        
    // check the sizes of the simplices, to make certain that they are consistent
    // with a model with nStates states    
    
    RateMatrix_GTR& m = static_cast<RateMatrix_GTR&>( *value );
    
    // set the off-diagonal portions of the rate matrix
    double gcoverTwo = gc/2;
    double OneminusgcoverTwo = (1-gc)/2;
    double kappatimesOneminusgcoverTwo = tstv * OneminusgcoverTwo;

    //std::cout <<"update:" << gcoverTwo << "\t" << OneminusgcoverTwo << "\t" << tstv << "\t" << kappatimesOneminusgcoverTwo << "\n";
    
//    m[0][1] = OneminusgcoverTwo ;
//    m[0][2] = kappatimesOneminusgcoverTwo ;
//    m[0][3] = OneminusgcoverTwo ;
//    m[1][0] = gcoverTwo;
//    m[1][2] = gcoverTwo;
//    m[1][3] = tstv * gcoverTwo;
//    m[2][0] = tstv * gcoverTwo;
//    m[2][1] = gcoverTwo;
//    m[2][3] = gcoverTwo;
//    m[3][0] = OneminusgcoverTwo;
//    m[3][1] = kappatimesOneminusgcoverTwo;
//    m[3][2] = OneminusgcoverTwo;
//    for (int i = 0; i < 4; i++)
//        for (int j = 0; j < 4; j++)
//            m[i][j] = 0.0;
    
    
    m[0][1] = gcoverTwo ;
    m[0][2] = tstv * gcoverTwo ;
    m[0][3] = OneminusgcoverTwo ;
    m[1][0] = OneminusgcoverTwo;
    m[1][2] = gcoverTwo;
    m[1][3] = kappatimesOneminusgcoverTwo;
    m[2][0] = kappatimesOneminusgcoverTwo;
    m[2][1] = gcoverTwo;
    m[2][3] = OneminusgcoverTwo;
    m[3][0] = OneminusgcoverTwo;
    m[3][1] = tstv * gcoverTwo;
    m[3][2] = gcoverTwo;
    
         
    
    // set the diagonal elements of the rate matrix
    m.setDiagonal();
    
    // Set the stationary frequencies for the rate matrix. Note that we
    // can do this in two ways. First, we can call calculateStationaryFrequencies
    // on the RateMatrix object. This function call calculates the stationary
    // frequencies using only knowledge of the rate matrix. Second, we can set
    // the stationary frequencies directly. This is what we do here, because the
    // stationary frequencies have been build directly into the rate matrix.
 //   m.setStationaryFrequencies( f );
    
    // rescale the rate matrix such that the average rate is 1.0
   // m.rescaleToAverageRate(1.0);
    
    // Now that we have set the rate matrix, we should update its eigen system
//    m.updateEigenSystem();
    
    //std::cout <<"update:" << m << std::endl;

}



void Tamura92RateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == equilibriumGc) {
        equilibriumGc = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == transitionTransversionRate) {
        transitionTransversionRate = static_cast<const TypedDagNode< double >* >( newP );
    }
}


