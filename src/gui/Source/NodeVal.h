#ifndef NodeVals_H
#define NodeVals_H

#include <vector>
#include <boost/dynamic_bitset.hpp>



class NodeVals {

	public:
                                        NodeVals(boost::dynamic_bitset<>& bf);
                                       ~NodeVals(void);
        bool                            operator==(const NodeVals& a) const;
        bool                            operator<(const NodeVals& a) const;
        void                            addSample(float w);
        void                            normalizeBitVector(void);
        float                           getNumSamples(void) { return count; }
        const boost::dynamic_bitset<>&  getPartition(void) const { return partDescription; }
        bool                            isSingleton(void);

    private:
                                        NodeVals(void);
        float                           count;
        boost::dynamic_bitset<>         partDescription;
};

struct comp {

bool operator()(const NodeVals* lhs, const NodeVals* rhs)
    {
    if ( (*lhs) < (*rhs) )
        return true;
    else if ( (*lhs) == (*rhs) )
        {
        return lhs->getPartition() < rhs->getPartition();
        }
    return false;
    }
};

#endif