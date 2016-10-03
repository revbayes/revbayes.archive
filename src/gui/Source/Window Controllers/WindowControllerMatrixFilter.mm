#import "RbData.h"
#import "ToolReadData.h"
#import "ToolMatrixFilter.h"
#import "WindowControllerMatrixFilter.h"



@implementation WindowControllerMatrixFilter

@synthesize numberOfOutlets;

- (void)awakeFromNib {
	
	if ( [myTool numDataMatrices] == 0 )
        [okButton setEnabled:NO];
    else
        [okButton setEnabled:YES];
    
	[includedTaxaView setHasHorizontalScroller:YES];
	[[includedTaxaView horizontalScroller] setControlSize:NSSmallControlSize];
	[includedTaxaView setHasVerticalScroller:YES];
	[[includedTaxaView verticalScroller] setControlSize:NSSmallControlSize];
    
	[excludedTaxaView setHasHorizontalScroller:YES];
	[[excludedTaxaView horizontalScroller] setControlSize:NSSmallControlSize];
	[excludedTaxaView setHasVerticalScroller:YES];
	[[excludedTaxaView verticalScroller] setControlSize:NSSmallControlSize];

	[includedCharView setHasHorizontalScroller:NO];
	[includedCharView setHasVerticalScroller:YES];
	[[includedCharView verticalScroller] setControlSize:NSSmallControlSize];

	[excludedCharView setHasHorizontalScroller:NO];
	[excludedCharView setHasVerticalScroller:YES];
	[[excludedCharView verticalScroller] setControlSize:NSSmallControlSize];
    
	[self setControlWindowSize];
}

- (IBAction)cancelButtonAction:(id)sender {

    // delete all of the matrices
	[includedCharMatrices removeAllObjects];
	[excludedCharMatrices removeAllObjects];
	[includedTaxaMatrices removeAllObjects];
	[excludedTaxaMatrices removeAllObjects];
    
    [myTool closeControlPanel];
}

- (IBAction)changeCharMatrix:(id)sender {

	int idx = (int)[charMatrixSelector indexOfSelectedItem];
    [taxaMatrixSelector selectItemAtIndex:idx];
	[includedCharView setDocumentView:[includedCharMatrices objectAtIndex:idx]];
	[excludedCharView setDocumentView:[excludedCharMatrices objectAtIndex:idx]];
	[includedTaxaView setDocumentView:[includedTaxaMatrices objectAtIndex:idx]];
	[excludedTaxaView setDocumentView:[excludedTaxaMatrices objectAtIndex:idx]];
}

- (IBAction)changeTaxaMatrix:(id)sender {

	int idx = (int)[taxaMatrixSelector indexOfSelectedItem];
    [charMatrixSelector selectItemAtIndex:idx];
	[includedTaxaView setDocumentView:[includedTaxaMatrices objectAtIndex:idx]];
	[excludedTaxaView setDocumentView:[excludedTaxaMatrices objectAtIndex:idx]];
	[includedCharView setDocumentView:[includedCharMatrices objectAtIndex:idx]];
	[excludedCharView setDocumentView:[excludedCharMatrices objectAtIndex:idx]];
}

- (IBAction)excludeCharButtonAction:(id)sender {

	// check that the tool has some data associated with it
	if ( [myTool numDataMatrices] == 0 )
		return;
        
    // get the index of the selected data matrix
    int idx = (int)[charMatrixSelector indexOfSelectedItem];
    
    // get pointers to the matrices containing the included and excluded taxa
	NSMatrix* itm = [includedCharMatrices objectAtIndex:idx];
	NSMatrix* etm = [excludedCharMatrices objectAtIndex:idx];
		
	// get the list of the taxa to exclude
	NSArray* lst = [[includedCharMatrices objectAtIndex:idx] selectedCells];
    if ( [lst count] == 0 )
        return;

	// enumerate the list of excluded taxa, moving each to the excluded list
	id element;
	NSEnumerator* charEnum = [lst objectEnumerator];
	while ( (element = [charEnum nextObject]) )
		{
        // find the row in the list of included taxa
        int eRow = -1;
        for (int i=0; i<[itm numberOfRows]; i++)
            {
            NSCell* aCell = [itm cellAtRow:i column:0];
            if ( [[aCell stringValue] isEqualToString:[element stringValue]] == YES )
                {
                eRow = i;
                break;
                }
            }
            
        // remove the row
        [itm removeRow:eRow];

        // find the row where the taxon should be inserted
		int chIdx = [[element stringValue] intValue];
        int insertionIdx = (int)[etm numberOfRows];
        for (int i=0; i<[etm numberOfRows]; i++)
            {
			int x = [[[etm cellAtRow:i column:0] stringValue] intValue];
            if ( chIdx < x )
                {
                insertionIdx = i;
                break;
                }
            }
        
        // add the row to the excluded list
        [etm insertRow:insertionIdx];
        NSTextFieldCell* bCell = [etm cellAtRow:insertionIdx column:0];
        [bCell setTag:1];
        [bCell setEditable:NO];
        [bCell setSelectable:NO];
        [bCell setDrawsBackground:YES];
        [bCell setBackgroundColor:[NSColor whiteColor]];
        [bCell setTextColor:[NSColor blackColor]];
        [bCell setAlignment:NSLeftTextAlignment];
        [bCell setStringValue:[element stringValue]];
		}

    NSRect f = [itm frame];
    NSRect r = [etm frame];
    f.size.height = 16.0 * [itm numberOfRows];
    r.size.height = 16.0 * [etm numberOfRows];
	[itm setFrame:f];
	[etm setFrame:r];
	[includedCharView setDocumentView:[includedCharMatrices objectAtIndex:idx]];
	[excludedCharView setDocumentView:[excludedCharMatrices objectAtIndex:idx]];
}

- (IBAction)excludeCharSetsButtonAction:(id)sender {

	if ( [[excludedCharSetsButton titleOfSelectedItem] isEqualToString:@"All"] == YES )
		{
		[[excludedCharMatrices objectAtIndex:[charMatrixSelector indexOfSelectedItem]] deselectAllCells];
		[[excludedCharMatrices objectAtIndex:[charMatrixSelector indexOfSelectedItem]] selectAll:self];
		[excludedCharSetsButton selectItemWithTitle:@""];
		}
	else if ( [[excludedCharSetsButton titleOfSelectedItem] isEqualToString:@"MissAmbig"] == YES )
		{
		if ( [myTool numDataMatrices] == 0 )
			return;
		RbData* d = [myTool dataMatrixIndexed:(int)[charMatrixSelector indexOfSelectedItem]];
		NSMatrix* m = [excludedCharMatrices objectAtIndex:[charMatrixSelector indexOfSelectedItem]];
		[m deselectAllCells];
		for (int i=0; i<[m numberOfRows]; i++)
			{
			int chIdx = [[[m cellAtRow:i column:0] stringValue] intValue] - 1;
			if ( [d isCharacterMissAmbig:chIdx] == YES )
				[m highlightCell:YES atRow:i column:0];
			}
		[excludedCharSetsButton selectItemWithTitle:@""];
		}
}

- (IBAction)excludeTaxaButtonAction:(id)sender {

	// check that the tool has some data associated with it
	if ( [myTool numDataMatrices] == 0 )
		return;
        
    // get the index of the selected data matrix
    int idx = (int)[taxaMatrixSelector indexOfSelectedItem];
    
    // and get a pointer to the data matrix
    RbData* d = [myTool dataMatrixIndexed:idx];
    
    // get pointers to the matrices containing the included and excluded taxa
	NSMatrix* itm = [includedTaxaMatrices objectAtIndex:idx];
	NSMatrix* etm = [excludedTaxaMatrices objectAtIndex:idx];
		
	// get the list of the taxa to exclude
	NSArray* lst = [[includedTaxaMatrices objectAtIndex:idx] selectedCells];
    if ( [lst count] == 0 )
        return;

	// enumerate the list of excluded taxa, moving each to the excluded list
	id element;
	NSEnumerator* taxaEnum = [lst objectEnumerator];
	while ( (element = [taxaEnum nextObject]) )
		{
        // find the row in the list of included taxa
        int eRow = -1;
        for (int i=0; i<[itm numberOfRows]; i++)
            {
            NSCell* aCell = [itm cellAtRow:i column:0];
            if ( [[aCell stringValue] isEqualToString:[element stringValue]] == YES )
                {
                eRow = i;
                break;
                }
            }
            
        // remove the row
        [itm removeRow:eRow];

        // find the row where the taxon should be inserted
        int txIdx = [d indexOfTaxonNamed:[element stringValue]];
        int insertionIdx = (int)[etm numberOfRows];
        for (int i=0; i<[etm numberOfRows]; i++)
            {
            int x = [d indexOfTaxonNamed:[[etm cellAtRow:i column:0] stringValue]];
            if ( txIdx < x )
                {
                insertionIdx = i;
                break;
                }
            }
        
        // add the row to the excluded list
        [etm insertRow:insertionIdx];
        NSTextFieldCell* bCell = [etm cellAtRow:insertionIdx column:0];
        [bCell setTag:1];
        [bCell setEditable:NO];
        [bCell setSelectable:NO];
        [bCell setDrawsBackground:YES];
        [bCell setBackgroundColor:[NSColor whiteColor]];
        [bCell setTextColor:[NSColor blackColor]];
        [bCell setAlignment:NSLeftTextAlignment];
        [bCell setStringValue:[element stringValue]];
		}

    NSRect f = [itm frame];
    NSRect r = [etm frame];
    f.size.height = 16.0 * [itm numberOfRows];
    r.size.height = 16.0 * [etm numberOfRows];
	[itm setFrame:f];
	[etm setFrame:r];
	[includedTaxaView setDocumentView:[includedTaxaMatrices objectAtIndex:idx]];
	[excludedTaxaView setDocumentView:[excludedTaxaMatrices objectAtIndex:idx]];
}

- (IBAction)excludeTaxaSetsButtonAction:(id)sender {

	if ( [[includedTaxaSetsButton titleOfSelectedItem] isEqualToString:@"All Taxa"] == YES )
		{
		[[includedTaxaMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]] deselectAllCells];
		[[includedTaxaMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]] selectAll:self];
		[includedTaxaSetsButton selectItemWithTitle:@""];
		}
}

- (IBAction)helpButtonAction:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"MatrixFilterTool_Anchor" inBook:locBookName];
}

- (IBAction)includeCharButtonAction:(id)sender {

	// check that the tool has some data associated with it
	if ( [myTool numDataMatrices] == 0 )
		return;
        
    // get the index of the selected data matrix
    int idx = (int)[charMatrixSelector indexOfSelectedItem];
    
    // get pointers to the matrices containing the included and excluded taxa
	NSMatrix* itm = [includedCharMatrices objectAtIndex:idx];
	NSMatrix* etm = [excludedCharMatrices objectAtIndex:idx];
		
	// get the list of the taxa to include
	NSArray* lst = [[excludedCharMatrices objectAtIndex:idx] selectedCells];
    if ( [lst count] == 0 )
        return;

	// enumerate the list of excluded taxa, moving each to the excluded list
	id element;
	NSEnumerator* charEnum = [lst objectEnumerator];
	while ( (element = [charEnum nextObject]) )
		{
        // find the row in the list of excluded characters
        int eRow = -1;
        for (int i=0; i<[etm numberOfRows]; i++)
            {
            NSCell* aCell = [etm cellAtRow:i column:0];
            if ( [[aCell stringValue] isEqualToString:[element stringValue]] == YES )
                {
                eRow = i;
                break;
                }
            }
        if ( eRow == -1 )
            {
            NSLog(@"Problem including characters");
            return;
            }
            
        // remove the row
        [etm removeRow:eRow];
        
        // find the row where the character should be inserted
		int chIdx = [[element stringValue] intValue];
        int insertionIdx = (int)[itm numberOfRows];
        for (int i=0; i<[itm numberOfRows]; i++)
            {
			int x = [[[itm cellAtRow:i column:0] stringValue] intValue];
            if ( chIdx < x )
                {
                insertionIdx = i;
                break;
                }
            }

        // add the row to the included list
        [itm insertRow:insertionIdx];
        NSTextFieldCell* bCell = [itm cellAtRow:insertionIdx column:0];
        [bCell setTag:1];
        [bCell setEditable:NO];
        [bCell setSelectable:NO];
        [bCell setDrawsBackground:YES];
        [bCell setBackgroundColor:[NSColor whiteColor]];
        [bCell setTextColor:[NSColor blackColor]];
        [bCell setAlignment:NSLeftTextAlignment];
        [bCell setStringValue:[element stringValue]];
		}

    NSRect f = [itm frame];
    NSRect r = [etm frame];
    f.size.height = 16.0 * [itm numberOfRows];
    r.size.height = 16.0 * [etm numberOfRows];
	[itm setFrame:f];
	[etm setFrame:r];
	[includedCharView setDocumentView:[includedCharMatrices objectAtIndex:idx]];
	[excludedCharView setDocumentView:[excludedCharMatrices objectAtIndex:idx]];
}

- (IBAction)includeCharSetsButtonAction:(id)sender {

	if ( [[includedCharSetsButton titleOfSelectedItem] isEqualToString:@"All"] == YES )
		{
		[[includedCharMatrices objectAtIndex:[charMatrixSelector indexOfSelectedItem]] deselectAllCells];
		[[includedCharMatrices objectAtIndex:[charMatrixSelector indexOfSelectedItem]] selectAll:self];
		[includedCharSetsButton selectItemWithTitle:@""];
		}
	else if ( [[includedCharSetsButton titleOfSelectedItem] isEqualToString:@"MissAmbig"] == YES )
		{
		ToolReadData* t = (ToolReadData*)[myTool getParentToolOfInletIndexed:0];
		if (t == nil)
			return;
		RbData* d = [t dataMatrixIndexed:(int)[charMatrixSelector indexOfSelectedItem]];
		NSMatrix* m = [includedCharMatrices objectAtIndex:[charMatrixSelector indexOfSelectedItem]];
		[m deselectAllCells];
		for (int i=0; i<[m numberOfRows]; i++)
			{
			int chIdx = [[[m cellAtRow:i column:0] stringValue] intValue] - 1;
			if ( [d isCharacterMissAmbig:chIdx] == YES )
				[m highlightCell:YES atRow:i column:0];
			}
		[includedCharSetsButton selectItemWithTitle:@""];
		}
}

- (IBAction)includeTaxaButtonAction:(id)sender {

	// check that the tool has some data associated with it
	if ( [myTool numDataMatrices] == 0 )
		return;
        
    // get the index of the selected data matrix
    int idx = (int)[taxaMatrixSelector indexOfSelectedItem];
    
    // and get a pointer to the data matrix
    RbData* d = [myTool dataMatrixIndexed:idx];
    
    // get pointers to the matrices containing the included and excluded taxa
	NSMatrix* itm = [includedTaxaMatrices objectAtIndex:idx];
	NSMatrix* etm = [excludedTaxaMatrices objectAtIndex:idx];
		
	// get the list of the taxa to include
	NSArray* lst = [[excludedTaxaMatrices objectAtIndex:idx] selectedCells];
    if ( [lst count] == 0 )
        return;

	// enumerate the list of excluded taxa, moving each to the excluded list
	id element;
	NSEnumerator* taxaEnum = [lst objectEnumerator];
	while ( (element = [taxaEnum nextObject]) )
		{
        // find the row in the list of excluded taxa
        int eRow = -1;
        for (int i=0; i<[etm numberOfRows]; i++)
            {
            NSCell* aCell = [etm cellAtRow:i column:0];
            if ( [[aCell stringValue] isEqualToString:[element stringValue]] == YES )
                {
                eRow = i;
                break;
                }
            }
        if ( eRow == -1 )
            {
            NSLog(@"Problem including taxa");
            return;
            }
            
        // remove the row
        [etm removeRow:eRow];
        
        // find the row where the taxon should be inserted
        int txIdx = [d indexOfTaxonNamed:[element stringValue]];
        int insertionIdx = (int)[itm numberOfRows];
        for (int i=0; i<[itm numberOfRows]; i++)
            {
            int x = [d indexOfTaxonNamed:[[itm cellAtRow:i column:0] stringValue]];
            if ( txIdx < x )
                {
                insertionIdx = i;
                break;
                }
            }

        // add the row to the included list
        [itm insertRow:insertionIdx];
        NSTextFieldCell* bCell = [itm cellAtRow:insertionIdx column:0];
        [bCell setTag:1];
        [bCell setEditable:NO];
        [bCell setSelectable:NO];
        [bCell setDrawsBackground:YES];
        [bCell setBackgroundColor:[NSColor whiteColor]];
        [bCell setTextColor:[NSColor blackColor]];
        [bCell setAlignment:NSLeftTextAlignment];
        [bCell setStringValue:[element stringValue]];
		}

    NSRect f = [itm frame];
    NSRect r = [etm frame];
    f.size.height = 16.0 * [itm numberOfRows];
    r.size.height = 16.0 * [etm numberOfRows];
	[itm setFrame:f];
	[etm setFrame:r];
	[includedTaxaView setDocumentView:[includedTaxaMatrices objectAtIndex:idx]];
	[excludedTaxaView setDocumentView:[excludedTaxaMatrices objectAtIndex:idx]];
}

- (IBAction)includeTaxaSetsButtonAction:(id)sender {

	if ( [[excludedTaxaSetsButton titleOfSelectedItem] isEqualToString:@"All Taxa"] == YES )
		{
		[[excludedTaxaMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]] deselectAllCells];
		[[excludedTaxaMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]] selectAll:self];
		[excludedTaxaSetsButton selectItemWithTitle:@""];
		}
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolMatrixFilter*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowMatrixFilter"]) )
        {
        myTool = t;
		includedCharMatrices = [[NSMutableArray alloc] init];
		excludedCharMatrices = [[NSMutableArray alloc] init];
		includedTaxaMatrices = [[NSMutableArray alloc] init];
		excludedTaxaMatrices = [[NSMutableArray alloc] init];
		[self setNumberOfOutlets:1];
        }
	return self;
}

- (float)lengthOfLongestName:(RbData*)matrix {

	NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
	[attrs setObject:[NSFont systemFontOfSize:12.0] forKey:NSFontAttributeName];
		
	float longestName = 0.0;
	for (int i=0; i<[matrix numTaxa]; i++)
		{
		NSSize aStrSize = [[[NSAttributedString alloc] initWithString:[matrix taxonWithIndex:i] attributes:attrs] size];
		if (aStrSize.width > longestName)
			longestName = aStrSize.width;
		}
		
	longestName += 10.8;
	int x = longestName;
	x++;
	longestName = (float)x;

	return longestName;
}

- (IBAction)okButtonAction:(id)sender {

    // delete the taxa/characters from the data matrices

	// check that the tool has some data associated with it
	if ( [myTool numDataMatrices] == 0 )
		return;
			
    // loop through the data matrices
	for (int i=0; i<[taxaMatrixSelector numberOfItems]; i++)
		{
        // get a pointer to the data matrix
        RbData* d = [myTool dataMatrixIndexed:i];

        // delete taxa
        [d includeAllTaxa];
        NSArray* tList = [[excludedTaxaMatrices objectAtIndex:i] cells];
        id element;
        NSEnumerator* taxaEnum = [tList objectEnumerator];
        while ( (element = [taxaEnum nextObject]) )
            {
            int idx = [d indexOfTaxonNamed:[element stringValue]];
            [d excludeTaxonIndexed:idx];
            }
        
        // delete characters
        [d includeAllCharacters];
        NSArray* cList = [[excludedCharMatrices objectAtIndex:i] cells];
        NSEnumerator* charEnum = [cList objectEnumerator];
        while ( (element = [charEnum nextObject]) )
            {
            int idx = [[element stringValue] intValue] - 1;
            [d excludeCharacterIndexed:idx];
            }
		}

    // delete all of the matrices
	[includedCharMatrices removeAllObjects];
	[excludedCharMatrices removeAllObjects];
	[includedTaxaMatrices removeAllObjects];
	[excludedTaxaMatrices removeAllObjects];

    [myTool closeControlPanel];
    [myTool makeDataInspector];
    
    [myTool resolveStateOnWindowOK];
}

- (void)setControlWindowSize {

	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;

	NSString* tabViewLabel = [NSString stringWithString:[[excludeTypeButton selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Taxa"] == YES )
		{
        newFrame.size.height = 643.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];
		}
	else if ( [tabViewLabel isEqualToString:@"Characters"] == YES )
		{
        newFrame.size.height = 540.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];
		}
}

- (void)setMatrixList {

	// check that the tool has some data associated with it
	if ( [myTool numDataMatrices] == 0 )
		{
		[taxaMatrixSelector removeAllItems];
		[charMatrixSelector removeAllItems];
		[includedTaxaSetsButton removeAllItems];
		[excludedTaxaSetsButton removeAllItems];
		[includedCharSetsButton removeAllItems];
		[excludedCharSetsButton removeAllItems];
		[includedTaxaView setDocumentView:nil];
		[excludedTaxaView setDocumentView:nil];
		[includedCharView setDocumentView:nil];
		[excludedCharView setDocumentView:nil];
		return;
		}
				
	// set the information in the control window
	[taxaMatrixSelector removeAllItems];
	[charMatrixSelector removeAllItems];
	for (int i=0; i<[myTool numDataMatrices]; i++)
		{
		// set the matrix selector pop-up button information
		[taxaMatrixSelector addItemWithTitle:[[[myTool dataMatrixIndexed:i] name] lastPathComponent]];
		[charMatrixSelector addItemWithTitle:[[[myTool dataMatrixIndexed:i] name] lastPathComponent]];
		
		// set the taxa matrix information
		[self setTaxaMatrixInfo:[myTool dataMatrixIndexed:i]];
		
		// set the character matrix information
		[self setCharMatrixInfo:[myTool dataMatrixIndexed:i]];
		}
	[includedTaxaView setDocumentView:[includedTaxaMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]]];
	[excludedTaxaView setDocumentView:[excludedTaxaMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]]];
	[includedCharView setDocumentView:[includedCharMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]]];
	[excludedCharView setDocumentView:[excludedCharMatrices objectAtIndex:[taxaMatrixSelector indexOfSelectedItem]]];
	
	// set the taxa and character sets to some default values
	[includedTaxaSetsButton removeAllItems];
	[includedTaxaSetsButton addItemWithTitle:@""];
	[includedTaxaSetsButton addItemWithTitle:@"All Taxa"];
	[excludedTaxaSetsButton removeAllItems];
	[excludedTaxaSetsButton addItemWithTitle:@""];
	[excludedTaxaSetsButton addItemWithTitle:@"All Taxa"];

	[includedCharSetsButton removeAllItems];
	[includedCharSetsButton addItemWithTitle:@""];
	[includedCharSetsButton addItemWithTitle:@"All"];
	[includedCharSetsButton addItemWithTitle:@"MissAmbig"];
	[excludedCharSetsButton removeAllItems];
	[excludedCharSetsButton addItemWithTitle:@""];
	[excludedCharSetsButton addItemWithTitle:@"All"];
	[excludedCharSetsButton addItemWithTitle:@"MissAmbig"];
}

- (BOOL)setCharMatrixInfo:(RbData*)matrix {

	// initialize some important variables
	float cellHeight = 16.0;
	NSRect aRect = NSMakeRect(0.0,0.0,0.0,(cellHeight*1));

	// create taxa matrix
	NSMatrix* itm = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:0 numberOfColumns:1]; 
	[includedCharMatrices addObject:itm];
	[itm setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[itm setAllowsEmptySelection:YES];
	[itm setDoubleAction:@selector(excludeCharButtonAction:)];

	NSMatrix* etm = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:0 numberOfColumns:1]; 
	[excludedCharMatrices addObject:etm];
	[etm setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[etm setAllowsEmptySelection:YES];
	[etm setDoubleAction:@selector(includeCharButtonAction:)];

	for (int i=0; i<[matrix numCharacters]; i++)
		{
        if ( [matrix isCharacterExcluded:i] == NO )
            {
            // included character
            [itm addRow];
            NSTextFieldCell* aCell = [itm cellAtRow:([itm numberOfRows]-1) column:0];
            [aCell setTag:1];
            [aCell setEditable:NO];
            [aCell setSelectable:NO];
            [aCell setDrawsBackground:YES];
            [aCell setBackgroundColor:[NSColor whiteColor]];
            [aCell setTextColor:[NSColor blackColor]];
            [aCell setAlignment:NSLeftTextAlignment];
            [aCell setStringValue:[NSString stringWithFormat:@"%d", i+1]];
            }
        else 
            {
            // excluded character
            [etm addRow];
            NSTextFieldCell* aCell = [etm cellAtRow:([etm numberOfRows]-1) column:0];
            [aCell setTag:1];
            [aCell setEditable:NO];
            [aCell setSelectable:NO];
            [aCell setDrawsBackground:YES];
            [aCell setBackgroundColor:[NSColor whiteColor]];
            [aCell setTextColor:[NSColor blackColor]];
            [aCell setAlignment:NSLeftTextAlignment];
            [aCell setStringValue:[NSString stringWithFormat:@"%d", i+1]];
            }
		}

	float cellWidth = [includedCharView visibleRect].size.width - 16.0;
        
	[itm setCellSize:NSMakeSize(cellWidth, cellHeight)];
	[etm setCellSize:NSMakeSize(cellWidth, cellHeight)];
	[itm setFrame:NSMakeRect(0.0,0.0,cellWidth,(cellHeight*[itm numberOfRows]))];
	[etm setFrame:NSMakeRect(0.0,0.0,cellWidth,(cellHeight*[etm numberOfRows]))];
    
	return YES;
}

- (BOOL)setTaxaMatrixInfo:(RbData*)matrix {

	// initialize some important variables
	float cellHeight = 16.0;
	NSRect aRect = NSMakeRect(0.0,0.0,0.0,(cellHeight*1));
	
	// create taxa matrix
	NSMatrix* itm = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:0 numberOfColumns:1]; 
	[includedTaxaMatrices addObject:itm];
	[itm setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[itm setAllowsEmptySelection:YES];
	[itm setDoubleAction:@selector(excludeTaxaButtonAction:)];

	NSMatrix* etm = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:0 numberOfColumns:1]; 
	[excludedTaxaMatrices addObject:etm];
	[etm setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[etm setAllowsEmptySelection:YES];
	[etm setDoubleAction:@selector(includeTaxaButtonAction:)];
	
	for (int i=0; i<[matrix numTaxa]; i++)
		{
        if ( [matrix isTaxonExcluded:i] == NO )
            {
            // included taxon
            [itm addRow];
            NSTextFieldCell* aCell = [itm cellAtRow:([itm numberOfRows]-1) column:0];
            [aCell setTag:1];
            [aCell setEditable:NO];
            [aCell setSelectable:NO];
            [aCell setDrawsBackground:YES];
            [aCell setBackgroundColor:[NSColor whiteColor]];
            [aCell setTextColor:[NSColor blackColor]];
            [aCell setAlignment:NSLeftTextAlignment];
            [aCell setStringValue:[matrix taxonWithIndex:i]];
            }
        else 
            {
            // excluded taxon
            [etm addRow];
            NSTextFieldCell* aCell = [etm cellAtRow:([etm numberOfRows]-1) column:0];
            [aCell setTag:1];
            [aCell setEditable:NO];
            [aCell setSelectable:NO];
            [aCell setDrawsBackground:YES];
            [aCell setBackgroundColor:[NSColor whiteColor]];
            [aCell setTextColor:[NSColor blackColor]];
            [aCell setAlignment:NSLeftTextAlignment];
            [aCell setStringValue:[matrix taxonWithIndex:i]];
            }
        
		}
	float nameWidth = [self lengthOfLongestName:matrix]; 
	float cellWidth = [includedTaxaView visibleRect].size.width - 16.0;
	if (nameWidth < cellWidth)
		nameWidth = cellWidth;
        
	[itm setCellSize:NSMakeSize(nameWidth, cellHeight)];
	[etm setCellSize:NSMakeSize(nameWidth, cellHeight)];
	[itm setFrame:NSMakeRect(0.0,0.0,nameWidth,(cellHeight*[itm numberOfRows]))];
	[etm setFrame:NSMakeRect(0.0,0.0,nameWidth,(cellHeight*[etm numberOfRows]))];
    
	return YES;
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {

	NSString* tabViewLabel = [NSString stringWithString:[tabViewItem label]];
	if ( [tabViewLabel isEqualToString:@"Taxa"] == YES )
		{

		}
	else if ( [tabViewLabel isEqualToString:@"Characters"] == YES )
		{

		}
	[self setControlWindowSize];
}

- (void)updateButtons {

	if ( [myTool numDataMatrices] == 0 )
        [okButton setEnabled:NO];
    else
        [okButton setEnabled:YES];
}

- (void)windowDidLoad {

}

@end
