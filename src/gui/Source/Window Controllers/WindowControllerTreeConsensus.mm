#import "ToolTreeConsensus.h"
#import "WindowControllerTreeConsensus.h"




@implementation WindowControllerTreeConsensus

@synthesize partitionFrequencies;
@synthesize oldPartitionFrequencies;
@synthesize showAllCompatiblePartitions;
@synthesize oldShowAllCompatiblePartitions;

- (IBAction)cancelAction:(id)sender {

    [self setPartitionFrequencies:oldPartitionFrequencies];
    [self setShowAllCompatiblePartitions:oldShowAllCompatiblePartitions];
    [myTool closeControlPanel];
}

- (id)initWithTool:(ToolTreeConsensus*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowConsensusTree"]) )
        {
        myTool = t;
        [self setPartitionFrequencies:[myTool partitionFrequencies]];
        [self setShowAllCompatiblePartitions:[myTool showAllCompatiblePartitions]];
        oldPartitionFrequencies = partitionFrequencies;
        oldShowAllCompatiblePartitions = showAllCompatiblePartitions;
        }
	return self;
}

- (IBAction)helpAction:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"TreeSetConsensus_Anchor" inBook:locBookName];
}

- (IBAction)okAction:(id)sender {

    [myTool setPartitionFrequencies:partitionFrequencies];
    [myTool setShowAllCompatiblePartitions:showAllCompatiblePartitions];
    [myTool closeControlPanel];
}

- (void)windowDidLoad {

    [super windowDidLoad];
    
    // initialization
}

@end
