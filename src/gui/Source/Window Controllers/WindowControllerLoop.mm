#import "ToolLoop.h"
#import "WindowControllerLoop.h"



@implementation WindowControllerLoop

@synthesize index;
@synthesize upperLimit;

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
        [self setIndex:@"i"];
        [self setUpperLimit:1];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {
    
    char c = [index characterAtIndex:0];
    [myTool setIndexLetter:c];
    [myTool setIndexUpperLimit:upperLimit];
    
    [myTool closeControlPanel];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
