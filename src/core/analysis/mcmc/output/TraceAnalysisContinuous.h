//
//  TraceAnalysis.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef TraceAnalysisContinuous_H
#define TraceAnalysisContinuous_H

#include <vector>

namespace RevBayesCore {

    #define MAX_LAG 1000

    class TraceAnalysisContinuous {
    
    public:
        TraceAnalysisContinuous();
    
        double                  getAct();    
        double                  getEss();    
        double                  getMean();    
        double                  getStdErrorOfMean();    
    
        void                    analyseMean(const std::vector<double>& v);                                                              //!< analyse the mean for the trace
        void                    analyseMean(const std::vector<double>& v,std::size_t burnin);                                           //!< analyse the mean for the trace and given burnin
        void                    analyseMean(const std::vector<double>& v,std::size_t begin,std::size_t end);                            //!< analyse the mean for the trace with begin and end indices of the values
        void                    analyseMean(const std::vector<std::vector<double> >& v, const std::vector<std::size_t>& burnin);        //!< analyse the mean for the trace and given burnin
        void                    analyseCorrelation(const std::vector<double>& v);                                                       //!< analyse the correlation statistics (act,ess,sem,...)
        void                    analyseCorrelation(const std::vector<double>& v,std::size_t burnin);                                    //!< analyse the correlation statistics (act,ess,sem,...) given the burnin
        void                    analyseCorrelation(const std::vector<double>& v,std::size_t begin,std::size_t end);                     //!< analyse the correlation statistics (act,ess,sem,...) with begin and end indices of the values
        void                    invalidateTraceStatistics();                                                                            //!< set the trace statistics (act,ess,sem,...) to invalid values
    
    private:
    
        // variable holding the data
        double                  act;                                            //!< autocorrelation time
        double                  ess;                                            //!< effective sample size
        double                  mean;                                           //!< mean of trace
        double                  sem;                                            //!< standard error of mean
    
    };

}

#endif



