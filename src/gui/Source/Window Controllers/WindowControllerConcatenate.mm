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
