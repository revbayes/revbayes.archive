#import "CharacterData.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolDataEntry.h"
#import "WindowControllerDataEntry.h"



@implementation WindowControllerDataEntry

- (void)awakeFromNib {
	
	[dataScrollView setPostsBoundsChangedNotifications:YES];

	[dataScrollView setHasHorizontalScroller:YES];
	[[dataScrollView horizontalScroller] setControlSize:NSSmallControlSize];
	[dataScrollView setHasVerticalScroller:YES];
	[[dataScrollView verticalScroller] setControlSize:NSSmallControlSize];

	[namesScrollView setHasHorizontalScroller:YES];
	[[namesScrollView horizontalScroller] setControlSize:NSSmallControlSize];
	[namesScrollView setHasVerticalScroller:NO];
    
    [self initDataMatrixWithMatrix:[myTool dataMatrixIndexed:0]];
    [self displayMatrix:self];
}

- (IBAction)cancelAction:(id)sender {
	
	[myTool closeControlPanel];
}

- (IBAction)displayMatrix:(id)sender {

    // set the data matrix display
	[dataScrollView setDocumentView:[dataMatrices objectAtIndex:0]];
	[dataScrollView display];

    // set the taxon names display
	[namesScrollView setDocumentView:[taxaMatrices objectAtIndex:0]];
	[namesScrollView display];

    // set the size of the split windows
	float nameWidth = [self lengthOfLongestName:[taxaMatrices objectAtIndex:0]] + 5.0;
	NSRect masterFrame = [windowSplitView frame];
	NSArray* mySubViews = [windowSplitView subviews];
	float splitDivWidth = [windowSplitView dividerThickness];
	[[mySubViews objectAtIndex:0] setFrame:NSMakeRect(masterFrame.origin.x, masterFrame.origin.y, (nameWidth), masterFrame.size.height)];
	[[mySubViews objectAtIndex:1] setFrame:NSMakeRect(masterFrame.origin.x, masterFrame.origin.y, (masterFrame.size.width-(nameWidth+splitDivWidth)), 0.0)];
}

- (void)dealloc {

	[dataMatrices release];
	[taxaMatrices release];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	[super dealloc];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (BOOL)initDataMatrixWithMatrix:(RbData*)matrix {

	int nRows = [matrix numTaxa];
	nRows += 2;
	
	// create taxa matrix
	NSRect aRect = NSMakeRect(0.0,0.0,0.0,(cellHeight*nRows));
	NSMatrix* taxaMatrix = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:nRows numberOfColumns:1]; 
	[taxaMatrices addObject:taxaMatrix];
    [taxaMatrix release];
	[taxaMatrix setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[taxaMatrix setAllowsEmptySelection:YES];
	NSArray* allCells = [taxaMatrix cells];
	for (int i=0; i<nRows; i++)
		{
		NSTextFieldCell* aCell = [allCells objectAtIndex:i];
		[aCell setTag:1];
		[aCell setEditable:NO];
		[aCell setSelectable:NO];
		[aCell setDrawsBackground:YES];
		if (i == 0)
			{
			[aCell setBackgroundColor:headerBackground];
			[aCell setTextColor:headerTextColor];
			[aCell setAlignment:NSLeftTextAlignment];
			[aCell setStringValue:@"Taxon"];
			}
		else if (i == 1)
			{
			[aCell setBackgroundColor:headerBackground];
			[aCell setTextColor:headerTextColor];
			[aCell setAlignment:NSLeftTextAlignment];
			[aCell setStringValue:@"Name"];
			}
		else 
			{
            [aCell setEditable:YES];
            [aCell setSelectable:YES];
			[aCell setBackgroundColor:namesBackground];
			[aCell setTextColor:namesTextColor];
			[aCell setAlignment:NSLeftTextAlignment];
			[aCell setStringValue:[matrix taxonWithIndex:(i-2)]];
			}
		}
	float nameWidth = [self lengthOfLongestName:taxaMatrix]; 
	[taxaMatrix setCellSize:NSMakeSize(nameWidth, cellHeight)];
	[taxaMatrix setFrame:NSMakeRect(0.0,0.0,nameWidth,(cellHeight*nRows))];
	
	// create data matrix
	int nCols = 0;
    if ( [matrix isHomologyEstablished] == YES )
        nCols = [matrix numCharacters];
    else
        nCols = [matrix maxNumCharacters];
	BOOL isContinuous = NO;
	if ( [[matrix cellWithRow:0 andColumn:0] dataType] == CONTINUOUS )
		isContinuous = YES;
	float cw = cellWidth;
	if ( isContinuous == YES )
		cw *= 10.0;
	aRect = NSMakeRect(0.0,0.0,(cw*nCols),(cellHeight*nRows));
	NSMatrix* dataMatrix = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:nRows numberOfColumns:nCols];
	[dataMatrices addObject:dataMatrix];
    //[dataMatrix release];
	[dataMatrix setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[dataMatrix setCellSize:NSMakeSize(cw, cellHeight)];
	[dataMatrix setAllowsEmptySelection:YES];
	allCells = [dataMatrix cells];
	for (int i=0; i<nRows; i++)
		{
        int nc = 0;
        if (i == 0 || i == 1)
            nc = nCols;
        else
            nc = [matrix numCharactersForTaxon:(i-2)];
		for (int j=0; j<nc; j++)
			{
			if (i == 0)
				{
				NSTextFieldCell* aCell = [allCells objectAtIndex:(i*nCols)+j];
				[aCell setTag:1];
				[aCell setEditable:NO];
				[aCell setSelectable:NO];
				[aCell setDrawsBackground:YES];
				[aCell setBackgroundColor:headerBackground];
				[aCell setTextColor:headerTextColor];
				[aCell setAlignment:NSCenterTextAlignment];
				[aCell setStringValue:@""];
				}
			else if (i == 1)
				{
				NSTextFieldCell* aCell = [allCells objectAtIndex:(i*nCols)+j];
				[aCell setTag:1];
				[aCell setEditable:NO];
				[aCell setSelectable:NO];
				[aCell setDrawsBackground:YES];
				[aCell setBackgroundColor:headerBackground];
				[aCell setTextColor:headerTextColor];
				[aCell setAlignment:NSCenterTextAlignment];
				if (isContinuous == YES)
					[aCell setStringValue:@"|"];
				else 
					{
					if ( j == 0 || (j+1) % 10 == 0 )
						[aCell setStringValue:@"|"];
					else 
						[aCell setStringValue:@"."];
					}
				}
			else 
				{
				// add character-state information
				RbDataCell* dataMatrixCell = [matrix cellWithRow:(i-2) andColumn:j];
				if ([dataMatrixCell dataType] == CONTINUOUS)
					{
					// setting up continuous matrix
					float state = [dataMatrixCell getContinuousState];
					NSString* stateStr = [NSString localizedStringWithFormat:@"%2.4f", state];

					NSColor* textColor = [NSColor blackColor];
					NSColor* bkgrndColor = [NSColor grayColor];
					bkgrndColor = [bkgrndColor colorWithAlphaComponent:0.5];

					NSTextFieldCell* aCell = [allCells objectAtIndex:(i*nCols)+j];
					[aCell setTag:1];
					[aCell setEditable:NO];
					[aCell setSelectable:NO];
					[aCell setDrawsBackground:YES];
					[aCell setBackgroundColor:bkgrndColor];
					[aCell setTextColor:textColor];
					[aCell setAlignment:NSCenterTextAlignment];
					[aCell setStringValue:stateStr];
					}
				else 
					{
					// setting up discrete matrix
					char state = [dataMatrixCell getDiscreteState];
                    if ( [dataMatrixCell isGapState] == YES )
                        state = '-';
					NSString* stateStr = [NSString localizedStringWithFormat:@"%c", state];
					
					NSDictionary* colorDict = [self standardColorsDict];
					NSColor* textColor = [NSColor blackColor];
					NSColor* bkgrndColor = [colorDict objectForKey:stateStr];
                    if ( [matrix isCharacterExcluded:j] == YES || [matrix isTaxonExcluded:(i-2)] == YES )
                        bkgrndColor = [NSColor grayColor];
					bkgrndColor = [bkgrndColor colorWithAlphaComponent:0.5];

					NSTextFieldCell* aCell = [allCells objectAtIndex:(i*nCols)+j];
					[aCell setTag:1];
					[aCell setEditable:YES];
					[aCell setSelectable:YES];
					[aCell setDrawsBackground:YES];
					[aCell setBackgroundColor:bkgrndColor];
					[aCell setTextColor:textColor];
					[aCell setAlignment:NSCenterTextAlignment];
					[aCell setStringValue:[NSString stringWithFormat:@"%c", state]];
					}
				}
			}
        for (int j=nc; j<nCols; j++)
            {
			if (i == 0)
				{

				}
			else if (i == 1)
				{

				}
			else 
				{
				// add place-holder 
                NSTextFieldCell* aCell = [allCells objectAtIndex:(i*nCols)+j];
                [aCell setTag:1];
                [aCell setEditable:NO];
                [aCell setSelectable:NO];
                [aCell setDrawsBackground:YES];
                [aCell setBackgroundColor:headerBackground];
                [aCell setTextColor:headerTextColor];
                [aCell setAlignment:NSCenterTextAlignment];
                [aCell setStringValue:@""];
				}
            }
		}

	for (int j=0; j<nCols; j++)
		{
		NSTextFieldCell* aCell = [dataMatrix cellAtRow:1 column:j];
		NSString* aStr = [NSString stringWithFormat:@"%d", j+1];
		if (isContinuous == YES)
			{
			[aCell setStringValue:aStr];
			}
		else 
			{
			if( [aCell stringValue] == @"|" && ([dataMatrix numberOfColumns]-j) >= [aStr length] )
				{
				for (int i=0; i<[aStr length]; i++)
					{
					NSTextFieldCell* bCell = [dataMatrix cellAtRow:0 column:j+i];
					[bCell setStringValue:[NSString stringWithFormat:@"%c", [aStr characterAtIndex:i]]];
					}
				}
			}
		}
		
	return YES;
}

- (id)initWithTool:(ToolDataEntry*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowDataEntry"]) )
        {
        myTool               = t;
		standardColorsDict   = nil;
		cellWidth            = 16.0;
		cellHeight           = 16.0;
		dataNamesFont        = [NSFont systemFontOfSize:12.0];
		headerFont           = [NSFont boldSystemFontOfSize:12.0];
		dataTextColor        = [NSColor blackColor];
		namesTextColor       = [NSColor blackColor];
		headerTextColor      = [NSColor whiteColor];
		headerBackground     = [NSColor darkGrayColor];
		namesBackground      = [NSColor whiteColor];
		dataMatrices         = [[NSMutableArray alloc] init];
		taxaMatrices         = [[NSMutableArray alloc] init];
		
		[self initStateColorDefaults];

		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(synchronizedViewContentBoundsDidChange:)
													 name:NSViewBoundsDidChangeNotification
												   object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(splitViewDidResizeSubviews:)
													 name:NSSplitViewDidResizeSubviewsNotification
												   object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(textDidEndEditing:)
													 name:NSControlTextDidEndEditingNotification 
												   object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(textDidBeginEditing:)
													 name:NSControlTextDidBeginEditingNotification 
												   object:nil];
                                                   
		}
    return self;
}

- (void)initStateColorDefaults {

	NSColorList* ColorListToUse = [NSColorList colorListNamed:@"Crayons"];
	NSMutableArray* StandardStateColors = [[NSMutableArray alloc] initWithCapacity:1];
	NSMutableArray* StandardStateLabels = [[NSMutableArray alloc] initWithCapacity:1];
	
	// set standard state labels and colors
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Maraschino"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Lemon"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Spring"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Turquoise"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Blueberry"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Magenta"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Tangerine"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Lime"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Sea Foam"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Aqua"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[StandardStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];

	[StandardStateLabels addObject:@"0"];
	[StandardStateLabels addObject:@"1"];
	[StandardStateLabels addObject:@"2"];
	[StandardStateLabels addObject:@"3"];
	[StandardStateLabels addObject:@"4"];
	[StandardStateLabels addObject:@"5"];
	[StandardStateLabels addObject:@"6"];
	[StandardStateLabels addObject:@"7"];
	[StandardStateLabels addObject:@"8"];
	[StandardStateLabels addObject:@"9"];
	[StandardStateLabels addObject:@"N"];
	[StandardStateLabels addObject:@"?"];
	[StandardStateLabels addObject:@"-"];
	
	// set the dictionaries
	[self setStandardColorsDictKeys:StandardStateLabels forValues:StandardStateColors];

	// release the arrays
	[StandardStateLabels release];
	[StandardStateColors release];
}

- (float)lengthOfLongestName:(NSMatrix*)matrix {

	NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
	[attrs setObject:[NSFont systemFontOfSize:12.0] forKey:NSFontAttributeName];
	
	NSMatrix* taxaMatrix = [taxaMatrices objectAtIndex:0]; 
	NSArray* allCells = [taxaMatrix cells];
	
	float longestName = 0.0;
	for (int i=0; i<[taxaMatrix numberOfRows]; i++)
		{
		NSTextFieldCell* aCell = [allCells objectAtIndex:i];
		NSSize aStrSize = [[[NSAttributedString alloc] initWithString:[aCell stringValue] attributes:attrs] size];
		if (aStrSize.width > longestName)
			longestName = aStrSize.width;
		}
		
	longestName += 10.8;
	int x = longestName;
	x++;
	longestName = (float)x;

	return longestName;
}

- (unsigned)missingForNumStates:(int)n {

	unsigned val = 0;
	for (int i=0; i<n; i++)
		{
        unsigned mask = 1 << i ;
        val |= mask;
		}
	return val;
}

- (IBAction)okButtonAction:(id)sender {

    
    [myTool closeControlPanel];
}

- (void)setControlsEnabledState {


}

- (void)setToolValues {

}

- (void)setStandardColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray {

	[standardColorsDict autorelease];
	standardColorsDict = [[NSDictionary alloc] initWithObjects:valueArray forKeys:keysArray];
}

- (void)splitViewDidResizeSubviews:(NSNotification*)aNotification {

	NSMatrix* taxaMatrix = [taxaMatrices objectAtIndex:0];
	float nameWidth = [self lengthOfLongestName:[taxaMatrices objectAtIndex:0]];
	
	NSArray* mySubViews = [windowSplitView subviews];
	NSRect svFrame = [[mySubViews objectAtIndex:0] frame];
	float w = nameWidth;
	if (svFrame.size.width > nameWidth)
		w = svFrame.size.width;

	if (svFrame.size.width >= nameWidth)
		[[namesScrollView horizontalScroller] setEnabled:NO];
	else 
		[[namesScrollView horizontalScroller] setEnabled:YES];

	[taxaMatrix setCellSize:NSMakeSize(w, cellHeight)];
	[taxaMatrix setFrame:NSMakeRect(0.0,0.0,w,(cellHeight*[taxaMatrix numberOfRows]))];
}

- (NSDictionary*)standardColorsDict {

	return standardColorsDict;
}

- (void)synchronizedViewContentBoundsDidChange:(NSNotification*)notification {

    NSPoint changedBoundsOrigin = [dataScrollView documentVisibleRect].origin;;
    NSPoint curOffset = [namesScrollView bounds].origin;
    NSPoint newOffset = curOffset;
    newOffset.y = changedBoundsOrigin.y;
    if ( !NSEqualPoints(curOffset, changedBoundsOrigin) || changedBoundsOrigin.y < 0.000001 )
		{
		[[namesScrollView contentView] scrollToPoint:newOffset];
		[namesScrollView reflectScrolledClipView:[namesScrollView contentView]];
		}
}

- (void)textDidBeginEditing:(NSNotification*)notification {

    NSArray* cells = [[notification object] selectedCells];
    NSTextFieldCell* c = [cells objectAtIndex:0];
    oldCellValue = [[c stringValue] intValue];
}

- (void)textDidEndEditing:(NSNotification*)notification {

    NSArray* cells = [[notification object] selectedCells];
    NSTextFieldCell* c = [cells objectAtIndex:0];
    NSString* newString = [c stringValue];
    BOOL isValidEntry = YES;
    char charVal = [newString characterAtIndex:0];
    if (charVal == '0' || charVal == '1' || charVal == '2' || charVal == '3' || charVal == '4' ||
        charVal == '5' || charVal == '6' || charVal == '7' || charVal == '8' || charVal == '9' || charVal == '?')
        isValidEntry = YES;
    else
        isValidEntry = NO;
    if ([newString length] != 1)
        isValidEntry = NO;
        
    if (isValidEntry == YES)
        {
        int row = (int)[[notification object] selectedRow] - 2;
        int col = (int)[[notification object] selectedColumn];
        RbDataCell* dc = [[myTool dataMatrixIndexed:0] cellWithRow:row andColumn:col];
        if (charVal == '?')
            {
            [dc setDiscreteStateTo:(-1)];
            }
        else 
            {
            int newVal = [[c stringValue] intValue];
            [dc setDiscreteStateTo:newVal];
            }
        [myTool makeDataInspector];


        for (int i=2; i<[[notification object] numberOfRows]; i++)
            {
            for (int j=0; j<[[notification object] numberOfColumns]; j++)
                {
                NSTextFieldCell* myCell = [[notification object] cellAtRow:i column:j];
				RbDataCell* dataMatrixCell = [[myTool dataMatrixIndexed:0] cellWithRow:(i-2) andColumn:j];
                char state = [dataMatrixCell getDiscreteState];
                if ( [dataMatrixCell isGapState] == YES )
                    state = '-';
                NSString* stateStr = [NSString localizedStringWithFormat:@"%c", state];
                
                NSDictionary* colorDict = [self standardColorsDict];
                NSColor* textColor = [NSColor blackColor];
                NSColor* bkgrndColor = [colorDict objectForKey:stateStr];
                if ( [[myTool dataMatrixIndexed:0] isCharacterExcluded:j] == YES || [[myTool dataMatrixIndexed:0] isTaxonExcluded:(i-2)] == YES )
                    bkgrndColor = [NSColor grayColor];
                bkgrndColor = [bkgrndColor colorWithAlphaComponent:0.5];

                [myCell setTag:1];
                [myCell setEditable:YES];
                [myCell setSelectable:YES];
                [myCell setDrawsBackground:YES];
                [myCell setBackgroundColor:bkgrndColor];
                [myCell setTextColor:textColor];
                [myCell setAlignment:NSCenterTextAlignment];
                [myCell setStringValue:[NSString stringWithFormat:@"%c", state]];
                }
            }
        
        
        }
    else
        {
        [c setStringValue:[NSString stringWithFormat:@"%d", oldCellValue]];
        NSAlert* alert = [NSAlert alertWithMessageText:@"Warning: Incorrect character state" 
                                         defaultButton:@"OK" 
                                       alternateButton:nil 
                                           otherButton:nil 
                             informativeTextWithFormat:@"Character states can only be the integers 0 to 9 or \"?\""];
        [alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
        }
}

- (void)windowDidLoad {

}

@end
