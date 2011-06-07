#import "ToolAlign.h"
#import "WindowControllerAlign.h"



@implementation WindowControllerAlign

@synthesize testGapPenalty;

- (void)awakeFromNib {

}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolAlign*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowAlign"]) )
        {
        myTool        = t;
		
		// set default values for the window
		[self setTestGapPenalty:5.0];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (void)windowDidLoad {

}

@end
