////
////  GslIntegrator.cpp
////  RevBayesCore
////
////  Created by Sebastian Hoehna on 8/20/12.
////  Copyright 2012 __MyCompanyName__. All rights reserved.
////
//
//#include "GslIntegrator.h"
//
//#include <gsl/gsl_integration.h>
//
//using namespace RevBayesCore;
//
//double GslIntegrator::integrate(const RevBayesCore::Function &func, double a, double b) const {
//    gsl_integration_cquad_workspace * w = gsl_integration_cquad_workspace_alloc (1000);
//    
//    double result, error;
//    
//    gsl_function F;
//    F.function = &fxt;
//    F.params = const_cast< Function *>( &func );
//    size_t n;
//    
//    gsl_integration_cquad (&F, a, b, 1E-6, 1e-6, w, &result, &error, &n); 
//    gsl_integration_cquad_workspace_free( w );
//    
//    return result;
//}
//
//
//double RevBayesCore::fxt(double x, void *params) {
//    return static_cast<Function *>( params )->evaluate(x);
//}
