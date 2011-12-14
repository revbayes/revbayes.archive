#import "ToolAlign.h"
#import "WindowControllerAlign.h"



@implementation WindowControllerAlign

@synthesize alignClustalAlign;
@synthesize alignClustalWordLength;
@synthesize alignClustalWindow;
@synthesize alignClustalScoreType;
@synthesize alignClustalNumberDiagonals;
@synthesize alignClustalPairGapPenalty;
@synthesize alignClustalMatrix;
@synthesize alignClustalGapOpenPenalty;
@synthesize alignClustalEndGaps;
@synthesize alignClustalGapExtensionCost;
@synthesize alignClustalGapSeparationPenalty;
@synthesize alignClustalIteration;
@synthesize alignClustalNumberOfIterations;

- (void)awakeFromNib {

}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolAlign*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowAlign"]) )
        {
		// initialize the address of the tool associated with this control window
        myTool = t;
            
        // pick up Clustal values from myTool and bind
        if (myTool != nil)
            {
            [self setAlignClustalAlign:                [myTool alignClustalAlign]];
            [self setAlignClustalWordLength:           [myTool alignClustalWordLength]];            
            [self setAlignClustalWindow:               [myTool alignClustalWindow]];
            [self setAlignClustalScoreType:            [myTool alignClustalScoreType]];
            [self setAlignClustalNumberDiagonals:      [myTool alignClustalNumberDiagonals]];
            [self setAlignClustalPairGapPenalty:       [myTool alignClustalPairGapPenalty]];
            [self setAlignClustalMatrix:               [myTool alignClustalMatrix]];
            [self setAlignClustalGapOpenPenalty:       [myTool alignClustalGapOpenPenalty]];
            [self setAlignClustalEndGaps:              [myTool alignClustalEndGaps]];
            [self setAlignClustalGapExtensionCost:     [myTool alignClustalGapExtensionCost]];
            [self setAlignClustalGapSeparationPenalty: [myTool alignClustalGapSeparationPenalty]];
            [self setAlignClustalIteration:            [myTool alignClustalIteration]];
            [self setAlignClustalNumberOfIterations:   [myTool alignClustalNumberOfIterations]];      
            }
        }
	return self;
}

- (void)windowDidLoad {
        
}

- (IBAction)okButtonAction:(id)sender {

	NSString* methodLabel = [NSString stringWithString:[[alignmentMethodSelectorTab selectedTabViewItem] label]];
	if ( [methodLabel isEqualToString:@"CLUSTAL"] == YES )
        {
        // the user wants us to use the CLUSTAL program
        // set the clustal parameters
        [myTool setAlignClustalAlign:                alignClustalAlign];
        [myTool setAlignClustalWordLength:           alignClustalWordLength];            
        [myTool setAlignClustalWindow:               alignClustalWindow];
        [myTool setAlignClustalScoreType:            alignClustalScoreType];
        [myTool setAlignClustalNumberDiagonals:      alignClustalNumberDiagonals];
        [myTool setAlignClustalPairGapPenalty:       alignClustalPairGapPenalty];
        [myTool setAlignClustalMatrix:               alignClustalMatrix];
        [myTool setAlignClustalGapOpenPenalty:       alignClustalGapOpenPenalty];
        [myTool setAlignClustalEndGaps:              alignClustalEndGaps];
        [myTool setAlignClustalGapExtensionCost:     alignClustalGapExtensionCost];
        [myTool setAlignClustalGapSeparationPenalty: alignClustalGapSeparationPenalty];
        [myTool setAlignClustalIteration:            alignClustalIteration];
        [myTool setAlignClustalNumberOfIterations:   alignClustalNumberOfIterations];     
        
        // run clustal
        [myTool helperRunClustal:(id)sender];
        }

    
    // close the window
    [myTool closeControlPanel];
}

- (IBAction)cancelButtonAction:(id)sender {
    
    [myTool closeControlPanel];
}

@end
