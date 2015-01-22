#ifndef TreeTrace_H
#define TreeTrace_H

#include "BranchLengthTree.h"
#include "TimeTree.h"

#include <string>
#include <vector>

#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2

namespace RevBayesCore {

    template<class treeType>
    class TreeTrace {
    
    public:
    
        TreeTrace(void);
        virtual                    ~TreeTrace();
    
        // overloaded functions from RbObject
        TreeTrace*                  clone(void) const;                                                          //!< Clone object
        void                        printValue(std::ostream& o) const;                                          //!< Print value for user
		
        void                        addObject(treeType *d);			
        treeType*                   objectAt(size_t index)                         { return values.at(index); }			
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
        std::vector<treeType*>      getValues()                                     { return values; }			
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
        void                        setValues(const std::vector<treeType> &v)       { values = v; }
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
    
        std::vector<treeType*>   values;                                     //!< the values of this TreeTrace
    
        std::string             parmName;
        std::string             fileName;
        std::string             outgroup;                                   //!< The outgroup species for artificially rooting unrooted trees
        
        int                     burnin;
        double                  ess;                                        //!< the effective sample saize for this TreeTrace
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

    
    template <>
    inline void RevBayesCore::TreeTrace<BranchLengthTree>::addObject(BranchLengthTree *t)
    {
        // re-root the tree so that we can compare the the trees
//        if ( outgroup == "" )
//            outgroup = t->getTipNode(0).getName();
//        t->reroot( outgroup );
        
        
        values.push_back(t);
        
        // invalidate for recalculation of meta data
        invalidate();
    }
    
    
    template <>
    inline void RevBayesCore::TreeTrace<TimeTree>::addObject(TimeTree *t)
    {
        
        values.push_back(t);
        
        // invalidate for recalculation of meta data
        invalidate();
    }
    
    
}


#include "RbUtil.h"

#include <sstream>
#include <string>
#include <vector>


template<class treeType>
RevBayesCore::TreeTrace<treeType>::TreeTrace()
{
    outgroup = "";
	values.clear();
    invalidate();
}


template<class treeType>
RevBayesCore::TreeTrace<treeType>::~TreeTrace() {
    values.clear();
}


/** Clone function */
template<class treeType>
RevBayesCore::TreeTrace<treeType>* RevBayesCore::TreeTrace<treeType>::clone() const {
    
    return new TreeTrace<treeType>(*this);
}


template<class treeType>
void RevBayesCore::TreeTrace<treeType>::invalidate() {
    // set values to defaults and mark for recalculation
    burnin                          = -1;
    ess                             = -1;
    stepSize                        = 1;
    
    converged                       = NOT_CHECKED;
    passedStationarityTest          = NOT_CHECKED;
    passedGewekeTest                = NOT_CHECKED;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = NOT_CHECKED;
    passedSemThreshold              = NOT_CHECKED;
    passedIidBetweenChainsStatistic = NOT_CHECKED;
    passedGelmanRubinTest           = NOT_CHECKED;
    
    
}

/** Print value for user */
template<class treeType>
void RevBayesCore::TreeTrace<treeType>::printValue(std::ostream &o) const {
    
    o << "TreeTrace values to be printed ...";
}


template<class treeType>
void RevBayesCore::TreeTrace<treeType>::removeObjectAtIndex (int index){
    // create a iterator for the vector
    std::vector<TimeTree>::iterator it = values.begin();
    
    //jump to the position to remove
    it += index;
    
    // remove the element
    values.erase(it);
    
    // invalidate for recalculation of meta data
    invalidate();
}

template<class treeType>
void RevBayesCore::TreeTrace<treeType>::removeLastObject() {
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}



#endif
