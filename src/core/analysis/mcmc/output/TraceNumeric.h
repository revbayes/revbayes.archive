//
//  TraceNumeric.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef TraceNumeric_H
#define TraceNumeric_H

#include "Trace.h"

#include <vector>

namespace RevBayesCore {

    class TraceNumeric : public Trace<double> {
    
    public:
        TraceNumeric();

        virtual TraceNumeric*   clone(void) const;                              //!< Clone object

        void                    computeCorrelation() const;                                            //!< compute the correlation statistics (act,ess,sem,...)
        void                    computeMean() const;                                                   //!< compute the mean for the trace

        void                    computeCorrelation(std::size_t begin,std::size_t end) const;           //!< compute the correlation statistics (act,ess,sem,...) with begin and end indices of the values
        void                    computeMean(std::size_t begin,std::size_t end) const;                  //!< compute the mean for the trace with begin and end indices of the values

        void                    computeStatistics(void);

        double                  getAct() const;
        double                  getEss() const;
        double                  getMean() const;
        double                  getSem() const;

        //int                     hasConverged() const                            { return converged; }
        int                     hasPassedGewekeTest() const                     { return passedGewekeTest; }
        int                     hasPassedStationarityTest() const               { return passedStationarityTest; }
        //int                     hasPassedEssThreshold() const                   { return passedEssThreshold; }
        //int                     hasPassedSemThreshold() const                   { return passedSemThreshold; }
        //int                     hasPassedIidBetweenChainsStatistic() const      { return passedIidBetweenChainsStatistic; }
        //int                     hasPassedGelmanRubinTest() const                { return passedGelmanRubinTest; }

    protected:

        // variable holding the data
        mutable double          act;                                        //!< autocorrelation time
        mutable double          ess;                                        //!< effective sample size
        mutable double          mean;                                       //!< mean of trace
        mutable double          sem;                                        //!< standard error of mean

        //int                     converged;                                  //!< Whether this parameter in itself has converged.
        int                     passedStationarityTest;                     //!< Whether this parameter passed the stationarity test.
        int                     passedGewekeTest;                           //!< Whether this parameter passed the Geweke statistic.
        //int                     passedEssThreshold;                         //!< Whether this parameter passed the threshold for the ESS.
        //int                     passedSemThreshold;                         //!< Whether this parameter passed the threshold for the SEM.
        //int                     passedIidBetweenChainsStatistic;            //!< Whether this parameter passed the iid test of chains.
        //int                     passedGelmanRubinTest;                      //!< Whether this parameter passed the Gelman-Rubin statistic.

        mutable bool            stats_dirty;
    
    };

}

#endif



