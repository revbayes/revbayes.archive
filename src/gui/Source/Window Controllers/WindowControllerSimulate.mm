#import "GuiTree.h"
#import "SimulateView.h"
#import "ToolSimulate.h"
#import "WindowControllerSimulate.h"



@implementation WindowControllerSimulate

@synthesize editingTree;
@synthesize fontSize;
@synthesize treeLength;
@synthesize alpha;
@synthesize rAC;
@synthesize rAG;
@synthesize rAT;
@synthesize rCG;
@synthesize rCT;
@synthesize rGT;
@synthesize piA;
@synthesize piC;
@synthesize piG;
@synthesize piT;
@synthesize sequenceLength;

- (IBAction)addTaxonAction:(id)sender {

    [myRemoteTree addTaxonToRandomBranch];
    [simulateView setNeedsDisplay:YES];
}

- (void)awakeFromNib {    

    [self setEditingTree:NO];
    [editButton setTitle:@"Edit Tree"];
    [addTaxonButton setEnabled:NO];
    [treeLengthField setEnabled:NO];
    [treeLengthLabel setEnabled:NO];
    [treeLengthLabel setTextColor:[NSColor grayColor]];

	lastTreeWindowSize = NSZeroRect;

    [self setControlWindow];
}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (IBAction)changeBaseFrequencies:(id)sender {

    if (piA + piC + piG > 1.0)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Warning: Incorrect Base Frequencies"];
        [alert setInformativeText:@"The four base frequencies must sum to one."];
        [alert beginSheetModalForWindow:[self window] completionHandler:nil];

        [self setPiA:oldPiA];
        [self setPiC:oldPiC];
        [self setPiG:oldPiG];
        [self setPiT:oldPiT];
        }
    else
        {
        [self setPiT:(1.0 - (piA+piC+piG))];
        oldPiA = piA;
        oldPiC = piC;
        oldPiG = piG;
        oldPiT = piT;
        }
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (IBAction)editButtonAction:(id)sender {

    if ( [self editingTree] == YES )
        {
        [self setEditingTree:NO];
        [editButton setTitle:@"Edit Tree"];
        [addTaxonButton setEnabled:NO];
        [treeLengthField setEnabled:NO];
        [treeLengthLabel setEnabled:NO];
        [treeLengthLabel setTextColor:[NSColor grayColor]];
        }
    else 
        {
        [self setEditingTree:YES];
        [editButton setTitle:@"Stop Editing Tree"];
        [addTaxonButton setEnabled:YES];
        [treeLengthField setEnabled:YES];
        [treeLengthLabel setEnabled:YES];
        [treeLengthLabel setTextColor:[NSColor blackColor]];
        }
    [simulateView setNeedsDisplay:YES];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolSimulate*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowSimulate"]) )
        {
        myTool         = t;
        fontSize       = 14.0;
        treeLength     = [myTool treeLength];
        alpha          = [myTool alpha];
        rAC            = [myTool rAC];
        rAG            = [myTool rAG];
        rAT            = [myTool rAT];
        rCG            = [myTool rCG];
        rCT            = [myTool rCT];
        rGT            = [myTool rGT];
        piA            = [myTool piA];
        piC            = [myTool piC];
        piG            = [myTool piG];
        piT            = [myTool piT];
        sequenceLength = [myTool sequenceLength];
        myRemoteTree   = [myTool simTree];
        
        oldPiA         = piA;
        oldPiC         = piC;
        oldPiG         = piG;
        oldPiT         = piT;

        NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];		 
		[defaultCenter addObserver:self
						  selector:@selector(windowDidResize:)
							  name:@"NSWindowDidResizeNotification"
							object:self];
        }
	return self;
}

- (GuiTree*)myTree {

    return myRemoteTree;
}

- (IBAction)okButtonAction:(id)sender {
    
    [myTool setTreeLength:treeLength];
    [myTool setAlpha:alpha];
    [myTool setRAC:rAC];
    [myTool setRAG:rAG];
    [myTool setRAT:rAT];
    [myTool setRCG:rCG];
    [myTool setRCT:rCT];
    [myTool setPiA:piA];
    [myTool setPiC:piC];
    [myTool setPiG:piG];
    [myTool setPiT:piT];
    [myTool setSequenceLength:sequenceLength];

    [myTool closeControlPanel];
    [myTool simulate];
}

- (void)setControlWindow {

	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    
	NSString* tabViewLabel = [NSString stringWithString:[[theTabView selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Tree Model"] == YES )
		{
        [editButton      setHidden:NO];
        [addTaxonButton  setHidden:NO];
        [treeLengthLabel setHidden:NO];
        [treeLengthField setHidden:NO];

        if ( lastTreeWindowSize.size.width < 0.001 && lastTreeWindowSize.size.height < 0.001 )
            {
            lastTreeWindowSize.size.width = 675.0;
            lastTreeWindowSize.size.height = 725.0;
            }
        newFrame.size.height = lastTreeWindowSize.size.height;
        newFrame.size.width  = lastTreeWindowSize.size.width;
            
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];

        [[self window] setMinSize:NSMakeSize(600.0, 600.0)];
        [[self window] setMaxSize:NSMakeSize(10000.0, 10000.0)];
        [[[self window] standardWindowButton:NSWindowZoomButton] setHidden:NO];
        [[self window] setShowsResizeIndicator:YES];
		}
	else if ( [tabViewLabel isEqualToString:@"Substitution Model"] == YES )
		{
        [editButton      setHidden:YES];
        [addTaxonButton  setHidden:YES];
        [treeLengthLabel setHidden:YES];
        [treeLengthField setHidden:YES];

        newFrame.size.height = 390.0;
        newFrame.size.width  = 600.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];

        [[self window] setMinSize:[[self window] frame].size];
        [[self window] setMaxSize:[[self window] frame].size];
        [[[self window] standardWindowButton:NSWindowZoomButton] setHidden:YES];
        [[self window] setShowsResizeIndicator:NO];
		}
}

- (void)setMyTree:(GuiTree*)t {

    myRemoteTree = t;
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {

	[self setControlWindow];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

- (void)windowDidResize:(NSNotification*)notification {
    
    if ( [notification object] == [self window] )
        {
        NSString* tabViewLabel = [NSString stringWithString:[[theTabView selectedTabViewItem] label]];
        if ( [tabViewLabel isEqualToString:@"Tree Model"] == YES )
            lastTreeWindowSize = [[self window] frame];
        }
}

@end
