#import "ToolSimulate.h"
#import "WindowControllerSimulateQuery.h"



@implementation WindowControllerSimulateQuery

@synthesize numTaxa;
@synthesize lambda;
@synthesize mu;
@synthesize theta;
@synthesize whichTreeSourceMethod;
@synthesize whichTreeGenerationMethod;
@synthesize closedWithCancel;

- (void)awakeFromNib {

    // set the radio buttons for the stochastic tree generation method
    for (int i=0; i<4; i++)
        {
        NSButtonCell* c = [treeType cellAtRow:i column:0];
        [c setTag:i];
        if (i > 0)
            [c setEnabled:NO];
        }
    
    // see if there is a tree in the tool alread
    GuiTree* t = [myTool simTree];
    if (t == nil)
        {
        [treeGenSelector removeItemWithTitle:@"Use Current Tree in Tool"];
        [treeGenSelector selectItemWithTitle:@"Generate Tree Under Stochastic Process"];
        whichTreeSourceMethod = TREE_FROM_SIM;
        }
    else
        {
        [treeGenSelector selectItemWithTitle:@"Use Current Tree in Tool"];
        whichTreeSourceMethod = TREE_FROM_TOOL;
        }

    [self setWindowSize];
}

- (IBAction)cancelButtonAction:(id)sender {

    closedWithCancel = YES;
    [myTool closeQueryPanel];
}

- (IBAction)changeTreeGenerationMethod:(id)sender {

    whichTreeGenerationMethod = (int)[[treeType selectedCell] tag];
    [self setWindowSize];
}

- (IBAction)changeTreeSourceMethod:(id)sender {

	NSString* popUpLabel = [NSString stringWithString:[treeGenSelector titleOfSelectedItem]];
	if ( [popUpLabel isEqualToString:@"Read Tree From File"] == YES )
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Unavailable Tree Source Method"];
        [alert setInformativeText:@"The ability to read trees from a file is not yet supported."];
        [alert beginSheetModalForWindow:[self window] completionHandler:nil];
        [treeGenSelector selectItemWithTitle:@"Generate Tree Under Stochastic Process"];
        whichTreeSourceMethod = TREE_FROM_SIM;
        }
	else if ( [popUpLabel isEqualToString:@"Use Current Tree in Tool"] == YES )
        {
        whichTreeSourceMethod = TREE_FROM_TOOL;
        }
    else
        {
        whichTreeSourceMethod = TREE_FROM_SIM;
        }

    [self setWindowSize];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
    [aCoder encodeInt:numTaxa                   forKey:@"numTaxa"];
    [aCoder encodeDouble:lambda                 forKey:@"lambda"];
    [aCoder encodeDouble:mu                     forKey:@"mu"];
    [aCoder encodeDouble:theta                  forKey:@"theta"];
    [aCoder encodeInt:whichTreeGenerationMethod forKey:@"whichTreeGenerationMethod"];
}

- (id)init {

    self = [self initWithTool:nil];
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
        numTaxa                   = [aDecoder decodeIntForKey:@"numTaxa"];
        lambda                    = [aDecoder decodeDoubleForKey:@"lambda"];
        mu                        = [aDecoder decodeDoubleForKey:@"mu"];
        theta                     = [aDecoder decodeDoubleForKey:@"theta"];
        whichTreeGenerationMethod = [aDecoder decodeIntForKey:@"whichTreeGenerationMethod"];
		}
	return self;
}

- (id)initWithTool:(ToolSimulate*)t {

	if ( (self = [super initWithWindowNibName:@"SimulateQuery"]) )
        {
        myTool                    = t;
        numTaxa                   = 10;
        lambda                    = 2.0;
        mu                        = 1.0;
        theta                     = 1.0;
        whichTreeGenerationMethod = 0;
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    closedWithCancel = NO;
    [myTool closeQueryPanel];
}

- (void)resizeWindowTo:(float)h {

	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    newFrame.size.height = h;
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

- (void)setWindowSize {

	NSString* popUpLabel = [NSString stringWithString:[treeGenSelector titleOfSelectedItem]];
	if ( [popUpLabel isEqualToString:@"Generate Tree Under Stochastic Process"] == YES )
        {
        [newickFinderButton setHidden:YES];
        [treeType           setHidden:NO];
        [numTaxaField       setHidden:NO];
        [numTaxaLabelField  setHidden:NO];
        [parm1Field         setHidden:NO];
        [parm2Field         setHidden:NO];
        [parm1LabelField    setHidden:NO];
        [parm2LabelField    setHidden:NO];
        
        if (whichTreeGenerationMethod == 0)
            {
            [parm1Field setHidden:YES];
            [parm2Field setHidden:YES];
            [parm3Field setHidden:YES];
            [parm1LabelField setHidden:YES];
            [parm2LabelField setHidden:YES];
            [parm3LabelField setHidden:YES];
            [self resizeWindowTo:255.0];
            }
        else if (whichTreeGenerationMethod == 1)
            {
            [parm1Field setHidden:YES];
            [parm2Field setHidden:YES];
            [parm3Field setHidden:YES];
            [parm1LabelField setHidden:YES];
            [parm2LabelField setHidden:YES];
            [parm3LabelField setHidden:YES];
            [self resizeWindowTo:255.0];
            }
        else if (whichTreeGenerationMethod == 2)
            {
            [parm1Field setHidden:YES];
            [parm2Field setHidden:YES];
            [parm3Field setHidden:NO];
            [parm1LabelField setHidden:YES];
            [parm2LabelField setHidden:YES];
            [parm3LabelField setHidden:NO];
            [parm3LabelField setStringValue:[NSString stringWithFormat:@"%C:", (short int)0x03B8]];
            [parm3LabelField setToolTip:@"Coalescence parameter (4Nu)"];
            NSRect r = [parm3Field frame];
            r.origin.y = 10.0;
            [parm3Field setFrame:r];
            r = [parm3LabelField frame];
            r.origin.y = 10.0;
            [parm3LabelField setFrame:r];
            [self resizeWindowTo:280.0];
            }
        else if (whichTreeGenerationMethod == 3)
            {
            [parm1Field setHidden:NO];
            [parm2Field setHidden:NO];
            [parm3Field setHidden:YES];
            [parm1LabelField setHidden:NO];
            [parm2LabelField setHidden:NO];
            [parm3LabelField setHidden:YES];
            [parm1LabelField setStringValue:[NSString stringWithFormat:@"%C:", (short int)0x03BB]];
            [parm2LabelField setStringValue:[NSString stringWithFormat:@"%C:", (short int)0x03BC]];
            [parm1LabelField setToolTip:@"Speciation rate"];
            [parm2LabelField setToolTip:@"Extinction rate"];
            [self resizeWindowTo:310.0];
            }
        }
	else if ( [popUpLabel isEqualToString:@"Read Tree From File"] == YES )
        {
        [newickFinderButton setHidden:NO];
        [treeType           setHidden:YES];
        [numTaxaField       setHidden:YES];
        [numTaxaLabelField  setHidden:YES];
        [parm1Field         setHidden:YES];
        [parm2Field         setHidden:YES];
        [parm3Field         setHidden:YES];
        [parm1LabelField    setHidden:YES];
        [parm2LabelField    setHidden:YES];
        [parm3LabelField    setHidden:YES];
        [self resizeWindowTo:165.0];
        }
    else
        {
        [newickFinderButton setHidden:YES];
        [treeType           setHidden:YES];
        [numTaxaField       setHidden:YES];
        [numTaxaLabelField  setHidden:YES];
        [parm1Field         setHidden:YES];
        [parm2Field         setHidden:YES];
        [parm3Field         setHidden:YES];
        [parm1LabelField    setHidden:YES];
        [parm2LabelField    setHidden:YES];
        [parm3LabelField    setHidden:YES];
        [self resizeWindowTo:115.0];
        }
}

- (void)windowDidLoad {

    [super windowDidLoad];

}

@end
