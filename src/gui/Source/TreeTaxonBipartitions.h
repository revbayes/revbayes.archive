#import <Cocoa/Cocoa.h>
#include <boost/dynamic_bitset.hpp>
#include "NodeKey.h"
#include <map>
class NodeVals;
@class GuiTree;




@interface TreeTaxonBipartitions : NSObject {

    int                                     numTaxa;
    int                                     numSamples;
    std::map<NodeKey,NodeVals*>             parts;
    NSMutableArray*                         names;
}

@property (nonatomic)        int       numSamples;

- (void)addPartitionsForTree:(GuiTree*)t;
- (id)initPartitionWithSize:(int)nt;
- (GuiTree*)consensusTree;
- (void)print;

@end
