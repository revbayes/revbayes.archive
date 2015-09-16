//
//  Trace.h
//  RevBayesGui
//
//  This class implements a single trace for a single parameter. It contains a mutable array of values
//  and additional meta information about the trace.
//
//  Created by Sebastian Hoehna on 3/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef Trace_H
#define Trace_H

#include <string>
#include <vector>

namespace RevBayesCore {

#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2

    class Trace {

    public:
    
        Trace(void);
        
        virtual                     ~Trace(void) {}
        
        bool                        operator==(const Trace &t) const                { return this == &t; }
        bool                        operator!=(const Trace &t) const                { return !this->operator==( t ); }
        bool                        operator<(const Trace &t) const                 { return this < &t; }
        
        // overloaded functions from RbObject
        Trace*                      clone(void) const;                                              //!< Clone object
    
        void                        addObject(double d);
        void                        computeStatistics(void);
        double                      objectAt(size_t index)                          { return values.at(index); }
        void                        removeLastObject();
        void                        removeObjectAtIndex(int index);
        size_t                      size() { return values.size(); }
    
    
        // getters and setters
        size_t                      getBurnin()                                     { return burnin; }
        double                      getEss()                                        { return ess; }
        std::string                 getFileName()                                   { return fileName; }
        double                      getMean()                                       { return mean; }
        double                      getMedian()                                     { return median; }
        const std::string&          getParameterName() const                        { return parmName; }
        size_t                      getSamples()                                    { return values.size(); }
        double                      getSem()                                        { return sem; }
        size_t                      getStepSize()                                   { return stepSize; }
        std::vector<double>         getValues()                                     { return values; }
        const std::vector<double>&  getValues() const                               { return values; }
        int                         hasConverged()                                  { return converged; }
        int                         hasPassedEssThreshold()                         { return passedEssThreshold; }
        int                         hasPassedGelmanRubinTest()                      { return passedGelmanRubinTest; }
        int                         hasPassedGewekeTest()                           { return passedGewekeTest; }
        int                         hasPassedIidBetweenChainsStatistic()            { return passedIidBetweenChainsStatistic; }
        int                         hasPassedSemThreshold()                         { return passedSemThreshold; }
        int                         hasPassedStationarityTest()                     { return passedStationarityTest; }
    
        void                        setBurnin(size_t b)                             { burnin = b; }
        void                        setEss(double e)                                { ess = e; }
        void                        setFileName(std::string fn)                     { fileName = fn; }
        void                        setMean(double m)                               { mean = m; }
        void                        setMedian(double m)                             { median = m; }
        void                        setParameterName(std::string pm)                { parmName = pm; }
        void                        setSem(double s)                                { sem = s; }
        void                        setStepSize( size_t s)                          { stepSize = s; }
        void                        setValues(std::vector<double> v)                { values = v; }
        void                        setConverged(bool c)                            { converged = c; }
        void                        setPassedEssThreshold(int p)                    { passedEssThreshold = p; }
        void                        setPassedGelmanRubinTest(int p)                 { passedGelmanRubinTest = p; }
        void                        setPassedGewekeTest(int p)                      { passedGewekeTest = p; }
        void                        setPassedIidBetweenChainsStatistic(int p)       { passedIidBetweenChainsStatistic = p; }
        void                        setPassedSemThreshold(int p)                    { passedSemThreshold = p; }
        void                        setPassedStationarityTest(int p)                { passedStationarityTest = p; }
    
    protected:
    
        void                        invalidate();
    

    private:
    
        std::vector<double>         values;                                     //!< the values of this trace
    
        std::string                 parmName;
        std::string                 fileName;

        size_t                      burnin;
        double                      ess;                                        //!< the effective sample saize for this trace
        double                      mean;                                       //!< the mean value for this trace
        double                      median;                                     //!< the median of the trace
        double                      sem;                                        //!< the standard error of the mean
        size_t                      stepSize;                                   //!< the step size between samples
    
        int                         converged;                                  //!< Whether this parameter in itself has converged.
        int                         passedStationarityTest;                     //!< Whether this parameter passed the stationarity test.
        int                         passedGewekeTest;                           //!< Whether this parameter passed the Geweke statistic.
//        int                     passedHeidelbergerWelcheStatistic;          //!< Whether this parameter passed the Heidelberger-Welch statistic.
        int                         passedEssThreshold;                         //!< Whether this parameter passed the threshold for the ESS.
        int                         passedSemThreshold;                         //!< Whether this parameter passed the threshold for the SEM.
        int                         passedIidBetweenChainsStatistic;            //!< Whether this parameter passed the iid test of chains.
        int                         passedGelmanRubinTest;                      //!< Whether this parameter passed the Gelman-Rubin statistic.
    
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Trace& x);                                //!< Overloaded output operator

}

#endif
