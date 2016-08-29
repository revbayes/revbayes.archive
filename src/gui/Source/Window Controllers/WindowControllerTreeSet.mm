#import "ToolTreeSet.h"
#import "WindowControllerTreeSet.h"



@implementation WindowControllerTreeSet

@synthesize numberOfInlets;

- (void)awakeFromNib {

    [self setButtonTitles];
}

- (IBAction)cancelAction:(id)sender {

    [self setNumberOfInlets:[myTool numberOfInlets]];
    [self setButtonTitles];
    [myTool closeControlPanel];
}

- (IBAction)changeTreeSource:(id)sender {

}

- (IBAction)changeNumberOfInlets:(id)sender {

    NSString* whichTab = [numberOfInletsSelector titleOfSelectedItem];
    if ( [whichTab isEqualToString:@"One Inlet"] == YES )
        numberOfInlets = 1;
    else if ( [whichTab isEqualToString:@"Two Inlets"] == YES )
        numberOfInlets = 2;
    else if ( [whichTab isEqualToString:@"Three Inlets"] == YES )
        numberOfInlets = 3;

    [self setButtonTitles];
}

- (IBAction)exportAction:(id)sender {

    [myTool closeControlPanel];
    [myTool exportTrees];
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
        [self setNumberOfInlets:[myTool numberOfInlets]];
        }
	return self;
}

- (IBAction)okAction:(id)sender {

    NSTabViewItem* tvi = [treeSourceTab selectedTabViewItem];
	NSString* tabViewLabel = [NSString stringWithString:[tvi label]];
	if ( [tabViewLabel isEqualToString:@"Trees from File"] == YES )
        {
        [myTool closeControlPanel];
        [myTool importTrees];
        }
    else 
        {
        [myTool closeControlPanel];
        [myTool setNumberOfInlets:numberOfInlets];
        }

}

- (void)setButtonTitles {

    if ([myTool numberOfTreesInSet] > 0)
        [exportButton setHidden:NO];
    else
        [exportButton setHidden:YES];

    NSTabViewItem* tvi = [treeSourceTab selectedTabViewItem];
	NSString* tabViewLabel = [NSString stringWithString:[tvi label]];
	if ( [tabViewLabel isEqualToString:@"Trees from File"] == YES )
        {
        [okButton setTitle:@"Import"];
        }
    else 
        {
        [numberOfInletsSelector selectItemWithTag:numberOfInlets];
        [okButton setTitle:@"OK"];
        }
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {

    [self setButtonTitles];
}

@end
