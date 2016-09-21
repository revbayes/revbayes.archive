#import "RbData.h"
#import "RbDataCell.h"
#import "ToolMatrixFilter.h"
#import "ToolData.h"
#import "WindowControllerCharacterMatrix.h"



@implementation WindowControllerCharacterMatrix

@synthesize cellWidth;
@synthesize cellHeight;

- (NSDictionary*)aminoColorsDict {

	return aminoColorsDict;
}

- (void)awakeFromNib {

	[dataScrollView setPostsBoundsChangedNotifications:YES];

	[dataScrollView setHasHorizontalScroller:YES];
	[[dataScrollView horizontalScroller] setControlSize:NSSmallControlSize];
	[dataScrollView setHasVerticalScroller:YES];
	[[dataScrollView verticalScroller] setControlSize:NSSmallControlSize];

	[namesScrollView setHasHorizontalScroller:YES];
	[[namesScrollView horizontalScroller] setControlSize:NSSmallControlSize];
	[namesScrollView setHasVerticalScroller:NO];
	//[[namesScrollView verticalScroller] setControlSize:NSSmallControlSize];

	[matrixSelector removeAllItems];
	for (int i=0; i<[myTool numDataMatrices]; i++)
		{
		[matrixSelector addItemWithTitle:[[[myTool dataMatrixIndexed:i] name] lastPathComponent]];
		[self initDataMatrixWithMatrix:[myTool dataMatrixIndexed:i]];
		}

	[self changeMatrix:self];
}

- (IBAction)changeMatrix:(id)sender {

    // set the data matrix display
	[dataScrollView setDocumentView:[dataMatrices objectAtIndex:[matrixSelector indexOfSelectedItem]]];
	[dataScrollView display];

    // set the taxon names display
	[namesScrollView setDocumentView:[taxaMatrices objectAtIndex:[matrixSelector indexOfSelectedItem]]];
	[namesScrollView display];

    // set the size of the split windows
	float nameWidth = [self lengthOfLongestName:[taxaMatrices objectAtIndex:[matrixSelector indexOfSelectedItem]]] + 5.0;
	NSRect masterFrame = [windowSplitView frame];
	NSArray* mySubViews = [windowSplitView subviews];
	float splitDivWidth = [windowSplitView dividerThickness];
	[[mySubViews objectAtIndex:0] setFrame:NSMakeRect(masterFrame.origin.x, masterFrame.origin.y, (nameWidth), masterFrame.size.height)];
	[[mySubViews objectAtIndex:1] setFrame:NSMakeRect(masterFrame.origin.x, masterFrame.origin.y, (masterFrame.size.width-(nameWidth+splitDivWidth)), 0.0)];
    
    // set the indicators for the matrix
    RbData* d = [myTool dataMatrixIndexed:(int)[matrixSelector indexOfSelectedItem]];
    NSString* m1 = [NSString stringWithFormat:@"Number of Taxa: %d", [d numTaxa]];
    NSString* m2 = [NSString stringWithFormat:@"Number of Characters: %d", [d numCharacters]];
    NSString* m3;
    if ([d dataType] == DNA)
        m3 = [NSString stringWithFormat:@"Data Type: DNA"];
    else if ([d dataType] == RNA)
        m3 = [NSString stringWithFormat:@"Data Type: RNA"];
    else if ([d dataType] == AA)
        m3 = [NSString stringWithFormat:@"Data Type: Amino Acid"];
    else if ([d dataType] == STANDARD)
        m3 = [NSString stringWithFormat:@"Data Type: Standard"];
    else if ([d dataType] == CONTINUOUS)
        m3 = [NSString stringWithFormat:@"Data Type: Continuous"];
    else if ([d dataType] == MIXED)
        m3 = [NSString stringWithFormat:@"Data Type: Mixed"];
    NSString* m4 = [NSString stringWithFormat:@"Number of Excluded Characters: %d", [d numExcludedCharacters]];
    NSString* m5 = [NSString stringWithFormat:@"Number of Excluded Taxa: %d", [d numExcludedTaxa]];

    [numTaxaIndicator         setStringValue:m1];
    [numCharIndicator         setStringValue:m2];
    [dataTypeIndicator        setStringValue:m3];
    [numExcludedCharIndicator setStringValue:m4];
    [numExcludedTaxaIndicator setStringValue:m5];
    if ( [d dataType] == DNA || [d dataType] == RNA || [d dataType] == AA )
        {
        if ( [d isHomologyEstablished] == YES )
            {
            [isAlignedIndicator setStringValue:@"Sequences Aligned: Yes"];
            NSString* m6 = @"Alignment Method: ";
            m6 = [m6 stringByAppendingString:[d alignmentMethod]];
            [alignmentMethodIndicator setStringValue:m6];
            [isAlignedIndicator setHidden:NO];
            [alignmentMethodIndicator setHidden:NO];
            }
        else
            {
            [isAlignedIndicator setStringValue:@"Sequences Aligned: No"];
            [isAlignedIndicator setHidden:NO];
            [alignmentMethodIndicator setHidden:YES];
            }
        }
    else
        {
        [isAlignedIndicator setHidden:YES];
        [alignmentMethodIndicator setHidden:YES];
        }
}

- (IBAction)closeAction:(id)sender {
    
    [self close];
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)drawerDidClose:(NSNotification*)notification {

    [showInfoButton setTitle:@"Show Information"];
}

- (void)drawerDidOpen:(NSNotification*)notification {

    [showInfoButton setTitle:@"Hide Information"];
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
                    NSString* stateStr;
                    if ( [dataMatrixCell isAmbig] == YES )
                        stateStr = [NSString localizedStringWithFormat:@"%c", '-'];
                    else
                        stateStr = [NSString localizedStringWithFormat:@"%2.4f", state];

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
					//char state = [dataMatrixCell getDiscreteState];
					char state = [matrix stateWithRow:(i-2) andColumn:j];

                    if ( [dataMatrixCell isGapState] == YES )
                        state = '-';
					NSString* stateStr = [NSString localizedStringWithFormat:@"%c", state];
					
					NSDictionary* colorDict;
					if ([dataMatrixCell dataType] == DNA || [dataMatrixCell dataType] == RNA)
						colorDict = [self nucleotideColorsDict];
					else if ([dataMatrixCell dataType] == AA)
						colorDict = [self aminoColorsDict];
					else if ([dataMatrixCell dataType] == STANDARD)
						colorDict = [self standardColorsDict];
					NSColor* textColor = [NSColor blackColor];
					NSColor* bkgrndColor = [colorDict objectForKey:stateStr];
                    if ( [matrix isCharacterExcluded:j] == YES || [matrix isTaxonExcluded:(i-2)] == YES )
                        bkgrndColor = [NSColor grayColor];
					bkgrndColor = [bkgrndColor colorWithAlphaComponent:0.5];
                    

					NSTextFieldCell* aCell = [allCells objectAtIndex:(i*nCols)+j];
					[aCell setTag:1];
					[aCell setEditable:NO];
					[aCell setSelectable:NO];
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
			if( [aCell isEqualTo:@"|"] == YES && ([dataMatrix numberOfColumns]-j) >= [aStr length] )
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

- (id)initWithTool:(id)t {

	if ( (self = [super initWithWindowNibName:@"CharacterMatrix"]) )
        {
        myTool               = t;
		aminoColorsDict      = nil;
		nucleotideColorsDict = nil;
		standardColorsDict   = nil;
		cellWidth            = 16.0;
		cellHeight           = 16.0;
		dataNamesFont        = [NSFont systemFontOfSize:12.0];
		headerFont           = [NSFont boldSystemFontOfSize:12.0];
		dataTextColor        = [NSColor blackColor];
		namesTextColor       = [NSColor blackColor];
		headerTextColor      = [NSColor whiteColor];
		headerBackground     = [NSColor darkGrayColor];
		//headerBackground     = [NSColor colorWithCalibratedRed:0.3 green:0.3 blue:0.3 alpha:1.0];
		namesBackground      = [NSColor whiteColor];
		dataMatrices         = [[NSMutableArray alloc] init];
		taxaMatrices         = [[NSMutableArray alloc] init];
		
		[self initStateColorDefaults];

		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(synchronizedViewContentBoundsDidChange:)
													 name:NSViewBoundsDidChangeNotification
												   object:nil];
		}
    return self;
}

- (float)lengthOfLongestName:(NSMatrix*)matrix {

	NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
	[attrs setObject:[NSFont systemFontOfSize:12.0] forKey:NSFontAttributeName];
	
	NSMatrix* taxaMatrix = [taxaMatrices objectAtIndex:[matrixSelector indexOfSelectedItem]]; 
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

- (void)initStateColorDefaults {

	NSColorList* ColorListToUse = [NSColorList colorListNamed:@"Crayons"];
	NSMutableArray* AminoAcidStateColors = [[NSMutableArray alloc] initWithCapacity:1];
	NSMutableArray* AminoAcidStateLabels = [[NSMutableArray alloc] initWithCapacity:1];
	NSMutableArray* NucleotideStateColors = [[NSMutableArray alloc] initWithCapacity:1];
	NSMutableArray* NucleotideStateLabels = [[NSMutableArray alloc] initWithCapacity:1];
	NSMutableArray* StandardStateColors = [[NSMutableArray alloc] initWithCapacity:1];
	NSMutableArray* StandardStateLabels = [[NSMutableArray alloc] initWithCapacity:1];

	// set nucleotide labels and colors
	[NucleotideStateColors addObject:[NSColor redColor]];
	[NucleotideStateColors addObject:[NSColor greenColor]];
	[NucleotideStateColors addObject:[NSColor blueColor]];
	[NucleotideStateColors addObject:[NSColor yellowColor]];
	[NucleotideStateColors addObject:[NSColor yellowColor]];
	for (int i=0; i<15; i++)
		[NucleotideStateColors addObject:[NSColor colorWithCalibratedRed:0.90 green:0.90 blue: 0.90 alpha:1.0]];

	[NucleotideStateLabels addObject:@"A"];
	[NucleotideStateLabels addObject:@"C"];
	[NucleotideStateLabels addObject:@"G"];
	[NucleotideStateLabels addObject:@"T"];
	[NucleotideStateLabels addObject:@"U"];
	[NucleotideStateLabels addObject:@"N"];
	[NucleotideStateLabels addObject:@"M"];
	[NucleotideStateLabels addObject:@"R"];
	[NucleotideStateLabels addObject:@"W"];
	[NucleotideStateLabels addObject:@"S"];
	[NucleotideStateLabels addObject:@"Y"];
	[NucleotideStateLabels addObject:@"K"];
	[NucleotideStateLabels addObject:@"V"];
	[NucleotideStateLabels addObject:@"H"];
	[NucleotideStateLabels addObject:@"D"];
	[NucleotideStateLabels addObject:@"B"];
	[NucleotideStateLabels addObject:@"X"];
	[NucleotideStateLabels addObject:@"-"];
	[NucleotideStateLabels addObject:@"?"];
	[NucleotideStateLabels addObject:@"."];

	// set amino acid labels and colors
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Maraschino"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Lemon"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Spring"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Turquoise"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Blueberry"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magenta"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Tangerine"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Lime"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Sea Foam"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Aqua"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Grape"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Strawberry"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Salmon"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Banana"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Flora"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Ice"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Orchid"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Bubblegum"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Sky"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Lavender"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];
	[AminoAcidStateColors addObject:[ColorListToUse colorWithKey:@"Magnesium"]];

	[AminoAcidStateLabels addObject:@"A"];
	[AminoAcidStateLabels addObject:@"R"];
	[AminoAcidStateLabels addObject:@"N"];
	[AminoAcidStateLabels addObject:@"D"];
	[AminoAcidStateLabels addObject:@"C"];
	[AminoAcidStateLabels addObject:@"Q"];
	[AminoAcidStateLabels addObject:@"E"];
	[AminoAcidStateLabels addObject:@"G"];
	[AminoAcidStateLabels addObject:@"H"];
	[AminoAcidStateLabels addObject:@"I"];
	[AminoAcidStateLabels addObject:@"L"];
	[AminoAcidStateLabels addObject:@"K"];
	[AminoAcidStateLabels addObject:@"M"];
	[AminoAcidStateLabels addObject:@"F"];
	[AminoAcidStateLabels addObject:@"P"];
	[AminoAcidStateLabels addObject:@"S"];
	[AminoAcidStateLabels addObject:@"T"];
	[AminoAcidStateLabels addObject:@"W"];
	[AminoAcidStateLabels addObject:@"Y"];
	[AminoAcidStateLabels addObject:@"V"];
	[AminoAcidStateLabels addObject:@"B"];
	[AminoAcidStateLabels addObject:@"Z"];
	[AminoAcidStateLabels addObject:@"X"];
	[AminoAcidStateLabels addObject:@"*"];
	[AminoAcidStateLabels addObject:@"?"];
	[AminoAcidStateLabels addObject:@"-"];
	[AminoAcidStateLabels addObject:@"."];
	
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
	[self setAminoColorsDictKeys:AminoAcidStateLabels forValues:AminoAcidStateColors];
	[self setNucleotideColorsDictKeys:NucleotideStateLabels forValues:NucleotideStateColors];
	[self setStandardColorsDictKeys:StandardStateLabels forValues:StandardStateColors];
}

- (NSDictionary*)nucleotideColorsDict {

	return nucleotideColorsDict;
}

- (void)setAminoColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray {

	aminoColorsDict = [[NSDictionary alloc] initWithObjects:valueArray forKeys:keysArray];
}

- (void)setNucleotideColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray {

	nucleotideColorsDict = [[NSDictionary alloc] initWithObjects:valueArray forKeys:keysArray];
}

- (void)setStandardColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray {

	standardColorsDict = [[NSDictionary alloc] initWithObjects:valueArray forKeys:keysArray];
}

- (void)splitViewDidResizeSubviews:(NSNotification*)aNotification {

	NSMatrix* taxaMatrix = [taxaMatrices objectAtIndex:[matrixSelector indexOfSelectedItem]];
	float nameWidth = [self lengthOfLongestName:[taxaMatrices objectAtIndex:[matrixSelector indexOfSelectedItem]]];
	
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

@end
