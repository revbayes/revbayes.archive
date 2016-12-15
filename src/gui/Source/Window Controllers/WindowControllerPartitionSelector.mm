#import "ParmTree.h"
#import "RbData.h"
#import "TaxonBipartition.h"
#import "ToolModel.h"
#import "WindowControllerPartitionSelector.h"



@implementation WindowControllerPartitionSelector

@synthesize bpName;

- (void)awakeFromNib {
	
	[firstTaxaView setHasHorizontalScroller:YES];
	[[firstTaxaView horizontalScroller] setControlSize:NSSmallControlSize];
	[firstTaxaView setHasVerticalScroller:YES];
	[[firstTaxaView verticalScroller] setControlSize:NSSmallControlSize];
    
	[secondTaxaView setHasHorizontalScroller:YES];
	[[secondTaxaView horizontalScroller] setControlSize:NSSmallControlSize];
	[secondTaxaView setHasVerticalScroller:YES];
	[[secondTaxaView verticalScroller] setControlSize:NSSmallControlSize];

    // initialize the taxon matrix
    [self initializeTaxonLists];
}

- (IBAction)cancelButtonAction:(id)sender {

    [NSApp stopModal];
    [self close];
}

- (IBAction)changeNameAction:(id)sender {
    
    if ( [myParm isBipartitionNameTaken:bpName] == YES )
        {
        NSString* alertMsg = [NSString stringWithFormat:@"The name \"%@\" has already been used", bpName];

        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Warning: Bipartition name already taken"];
        [alert setInformativeText:alertMsg];
        [alert beginSheetModalForWindow:[self window] completionHandler:nil];

        [self setBpName:[myParm getNextDefaultPartitionName]];
        }
}

- (IBAction)firstTaxaSetsButtonAction:(id)sender {
    
	if ( [[firstTaxaSetsButton titleOfSelectedItem] isEqualToString:@"All Taxa"] == YES )
		{
		[firstMatrix deselectAllCells];
		[firstMatrix selectAll:self];
		[firstTaxaSetsButton selectItemWithTitle:@""];
		}
}

- (IBAction)helpButtonAction:(id)sender {

}

- (IBAction)moveDownButtonAction:(id)sender {
    
	// get the list of the taxa to exclude
	NSArray* lst = [firstMatrix selectedCells];
    if ( [lst count] == 0 )
        return;
    
	// enumerate the list of taxa, moving each to the bottom list
	id element;
	NSEnumerator* charEnum = [lst objectEnumerator];
	while ( (element = [charEnum nextObject]) )
		{        
        // find the row in the list of taxa in top list
        int eRow = -1;
        for (int i=0; i<[firstMatrix numberOfRows]; i++)
            {
            NSCell* aCell = [firstMatrix cellAtRow:i column:0];
            if ( [[aCell stringValue] isEqualToString:[element stringValue]] == YES )
                {
                eRow = i;
                break;
                }
            }
        
        // remove the row
        [firstMatrix removeRow:eRow];
        
        // find the row where the taxon should be inserted
		int chIdx = [[element stringValue] intValue];
        int insertionIdx = (int)[secondMatrix numberOfRows];
        for (int i=0; i<[secondMatrix numberOfRows]; i++)
            {
			int x = [[[secondMatrix cellAtRow:i column:0] stringValue] intValue];
            if ( chIdx < x )
                {
                insertionIdx = i;
                break;
                }
            }
        
        // add the row to the excluded list
        [secondMatrix insertRow:insertionIdx];
        NSTextFieldCell* bCell = [secondMatrix cellAtRow:insertionIdx column:0];
        [bCell setTag:1];
        [bCell setEditable:NO];
        [bCell setSelectable:NO];
        [bCell setDrawsBackground:YES];
        [bCell setBackgroundColor:[NSColor whiteColor]];
        [bCell setTextColor:[NSColor blackColor]];
        [bCell setAlignment:NSLeftTextAlignment];
        [bCell setStringValue:[element stringValue]];
        
        // find the name and remove it from the first list ...
        NSString* nme = [element stringValue];
        for (int i=0; i<[firstTaxonList count]; i++)
            {
            if ( [[firstTaxonList objectAtIndex:i] isEqualToString:nme] == YES )
                {
                [firstTaxonList removeObjectAtIndex:i];
                break;
                }
            }
        
        // ... and add it to the second list
        [secondTaxonList addObject:nme];
		}
    
    NSRect f = [firstMatrix  frame];
    NSRect r = [secondMatrix frame];
    f.size.height = 16.0 * [firstMatrix  numberOfRows];
    r.size.height = 16.0 * [secondMatrix numberOfRows];
	[firstMatrix  setFrame:f];
	[secondMatrix setFrame:r];
	[firstTaxaView  setDocumentView:firstMatrix];
	[secondTaxaView setDocumentView:secondMatrix];    
}

- (IBAction)moveUpButtonAction:(id)sender {

	// get the list of the taxa to exclude
	NSArray* lst = [secondMatrix selectedCells];
    if ( [lst count] == 0 )
        return;
    
	// enumerate the list of taxa, moving each to the bottom list
	id element;
	NSEnumerator* charEnum = [lst objectEnumerator];
	while ( (element = [charEnum nextObject]) )
        {
        // find the row in the list of taxa in top list
        int eRow = -1;
        for (int i=0; i<[secondMatrix numberOfRows]; i++)
            {
            NSCell* aCell = [secondMatrix cellAtRow:i column:0];
            if ( [[aCell stringValue] isEqualToString:[element stringValue]] == YES )
                {
                eRow = i;
                break;
                }
            }
        
        // remove the row
        [secondMatrix removeRow:eRow];
        
        // find the row where the taxon should be inserted
		int chIdx = [[element stringValue] intValue];
        int insertionIdx = (int)[firstMatrix numberOfRows];
        for (int i=0; i<[firstMatrix numberOfRows]; i++)
            {
			int x = [[[firstMatrix cellAtRow:i column:0] stringValue] intValue];
            if ( chIdx < x )
                {
                insertionIdx = i;
                break;
                }
            }
        
        // add the row to the excluded list
        [firstMatrix insertRow:insertionIdx];
        NSTextFieldCell* bCell = [firstMatrix cellAtRow:insertionIdx column:0];
        [bCell setTag:1];
        [bCell setEditable:NO];
        [bCell setSelectable:NO];
        [bCell setDrawsBackground:YES];
        [bCell setBackgroundColor:[NSColor whiteColor]];
        [bCell setTextColor:[NSColor blackColor]];
        [bCell setAlignment:NSLeftTextAlignment];
        [bCell setStringValue:[element stringValue]];
        
        // find the name and remove it from the first list ...
        NSString* nme = [element stringValue];
        for (int i=0; i<[secondTaxonList count]; i++)
            {
            if ( [[secondTaxonList objectAtIndex:i] isEqualToString:nme] == YES )
                {
                [secondTaxonList removeObjectAtIndex:i];
                break;
                }
            }
        
        // ... and add it to the second list
        [firstTaxonList addObject:nme];
        }
    
    NSRect f = [firstMatrix  frame];
    NSRect r = [secondMatrix frame];
    f.size.height = 16.0 * [firstMatrix  numberOfRows];
    r.size.height = 16.0 * [secondMatrix numberOfRows];
	[firstMatrix  setFrame:f];
	[secondMatrix setFrame:r];
	[firstTaxaView  setDocumentView:firstMatrix];
	[secondTaxaView setDocumentView:secondMatrix];
}

- (id)init {

    return [self initWithParm:nil andTool:nil];
}

- (id)initWithParm:(ParmTree*)p andTool:(ToolModel*)t{

	if ( (self = [super initWithWindowNibName:@"ControlWindowPartitionSelector"]) )
        {
        myParm          = p;
        myTool          = t;
		cellHeight      = 16.0;
		firstTaxonList  = [[NSMutableArray alloc] init];
        secondTaxonList = [[NSMutableArray alloc] init];
		taxonList       = [[NSMutableArray alloc] init];
        
        // set up an array of the taxon names
        NSMutableArray* dm = [myTool dataMatrices];
        id matrix = [dm objectAtIndex:0];
        if ( [[matrix className] isEqualToString:@"RbData"] == YES )
            {
            int nRows = [matrix numTaxa];
            for (int i=0; i<nRows; i++)
                {
                NSString* ts = [NSString stringWithString:[matrix taxonWithIndex:i]];
                [taxonList addObject:ts];
                [firstTaxonList addObject:ts];
                }
            }
    
        // get the length of the longest taxon name
        longestNameLength = [self lengthOfLongestName];
        
        // get the default name of the bipartition
        bpName = [[NSString alloc] init];
        [self setBpName:[myParm getNextDefaultPartitionName]];
        }
	return self;
}

- (float)lengthOfLongestName:(RbData*)matrix {

    return 0.0;
}

- (IBAction)okButtonAction:(id)sender {
    
    // check that the bipartition the user selected is not trivial
    if ( [firstTaxonList count] == 0 || [secondTaxonList count] == 0 )
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Warning: Bipartition mis-specification"];
        [alert setInformativeText:@"This bipartition does not specify a branch on a tree"];
        [alert beginSheetModalForWindow:[self window] completionHandler:nil];
        }
    else 
        {
        TaxonBipartition* newBp = [[TaxonBipartition alloc] init];
        [newBp setPartitionName:[self bpName]];
        [newBp setPartitionType:USER_PART];
        
        NSString* firstName = [taxonList objectAtIndex:0];
        BOOL isFirstNameInFirstList = NO;
        for (int i=0; i<[firstTaxonList count]; i++)
            {
            if ( [[firstTaxonList objectAtIndex:i] isEqualToString:firstName] == YES )
                {
                isFirstNameInFirstList = YES;
                break;
                }
            }
        if (isFirstNameInFirstList == YES)
            {
            for (int i=0; i<[firstTaxonList count]; i++)
                [newBp addNameToPartitionSet:[firstTaxonList objectAtIndex:i]];
            }
        else 
            {
            for (int i=0; i<[secondTaxonList count]; i++)
                [newBp addNameToPartitionSet:[secondTaxonList objectAtIndex:i]];
            }
        
        [myParm addUserBipartition:newBp];
        [NSApp stopModal];
        [self close];
        }
}

- (IBAction)secondTaxaSetsButtonAction:(id)sender {
    
	if ( [[secondTaxaSetsButton titleOfSelectedItem] isEqualToString:@"All Taxa"] == YES )
		{
		[secondMatrix deselectAllCells];
		[secondMatrix selectAll:self];
		[secondTaxaSetsButton selectItemWithTitle:@""];
		}
}

- (void)setMatrixList {

}

- (BOOL)setMatrixInfo:(RbData*)matrix {

    return YES;
}

- (void)showControlPanel {
    
	[self showWindow:self];
	[[self window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[self window]];
}

- (void)initializeTaxonLists {
    
    // fill in the name
    //[nameField setStringValue:name];
    
	// initialize some important variables
	NSRect aRect = NSMakeRect(0.0,0.0,0.0,(cellHeight*1));
	
	// create taxa matrix
	firstMatrix = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:0 numberOfColumns:1]; 
	[firstMatrix setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[firstMatrix setAllowsEmptySelection:YES];
	[firstMatrix setDoubleAction:@selector(moveDownButtonAction:)];
    
	secondMatrix = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:0 numberOfColumns:1]; 
	[secondMatrix setIntercellSpacing:NSMakeSize(0.0,0.0)];
	[secondMatrix setAllowsEmptySelection:YES];
	[secondMatrix setDoubleAction:@selector(moveUpButtonAction:)];
	
	for (int i=0; i<[firstTaxonList count]; i++)
		{
        [firstMatrix addRow];
        NSTextFieldCell* aCell = [firstMatrix cellAtRow:([firstMatrix numberOfRows]-1) column:0];
        [aCell setTag:1];
        [aCell setEditable:NO];
        [aCell setSelectable:NO];
        [aCell setDrawsBackground:YES];
        [aCell setBackgroundColor:[NSColor whiteColor]];
        [aCell setTextColor:[NSColor blackColor]];
        [aCell setAlignment:NSLeftTextAlignment];
        NSString* tn = [firstTaxonList objectAtIndex:i];
        [aCell setStringValue:tn];
		}

	
    for (int i=0; i<[secondTaxonList count]; i++)
		{
        [secondMatrix addRow];
        NSTextFieldCell* aCell = [secondMatrix cellAtRow:([secondMatrix numberOfRows]-1) column:0];
        [aCell setTag:1];
        [aCell setEditable:NO];
        [aCell setSelectable:NO];
        [aCell setDrawsBackground:YES];
        [aCell setBackgroundColor:[NSColor whiteColor]];
        [aCell setTextColor:[NSColor blackColor]];
        [aCell setAlignment:NSLeftTextAlignment];
        NSString* tn = [secondTaxonList objectAtIndex:i];
        [aCell setStringValue:tn];
		}

    float nameWidth = longestNameLength; 
	float cellWidth = [firstTaxaView visibleRect].size.width - 16.0;
	if (nameWidth < cellWidth)
		nameWidth = cellWidth;
    
	[firstMatrix  setCellSize:NSMakeSize(nameWidth, cellHeight)];
	[secondMatrix setCellSize:NSMakeSize(nameWidth, cellHeight)];
	[firstMatrix  setFrame:NSMakeRect(0.0,0.0,nameWidth,(cellHeight*[firstMatrix  numberOfRows]))];
	[secondMatrix setFrame:NSMakeRect(0.0,0.0,nameWidth,(cellHeight*[secondMatrix numberOfRows]))];

	[firstTaxaView  setDocumentView:firstMatrix];
	[secondTaxaView setDocumentView:secondMatrix];

	// set the taxa and character sets to some default values
	[firstTaxaSetsButton removeAllItems];
	[firstTaxaSetsButton addItemWithTitle:@""];
	[firstTaxaSetsButton addItemWithTitle:@"All Taxa"];
	[secondTaxaSetsButton removeAllItems];
	[secondTaxaSetsButton addItemWithTitle:@""];
	[secondTaxaSetsButton addItemWithTitle:@"All Taxa"];
}

- (float)lengthOfLongestName {
    
	NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
	[attrs setObject:[NSFont systemFontOfSize:12.0] forKey:NSFontAttributeName];
    
	float longestName = 0.0;
	for (int i=0; i<[firstTaxonList count]; i++)
		{
		NSSize aStrSize = [[[NSAttributedString alloc] initWithString:[taxonList objectAtIndex:i] attributes:attrs] size];
		if (aStrSize.width > longestName)
			longestName = aStrSize.width;
		}
    
	longestName += 10.8;
	int x = longestName;
	x++;
	longestName = (float)x;
    
	return longestName;
}

@end
