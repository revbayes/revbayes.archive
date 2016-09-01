#import "ToolParsimony.h"
#import "WindowControllerParsimony.h"

#define HEURISTIC_HEIGHT    460.0
#define BANDB_HEIGHT        195.0
#define EXHAUSTIVE_HEIGHT   165.0

@implementation WindowControllerParsimony

@synthesize searchMethod;
@synthesize hsSwap;
@synthesize hsKeep;
@synthesize hsMulTrees;
@synthesize hsRearrLimit;
@synthesize hsReconLimit;
@synthesize hsNBest;
@synthesize hsRetain;
@synthesize hsAllSwap;
@synthesize hsUseNonMin;
@synthesize hsSteepest;
@synthesize hsNChuck;
@synthesize hsChuckScore;
@synthesize hsAbortRep;
@synthesize hsRandomize;
@synthesize hsAddSeq;
@synthesize hsNReps;
@synthesize hsHold;
@synthesize bbKeep;
@synthesize bbMulTrees;
@synthesize bbUpBound;
@synthesize bbAddSeq;
@synthesize exKeep;

- (void)awakeFromNib {


    [self resizeSearchWindow];

    [hsKeepSelector removeAllItems];
    [hsKeepSelector addItemWithObjectValue:@"No"];

    [hsRearrLimitSelector removeAllItems];
    [hsRearrLimitSelector addItemWithObjectValue:@"None"];
    
    [hsReconLimitSelector removeAllItems];
    [hsReconLimitSelector addItemWithObjectValue:@"Infinity"];
    
    [hsNBestSelector removeAllItems];
    [hsNBestSelector addItemWithObjectValue:@"All"];
    
    [hsChuckScoreSelector removeAllItems];
    [hsChuckScoreSelector addItemWithObjectValue:@"No"];
    
    [hsHoldSelector removeAllItems];
    [hsHoldSelector addItemWithObjectValue:@"No"];
    
    [bbKeepSelector removeAllItems];
    [bbKeepSelector addItemWithObjectValue:@"No"];
    
    [exKeepSelector removeAllItems];
    [exKeepSelector addItemWithObjectValue:@"No"];
}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanelWithCancel];
}

- (void)comboBoxWillDismiss:(NSNotification*)notification {

    NSComboBox* theComboBox = (NSComboBox*)[notification object];
    
    NSArray* possibleVals = nil;
    if (theComboBox == hsKeepSelector)
        possibleVals = [NSArray arrayWithObjects:@"No", @"<real number>", nil];
    else if (theComboBox == hsRearrLimitSelector)
        possibleVals = [NSArray arrayWithObjects:@"None", @"<integer value>", nil];
    else if (theComboBox == hsReconLimitSelector)
        possibleVals = [NSArray arrayWithObjects:@"Infinity", @"<integer value>", nil];
    else if (theComboBox == hsNBestSelector)
        possibleVals = [NSArray arrayWithObjects:@"All", @"<integer value>", nil];
    else if (theComboBox == hsChuckScoreSelector)
        possibleVals = [NSArray arrayWithObjects:@"No", @"<real number>", nil];
    else if (theComboBox == hsHoldSelector)
        possibleVals = [NSArray arrayWithObjects:@"No", @"<integer value>", nil];
    else if (theComboBox == bbKeepSelector)
        possibleVals = [NSArray arrayWithObjects:@"No", @"<real number>", nil];
    else if (theComboBox == exKeepSelector)
        possibleVals = [NSArray arrayWithObjects:@"No", @"<real number>", nil];
    
    if (possibleVals != nil)
        {
        BOOL isValidEntry = [self checkEntry:[theComboBox stringValue] forValidPossibilities:possibleVals];
        if ( isValidEntry == NO )
            {
            // format and display the error window
            NSString* errStr = @"Expect: ";
            for (size_t i=0; i<[possibleVals count]; i++)
                {
                if (i > 0)
                    {
                    if (i == [possibleVals count]-1 && [possibleVals count] == 2)
                        errStr = [errStr stringByAppendingString:@" or "];
                    else if (i == [possibleVals count]-1 && [possibleVals count] > 2)
                        errStr = [errStr stringByAppendingString:@", or "];
                    else
                        errStr = [errStr stringByAppendingString:@", "];
                    }
                if ( [(NSString*)([possibleVals objectAtIndex:i]) isEqualToString:@"<real number>" ] ||
                     [(NSString*)([possibleVals objectAtIndex:i]) isEqualToString:@"<integer value>"] )
                    errStr = [errStr stringByAppendingString:(NSString*)([possibleVals objectAtIndex:i])];
                else
                    errStr = [errStr stringByAppendingFormat:@"\"%@\"", (NSString*)([possibleVals objectAtIndex:i])];
                }
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Invalid Entry for Search Parameter"];
            [alert setInformativeText:errStr];
            [alert beginSheetModalForWindow:[self window] completionHandler:nil];
            
            // restore the value
            if (theComboBox == hsKeepSelector)
                [self setHsKeep:old_hsKeep];
            else if (theComboBox == hsRearrLimitSelector)
                [self setHsRearrLimit:old_hsRearrLimit];
            else if (theComboBox == hsReconLimitSelector)
                [self setHsReconLimit:old_hsReconLimit];
            else if (theComboBox == hsNBestSelector)
                [self setHsNBest:old_hsNBest];
            else if (theComboBox == hsChuckScoreSelector)
                [self setHsChuckScore:old_hsChuckScore];
            else if (theComboBox == hsHoldSelector)
                [self setHsHold:old_hsHold];
            else if (theComboBox == bbKeepSelector)
                [self setBbKeep:old_bbKeep];
            else if (theComboBox == exKeepSelector)
                [self setExKeep:old_exKeep];
            }
        else
            {
            old_hsKeep       = hsKeep;
            old_hsRearrLimit = hsRearrLimit;
            old_hsReconLimit = hsReconLimit;
            old_hsNBest      = hsNBest;
            old_hsChuckScore = hsChuckScore;
            old_hsHold       = hsHold;
            old_bbKeep       = bbKeep;
            old_exKeep       = exKeep;
            }
        }
}

- (BOOL)checkEntry:(NSString*)val forValidPossibilities:(NSArray*)possibilities {

    BOOL isValid = NO;
    for (size_t i=0; i<[possibilities count]; i++)
        {
        NSString* v = (NSString*)[possibilities objectAtIndex:i];
        if ( [v isEqualToString:@"<real number>"] == YES && [self isReal:val] == YES)
            {
            isValid = YES;
            break;
            }
        else if ( [v isEqualToString:@"<integer value>"] == YES && [self isInteger:val] == YES)
            {
            isValid = YES;
            break;
            }
        else if ( [val isEqualToString:v] == YES )
            {
            isValid = YES;
            break;
            }
        }
    return isValid;
}

- (IBAction)helpButtonAction:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"ParsimonyTool_Anchor" inBook:locBookName];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolParsimony*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowParsimony"]) )
        {
        // initialize this window with its tool
        myTool = t;

        // listen for changes in combo boxes (actually, listen for changes in text fields)
        NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
        [defaultCenter addObserver:self
                          selector:@selector(comboBoxWillDismiss:)
                              name:NSControlTextDidEndEditingNotification
                            object:nil];
            
        // initialize values for interface elements from tool
        [self setSearchMethod: [myTool searchMethod]];
        [self setHsSwap:       [myTool hsSwap]];
        [self setHsKeep:       [myTool hsKeep]];
        [self setHsMulTrees:   [myTool hsMulTrees]];
        [self setHsRearrLimit: [myTool hsRearrLimit]];
        [self setHsReconLimit: [myTool hsReconLimit]];
        [self setHsNBest:      [myTool hsNBest]];
        [self setHsRetain:     [myTool hsRetain]];
        [self setHsAllSwap:    [myTool hsAllSwap]];
        [self setHsUseNonMin:  [myTool hsUseNonMin]];
        [self setHsSteepest:   [myTool hsSteepest]];
        [self setHsNChuck:     [myTool hsNChuck]];
        [self setHsChuckScore: [myTool hsChuckScore]];
        [self setHsAbortRep:   [myTool hsAbortRep]];
        [self setHsRandomize:  [myTool hsRandomize]];
        [self setHsAddSeq:     [myTool hsAddSeq]];
        [self setHsNReps:      [myTool hsNReps]];
        [self setHsHold:       [myTool hsHold]];
        [self setBbKeep:       [myTool bbKeep]];
        [self setBbMulTrees:   [myTool bbMulTrees]];
        [self setBbUpBound:    [myTool bbUpBound]];
        [self setBbAddSeq:     [myTool bbAddSeq]];
        [self setExKeep:       [myTool exKeep]];

        old_hsKeep        = hsKeep;
        old_hsRearrLimit  = hsRearrLimit;
        old_hsReconLimit  = hsReconLimit;
        old_hsNBest       = hsNBest;
        old_hsChuckScore  = hsChuckScore;
        old_hsHold        = hsHold;
        old_bbKeep        = bbKeep;
        old_exKeep        = exKeep;
        }
	return self;
}

- (BOOL)isReal:(NSString*)v {

    NSNumberFormatter* f = [[NSNumberFormatter alloc] init];
    [f setAllowsFloats:YES];
    if ( [f numberFromString:v] == nil )
        return NO;
    return YES;
}

- (BOOL)isInteger:(NSString*)v {

    NSNumberFormatter* f = [[NSNumberFormatter alloc] init];
    [f setAllowsFloats:NO];
    if ( [f numberFromString:v] == nil )
        return NO;
    return YES;
}

- (IBAction)okButtonAction:(id)sender {
    
	NSString* methodLabel = [NSString stringWithString:[[searchMethodSelectorTab selectedTabViewItem] label]];
    if ( [methodLabel isEqualToString:@"Exhaustive"] == YES )
        searchMethod = EXHAUSTIVE;
    else if ( [methodLabel isEqualToString:@"Branch & Bound"] == YES )
        searchMethod = BANDB;
    else
        searchMethod = HEURISTIC;
    [myTool setSearchMethod:searchMethod];
    
    [myTool setHsSwap:       [self hsSwap]];
    [myTool setHsKeep:       [self hsKeep]];
    [myTool setHsMulTrees:   [self hsMulTrees]];
    [myTool setHsRearrLimit: [self hsRearrLimit]];
    [myTool setHsReconLimit: [self hsReconLimit]];
    [myTool setHsNBest:      [self hsNBest]];
    [myTool setHsRetain:     [self hsRetain]];
    [myTool setHsAllSwap:    [self hsAllSwap]];
    [myTool setHsUseNonMin:  [self hsUseNonMin]];
    [myTool setHsSteepest:   [self hsSteepest]];
    [myTool setHsNChuck:     [self hsNChuck]];
    [myTool setHsChuckScore: [self hsChuckScore]];
    [myTool setHsAbortRep:   [self hsAbortRep]];
    [myTool setHsRandomize:  [self hsRandomize]];
    [myTool setHsAddSeq:     [self hsAddSeq]];
    [myTool setHsNReps:      [self hsNReps]];
    [myTool setHsHold:       [self hsHold]];
    [myTool setBbKeep:       [self bbKeep]];
    [myTool setBbMulTrees:   [self bbMulTrees]];
    [myTool setBbUpBound:    [self bbUpBound]];
    [myTool setBbAddSeq:     [self bbAddSeq]];
    [myTool setExKeep:       [self exKeep]];
    
    [myTool closeControlPanelWithOK];
}

- (void)resizeSearchWindow {

	NSString* methodLabel = [NSString stringWithString:[[searchMethodSelectorTab selectedTabViewItem] label]];

    float h = 0.0;
	if ( [methodLabel isEqualToString:@"Exhaustive"] == YES )
        h = EXHAUSTIVE_HEIGHT;
    else if ( [methodLabel isEqualToString:@"Branch & Bound"] == YES)
        h = BANDB_HEIGHT;
    else if ( [methodLabel isEqualToString:@"Heuristic"] == YES)
        h = HEURISTIC_HEIGHT;
    
    [self resizeSearchWindowToHeight:h];
}

- (void)resizeSearchWindowToHeight:(float)h {

    NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    newFrame.size.height = h;
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

- (BOOL)tabView:(NSTabView*)tabView shouldSelectTabViewItem:(NSTabViewItem*)tabViewItem {

    NSLog(@"resizing");
    [self resizeSearchWindow];

	NSString* methodLabel = [NSString stringWithString:[tabViewItem label]];
	if ( [methodLabel isEqualToString:@"Exhaustive"] == YES )
        {
        searchMethod = EXHAUSTIVE;
        [self resizeSearchWindowToHeight:EXHAUSTIVE_HEIGHT];
        return YES;
        }
    else if ( [methodLabel isEqualToString:@"Branch & Bound"] == YES)
        {
        searchMethod = BANDB;
        [self resizeSearchWindowToHeight:BANDB_HEIGHT];
        return YES;
        }
    else if ( [methodLabel isEqualToString:@"Heuristic"] == YES)
        {
        searchMethod = HEURISTIC;
        [self resizeSearchWindowToHeight:HEURISTIC_HEIGHT];
        return YES;
        }
    
    return NO;
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
