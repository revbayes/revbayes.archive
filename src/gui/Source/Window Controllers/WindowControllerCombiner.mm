#import "ToolCombiner.h"
#import "WindowControllerCombiner.h"



@implementation WindowControllerCombiner

@synthesize numberOfInlets;

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (IBAction)helpButtonAction:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"CombinerTool_Anchor" inBook:locBookName];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolCombiner*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowCombiner"]) )
        {
        myTool = t;
		[self setNumberOfInlets:[myTool numberOfInlets]];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    if ( [myTool startingNumberOfInlets] != [self numberOfInlets] )
        {
        [myTool setNumberOfInlets:[self numberOfInlets]];
        [myTool setInletsAndOutlets];
        }
    [myTool closeControlPanel];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
