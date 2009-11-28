#ifndef RbMcmc_H
#define RbMcmc_H

#include "RbComplex.h"
#include "StringVector.h"
#include <string>
#include <ostream>

class RbModel;
class RbObject;
class DAGNode;

class RbMcmc : public RbComplex {

    public:
        static const StringVector   rbClass;            //!< Static class attribute
        
        RbMcmc(RbModel* mp);
        RbMcmc(RbMcmc& m);
        virtual ~RbMcmc(void);
        void runChain(void);
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object

        // overloaded operators
        RbObject&                   operator=(const RbObject& o);
        RbMcmc&                     operator=(const RbMcmc& o);
    private:
        DAGNode*                    getDagToChange();
        double                      getLnPriorRatio(DAGNode* d);
        double                      update(DAGNode* d);
        double                      getLnLikelihoodRatio(DAGNode* d);
        void                        accept(DAGNode* d);
        void                        reject(DAGNode* d);

   	    RbModel*                    modelPtr;
   	 
};

#endif 
