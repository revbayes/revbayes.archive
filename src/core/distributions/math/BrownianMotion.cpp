#include "BrownianMotion.h"
#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

BrownianMotion::BrownianMotion(const TypedDagNode<double> *m, const TypedDagNode<double> *s, const TypedDagNode<double> *t, int k) : RandomWalkProcess( new LinearStepFunction( NULL, std::vector<double>(), 0, 1 ) ), initialValue( m ), sigma( s ), time( t ), n( k ) {
    // add the parameters as a parent
    addParameter( sigma );
    addParameter( time );
    addParameter( initialValue );
    
    delete value;
    value = simulate();
}


BrownianMotion::BrownianMotion(const BrownianMotion &b) : RandomWalkProcess( b ), initialValue( b.initialValue ), sigma( b.sigma ), time( b.time ), n( b.n ) {
    // parameters are automatically copied
}


BrownianMotion::~BrownianMotion( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BrownianMotion* BrownianMotion::clone( void ) const {
    return new BrownianMotion( *this );
}


double BrownianMotion::computeLnProbability( void ) {
    double lnProb = 0;
    double intervalSize = time->getValue() / n;
    double stdDev = intervalSize * sigma->getValue();
    
    double prevValue = initialValue->getValue();
    // check that we have the correct starting value
    if ( value->evaluate( 0.0 ) != prevValue ) {
        return RbConstants::Double::neginf;
    }
        
    double x = intervalSize;
    for (int i = 1; i <= n; ++i, x += intervalSize) {
        double val = value->evaluate( x );
        lnProb += RbStatistics::Normal::lnPdf(prevValue, stdDev, val);
        prevValue = val;
    }
    
    return lnProb;
}



int BrownianMotion::getNumberOfIntervals( void ) const {
    return n;
}


void BrownianMotion::redrawValue( void ) {
    delete value;
    value = simulate();
}


LinearStepFunction* BrownianMotion::simulate( void ) {
    
    std::vector<double> values;
    
    double t = time->getValue();
    double stdDev = t / n * sigma->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double prevValue = initialValue->getValue();
    values.push_back( prevValue );
    
    for (int i = 1; i <= n; ++i) {
        double val = RbStatistics::Normal::rv(prevValue, stdDev, *rng);
        values.push_back( prevValue );
        
        // store value
        prevValue = val;
    }
    
    return new LinearStepFunction(new ConstantNode<double>("", new double(0.0) ), values, 0.0, t);
}


void BrownianMotion::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == sigma) {
        sigma = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == time) {
        time = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == initialValue) {
        initialValue = static_cast<const TypedDagNode<double>* >( newP );
    }
}


void BrownianMotion::touchSpecialization( DagNode *toucher ) {
    if ( toucher == initialValue ) {
        static_cast< LinearStepFunction *>( value )->getPathValues()[0] = initialValue->getValue();
    }
}


