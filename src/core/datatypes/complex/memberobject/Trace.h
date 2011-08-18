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

#include "MemberObject.h"
#include "XmlElementAttributed.h"

#include <string>
#include <vector>

#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2

const std::string Trace_name = "Trace";

class Trace : public MemberObject {
    public:
    
    Trace(void);
//    Trace(int capacity);
    Trace(const Trace& t);                                                                        //!< copy constructor
    Trace(const XmlDocument& doc, const XmlElementAttributed& element);
    ~Trace();
    
    // overloaded functions from RbObject
    MemberObject*           clone(void) const;                                              //!< Clone object
    const VectorString&     getClass(void) const;                                           //!< Get class vector
    void                    printValue(std::ostream& o) const;                              //!< Print value for user
    std::string             richInfo(void) const;                                           //!< Complete info about object
    const XmlElement*       encode(XmlDocument* doc, const std::string& name);
    
    void                    addObject(double d);
    double                  objectAt(int index)                             { return values.at(index); }
    void                    removeLastObject();
    void                    removeObjectAtIndex(int index);
    int                     size() { return (int)values.size(); }
    
    
    // getters and setters
    int                     getBurnin()                                     { return burnin; }
    double                  getEss()                                        { return ess; }
    std::string             getFileName()                                   { return fileName; }
    double                  getMean()                                       { return mean; }
    double                  getMedian()                                     { return median; }
    std::string             getParameterName()                              { return parmName; }
    int                     getSamples()                                    { return (int)values.size(); }
    double                  getSem()                                        { return sem; }
    int                     getStepSize()                                   { return stepSize; }
    std::vector<double>     getValues()                                     { return values; }
    int                     hasConverged()                                  { return converged; }
    int                     hasPassedEssThreshold()                         { return passedEssThreshold; }
    int                     hasPassedGelmanRubinTest()                      { return passedGelmanRubinTest; }
    int                     hasPassedGewekeTest()                           { return passedGewekeTest; }
    int                     hasPassedIidBetweenChainsStatistic()            { return passedIidBetweenChainsStatistic; }
    int                     hasPassedSemThreshold()                         { return passedSemThreshold; }
    int                     hasPassedStationarityTest()                     { return passedStationarityTest; }
    
    void                    setBurnin(int b)                                { burnin = b; }
    void                    setEss(double e)                                { ess = e; }
    void                    setFileName(std::string fn)                     { fileName = fn; }
    void                    setMean(double m)                               { mean = m; }
    void                    setMedian(double m)                             { median = m; }
    void                    setParameterName(std::string pm)                { parmName = pm; }
    void                    setSem(double s)                                { sem = s; }
    void                    setStepSize( int s)                             { stepSize = s; }
    void                    setValues(std::vector<double> v)                { values = v; }
    void                    setConverged(bool c)                            { converged = c; }
    void                    setPassedEssThreshold(int p)                    { passedEssThreshold = p; }
    void                    setPassedGelmanRubinTest(int p)                 { passedGelmanRubinTest = p; }
    void                    setPassedGewekeTest(int p)                      { passedGewekeTest = p; }
    void                    setPassedIidBetweenChainsStatistic(int p)       { passedIidBetweenChainsStatistic = p; }
    void                    setPassedSemThreshold(int p)                    { passedSemThreshold = p; }
    void                    setPassedStationarityTest(int p)                { passedStationarityTest = p; }
    
    protected:
    
    void                    invalidate();
    

    private:
    
    std::vector<double>     values;                                     //!< the values of this trace
    
    std::string             parmName;
    std::string             fileName;

    int                     burnin;
    double                  ess;                                        //!< the effective sample saize for this trace
    double                  mean;                                       //!< the mean value for this trace
    double                  median;                                     //!< the median of the trace
    double                  sem;                                        //!< the standard error of the mean
    int                     stepSize;                                   //!< the step size between samples
    
    int                     converged;                                  //!< Whether this parameter in itself has converged.
    int                     passedStationarityTest;                     //!< Whether this parameter passed the stationarity test.
    int                     passedGewekeTest;                           //!< Whether this parameter passed the Geweke statistic.
//    int                     passedHeidelbergerWelcheStatistic;          //!< Whether this parameter passed the Heidelberger-Welch statistic.
    int                     passedEssThreshold;                         //!< Whether this parameter passed the threshold for the ESS.
    int                     passedSemThreshold;                         //!< Whether this parameter passed the threshold for the SEM.
    int                     passedIidBetweenChainsStatistic;            //!< Whether this parameter passed the iid test of chains.
    int                     passedGelmanRubinTest;                      //!< Whether this parameter passed the Gelman-Rubin statistic.
    
};

#endif
