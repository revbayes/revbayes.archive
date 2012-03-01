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
