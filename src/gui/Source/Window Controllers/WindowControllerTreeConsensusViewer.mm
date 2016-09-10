#import "ConTreeView.h"
#import "ToolTreeConsensus.h"
#import "WindowControllerTreeConsensusViewer.h"



@interface WindowControllerTreeConsensusViewer ()

@end

@implementation WindowControllerTreeConsensusViewer

- (IBAction)closeAction:(id)sender {

    [myTool closeInspectorPanel];
}

- (GuiTree*)getConsensusTree {

    return [myTool getConsensusTree];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolTreeConsensus*)t {

	if ( (self = [super initWithWindowNibName:@"ConTreeViewer"]) )
        {
        myTool = t;
        }
	return self;
}

- (void)update {

    [myView setNeedsDisplay:YES];
}

- (void)windowDidLoad {

    [super windowDidLoad];
    
    // initialize here
}

@end
