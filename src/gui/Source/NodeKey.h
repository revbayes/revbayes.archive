#ifndef NodeKey_H
#define NodeKey_H

#include <boost/dynamic_bitset.hpp>



class NodeKey {

	public:
                                        NodeKey(boost::dynamic_bitset<>& bf);
                                       ~NodeKey(void);
        bool                            operator<(const NodeKey &a) const;
        void                            print(void);
        const boost::dynamic_bitset<>&  getPartition(void) const { return partDescription; }

    private:
        boost::dynamic_bitset<>         partDescription;
};


#endif