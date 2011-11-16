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
        myTool        = t;


		// set default values for the window
            
        // pick up Clustal values from myTool and bind
            [self setAlignClustalAlign:[myTool alignClustalAlign]];
            [self setAlignClustalWordLength: [myTool alignClustalWordLength]];            
            [self setAlignClustalWindow: [myTool alignClustalWindow]];
            [self setAlignClustalScoreType: [myTool alignClustalScoreType]];
            [self setAlignClustalNumberDiagonals: [myTool alignClustalNumberDiagonals]];
            [self setAlignClustalPairGapPenalty: [myTool alignClustalPairGapPenalty]];
            [self setAlignClustalMatrix: [myTool alignClustalMatrix]];
            [self setAlignClustalGapOpenPenalty: [myTool alignClustalGapOpenPenalty]];
            [self setAlignClustalEndGaps: [myTool alignClustalEndGaps]];
            [self setAlignClustalGapExtensionCost: [myTool alignClustalGapExtensionCost]];
            [self setAlignClustalGapSeparationPenalty: [myTool alignClustalGapSeparationPenalty]];
            [self setAlignClustalIteration: [myTool alignClustalIteration]];
            [self setAlignClustalNumberOfIterations: [myTool alignClustalNumberOfIterations]];        
        }
	return self;
}

- (void)windowDidLoad {
    
    NSLog (@"Window Did Load");
    
}

- (IBAction)okButtonAction:(id)sender {

    NSLog (@"okButtonAction");

    [myTool setAlignClustalAlign: alignClustalAlign];
    NSLog(@"controller align = %@, tool align = %@", alignClustalAlign, [myTool alignClustalAlign]);
    
    [myTool setAlignClustalWordLength: alignClustalWordLength];            
    [myTool setAlignClustalWindow: alignClustalWindow];
    [myTool setAlignClustalScoreType: alignClustalScoreType];
    [myTool setAlignClustalNumberDiagonals: alignClustalNumberDiagonals];
    [myTool setAlignClustalPairGapPenalty: alignClustalPairGapPenalty];
    [myTool setAlignClustalMatrix: alignClustalMatrix];
    [myTool setAlignClustalGapOpenPenalty: alignClustalGapOpenPenalty];
    [myTool setAlignClustalEndGaps: alignClustalEndGaps];
    [myTool setAlignClustalGapExtensionCost: alignClustalGapExtensionCost];
    [myTool setAlignClustalGapSeparationPenalty: alignClustalGapSeparationPenalty];
    [myTool setAlignClustalIteration: alignClustalIteration];
    [myTool setAlignClustalNumberOfIterations: alignClustalNumberOfIterations];        

    [myTool helperRunClustal: (id)sender];
    
    [myTool closeControlPanel];
}

- (IBAction)cancelButtonAction:(id)sender {
    NSLog (@"cancelButtonAction");
    
    [myTool closeControlPanel];

}
@end
