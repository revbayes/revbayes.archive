#import "ToolConcatenate.h"
#import "WindowControllerConcatenate.h"



@implementation WindowControllerConcatenate

@synthesize matchMethod;
@synthesize mergeMethod;

- (void)awakeFromNib {

    if ( [self mergeMethod] == MERGE_BY_DATA_TYPE )
        [mergeMethodButton selectItemWithTitle:@"By data type"];
    else
        [mergeMethodButton selectItemWithTitle:@"Unconditionally"];
    
    if ( [self matchMethod] == INTERSECTION_OVERLAP_METHOD )
        [matchMethodButton selectItemWithTitle:@"Intersection"];
    else
        [matchMethodButton selectItemWithTitle:@"Union"];
}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (IBAction)changeMatchMethod:(id)sender {

    if ( [[matchMethodButton titleOfSelectedItem] isEqualToString:@"Intersection"] == YES )
        [self setMatchMethod:INTERSECTION_OVERLAP_METHOD];
    else
        [self setMatchMethod:UNION_OVERLAP_METHOD];
}

- (IBAction)changeMergeMethod:(id)sender {

    if ( [[mergeMethodButton titleOfSelectedItem] isEqualToString:@"By data type"] == YES )
        [self setMergeMethod:MERGE_BY_DATA_TYPE];
    else
        [self setMergeMethod:MERGE_UNCONDITIONALLY];
}

- (IBAction)helpButtonAction:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"ConcatenateTool_Anchor" inBook:locBookName];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolConcatenate*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowConcatenate"]) )
        {
        myTool = t;
        [self setMatchMethod:[myTool matchMethod]];
        [self setMergeMethod:[myTool mergeMethod]];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    [myTool setMatchMethod:[self matchMethod]];
    [myTool setMergeMethod:[self mergeMethod]];
    
    [myTool closeControlPanel];
    [myTool resolveStateOnWindowOK];
}

- (IBAction)showWindow:(id)sender {

    if ( [self mergeMethod] == MERGE_BY_DATA_TYPE )
        [mergeMethodButton selectItemWithTitle:@"By data type"];
    else
        [mergeMethodButton selectItemWithTitle:@"Unconditionally"];
    
    if ( [self matchMethod] == INTERSECTION_OVERLAP_METHOD )
        [matchMethodButton selectItemWithTitle:@"Intersection"];
    else
        [matchMethodButton selectItemWithTitle:@"Union"];
    
    int n1 = [myTool numberOfIncomingAlignments];
    if (n1 == -1)
        {
        NSString* myString1 = @"Number of alignments: No parent tool found";
        [numAlignments setStringValue:myString1];
        NSString* myString2 = @"Number of sequences: No parent tool found";
        [numSequences setStringValue:myString2];
        }
    else
        {
        if (n1 == 0)
            {
            NSString* myString1 = @"Number of alignments: 0";
            [numAlignments setStringValue:myString1];
            NSString* myString2 = @"Number of sequences: N/A";
            [numSequences setStringValue:myString2];
            }
        else
            {
            int min1 = [myTool mininumNumberOfIncomingSequences];
            int max1 = [myTool maximumNumberOfIncomingSequences];
            
            NSString* myString1 = [NSString stringWithFormat:@"Number of alignments: %d", n1];
            [numAlignments setStringValue:myString1];
            if (min1 == max1)
                {
                NSString* myString2 = [NSString stringWithFormat:@"Number of sequences: %d", min1];
                [numSequences setStringValue:myString2];
                }
            else
                {
                NSString* myString2 = [NSString stringWithFormat:@"Number of sequences: %d - %d", min1, max1];
                [numSequences setStringValue:myString2];
                }
            }
        }

    [super showWindow:sender];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
