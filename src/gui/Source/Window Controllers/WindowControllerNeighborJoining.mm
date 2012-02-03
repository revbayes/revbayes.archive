#import "ToolNeighborJoining.h"
#import "WindowControllerNeighborJoining.h"



@implementation WindowControllerNeighborJoining

- (void)awakeFromNib {

}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolNeighborJoining*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowNeighborJoining"]) )
        {
        myTool = t;
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {
    
    [myTool closeControlPanel];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
