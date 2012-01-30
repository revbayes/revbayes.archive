#import "ToolDistanceMatrix.h"
#import "WindowControllerDistanceViewer.h"




@implementation WindowControllerDistanceViewer

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolDistanceMatrix*)t {

	if ( (self = [super initWithWindowNibName:@"DistanceMatrix"]) )
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
