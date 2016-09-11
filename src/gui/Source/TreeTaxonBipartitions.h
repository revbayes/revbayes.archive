#import <Cocoa/Cocoa.h>
#include <boost/dynamic_bitset.hpp>
#include "NodeKey.h"
#include <map>
class NodeVals;
@class GuiTree;
@class ToolTreeConsensus;




@interface TreeTaxonBipartitions : NSObject {

    ToolTreeConsensus*                      myTool;
    int                                     numTaxa;
    float                                   numSamples;
    std::map<NodeKey,NodeVals*>             parts;
    NSMutableArray*                         names;
}

@property (nonatomic)        float          numSamples;

- (void)addPartitionsForTree:(GuiTree*)t withWeight:(float)w;
- (GuiTree*)consensusTree;
- (id)initWithTool:(ToolTreeConsensus*)t;
- (int)numberOfPartitions;
- (void)print;
- (void)removePartitions;
- (NSString*)taxonIndexed:(int)idx;

@end
