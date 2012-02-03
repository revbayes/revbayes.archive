#import "ToolBootstrap.h"
#import "WindowControllerBootstrap.h"



@implementation WindowControllerBootstrap

- (void)awakeFromNib {

}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolBootstrap*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowBootstrap"]) )
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
