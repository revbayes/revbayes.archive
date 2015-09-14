/* 
 * File:   RealNodeContainer.h
 * Author: nl
 *
 * Created on 26 juillet 2014, 12:39
 */

#ifndef REALNODECONTAINER_H
#define	REALNODECONTAINER_H

#include "MemberObject.h"
#include "TimeTree.h"
#include "ContinuousCharacterData.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class RealNodeContainer : public std::vector<double>, public MemberObject<double> {
    
    public:
        RealNodeContainer(void);                                                                                                                 //!< Default constructor
        RealNodeContainer(const TimeTree* intree);                                                                                             //!< Default constructor
        RealNodeContainer(const RealNodeContainer& t);                                                                                    //!< Copy constructor
        virtual                                    ~RealNodeContainer(void);                                                                                                        //!< Destructor
    
        RealNodeContainer&                   operator=(const RealNodeContainer& t);
    
        // Basic utility functions
        RealNodeContainer*                   clone(void) const;                                                                                  //!< Clone object
    
        void                                        executeMethod(const std::string &n, const std::vector<const DagNode*> &args, double &rv) const;     //!< Map the member methods to internal function calls
        const TimeTree*                             getTimeTree() const {return tree;}        

        // RealNodeContainer functions
        double                                      getRootVal() const;
        double                                      getMean() const;
        double                                      getMeanOverTips() const;
        double                                      getStdev() const;
        void                                        recursiveGetStats(const TopologyNode& from, double& e1, double& e2, int& n) const;
        void                                        recursiveGetStatsOverTips(const TopologyNode& from, double& e1, double& e2, int& n) const;
        ContinuousCharacterData*                    getTipValues() const;
        void                                        recursiveGetTipValues(const TopologyNode& from, ContinuousCharacterData& nameToVal) const ;


        std::string                                 getNewick() const;
        std::string                                 recursiveGetNewick(const TopologyNode& from) const;

        bool                                        isClamped(size_t index) const;
        void                                        clampAt(const ContinuousCharacterData* data, size_t k);
        void                                        recursiveClampAt(const TopologyNode& from, const ContinuousCharacterData* data, size_t k);
        
    private:

        const TimeTree*                     tree;
        std::vector<bool>                   clampVector;

    };

    std::ostream&                       operator<<(std::ostream& o, const RealNodeContainer& x);                                         //!< Overloaded output operator

}

#endif	/* REALNODECONTAINER_H */

