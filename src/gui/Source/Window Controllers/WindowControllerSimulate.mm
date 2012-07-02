#import "ToolSimulate.h"
#import "Tree.h"
#import "WindowControllerSimulate.h"



@implementation WindowControllerSimulate

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (void)dealloc {

	[myTree release];
	[super dealloc];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolSimulate*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowSimulate"]) )
        {
        myTool = t;
        
        myTree = [[Tree alloc] initWithTipSize:3];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    NSLog(@"number of nodes = %d", [myTree numberOfNodes]);
    [myTool closeControlPanel];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
