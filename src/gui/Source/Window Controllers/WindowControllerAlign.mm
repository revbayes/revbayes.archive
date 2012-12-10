#import "ToolAlign.h"
#import "WindowControllerAlign.h"



@implementation WindowControllerAlign

@synthesize alignmentMethod;
@synthesize clustalAlign;
@synthesize clustalWordLength;
@synthesize clustalWindow;
@synthesize clustalScoreType;
@synthesize clustalNumberDiagonals;
@synthesize clustalPairGapPenalty;
@synthesize clustalMatrix;
@synthesize clustalGapOpenPenalty;
@synthesize clustalEndGaps;
@synthesize clustalGapExtensionCost;
@synthesize clustalGapSeparationPenalty;
@synthesize clustalIteration;
@synthesize clustalNumberOfIterations;

- (void)awakeFromNib {

}

- (BOOL)tabView:(NSTabView*)tabView shouldSelectTabViewItem:(NSTabViewItem*)tabViewItem {

	NSString* methodLabel = [NSString stringWithString:[tabViewItem label]];
	if ( [methodLabel isEqualToString:@"CLUSTAL"] == YES )
        return YES;
    
    
    NSAlert* alert = [NSAlert alertWithMessageText:@"Unavailable Alignment Method" 
                                     defaultButton:@"OK"
                                   alternateButton:nil 
                                       otherButton:nil 
                         informativeTextWithFormat:@"This alignment method is not yet implemented."];
    [alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];


    return NO;
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
            [self setAlignmentMethod:             [myTool alignmentMethod]];
            
            [self setClustalAlign:                [myTool clustalAlign]];
            [self setClustalWordLength:           [myTool clustalWordLength]];            
            [self setClustalWindow:               [myTool clustalWindow]];
            [self setClustalScoreType:            [myTool clustalScoreType]];
            [self setClustalNumberDiagonals:      [myTool clustalNumberDiagonals]];
            [self setClustalPairGapPenalty:       [myTool clustalPairGapPenalty]];
            [self setClustalMatrix:               [myTool clustalMatrix]];
            [self setClustalGapOpenPenalty:       [myTool clustalGapOpenPenalty]];
            [self setClustalEndGaps:              [myTool clustalEndGaps]];
            [self setClustalGapExtensionCost:     [myTool clustalGapExtensionCost]];
            [self setClustalGapSeparationPenalty: [myTool clustalGapSeparationPenalty]];
            [self setClustalIteration:            [myTool clustalIteration]];
            [self setClustalNumberOfIterations:   [myTool clustalNumberOfIterations]];      
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
        // close the window
        
        [myTool closeControlPanel];
        [myTool setAlignmentMethod:alignmentMethod];

        if ( alignmentMethod == ALN_CLUSTAL )
            {
            // the user wants us to use the CLUSTAL program
            // set the clustal parameters
            [myTool setClustalAlign:                clustalAlign];
            [myTool setClustalWordLength:           clustalWordLength];            
            [myTool setClustalWindow:               clustalWindow];
            [myTool setClustalScoreType:            clustalScoreType];
            [myTool setClustalNumberDiagonals:      clustalNumberDiagonals];
            [myTool setClustalPairGapPenalty:       clustalPairGapPenalty];
            [myTool setClustalMatrix:               clustalMatrix];
            [myTool setClustalGapOpenPenalty:       clustalGapOpenPenalty];
            [myTool setClustalEndGaps:              clustalEndGaps];
            [myTool setClustalGapExtensionCost:     clustalGapExtensionCost];
            [myTool setClustalGapSeparationPenalty: clustalGapSeparationPenalty];
            [myTool setClustalIteration:            clustalIteration];
            [myTool setClustalNumberOfIterations:   clustalNumberOfIterations];  
            }
        
        [myTool resolveStateOnWindowOK];
        }
    else
        {
        // close the window
        [myTool closeControlPanel];
        }
}

- (IBAction)cancelButtonAction:(id)sender {
    
    [myTool closeControlPanel];
}

@end
