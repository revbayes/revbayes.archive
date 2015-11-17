#ifndef TraceTree_H
#define TraceTree_H

#include "Tree.h"
#include "Trace.h"

#include <string>
#include <vector>

#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2

namespace RevBayesCore {

    class TraceTree : public Trace {
    
    public:
    
        TraceTree( bool c );
        virtual                    ~TraceTree();
    
        // overloaded functions from RbObject
        TraceTree*                  clone(void) const;                                                          //!< Clone object
        void                        printValue(std::ostream& o) const;                                          //!< Print value for user
		
        void                        addObject(Tree *d);
        void                        addValueFromString(const std::string &s);
        bool                        isCoveredInInterval(const std::string &v, double i) const;
        const Tree&                 objectAt(size_t index)                          { return values.at(index); }
        void                        removeLastObject();
        void                        removeObjectAtIndex(int index);
        size_t                      size() const { return values.size(); }
    
        // getters and setters
        int                         getBurnin()                                     { return burnin; }
        double                      getEss()                                        { return ess; }
        std::string                 getFileName()                                   { return fileName; }
        std::string                 getParameterName()                              { return parmName; }
        int                         getSamples()                                    { return (int)values.size(); }
        int                         getStepSize()                                   { return stepSize; }		
        std::vector<Tree>           getValues()                                     { return values; }
        int                         hasConverged()                                  { return converged; }
        int                         hasPassedEssThreshold()                         { return passedEssThreshold; }
        int                         hasPassedGelmanRubinTest()                      { return passedGelmanRubinTest; }
        int                         hasPassedGewekeTest()                           { return passedGewekeTest; }
        int                         hasPassedIidBetweenChainsStatistic()            { return passedIidBetweenChainsStatistic; }
        int                         hasPassedSemThreshold()                         { return passedSemThreshold; }
        int                         hasPassedStationarityTest()                     { return passedStationarityTest; }
        bool                        isClock(void) const                             { return clock; }
        
        void                        setBurnin(int b)                                { burnin = b; }
        void                        setEss(double e)                                { ess = e; }
        void                        setFileName(std::string fn)                     { fileName = fn; }
        void                        setParameterName(std::string pm)                { parmName = pm; }
        void                        setStepSize( int s)                             { stepSize = s; }
        void                        setValues(const std::vector<Tree> &v)           { values = v; }
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
    
        std::vector<Tree>           values;                                     //!< the values of this TraceTree
    
        bool                        clock;
        
        std::string                 parmName;
        std::string                 fileName;
        std::string                 outgroup;                                   //!< The outgroup species for artificially rooting unrooted trees
        
        int                         burnin;
        double                      ess;                                        //!< the effective sample saize for this TraceTree
        int                         stepSize;                                   //!< the step size between samples
    
        int                         converged;                                  //!< Whether this parameter in itself has converged.
        int                         passedStationarityTest;                     //!< Whether this parameter passed the stationarity test.
        int                         passedGewekeTest;                           //!< Whether this parameter passed the Geweke statistic.
        //    int                     passedHeidelbergerWelcheStatistic;          //!< Whether this parameter passed the Heidelberger-Welch statistic.
        int                         passedEssThreshold;                         //!< Whether this parameter passed the threshold for the ESS.
        int                         passedSemThreshold;                         //!< Whether this parameter passed the threshold for the SEM.
        int                         passedIidBetweenChainsStatistic;            //!< Whether this parameter passed the iid test of chains.
        int                         passedGelmanRubinTest;                      //!< Whether this parameter passed the Gelman-Rubin statistic.
    
    };

    
}



#endif
