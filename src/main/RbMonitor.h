#ifndef Monitor_H
#define Monitor_H

#include <string>
#include <vector>
#include <ostream>
#include "RbComplex.h"

class DAGNode;
class RbObject;
class RbInt;

class RbMonitor : public RbComplex {

public:
        RbMonitor(std::ostream& o, DAGNode* n, RbInt& f);
        RbMonitor(const RbMonitor& m);
        virtual ~RbMonitor(void);

        // Basic utility functions
        RbObject*                  clone() const;                                  //!< Clone object
        bool                       equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&        getClass() const { return rbClass; }            //!< Get class
        void                       print(std::ostream& o) const;                   //!< Print complete object info
        void                       printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                toString(void) const;                           //!< General info on object

        // overloaded operators
        RbObject&                   operator=(const RbObject& o);
        RbMonitor&                  operator=(const RbMonitor& o);

        void                        monitor(int i);

private:
	int sampleFrequency;
	std::ostream* outStrm;
	DAGNode* node;
};

#endif
