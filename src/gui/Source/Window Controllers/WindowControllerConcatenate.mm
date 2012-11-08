#import "ToolConcatenate.h"
#import "WindowControllerConcatenate.h"



@implementation WindowControllerConcatenate

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolConcatenate*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowConcatenate"]) )
        {
        myTool = t;
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    [myTool closeControlPanel];
    
    int overlapMethod, mergeMethod;
    if ( [[minOrMaxOverlapButton titleOfSelectedItem] isEqualToString:@"Minimal overlap among sequences"] == YES )
        overlapMethod = MINIMAL_OVERLAP_METHOD;
    else
        overlapMethod = MAXIMAL_OVERLAP_METHOD;
    if ( [[seqMatchMethodButton titleOfSelectedItem] isEqualToString:@"Merge by data type"] == YES )
        mergeMethod = MERGE_BY_DATA_TYPE;
    else
        mergeMethod = MERGE_UNCONDITIONALLY;
    [myTool concatenateWithOverlap:overlapMethod andMergeMethod:mergeMethod];
}

- (int)mergeMethod {

    if ( [[seqMatchMethodButton titleOfSelectedItem] isEqualToString:@"Merge by data type"] == YES )
        return MERGE_BY_DATA_TYPE;
    return MERGE_UNCONDITIONALLY;
}

- (int)matchingMethod {

    if ( [[minOrMaxOverlapButton titleOfSelectedItem] isEqualToString:@"Minimal overlap among sequences"] == YES )
        return MINIMAL_OVERLAP_METHOD;
    return MAXIMAL_OVERLAP_METHOD;
}

- (IBAction)showWindow:(id)sender {

    if ( [myTool useMinimalSet] == YES )
        {
        [minOrMaxOverlapButton selectItemWithTag:0];
        }
    else
        {
        [minOrMaxOverlapButton selectItemWithTag:1];
        }

    if ( [myTool matchUsingNames] == YES )
        {
        [seqMatchMethodButton selectItemWithTag:0];
        }
    else
        {
        [seqMatchMethodButton selectItemWithTag:1];
        }
        
    NSString* myString1 = [NSString stringWithFormat:@"Number of alignments: %d", 0];
    [numAlignments setStringValue:myString1];
    NSString* myString2 = [NSString stringWithFormat:@"Number of sequences: %d - %d", 0, 10];
    [numSequences setStringValue:myString2];

    [super showWindow:sender];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
