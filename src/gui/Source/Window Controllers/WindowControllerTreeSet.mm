#import "ToolTreeSet.h"
#import "WindowControllerTreeSet.h"



@implementation WindowControllerTreeSet

- (IBAction)cancelAction:(id)sender {

    [myTool closeControlPanel];
}

- (IBAction)changeTreeSource:(id)sender {

}

- (IBAction)changeNumberOfInlets:(id)sender {

}

- (IBAction)exportAction:(id)sender {

    [myTool closeControlPanel];
}

- (IBAction)helpAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolTreeSet*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowTreeSet"]) )
        {
        myTool = t;
        }
	return self;
}

- (IBAction)okAction:(id)sender {

    [myTool closeControlPanel];
}

@end
