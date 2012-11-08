#import "GuiTree.h"
#import "Node.h"
#import "ToolTreeSet.h"
#import "TreeSetView.h"
#import "WindowControllerTreeViewer.h"




@implementation WindowControllerTreeViewer

@synthesize selectedTree;
@synthesize fontSize;

- (GuiTree*)activeTree {

    return [myTool getTreeIndexed:selectedTree];
}

- (void)awakeFromNib {

    [self initializeTreeInformation];
}

- (IBAction)changeFontSize:(id)sender {

    [treeView setNeedsDisplay:YES];
}

- (IBAction)changeOutgroup:(id)sender {

    int n = [myTool numberOfTreesInSet];
    if (n > 0)
        {
        // get the name of the desired outgroup name
        NSString* outgroupName = [[outgroupList selectedItem] title];
        NSLog(@"outgroupName=%@", outgroupName);
        
        for (int i=0; i<n; i++)
            {
            // get the tree
            GuiTree* t = [myTool getTreeIndexed:i];
            
            // find the node in the tree with the outgroup name
            Node* outgroupNode = nil;
            for (int j=0; j<[t numberOfNodes]; j++)
                {
                Node* p = [t downPassNodeIndexed:j];
                NSString* nodeName = [p name];
                if ( [nodeName isEqualToString:outgroupName] == YES )
                    {
                    outgroupNode = p;
                    break;
                    }
                }
                
            // reroot the tree
            if (outgroupNode != nil)
                {
                [t rootTreeOnNode:outgroupNode];
                }
            }
        }

    [treeView setNeedsDisplay:YES];
}

- (IBAction)closeButtonAction:(id)sender {

    [myTool closeInspectorPanel];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolTreeSet*)t {

	if ( (self = [super initWithWindowNibName:@"TreeViewer"]) )
        {
        myTool       = t;
        selectedTree = 0;
        fontSize     = 14.0;
        }
	return self;
}

- (void)initializeTreeInformation {

    int n = [myTool numberOfTreesInSet];
    if (n == 0)
        {
        [[self window] setTitle:@"Tree Set (Contains No Trees)"];
        [treeCounter setStringValue:@"Tree: xxx"];
        [treeCounter setHidden:YES];
        [treeStepper setHidden:YES];
        [fontLabel setHidden:YES];
        [fontEntry setHidden:YES];
        [self populateOutgroupList];
        }
    else
        {
        if (n == 1)
            [[self window] setTitle:@"Tree Set (Contains One Tree)"];
        else
            [[self window] setTitle:[NSString stringWithFormat:@"Tree Set (Contains %d Trees)", n]];
        [treeCounter setStringValue:[NSString stringWithFormat:@"Tree: %d", 1]];
        [fontLabel setHidden:NO];
        [fontEntry setHidden:NO];
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
        [treeStepper setMinValue:0];
        [treeStepper setMaxValue:(n-1)];
        [treeStepper setIncrement:1];
        [self populateOutgroupList];
        }
}

- (void)populateOutgroupList {

    [outgroupList removeAllItems];
    int n = [myTool numberOfTreesInSet];
    if (n > 0)
        {
        GuiTree* t = [myTool getTreeIndexed:0];
        [t initializeDownPassSequence];
        if (t != nil)
            {
            for (int i=0; i<[t numberOfNodes]; i++)
                {
                Node* p = [t downPassNodeIndexed:i];
                if ([p numberOfDescendants] == 0)
                    [outgroupList addItemWithTitle:[p name]];
                }
            }
        }
}

- (IBAction)stepAction:(id)sender {

    [self setSelectedTree:([treeStepper intValue])];
    [treeCounter setStringValue:[NSString stringWithFormat:@"Tree: %d", selectedTree+1]];
    [treeView setNeedsDisplay:YES];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
