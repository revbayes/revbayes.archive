#ifndef RbUnrootedTopology_H
#define RbUnrootedTopology_H

#include "RbTopology.h"
#include "StringVector.h"
#include <string>
#include <ostream>

class RbInt;
class RbModel;
class RbObject;
class DAGNode;
class RandomNumberGenerator;

class RbUnrootedTopology : public RbTopology {

    public:
        static const StringVector   rbClass;            //!< Static class attribute
        
        RbUnrootedTopology(RbNode* bn);
        RbUnrootedTopology(const RbUnrootedTopology& m);
        virtual ~RbUnrootedTopology(void);

        void runChain(void);
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object

        // overloaded operators
        RbObject&                   operator=(const RbObject& o);
        RbUnrootedTopology&         operator=(const RbUnrootedTopology& o);
    
    private:
   	 
};

#endif 
