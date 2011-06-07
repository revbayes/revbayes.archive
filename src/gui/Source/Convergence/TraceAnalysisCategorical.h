//
//  TraceAnalysisCategorical.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef TraceAnalysisCategorical_H
#define TraceAnalysisCategorical_H

#include <vector>

class TraceAnalysisCategorical {
    
public:
    TraceAnalysisCategorical();
    
    double                  getAct();    
    double                  getEss();    
    double                  getMean();    
    double                  getStdErrorOfMean();    
    
    void                    analyseMean(const std::vector<bool>& v);                            //!< analyse the mean for the trace
    void                    analyseMean(const std::vector<bool>& v, int burnin);                //!< analyse the mean for the trace and given burnin
    void                    analyseMean(const std::vector<bool>& v, int begin, int end);        //!< analyse the mean for the trace with begin and end indices of the values
    void                    analyseMean(const std::vector<std::vector<bool> >& v, const std::vector<int>& burnin);                //!< analyse the mean for the trace and given burnin
    void                    analyseCorrelation(const std::vector<bool>& v);                     //!< analyse the correlation statistics (act,ess,sem,...)
    void                    analyseCorrelation(const std::vector<bool>& v, int burnin);         //!< analyse the correlation statistics (act,ess,sem,...) given the burnin
    void                    analyseCorrelation(const std::vector<bool>& v, int begin, int end); //!< analyse the correlation statistics (act,ess,sem,...) with begin and end indices of the values
//    void                    invalidateTraceStatistics();                    //!< set the trace statistics (act,ess,sem,...) to invalid values
    
private:
    
    // variable holding the data
    double                  act;                                            //!< autocorrelation time
    int                     burnin;                                         //!< number of samples before trace has reached stationarity
    double                  ess;                                            //!< effective sample size
    double                  mean;                                           //!< mean of trace
    double                  sem;                                            //!< standard error of mean
    
};

#endif