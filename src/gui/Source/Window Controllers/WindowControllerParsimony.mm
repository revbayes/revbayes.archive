#import "ToolParsimony.h"
#import "WindowControllerParsimony.h"



@implementation WindowControllerParsimony

- (void)awakeFromNib {

}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolParsimony*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowParsimony"]) )
        {
        myTool = t;
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {
    
    [myTool closeControlPanel];
    [myTool exhaustiveSearch];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
