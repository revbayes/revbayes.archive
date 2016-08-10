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
@synthesize muscleAnchorSpacing;
@synthesize muscleCenter;
@synthesize muscleCluster1;
@synthesize muscleCluster2;
@synthesize muscleDiagLength;
@synthesize muscleDiagMargin;
@synthesize muscleDistance1;
@synthesize muscleDistance2;
@synthesize muscleGapOpen;
@synthesize muscleHydro;
@synthesize muscleHydroFactor;
@synthesize muscleMaxDiagBreak;
@synthesize muscleMaxIters;
@synthesize muscleMaxTrees;
@synthesize muscleMinBestColScore;
@synthesize muscleMinSmoothScore;
@synthesize muscleObjScore;
@synthesize muscleRoot1;
@synthesize muscleRoot2;
@synthesize muscleSmoothScoreCeil;
@synthesize muscleSmoothWindow;
@synthesize muscleSUEFF;
@synthesize muscleWeight1;
@synthesize muscleWeight2;

- (void)awakeFromNib {

    [self resizeAlignmentWindow];
}

- (BOOL)tabView:(NSTabView*)tabView shouldSelectTabViewItem:(NSTabViewItem*)tabViewItem {

    [self resizeAlignmentWindow];

	NSString* methodLabel = [NSString stringWithString:[tabViewItem label]];
	if ( [methodLabel isEqualToString:@"CLUSTAL"] == YES )
        {
        alignmentMethod = ALN_CLUSTAL;
        [self resizeAlignmentWindowToHeight:400.0];
        return YES;
        }
    else if ( [methodLabel isEqualToString:@"MUSCLE"] == YES)
        {
        alignmentMethod = ALN_MUSCLE;
        [self resizeAlignmentWindowToHeight:540.0];
        return YES;
        }
    
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Unavailable Alignment Method"];
    [alert setInformativeText:@"This alignment method is not yet implemented."];
    [alert beginSheetModalForWindow:[self window] completionHandler:nil];

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

            [self setMuscleAnchorSpacing:         [myTool muscleAnchorSpacing]];
            [self setMuscleCenter:                [myTool muscleCenter]];
            [self setMuscleCluster1:              [myTool muscleCluster1]];
            [self setMuscleCluster2:              [myTool muscleCluster2]];
            [self setMuscleDiagLength:            [myTool muscleDiagLength]];
            [self setMuscleDiagMargin:            [myTool muscleDiagMargin]];
            [self setMuscleDistance1:             [myTool muscleDistance1]];
            [self setMuscleDistance2:             [myTool muscleDistance2]];
            [self setMuscleGapOpen:               [myTool muscleGapOpen]];
            [self setMuscleHydro:                 [myTool muscleHydro]];
            [self setMuscleHydroFactor:           [myTool muscleHydroFactor]];
            [self setMuscleMaxDiagBreak:          [myTool muscleMaxDiagBreak]];
            [self setMuscleMaxIters:              [myTool muscleMaxIters]];
            [self setMuscleMaxTrees:              [myTool muscleMaxTrees]];
            [self setMuscleMinBestColScore:       [myTool muscleMinBestColScore]];
            [self setMuscleMinSmoothScore:        [myTool muscleMinSmoothScore]];
            [self setMuscleObjScore:              [myTool muscleObjScore]];
            [self setMuscleRoot1:                 [myTool muscleRoot1]];
            [self setMuscleRoot2:                 [myTool muscleRoot2]];
            [self setMuscleSmoothScoreCeil:       [myTool muscleSmoothScoreCeil]];
            [self setMuscleSmoothWindow:          [myTool muscleSmoothWindow]];
            [self setMuscleSUEFF:                 [myTool muscleSUEFF]];
            [self setMuscleWeight1:               [myTool muscleWeight1]];
            [self setMuscleWeight2:               [myTool muscleWeight2]];
            }
        }
	return self;
}

- (void)windowDidLoad {
        
}

- (IBAction)okButtonAction:(id)sender {

	NSString* methodLabel = [NSString stringWithString:[[alignmentMethodSelectorTab selectedTabViewItem] label]];
    [myTool setAlignmentMethod:alignmentMethod];
    
	if ( [methodLabel isEqualToString:@"CLUSTAL"] == YES )
        {
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
    else if ( [methodLabel isEqualToString:@"MUSCLE"] == YES )
        {
        [myTool setMuscleAnchorSpacing:         muscleAnchorSpacing];
        [myTool setMuscleCenter:                muscleCenter];
        [myTool setMuscleCluster1:              muscleCluster1];
        [myTool setMuscleCluster2:              muscleCluster2];
        [myTool setMuscleDiagLength:            muscleDiagLength];
        [myTool setMuscleDiagMargin:            muscleDiagMargin];
        [myTool setMuscleDistance1:             muscleDistance1];
        [myTool setMuscleDistance2:             muscleDistance2];
        [myTool setMuscleGapOpen:               muscleGapOpen];
        [myTool setMuscleHydro:                 muscleHydro];
        [myTool setMuscleHydroFactor:           muscleHydroFactor];
        [myTool setMuscleMaxDiagBreak:          muscleMaxDiagBreak];
        [myTool setMuscleMaxIters:              muscleMaxIters];
        [myTool setMuscleMaxTrees:              muscleMaxTrees];
        [myTool setMuscleMinBestColScore:       muscleMinBestColScore];
        [myTool setMuscleMinSmoothScore:        muscleMinSmoothScore];
        [myTool setMuscleObjScore:              muscleObjScore];
        [myTool setMuscleRoot1:                 muscleRoot1];
        [myTool setMuscleRoot2:                 muscleRoot2];
        [myTool setMuscleSmoothScoreCeil:       muscleSmoothScoreCeil];
        [myTool setMuscleSmoothWindow:          muscleSmoothWindow];
        [myTool setMuscleSUEFF:                 muscleSUEFF];
        [myTool setMuscleWeight1:               muscleWeight1];
        [myTool setMuscleWeight2:               muscleWeight2];
        }
    else
        {
        }

    [myTool closeControlPanelWithOK];
}

- (IBAction)cancelButtonAction:(id)sender {
    
    [myTool closeControlPanelWithCancel];
}

- (void)resizeAlignmentWindow {

	NSString* methodLabel = [NSString stringWithString:[[alignmentMethodSelectorTab selectedTabViewItem] label]];

    float h = 0.0;
	if ( [methodLabel isEqualToString:@"CLUSTAL"] == YES )
        h = 400.0;
    else if ( [methodLabel isEqualToString:@"MUSCLE"] == YES)
        h = 540.0;
    
    [self resizeAlignmentWindowToHeight:h];
}

- (void)resizeAlignmentWindowToHeight:(float)h {

    NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    newFrame.size.height = h;
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

@end
