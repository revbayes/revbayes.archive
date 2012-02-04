#import "ToolLoop.h"
#import "WindowControllerLoop.h"



@implementation WindowControllerLoop

- (void)awakeFromNib {

}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolLoop*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowLoop"]) )
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
