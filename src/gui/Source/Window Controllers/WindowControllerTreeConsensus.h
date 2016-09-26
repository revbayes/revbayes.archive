#import <Cocoa/Cocoa.h>
@class ToolTreeConsensus;




@interface WindowControllerTreeConsensus : NSWindowController {

    ToolTreeConsensus*      myTool;
    float                   partitionFrequencies;
    float                   oldPartitionFrequencies;
    BOOL                    showAllCompatiblePartitions;
    BOOL                    oldShowAllCompatiblePartitions;
}

@property (nonatomic) float partitionFrequencies;
@property (nonatomic) float oldPartitionFrequencies;
@property (nonatomic) BOOL  showAllCompatiblePartitions;
@property (nonatomic) BOOL  oldShowAllCompatiblePartitions;

- (IBAction)helpAction:(id)sender;
- (id)initWithTool:(ToolTreeConsensus*)t;

@end
