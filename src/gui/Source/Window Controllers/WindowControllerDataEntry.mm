#import "MatrixRowInformation.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolDataEntry.h"
#import "WindowControllerDataEntry.h"



@implementation WindowControllerDataEntry

@synthesize numCharacters;
@synthesize numTaxa;

- (IBAction)addColumn:(id)sender {

    numCharacters++;
    
    for (int i=0; i<[rowData count]; i++)
        {
        MatrixRowInformation* rd = [rowData objectAtIndex:i];
        [rd addEntryToEnd];
        }

    NSTableColumn* newColumn = [[NSTableColumn alloc] init];
    [tableView addTableColumn:newColumn];
    [[newColumn headerCell] setStringValue:[NSString stringWithFormat:@"%d", numCharacters]];
    [newColumn setIdentifier:[NSString stringWithFormat:@"%d", numCharacters]];
    [newColumn setWidth:25.0];
    [newColumn setMaxWidth:25.0];
    [newColumn setMinWidth:25.0];

    [tableView reloadData];
}

- (IBAction)addRow:(id)sender {

    numTaxa++;
    [rowData addObject:[[MatrixRowInformation alloc] initWithController:self]];
    [tableView reloadData];
}

- (void)awakeFromNib {
	
    // remove all of the table columns
    NSArray* theTableColumns = [tableView tableColumns];
    for (int i=(int)[theTableColumns count]-1; i>=0; i--)
        {
        NSTableColumn* tc = [theTableColumns objectAtIndex:i];
        [tableView removeTableColumn:tc];
        }
    
    // add columns back in
    int targetNumberOfColumns = [self numberOfColumns];
    for (int i=0; i<targetNumberOfColumns; i++)
        {
        NSTableColumn* newColumn = [[NSTableColumn alloc] init];
        [tableView addTableColumn:newColumn];
        if (i == 0)
            {
            [[newColumn headerCell] setStringValue:@"Taxon Name"];
            [newColumn setIdentifier:[NSString stringWithString:@"Taxon Name"]];
            }
        else 
            {
            [[newColumn headerCell] setStringValue:[NSString stringWithFormat:@"%d", i]];
            [newColumn setIdentifier:[NSString stringWithFormat:@"%d", i]];
            [newColumn setWidth:25.0];
            [newColumn setMaxWidth:25.0];
            [newColumn setMinWidth:25.0];
            }
        }
        
    // loop over the data, filling in each data element appropriately
    RbData* d = [myTool dataMatrix];
    for (int i=0; i<[d numTaxa]; i++)
        {
        MatrixRowInformation* ri = [rowData objectAtIndex:i];
        [ri setValue:[d taxonWithIndex:i] forKey:@"Taxon Name"];
        for (int j=0; j<[d numCharacters]; j++)
            {
            RbDataCell* c = [d cellWithRow:i andColumn:j];
            char charVal = [c getDiscreteState];
            NSLog(@"%d %d -- %c", i, j, charVal);
            [ri setValue:[NSString stringWithFormat:@"%c", charVal] forKey:[NSString stringWithFormat:@"%d", j+1]];
            
            }
        }
}

- (ToolDataEntry*)dataEntryTool {

    return myTool;
}

- (void)dealloc {

    [rowData release];
	[super dealloc];
}

- (IBAction)deleteColumn:(id)sender {

    int whichColumnToDelete = (int)[tableView selectedColumn];
    [tableView abortEditing];
    if ( whichColumnToDelete != -1 && whichColumnToDelete != 0 )
        {
        for (int i=0; i<[rowData count]; i++)
            {
            MatrixRowInformation* rd = [rowData objectAtIndex:i];
            [rd deleteCharacterWithIndex:whichColumnToDelete];
            }
            
        NSArray* theTableColumns = [tableView tableColumns];
        NSTableColumn* columnToDelete = nil;
        for (int i=1, j=1; i<[theTableColumns count]; i++)
            {
            NSTableColumn* tc = [theTableColumns objectAtIndex:i];
            if ( i == whichColumnToDelete )
                {
                columnToDelete = tc;
                }
            else 
                {
                [tc setIdentifier:[NSString stringWithFormat:@"%d", j]];
                [[tc headerCell] setStringValue:[NSString stringWithFormat:@"%d", j]];
                j++;
                }
            }
        if (columnToDelete != nil)
            [tableView removeTableColumn:columnToDelete];
        [tableView reloadData];

        numCharacters--;
        if (numCharacters < 0)
            numCharacters = 0;
        }
}

- (IBAction)deleteRow:(id)sender {

    numTaxa--;
    if (numTaxa < 0)
        numTaxa = 0;
    NSIndexSet* selectedRows = [tableView selectedRowIndexes];
    [tableView abortEditing];
    [rowData removeObjectsAtIndexes:selectedRows];
    [tableView reloadData];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolDataEntry*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowDataEntry"]) )
        {
        NSLog(@"start initWithTool for %@", self);
        myTool = t;
        rowData = [[NSMutableArray alloc] init];
        
        RbData* d = [myTool dataMatrix];
        numCharacters = [d numCharacters];
        numTaxa = 0; // this will be incremented, below, when we add the rows to the matrix one-by-one
        
        NSLog(@"d = %@", d);
        NSLog(@"numCharacters = %d", numCharacters);
        NSLog(@"numTaxa = %d", numTaxa);
        
        for (int i=0; i<[d numTaxa]; i++)
            [self addRow:self];
            
        NSLog(@"1");

        for (int i=0; i<[rowData count]; i++)
            {
            MatrixRowInformation* r = [rowData objectAtIndex:i];
            int rSize = [r numberOfColumns];
            for (int j=1; j<rSize; j++)
                [r deleteCharacterWithIndex:rSize-j];
            for (int j=0; j<numCharacters; j++)
                [r addEntryToEnd];
            }
        NSLog(@"end initWithTool for %@", self);
        }
    return self;
}

- (IBAction)okButtonAction:(id)sender {

    [self saveMatrixToTool];
    [myTool closeControlPanel];
}

- (int)numberOfColumns {

    MatrixRowInformation* d = [rowData objectAtIndex:0];
    return [d numberOfColumns];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView {

    return [rowData count];
}

- (void)saveMatrixToTool {

    RbData* d = [myTool dataMatrix];
    [d setStandardMatrixToHave:numTaxa andToHave:numCharacters];
    
    for (int i=0; i<[d numTaxa]; i++)
        {
        RbTaxonData* td = [d getDataForTaxonIndexed:i];
        MatrixRowInformation* rd = [rowData objectAtIndex:i];
        NSString* str = [NSString stringWithString:[[rowData objectAtIndex:i] valueForKey:@"Taxon Name"]];
        [d setNameOfTaxonWithIndex:i to:str];
        for (int j=0; j<[td numCharacters]; j++)
            {
            RbDataCell* dc = [td dataCellIndexed:j];
            int v = [rd integerRepresentationForCharacter:j];
            [dc setDiscreteStateTo:v];
            }
        }
}

- (id)tableView:(NSTableView*)tableView objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {

    MatrixRowInformation* d = [rowData objectAtIndex:row];
    NSString* identifier = [tableColumn identifier];
    return [d valueForKey:identifier];
}

- (void)tableView:(NSTableView *)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {

    MatrixRowInformation* d = [rowData objectAtIndex:row];
    NSString* identifier = [tableColumn identifier];
    [d setValue:object forKey:identifier];
}

@end
