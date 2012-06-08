#import "ToolSimulate.h"
#import "WindowControllerSimulate.h"



@implementation WindowControllerSimulate

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
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
