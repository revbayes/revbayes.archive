/* 
 * File:   MultivariatePhyloProcess.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 20:17
 */

#ifndef MULTIVARIATEPHYLOPROCESS_H
#define	MULTIVARIATEPHYLOPROCESS_H


#include "MatrixReal.h"
#include "TimeTree.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class MultivariatePhyloProcess : public MatrixReal {
    
    public:
        MultivariatePhyloProcess(void);                                                                                             //!< Default constructor
        MultivariatePhyloProcess(const TimeTree* intree, size_t indim);                                                                                             //!< Default constructor
        MultivariatePhyloProcess(const MultivariatePhyloProcess& t);                                                                        //!< Copy constructor
        virtual                                    ~MultivariatePhyloProcess(void);                                                                                                        //!< Destructor
    
        MultivariatePhyloProcess&                           operator=(const MultivariatePhyloProcess& t);
    
        // Basic utility functions
        MultivariatePhyloProcess*                   clone(void) const;                                                      //!< Clone object
    
        const TimeTree*                             getTimeTree() const {return tree;}
        size_t                                      getDim() const {return getNumberOfColumns();}
        
        // MultivariatePhyloProcess functions
        /*
        double                                      getAge(size_t idx) const;
        double                                      getBranchLength(size_t idx) const;
        double                                      getTime(size_t idx) const;
        double                                      getTreeLength(void) const;
        void                                        setBranchLength(size_t idx, double bl);
        */
        
    protected:
        // void                                        resizeElementVectors(size_t n);
    
    private:

        const TimeTree*                     tree;

    };

}

// Global functions using the class
// std::ostream&                       operator<<(std::ostream& o, const RevBayesCore::MultivariatePhyloProcess& x);                                         //!< Overloaded output operator

#endif	/* MULTIVARIATEPHYLOPROCESS_H */

