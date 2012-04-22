#import "MatrixController.h"
#import "MatrixRowInformation.h"



@implementation MatrixController

@synthesize numCharacters;

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
    //NSTextFieldCell* tc = [newColumn dataCell];
    //[tc setTextColor:[NSColor redColor]];

    [tableView reloadData];
}

- (IBAction)addRow:(id)sender {

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
        NSLog(@"whichColumnToDelete = %d", whichColumnToDelete);
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
        }
}

- (IBAction)deleteRow:(id)sender {

    NSIndexSet* selectedRows = [tableView selectedRowIndexes];
    [tableView abortEditing];
    [rowData removeObjectsAtIndexes:selectedRows];
    [tableView reloadData];
}

- (id)init {

    if ( (self = [super init]) ) 
        {
        rowData = [[NSMutableArray alloc] init];
        
        // TODO: Should read a data object eventually
        numCharacters = 1;
        for (int i=0; i<3; i++)
            [self addRow:self];
        }
    return self;
}

- (void)keyDown:(NSEvent*)event {
    
    NSString *chars = [event characters];
    unichar character = [chars characterAtIndex:0];
    if (character == NSDeleteCharacter)
		{
        NSLog(@"deleting!");
        }
}

- (int)numberOfColumns {

    MatrixRowInformation* d = [rowData objectAtIndex:0];
    return [d numberOfColumns];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView {

    return [rowData count];
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
