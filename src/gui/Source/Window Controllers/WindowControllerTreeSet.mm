#import "ToolTreeSet.h"
#import "WindowControllerTreeSet.h"




@implementation WindowControllerTreeSet

@synthesize selectedTree;

- (Tree*)activeTree {

    return [myTool getTreeIndexed:selectedTree];
}

- (void)awakeFromNib {

    int n = [myTool numberOfTreesInSet];
    if (n == 0)
        {
        [[self window] setTitle:[NSString stringWithString:@"Tree Set (Contains No Trees)"]];
        [treeCounter setStringValue:@"Tree: xxx"];
        [treeCounter setHidden:YES];
        [treeStepper setHidden:YES];
        }
    else
        {
        [[self window] setTitle:[NSString stringWithFormat:@"Tree Set (Contains %d Trees)", n]];
        [treeCounter setStringValue:[NSString stringWithFormat:@"Tree: %d", n]];
        if (n == 1)
            {
            [treeCounter setHidden:YES];
            [treeStepper setHidden:YES];
            }
        else 
            {
            [treeCounter setHidden:NO];
            [treeStepper setHidden:NO];
            }
        }
}

- (IBAction)closeButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolTreeSet*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowTreeSet"]) )
        {
        myTool = t;
        selectedTree = 0;
        }
	return self;
}

- (IBAction)stepAction:(id)sender {

}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
