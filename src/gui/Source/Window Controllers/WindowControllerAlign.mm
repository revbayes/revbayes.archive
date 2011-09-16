#import "ToolAlign.h"
#import "WindowControllerAlign.h"



@implementation WindowControllerAlign

@synthesize testGapPenalty;
@synthesize gapPenalty;
@synthesize wordLength;
@synthesize windowVal;



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

        gapPenalty = 1.1;
        wordLength = 2.2;
        windowVal  = 3.3;

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
