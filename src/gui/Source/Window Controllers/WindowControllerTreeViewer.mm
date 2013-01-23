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

- (IBAction)changedDrawMonophyleticTreeCheck:(id)sender {

    if ([drawMonophyleticTreeCheck state] == NSOnState)
        [drawMonophyleticTreeCheck setTitle:@"Yes"];
    else
        [drawMonophyleticTreeCheck setTitle:@"No"];
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

- (IBAction)incrementFontSize:(id)sender {

    int x = [fontStepper intValue];
    [self setFontSize:(float)x];
    [treeView setNeedsDisplay:YES];
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

    selectedTree = 0;
    int n = [myTool numberOfTreesInSet];
    if (n == 0)
        {
        [[self window] setTitle:@"Tree Set (Contains No Trees)"];
        [treeCounter setStringValue:@"N/A"];
        [treeCounter setHidden:YES];
        [fontLabel setHidden:YES];
        [fontEntry setHidden:YES];
        [self populateOutgroupList];
        [leftTree setEnabled:NO];
        [rightTree setEnabled:NO];
        }
    else
        {
        if (n == 1)
            [[self window] setTitle:@"Tree Set (Contains One Tree)"];
        else
            [[self window] setTitle:[NSString stringWithFormat:@"Tree Set (Contains %d Trees)", n]];
        [treeCounter setStringValue:[NSString stringWithFormat:@"%d", 1]];
        [fontLabel setHidden:NO];
        [fontEntry setHidden:NO];
        if (n == 1)
            {
            [treeCounter setHidden:YES];
            [leftTree setEnabled:NO];
            [rightTree setEnabled:NO];
            [leftTree setHidden:YES];
            [rightTree setHidden:YES];
            [treeStepLabel setHidden:YES];
            }
        else 
            {
            [treeCounter setHidden:NO];
            [leftTree setEnabled:NO];
            [rightTree setEnabled:YES];
            [leftTree setHidden:NO];
            [rightTree setHidden:NO];
            [treeStepLabel setHidden:NO];
            }
        [self populateOutgroupList];
        }
}

- (IBAction)leftTreeAction:(id)sender {

    [self setSelectedTree:(selectedTree-1)];
    [treeCounter setStringValue:[NSString stringWithFormat:@"%d", selectedTree+1]];
    [treeView setNeedsDisplay:YES];
    if (selectedTree == 0)
        [leftTree setEnabled:NO];
    else
        [leftTree setEnabled:YES];
    if (selectedTree+1 >= [myTool numberOfTreesInSet])
        [rightTree setEnabled:NO];
    else
        [rightTree setEnabled:YES];
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

- (IBAction)rightTreeAction:(id)sender {

    [self setSelectedTree:(selectedTree+1)];
    [treeCounter setStringValue:[NSString stringWithFormat:@"%d", selectedTree+1]];
    [treeView setNeedsDisplay:YES];
    if (selectedTree == 0)
        [leftTree setEnabled:NO];
    else
        [leftTree setEnabled:YES];
    if (selectedTree+1 >= [myTool numberOfTreesInSet])
        [rightTree setEnabled:NO];
    else
        [rightTree setEnabled:YES];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
