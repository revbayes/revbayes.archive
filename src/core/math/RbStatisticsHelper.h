/**
 * @file
 * This file contains commonly used statistics functions that are used
 * in RevBayes. The probability density (pdf), log of the probability
 * density (lnPdf), cumulative probability (cdf), and quantiles of
 * common probability distributions.
 *
 * @brief Namespace containing statistical functions
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-11 16:58:43 -0800 (Fre, 11 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RbStatistics.h 203 2009-12-23 14:33:11Z Hoehna $
 */
#ifndef RbStatisticsHelper_H
#define RbStatisticsHelper_H

#include <vector>
#include "RbPtr.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"


namespace RbStatistics {
    	
#pragma mark Helper Functions
    
	namespace Helper {
        
		double                      dppConcParamFromNumTables(double tables, double num);
		double                      dppExpectNumTableFromConcParam(double conp, double num);
        int                         poissonInver(double lambda, RandomNumberGenerator& rng);
        int                         poissonLow(double lambda, RandomNumberGenerator& rng);
        int                         poissonRatioUniforms(double lambda, RandomNumberGenerator& rng);
        double                      rndGamma(double s, RandomNumberGenerator& rng);
        double                      rndGamma1(double s, RandomNumberGenerator& rng);
        double                      rndGamma2(double s, RandomNumberGenerator& rng);
		
        template <class T> void		randomlySelectFromVectorWithReplacement(std::vector<T>& sourceV, std::vector<T>& destV, int k, RbPtr<RandomNumberGenerator> rng) {
            
            destV.clear();
            for (int i=0; i<k; i++)
                destV.push_back( sourceV[(int)(rng->uniform01()*(sourceV.size()))] );
        }
        template <class T> void		randomlySelectFromVectorWithoutReplacement(std::vector<T>& sourceV, std::vector<T>& destV, int k, RbPtr<RandomNumberGenerator> rng) {
            
            if ( (int)sourceV.size() < k )
                throw (RbException("Attempting to sample too many elements from source vector"));
            destV.clear();
            std::vector<T> tmpV = sourceV;
            size_t n = tmpV.size();
            for (int i=0; i<k; i++)
                {
                int whichElement = (int)(rng->uniform01()*(n-i));
                destV.push_back( tmpV[whichElement] );
                tmpV[whichElement] = tmpV[n-i-1];
                }
        }
        template <class T> void		permuteVector(std::vector<T>& v, RandomNumberGenerator* rng) {
            
            RbStatistics::Helper::randomlySelectFromVectorWithoutReplacement<T>(v, v, v.size(), rng);
        }
        
	}
}

//#pragma warning (default: 4068)

#endif