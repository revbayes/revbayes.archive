#include "ConstantNode.h"
#include "DistributionUniform.h"
#include "RandomNumberFactory.h"
#include "Skyline.h"

#include <cmath>

using namespace RevBayesCore;

Skyline::Skyline(const TypedDagNode<double> *l, const TypedDagNode<double> *u, const TypedDagNode<double> *t, int b) : TypedDistribution<UnivariateFunction>( new ConstantStepFunction(NULL, std::vector<double>(), 0, 1) ), lower( l ), upper( u ), time( t ), blocks( b ) {
    // add the parameters as a parent
    addParameter( lower );
    addParameter( upper );
    addParameter( time );
    
    delete value;
    value = simulate();
}


Skyline::Skyline(const Skyline &b) : TypedDistribution<UnivariateFunction>( b ), lower( b.lower ), upper( b.upper ), time( b.time ), blocks( b.blocks ) {
    // parameters are automatically copied
}


Skyline::~Skyline( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



Skyline* Skyline::clone( void ) const {
    return new Skyline( *this );
}


double Skyline::computeLnProbability( void ) {
    double lnProb = 0;
    
    // parameter values
    double l = lower->getValue();
    double u = upper->getValue();
    
    const ConstantStepFunction &f = static_cast<const ConstantStepFunction &>( *value );
    const std::vector<double>& vals = f.getPathValues();
    for (int i = 0; i <= blocks; ++i) {
        lnProb += RbStatistics::Uniform::lnPdf(l, u, vals[i]);
    }
    
    return lnProb;
}


int Skyline::getNumberOfIntervals( void ) const {
    return blocks;
}


void Skyline::redrawValue( void ) {
    delete value;
    value = simulate();
}



ConstantStepFunction* Skyline::simulate( void ) {
    
    // parameter values
    double l = lower->getValue();
    double u = upper->getValue();
    
    std::vector<double> values;
    
    double t = time->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    
    for (int i = 0; i < blocks; ++i) {
        double val = RbStatistics::Uniform::rv(l, u, *rng);
        values.push_back( val );
    }
    
    return new ConstantStepFunction(new ConstantNode<double>("", new double(0.0)), values, 0.0, t);
    
}


void Skyline::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == lower) {
        lower = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == upper) {
        upper = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == time) {
        time = static_cast<const TypedDagNode<double>* >( newP );
    }
}


