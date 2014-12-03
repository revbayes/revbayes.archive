/**
 * @file
 * This file contains the declaration of AncestralStateTrace which
 * holds ancestral state data.
 *
 * @brief Declaration of AncestralStateTrace
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */

#ifndef AncestralStateTrace_H
#define AncestralStateTrace_H



#include <string>
#include <vector>

#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2

namespace RevBayesCore {
	

    class AncestralStateTrace {
		
    public:
		
        AncestralStateTrace(void);
        AncestralStateTrace(const AncestralStateTrace& t);                                                                          //!< copy constructor
        virtual                    ~AncestralStateTrace();
				
        // overloaded functions from RbObject
        AncestralStateTrace*                  clone(void) const;                                                          //!< Clone object
        void                        printValue(std::ostream& o) const;                                          //!< Print value for user
		
        void                        addObject(const std::string& d);
        std::string&                   objectAt(size_t index)                         { return values.at(index); }
        void                        removeLastObject();
        void                        removeObjectAtIndex(int index);
        size_t                      size() { return values.size(); }
		
		
        // getters and setters
        int                         getBurnin()                                     { return burnin; }
        double                      getEss()                                        { return ess; }
        std::string                 getFileName()                                   { return fileName; }
        std::string                 getParameterName()                              { return parmName; }
        int                         getSamples()                                    { return (int)values.size(); }
        int                         getStepSize()                                   { return stepSize; }
        std::vector<std::string>&      getValues()                                     { return values; }
        int                         hasConverged()                                  { return converged; }
        int                         hasPassedEssThreshold()                         { return passedEssThreshold; }
        int                         hasPassedGelmanRubinTest()                      { return passedGelmanRubinTest; }
        int                         hasPassedGewekeTest()                           { return passedGewekeTest; }
        int                         hasPassedIidBetweenChainsStatistic()            { return passedIidBetweenChainsStatistic; }
        int                         hasPassedSemThreshold()                         { return passedSemThreshold; }
        int                         hasPassedStationarityTest()                     { return passedStationarityTest; }
		
        void                        setBurnin(int b)                                { burnin = b; }
        void                        setEss(double e)                                { ess = e; }
        void                        setFileName(std::string fn)                     { fileName = fn; }
        void                        setParameterName(std::string pm)                { parmName = pm; }
        void                        setStepSize( int s)                             { stepSize = s; }
        void                        setValues(const std::vector<std::string> &v)    { values = v; }
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
		
        std::vector<std::string>   values;                                     //!< the values of this AncestralStateTrace
		
        std::string             parmName;
        std::string             fileName;
		
        int                     burnin;
        double                  ess;                                        //!< the effective sample saize for this AncestralStateTrace
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
		
}





#endif
