/* 
 * File:   MultivariateRealNodeContainer.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 20:17
 */

#ifndef PhyloMultivariatePROCESS_H
#define	PhyloMultivariatePROCESS_H


#include "MatrixReal.h"
#include "MatrixRealSymmetric.h"
#include "MemberObject.h"
#include "TimeTree.h"
#include "ContinuousCharacterData.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class MultivariateRealNodeContainer : public MatrixReal, public MemberObject<double> {
    
    public:
        MultivariateRealNodeContainer(void);                                                                                                                 //!< Default constructor
        MultivariateRealNodeContainer(const TimeTree* intree, size_t indim);                                                                                             //!< Default constructor
        MultivariateRealNodeContainer(const MultivariateRealNodeContainer& t);                                                                                    //!< Copy constructor
        virtual                                    ~MultivariateRealNodeContainer(void);                                                                                                        //!< Destructor
    
        MultivariateRealNodeContainer&                   operator=(const MultivariateRealNodeContainer& t);
    
        // Basic utility functions
        MultivariateRealNodeContainer*                   clone(void) const;                                                                                  //!< Clone object
    
        void                                        executeMethod(const std::string &n, const std::vector<const DagNode*> &args, double &rv) const;     //!< Map the member methods to internal function calls
        const TimeTree*                             getTimeTree() const {return tree;}
        void                                        setTimeTree(const TimeTree* intree) {tree = intree;}
        
        size_t                                      getDim() const {return getNumberOfColumns();}
        
        // MultivariateRealNodeContainer functions
        double                                      getRootVal(int k) const;
        double                                      getMean(int k) const;
        double                                      getMeanOverTips(int k) const;
        double                                      getStdev(int k) const;
        void                                        recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n) const;
        void                                        recursiveGetStatsOverTips(int k, const TopologyNode& from, double& e1, double& e2, int& n) const;

        std::string                                 getNewick(int k) const;
        std::string                                 recursiveGetNewick(const TopologyNode& from, int k) const;        
        std::string                                 getNewick() const;
        std::string                                 recursiveGetNewick(const TopologyNode& from) const;
        
        void                                        printBranchContrasts(std::ostream& os) const;
        MatrixRealSymmetric                         getBranchContrasts(int& nnode) const ;

        bool                                        isClamped(size_t index, size_t k) const;
        void                                        clampAt(const ContinuousCharacterData* data, size_t k, size_t l);
        void                                        recursiveClampAt(const TopologyNode& from, const ContinuousCharacterData* data, size_t k, size_t l);
        
    protected:

        void                                        recursiveGetBranchContrasts(const TopologyNode& from, MatrixRealSymmetric& c, int& n) const;

    private:

        const TimeTree*                     tree;
        std::vector<std::vector<bool> >     clampVector;

    };

    std::ostream&                       operator<<(std::ostream& o, const MultivariateRealNodeContainer& x);                                         //!< Overloaded output operator

}

#endif	/* PhyloMultivariatePROCESS_H */

