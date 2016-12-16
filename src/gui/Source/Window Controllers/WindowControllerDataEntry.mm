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
            [newColumn setIdentifier:@"Taxon Name"];
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
            [ri setValue:[NSString stringWithFormat:@"%c", charVal] forKey:[NSString stringWithFormat:@"%d", j+1]];
            
            }
        }
}

- (BOOL)checkValidityOfCharacterState:(id)cs {

    NSString* newCharVal = [NSString stringWithString:cs];
    if ( [newCharVal length] != 1 )
        return NO;
    char v = [newCharVal characterAtIndex:0];
    if ( v != '?' && v != '0' && v != '1' && v != '2' && v != '3' && 
         v != '4' && v != '5' && v != '6' && v != '7' && v != '8' && v != '9' )
        return NO;
    return YES;
}

- (ToolDataEntry*)dataEntryTool {

    return myTool;
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

    int whichRowToDelete = (int)[tableView selectedRow];
    if (whichRowToDelete != -1)
        {
        numTaxa--;
        if (numTaxa < 0)
            numTaxa = 0;
        [tableView abortEditing];
        [rowData removeObjectAtIndex:whichRowToDelete];
        [tableView reloadData];
        }
}

- (IBAction)helpButtonAction:(id)sender {

    NSString* locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"DataEntryTool_Anchor" inBook:locBookName];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolDataEntry*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowDataEntry"]) )
        {
        myTool = t;
        rowData = [[NSMutableArray alloc] init];
        
        RbData* d = [myTool dataMatrix];
        numCharacters = [d numCharacters];
        numTaxa = 0; // this will be incremented, below, when we add the rows to the matrix one-by-one
        
        for (int i=0; i<[d numTaxa]; i++)
            [self addRow:self];
            
        for (int i=0; i<[rowData count]; i++)
            {
            MatrixRowInformation* r = [rowData objectAtIndex:i];
            int rSize = [r numberOfColumns];
            for (int j=1; j<rSize; j++)
                [r deleteCharacterWithIndex:rSize-j];
            for (int j=0; j<numCharacters; j++)
                [r addEntryToEnd];
            }
        }
    return self;
}

- (IBAction)okButtonAction:(id)sender {

    [self saveMatrixToTool];
    [myTool closeControlPanel];
    
    [myTool removeDataInspector];
    [myTool makeDataInspector];
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
        [td setTaxonName:str];
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

- (void)tableView:(NSTableView *)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {

    MatrixRowInformation* d = [rowData objectAtIndex:row];
    NSString* identifier = [tableColumn identifier];
    BOOL isValidCharacterEntry = YES;
    if ( [identifier isEqualToString:@"Taxon Name"] == NO )
        isValidCharacterEntry = [self checkValidityOfCharacterState:object];
    if ( isValidCharacterEntry == NO )
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Warning: Invalid character state"];
        [alert setInformativeText:@"Valid character states are the integers from 0 to 9 and \"?\""];
        [alert runModal];
        }
    else 
        {
        [d setValue:object forKey:identifier];
        }
}

@end
