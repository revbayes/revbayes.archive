#import "AnalysisDocument.h"
#import "AnalysisSplitViewDelegate.h"
#import "AnalysisTools.h"
#import "AnalysisView.h"
#import "ExecuteTree.h"
#import "Tool.h"
#import "ToolLoop.h"
#import "ToolTreeSet.h"
#import "WindowControllerProgressBar.h"

#define LEFT_VIEW_INDEX 0
#define LEFT_VIEW_PRIORITY 2
#define LEFT_VIEW_MINIMUM_WIDTH 0.0
#define RIGHT_VIEW_INDEX 1
#define RIGHT_VIEW_PRIORITY 0
#define RIGHT_VIEW_MINIMUM_WIDTH 130.0



@implementation AnalysisDocument

@synthesize analysisViewPtr;
@synthesize isRbTimerActive;

- (IBAction)addAnalysis:(id)sender {

	NSString* uniqueName = [self findUniqueNameForAnalysis];
    AnalysisTools* t = [[AnalysisTools alloc] init];
    [t setAnalysisName:uniqueName];
    [analysesController addObject:t];
    [self updateChangeCount:NSChangeDone];
}

- (void)analysisError:(Tool*)badTool {

    NSString* alertStr = [NSString stringWithFormat:@"Problem with \"%@\" tool", [badTool toolName]];
    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Warning: Analysis prematurely quit"];
    [alert setInformativeText:alertStr];
    [alert beginSheetModalForWindow:[analysisViewPtr window] completionHandler:nil];
}

- (void)awakeFromNib {

    [executeButton setTitle:@"Execute"];
    [statusField setHidden:YES];

	// set the behavior of the analysis tool view and tool kit view
	[analysisViewPtr setAnalysisDocumentPtr:self];
	[analysisViewPtr setSnapToGrid:snapToGrid];
	[analysisViewPtr setShowGrid:showGrid];
	[[toolScrollView horizontalScroller] setControlSize:NSSmallControlSize];

	// set up the analyses controller object
    [analysesController setAvoidsEmptySelection:YES];
	NSArray* so = [NSArray arrayWithObject:[analyses objectAtIndex:0]];
	[analysesController setSelectedObjects:so];
	[self toolSourceChanged:nil];

	// set up the split view delegate (to take care of view resizing properly)
	splitViewDelegate = [[AnalysisSplitViewDelegate alloc] init];
	[splitViewDelegate
		setPriority:LEFT_VIEW_PRIORITY
		forViewAtIndex:LEFT_VIEW_INDEX];
	[splitViewDelegate
		setMinimumLength:LEFT_VIEW_MINIMUM_WIDTH
		forViewAtIndex:LEFT_VIEW_INDEX];
	[splitViewDelegate
		setPriority:RIGHT_VIEW_PRIORITY
		forViewAtIndex:RIGHT_VIEW_INDEX];
	[splitViewDelegate
		setMinimumLength:RIGHT_VIEW_MINIMUM_WIDTH
		forViewAtIndex:RIGHT_VIEW_INDEX];
	[splitView setDelegate:splitViewDelegate];
}

- (void)canCloseDocumentWithDelegate:(id)delegate shouldCloseSelector:(SEL)shouldCloseSelector contextInfo:(void *)contextInfo {

    [super canCloseDocumentWithDelegate:delegate shouldCloseSelector:shouldCloseSelector contextInfo:contextInfo];
}

- (NSMutableDictionary*)checkAnalysisForErrors {

    // look for tools with an unresolved state or who are unconnected
    NSMutableDictionary* errors = [NSMutableDictionary dictionary];
    for (Tool* t in tools)
        {
        if ( [t checkForExecute:errors] == NO )
            [t setFlagCount:1];
        }
    return errors;
}

- (NSMutableDictionary*)checkAnalysisForWarnings {

    // look for tools with an unresolved state or who are unconnected
    NSMutableDictionary* warnings = [NSMutableDictionary dictionary];
    for (Tool* t in tools)
        {
        [t checkForWarning:warnings];
        }
    return warnings;
}

- (IBAction)copyAnalysis:(id)sender {

	NSArray* selectionArray = [analysesController selectedObjects];
	if ( [selectionArray count] > 0 )
		{
		// convert the selected analysis into a NSData object
		AnalysisTools* analysisTool = [selectionArray objectAtIndex:0];
		NSData* analysisData = [NSKeyedArchiver archivedDataWithRootObject:analysisTool];
		
		// create a new analysis from the data object
		AnalysisTools* newTool = [NSKeyedUnarchiver unarchiveObjectWithData:analysisData];
		
		// size the tools in the analysis based on the current scale factor of the analsyis view
		float s = [analysisViewPtr scaleFactor];
		[newTool scaleToolsByScale:s andFactor:s];
		
		// rename the tool (append a "copy" to the previous name)
		NSString* tName = [newTool analysisName];
		tName = [tName stringByAppendingString:@" copy"];
		[newTool setAnalysisName:tName];
		
		// add the tool to the array controller (that controls the mutable array holding all of the analyses)
		[analysesController addObject:newTool];
		[self updateChangeCount:NSChangeDone];
		}
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NSTableViewSelectionDidChangeNotification" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"badAnalysis"                               object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"RbStatusBarMessage"                        object:nil];
}

- (IBAction)executeButton:(id)sender {

    if ( [[executeButton title] isEqualToString:@"Cancel"] == YES )
        {
        // cancel then change the title back to execute
        [analysisViewPtr setCancelAnalysis:YES];
        }
    else
        {
        // execute the analysis specified by the user
        
        [analysisViewPtr setCancelAnalysis:NO];
        
        // check for errors in the set up of the analysis that will prevent the analysis from occurring
        NSMutableDictionary* errors = [self checkAnalysisForErrors];
        if ([errors count] > 0)
            {
            NSArray* cntArray = [NSArray arrayWithObjects:@"One",@"Two",@"Three",@"Four",@"Five",@"Six",@"Seven",@"Eight",@"Nine",@"Ten",@"Eleven",@"Twelve",nil];
            NSString* alertMsg;
            if ([errors count] == 1)
                alertMsg = @"A tool has been incompletely specified or lacks connections:";
            else if ([errors count] <= 12)
                alertMsg = [NSString stringWithFormat:@"%@ tools have been incompletely specified or lack connections:", [cntArray objectAtIndex:([errors count]-1)]];
            else 
                alertMsg = [NSString stringWithFormat:@"Some tools (%d) have been incompletely specified or lack connections:", (int)[errors count]];
            NSEnumerator* enumerator = [errors objectEnumerator];
            NSString* val;
            int i=0;
            while ((val = [enumerator nextObject]))
                {
                i++;
                alertMsg = [alertMsg stringByAppendingFormat:@"\n  %d. %@", i, val];
                }
            
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Error: Incomplete Analysis"];
            [alert setInformativeText:alertMsg];
            [alert addButtonWithTitle:@"OK"];
            [alert beginSheetModalForWindow:[analysisViewPtr window] completionHandler:nil];

            rbTimer = [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(removeFlags) userInfo:nil repeats:YES];
            isRbTimerActive = YES;
            return;
            }
        
        // check for warnings that the user should be aware of before performing the analysis
        NSMutableDictionary* warnings = [self checkAnalysisForWarnings];
        if ([warnings count] > 0)
            {
            NSArray* cntArray = [NSArray arrayWithObjects:@"One",@"Two",@"Three",@"Four",@"Five",@"Six",@"Seven",@"Eight",@"Nine",@"Ten",@"Eleven",@"Twelve",nil];
            NSString* alertMsg;
            if ([warnings count] == 1)
                alertMsg = @"A tool may lose information:";
            else if ([warnings count] <= 12)
                alertMsg = [NSString stringWithFormat:@"%@ tools may lose information:", [cntArray objectAtIndex:([warnings count]-1)]];
            else 
                alertMsg = [NSString stringWithFormat:@"Some tools (%d) may lose information:", (int)[warnings count]];
            NSEnumerator* enumerator = [warnings objectEnumerator];
            NSString* val;
            int i=0;
            while ((val = [enumerator nextObject]))
                {
                i++;
                alertMsg = [alertMsg stringByAppendingFormat:@"\n  %d. %@", i, val];
                }
            
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Warning"];
            [alert setInformativeText:alertMsg];
            [alert addButtonWithTitle:@"Continue"];
            [alert addButtonWithTitle:@"Cancel"];
            [alert beginSheetModalForWindow:[analysisViewPtr window] completionHandler:^(NSModalResponse returnCode) {
                
                if (returnCode == NSAlertFirstButtonReturn)
                    {
                    goodAnalysis = YES;
                    [NSThread detachNewThreadSelector:@selector(executeAnalysis) toTarget:self withObject:nil];
                    }
                else if (returnCode == NSAlertSecondButtonReturn)
                    {
                    return;
                    }
                
                }];
            }
            
        // execute the analysis
        if ([warnings count] == 0)
            {
            goodAnalysis = YES;
            [NSThread detachNewThreadSelector:@selector(executeAnalysis) toTarget:self withObject:nil];
            }
        }
}

- (void)executeAnalysis {

    // prepare all of the tools for execution
    [self prepareToolsForExecution];
    
    // lock down the view
    [executeButton setEnabled:YES];
    [executeButton setTitle:@"Cancel"];
    [analysisViewPtr setIsLocked:YES];
    [analysisViewPtr unselectAllItems];
    
    // set the isCurrentlyExecuting flag for each tool to "NO"
	NSEnumerator* toolEnumerator = [tools objectEnumerator];
    Tool* t = nil;
	while ( (t = [toolEnumerator nextObject]) )
        [t setIsCurrentlyExecuting:NO];

    // get the preorder traversal of the graph
    NSMutableArray* depthFirstOrder = [NSMutableArray arrayWithCapacity:0];
    [analysisViewPtr initializeDepthFirstOrderForTools:depthFirstOrder];
    NSMutableArray* orderedTools = [NSMutableArray array];
    toolEnumerator = [depthFirstOrder reverseObjectEnumerator];
    id element;
	while ( (element = [toolEnumerator nextObject]) )
        {
        if ([element isLoop] == NO)
            [orderedTools addObject:element];
        }
    for (int i=0; i<[orderedTools count]; i++)
        {
        Tool* t = [orderedTools objectAtIndex:i];
        [t setExecuteOrder:i];
        }

    // add in the loop information, checking for correctness as we go
    NSArray* loops = [analysisViewPtr getLoops];
    ExecuteTree* eTree = [[ExecuteTree alloc] initWithTools:orderedTools andView:analysisViewPtr];
    for (ToolLoop* loop in loops)
        {
        NSMutableArray* loopTools = [NSMutableArray array];
        for (int i=0; i<[orderedTools count]; i++)
            {
            Tool* t = [orderedTools objectAtIndex:i];
            if ( [[(Tool*)loop loopMembership] containsObject:t] == YES )
                [loopTools addObject:t];
            }
        [eTree addLoop:loopTools repeated:[loop indexUpperLimit] loopId:loop];
        //[eTree print];
        }
    
    // execute the analysis
    Tool* badTool = [eTree executeTree];
    if (badTool != nil)
        {
        NSLog(@"report an error here");
        }

    // unlock the view
    [executeButton setEnabled:YES];
    [executeButton setTitle:@"Execute"];
    [analysisViewPtr setIsLocked:NO];
    
    //if (goodAnalysis == NO)
    //    [self analysisError:badTool];
}

- (NSFileWrapper*)fileWrapperOfType:(NSString*)typeName error:(NSError**)outError {

	// create the file wrapper directory
	// (Compiler warning about the "nil" passed into this function. However,
    // every example I've found does it this way.)
	NSFileWrapper* fw = [[NSFileWrapper alloc] initDirectoryWithFileWrappers:nil];

	// archive the analyses
	NSData* analysesData = [NSKeyedArchiver archivedDataWithRootObject:analyses];
	if( analysesData == nil )
		{
		*outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
		return nil;
		}
	[fw addRegularFileWithContents:analysesData preferredFilename:@"analyses"];
	
	// archive the settings for the analysis
	NSMutableArray* settingsArray = [NSMutableArray arrayWithCapacity:2];
	[settingsArray addObject:[NSNumber numberWithBool:showGrid]];
	[settingsArray addObject:[NSNumber numberWithBool:snapToGrid]];
	NSData* settingsData = [NSKeyedArchiver archivedDataWithRootObject:settingsArray];
	if( settingsData == nil )
		{
		*outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
		return nil;
		}
	[fw addRegularFileWithContents:settingsData preferredFilename:@"settings"];
    
	// return the file wrapper object
    return fw;
}

- (NSString*)findUniqueNameForAnalysis {

	NSString* uniqueStr = @"My Analysis";
	int i = 0;
	while ( [self isAnaysisNameUnique:uniqueStr] == NO )
		{
		i++;
		NSString* aString = [NSString stringWithFormat:@"My Analysis %d", i];
		uniqueStr = [uniqueStr stringByReplacingOccurrencesOfString:uniqueStr withString:aString];
		}
	return uniqueStr;
}

- (IBAction)helpButton:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"Workbench_Anchor" inBook:locBookName];
}

- (id)init {

    if ( (self = [super init]) ) 
		{
		NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
		/*[defaultCenter addObserver:self
						  selector:@selector(viewResized:)
							  name:@"boundsChangeNotification"
						    object:analysisViewPtr];*/
		[defaultCenter addObserver:self
						  selector:@selector(toolSourceChanged:)
							  name:@"NSTableViewSelectionDidChangeNotification"
						    object:nil];
		[defaultCenter addObserver:self
						  selector:@selector(analysisError:)
							  name:@"badAnalysis"
						    object:nil];
		[defaultCenter addObserver:self
						  selector:@selector(setStatusMessage:)
							  name:@"RbStatusBarMessage"
						    object:nil];

		// allocate an array holding the pointers to the tools
        analyses = [[NSMutableArray alloc] init];
		AnalysisTools* t = [[AnalysisTools alloc] init];
		[analyses addObject:t];
		selectedAnalysis = t;
		
		// set some default values
		rbTimer         = nil;
		isRbTimerActive = NO;
		showGrid        = YES;
		snapToGrid      = NO;
		}
    return self;
}

- (void)invalidateTimer {

	if (isRbTimerActive == YES)
		{
		[rbTimer invalidate];
		isRbTimerActive = NO;
		NSEnumerator* enumerator = [tools objectEnumerator];
		id element;
		while ( (element = [enumerator nextObject]) )
			[element setFlagCount:0];
		}
}

- (BOOL)isAnaysisNameUnique:(NSString*)str {

	NSArray* analysisObjects = [analysesController content];
	NSEnumerator* enumerator = [analysisObjects objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ( [str isEqualToString:[element analysisName]] == YES )
			return NO;
		}
	return YES;
}

- (IBAction)notebookButton:(id)sender {

    [self updateChangeCount:NSChangeDone];
	[selectedAnalysis showNoteBook];
}

- (int)numTools {

    return (int)[tools count];
}

- (void)prepareToolsForExecution {

    for (Tool* t in tools)
        {
        [t prepareForExecution];
        }
}

- (BOOL)readFromFileWrapper:(NSFileWrapper*)fileWrapper ofType:(NSString*)typeName error:(NSError**)outError {

    WindowControllerProgressBar* progressWin = [[WindowControllerProgressBar alloc] init];
    [progressWin setMessage:@"Reading Analysis File (Really!)"];
    NSWindow* w = [progressWin window];
    [w center];
    [w makeKeyAndOrderFront:nil];
    [w display];
    
	// get a list of the bundle's files
	NSDictionary* files = [fileWrapper fileWrappers];
	
	// read the analyses
	NSFileWrapper* analysesFile = [files objectForKey:@"analyses"];
	NSData* analysesData = [analysesFile regularFileContents];
	analyses = [NSKeyedUnarchiver unarchiveObjectWithData:analysesData];

	NSEnumerator* enumerator = [analyses objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		[element touchAllTools];
		}
			
	// read the settings for the analysis
	NSFileWrapper* settingsFile = [files objectForKey:@"settings"];
	NSData* settingsData = [settingsFile regularFileContents];
	NSMutableArray* settingsArray = [NSKeyedUnarchiver unarchiveObjectWithData:settingsData];
	showGrid = [[settingsArray objectAtIndex:0] boolValue];
	snapToGrid = [[settingsArray objectAtIndex:1] boolValue];
    
    [w orderOut:nil];
        
	return YES;
}

- (IBAction)removeAnalysis:(id)sender {

    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Warning: Delete Analysis"];
    [alert setInformativeText:@"You are about to delete an analysis, with all of its associated information. Do you wish to continue with this operation?"];
    [alert addButtonWithTitle:@"Delete"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert beginSheetModalForWindow:[analysisViewPtr window] completionHandler:^(NSModalResponse returnCode) {
        
        if (returnCode == NSAlertFirstButtonReturn)
            {
            [analysesController removeObject:[[analysesController selectedObjects] objectAtIndex:0]];
            selectedAnalysis = nil;
            [self updateChangeCount:NSChangeDone];

            if ( [analyses count] == 0 )
                [self addAnalysis:self];
            if (selectedAnalysis == nil)
                [self toolSourceChanged:nil];
            [analysisViewPtr setNeedsDisplay:YES];
            }
        else if (returnCode == NSAlertSecondButtonReturn)
            {
            }
        
        }];
}

- (void)removeFlags {

	BOOL killTimer = NO;
	NSEnumerator* enumerator = [tools objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element flagCount] > 0)
			[element setFlagCount:([element flagCount]+1)];
		if ( [element flagCount] > 20)
			{
			[element setFlagCount:0];
			killTimer = YES;
			}
		}
	if (killTimer == YES)
		{
		[rbTimer invalidate];
		}
	[analysisViewPtr setNeedsDisplay:YES];
}

- (void)scaleToolsByScale:(float)s andFactor:(float)f {

	// rescale the size of all of the tools in all of the analyses
	NSArray* analysisObjects = [analysesController content];
	NSEnumerator* enumerator = [analysisObjects objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        [element scaleToolsByScale:s andFactor:f];
}

- (void)selectAllTools {

    [analysisViewPtr selectAllItems];
}

- (void)setShowGrid:(BOOL)x {

    showGrid = x;
    [analysisViewPtr setShowGrid:x];
}

- (void)setSnapToGrid:(BOOL)x {

    snapToGrid = x;
    [analysisViewPtr setSnapToGrid:x];
}

- (void)setExecutionFlagForAllToolsTo:(BOOL)tf {

	NSEnumerator* toolEnumerator = [tools objectEnumerator];
    Tool* t = nil;
	while ( (t = [toolEnumerator nextObject]) )
        [t setIsCurrentlyExecuting:tf];
}

- (void)setStatusMessage:(NSNotification*)notification {

    // retrieve information about the notification
    NSDictionary* info = [notification userInfo];
    NSString* msg = [info objectForKey:@"statusMessage"];
    Tool* postingObject = [notification object];
    
    // check that the posting object is owned by this AnalysisDocument
    BOOL isThisMyTool = NO;
    for (Tool* t in tools)
        {
        if (t == postingObject)
            {
            isThisMyTool = YES;
            break;
            }
        }
    if (isThisMyTool == NO)
        return;
    
    // change the status field
    [statusField setEditable:NO];
    if ( [msg isEqualToString:@""] == YES )
        {
        [statusField setHidden:YES];
        }
    else
        {
        [statusField setHidden:NO];
        [statusField setTextColor:[NSColor blueColor]];
        [statusField setStringValue:msg];
        }
}

- (BOOL)showGrid {

    return showGrid;
}

- (BOOL)snapToGrid {

    return snapToGrid;
}

- (void)toolSourceChanged:(NSNotification*)notification {

    NSArray* selectedTools = [analysesController selectedObjects];
    if ([selectedTools count] > 0)
        {
        AnalysisTools* selectedTool = [[analysesController selectedObjects] objectAtIndex:0];
        selectedAnalysis = selectedTool;
        tools = [selectedAnalysis tools];
        [analysisViewPtr setItems:tools];
        [analysisViewPtr setCorners];
	    [analysisViewPtr updateScrollBars];
		[analysisViewPtr updateAllTrackingAreas];
        [analysisViewPtr setNeedsDisplay:YES];
        }
	else if ([selectedTools count] == 0)
		{
		if (selectedAnalysis == nil)
			{
			selectedAnalysis = [[analysesController content] objectAtIndex:0];
			tools = [selectedAnalysis tools];
			[analysisViewPtr setItems:tools];
            [analysisViewPtr setCorners];
            [analysisViewPtr updateScrollBars];
			[analysisViewPtr updateAllTrackingAreas];
            [analysisViewPtr setNeedsDisplay:YES];
			}
		}
}

- (void)windowControllerDidLoadNib:(NSWindowController*)aController {

    [super windowControllerDidLoadNib:aController];
    [analysisViewPtr setItems:tools];
    [analysisViewPtr setCorners];
    [analysisViewPtr updateScrollBars];
    
    // set the analysis view for all of the tools 
	NSEnumerator* enumerator = [analyses objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		[element setAnalysisView:analysisViewPtr];
		}
}

- (NSString *)windowNibName {

    return @"AnalysisDocument";
}

@end
