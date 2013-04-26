#include "GeometricBrownianMotion.h"
#include "DistributionLognormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

#include <cmath>

using namespace RevBayesCore;

GeometricBrownianMotion::GeometricBrownianMotion(const TypedDagNode<double> *i, const TypedDagNode<double> *s, const TypedDagNode<double> *t, int n) : RandomWalkProcess( new LinearStepFunction(NULL, std::vector<double>(), 0, 1) ), initialValue( i ), sigma( s ), time( t ), blocks( n ) {
    // add the parameters as a parent
    addParameter( sigma );
    addParameter( time );
    addParameter( initialValue );
    
    delete value;
    value = simulate();
}


GeometricBrownianMotion::GeometricBrownianMotion(const GeometricBrownianMotion &b) : RandomWalkProcess( b ), initialValue( b.initialValue ), sigma( b.sigma ), time( b.time ), blocks( b.blocks ) {
    // parameters are automatically copied
}


GeometricBrownianMotion::~GeometricBrownianMotion( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



GeometricBrownianMotion* GeometricBrownianMotion::clone( void ) const {
    return new GeometricBrownianMotion( *this );
}


double GeometricBrownianMotion::computeLnProbability( void ) {
//    double lnProb = 0;
//    double t = time->getValue();
//    double stdDev = t/blocks * sigma->getValue();
    
    throw RbException("Missing implementation in GeometricBrownianMotion::computeLnProbability.");
    return -1;
    
    // @todo: reimplement
    //    double prevValue = initialValue->getValue();
    //    // check for correct initial value
    //    if ( prevValue != value->evaluate( 0.0 ) ) {
    //        return RbConstants::Double::neginf;
    //    }
    //    for (int i = 1; i <= blocks; ++i) {
    //        double val = value->evaluate( (i/double(blocks)) * t );                          // needs to be this way because of rounding errors when adding interval sizes
    //        lnProb += RbStatistics::Lognormal::lnPdf(log(prevValue), stdDev, val);
    //        prevValue = val;
    //    }
    
//    return lnProb;
}


void GeometricBrownianMotion::getAffectedSpecialization(std::set<DagNode *> &affected, DagNode* affecter) {
//    if ( affecter == initialValue )
//        this->getAffectedNodes( affected );
}


int GeometricBrownianMotion::getNumberOfIntervals( void ) const {
    return blocks;
}


void GeometricBrownianMotion::keepSpecialization( DagNode* affecter ) {
//    if ( affecter == initialValue )
//        this->keepAffected();
}


void GeometricBrownianMotion::redrawValue( void ) {
    delete value;
    value = simulate();
}



LinearStepFunction* GeometricBrownianMotion::simulate( void ) {
    
    std::vector<double> v;
    
    double t = time->getValue();
    double stdDev = t/blocks * sigma->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double prevValue = initialValue->getValue();
    v.push_back( prevValue );
    
    for (int i = 1; i <= blocks; ++i) {
        double val = RbStatistics::Lognormal::rv(log(prevValue), stdDev, *rng);
        v.push_back( prevValue );
        
        // store value
        prevValue = val;
    }
    
    return new LinearStepFunction(new ConstantNode<double>("", new double(0.0)), v, 0.0, t);
}


void GeometricBrownianMotion::swapParameter(const DagNode *oldP, const DagNode *newP) {
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


void GeometricBrownianMotion::restoreSpecialization( DagNode *restorer ) {
    if ( restorer == initialValue ) {
        values->getValue()[0] = initialValue->getValue();
//        restoreAffected();
    }
}


void GeometricBrownianMotion::touchSpecialization( DagNode *toucher ) {
    if ( toucher == initialValue ) {
        values->getValue()[0] = initialValue->getValue();
//        touchAffected();
    }
}


