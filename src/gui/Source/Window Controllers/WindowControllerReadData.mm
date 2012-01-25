#import "CharacterData.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolReadData.h"
#import "WindowControllerReadData.h"



@implementation WindowControllerReadData

@synthesize dataAlignment;
@synthesize dataFormat;
@synthesize dataInterleaved;
@synthesize dataType;
@synthesize dataTypeSimulated;
@synthesize numberOfCharacters;
@synthesize numberOfTaxa;
@synthesize numberOfStates;
@synthesize fileName;
@synthesize pathName;
@synthesize isDataFormatAutomaticallyDetermined;

- (void)addBlankDataMatrix {

    // allocate the matrix
    RbData* m = [[RbData alloc] init];
    
    // set the dimensions
    [m setNumTaxa:[self numberOfTaxa]];
    [m setNumCharacters:[self numberOfCharacters]];
    
    // set the name
    [m setName:@"Blank Data Matrix"];
    
    // set the data type
    if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"DNA"] == YES )
        [m setDataType:DNA];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"RNA"] == YES )
        [m setDataType:RNA];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Protein"] == YES )
        [m setDataType:AA];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Standard"] == YES )
        [m setDataType:STANDARD];
    else if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Continuous"] == YES )
        [m setDataType:CONTINUOUS];
        
    // set a flag if the data is discrete or continuous
    BOOL isDiscrete = YES;
    if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:@"Continuous"] == YES )
        isDiscrete = NO;
        
    // fill in the matrix with missing data entries
    for (int i=0; i<[m numTaxa]; i++)
        {
        NSString* tn = [NSString stringWithFormat:@"Taxon_%d", i+1];
        [m addTaxonName:tn];
        RbTaxonData* td = [[RbTaxonData alloc] init];
        [td setTaxonName:tn];
        for (int j=0; j<[m numCharacters]; j++)
            {
            RbDataCell* cell = [[RbDataCell alloc] init];
            if ( isDiscrete == YES )
                {
                [cell setIsDiscrete:YES];
                [cell setDataType:[m dataType]];
                if ( [m dataType] == DNA || [m dataType] == RNA )
                    {
                    [cell setNumStates:4];
                    [cell setVal:[NSNumber numberWithUnsignedInt:[self missingForNumStates:4]]];
                    }
                else if ( [m dataType] == AA )
                    {
                    [cell setNumStates:20];
                    [cell setVal:[NSNumber numberWithUnsignedInt:[self missingForNumStates:20]]];
                   }
                else if ( [m dataType] == STANDARD )
                    {
                    [cell setNumStates:2];
                    [cell setVal:[NSNumber numberWithUnsignedInt:[self missingForNumStates:2]]];
                    }
                
                    
                }
            else 
                {
                NSNumber* n = [NSNumber numberWithDouble:0.0];
                [cell setVal:n];
                [cell setIsDiscrete:NO];
                [cell setDataType:CONTINUOUS];
                [cell setNumStates:0];
                }
            [cell setRow:i];
            [cell setColumn:j];
            [td addObservation:cell];
            [cell release];
            }
        [m addTaxonData:td];
        }
        
    // add the matrix to the tool
    [myTool addMatrix:m];
}

- (void)awakeFromNib {
	
	[matrixTypeTab selectTabViewItemAtIndex:[myTool matrixType]];
	[self setControlsEnabledState];
	[self setControlWindowSize];
}

- (IBAction)cancelAction:(id)sender {
	
	[self setToolValues];
	[myTool closeControlPanel];
}

- (IBAction)changeBlankDataType:(id)sender {

}

- (IBAction)changeDataType:(id)sender {

}

- (IBAction)changeFileFormat:(id)sender {

	[self setControlsEnabledState];
}

- (IBAction)changeFileFormatDeterminationMethod:(id)sender {

    [self setControlsEnabledState];
}

- (void)dealloc {

	[fileName release];
	[pathName release];
	[super dealloc];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolReadData*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowReadData"]) )
        {
		// allocate objects
		fileName = [[NSString alloc] initWithString:@""];
		pathName = [[NSString alloc] initWithString:@""];

		// initialize the address of the tool associated with this control window
        myTool = t;
		
		// initialize the data
		[self setDataAlignment:[myTool dataAlignment]];
        [self setDataFormat:[myTool dataFormat]];
		[self setDataType:[myTool dataType]];
		[self setDataTypeSimulated:[myTool dataTypeSimulated]];
		[self setDataInterleaved:[myTool dataInterleaved]];
		[self setNumberOfTaxa:[myTool numberOfTaxa]];
		[self setNumberOfCharacters:[myTool numberOfCharacters]];
        [self setNumberOfStates:2];
		[self setFileName:[myTool fileName]];
		[self setPathName:[myTool pathName]];
        }
	return self;
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

    // set the tool state to unresolved
    [myTool setIsResolved:NO];
    
	// remember the state of the control panel
	[self setToolValues];

	// perform the action
	NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
        {
		// user selected "OK" for a data matrix (to be read into computer memory)
        [myTool closeControlPanel];
		BOOL isSuccessful = [myTool readDataFile];
		if (isSuccessful == YES)
			{
			}
		else 
			{
			}
		}
	else 
		{
		// user selected "OK" for a blank data matrix
		[myTool closeControlPanel];
		[myTool removeAllDataMatrices];
        [self addBlankDataMatrix];
		[myTool updateForChangeInState];
        [myTool setIsResolved:YES];
		}
}

- (void)setControlWindowSize {

	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;

	NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
		{
        newFrame.size.height = 290.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];
		}
	else if ( [tabViewLabel isEqualToString:@"Blank Matrix"] == YES )
		{
        newFrame.size.height = 220.0;
        newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
        [[self window] setFrame:newFrame display:YES animate:YES];
		}
}

- (void)setControlsEnabledState {

	if ( [[fileFormatDeterminationMethod titleOfSelectedItem] isEqualToString:@"By RevBayes"] == YES )
        {
        [self setIsDataFormatAutomaticallyDetermined:YES];
        [dataTypeButton1         setEnabled:NO];
        [dataFormatButton        setEnabled:NO];
        [dataAlignmentButton     setEnabled:NO];
        [interleavedFormatButton setEnabled:NO];
        [dataFormatField         setEnabled:NO];
        [dataTypeField           setEnabled:NO];
        [dataAlignmentField      setEnabled:NO]; 
        [interleavedFormatField  setEnabled:NO];
        [dataFormatField        setTextColor:[NSColor grayColor]];
        [dataTypeField          setTextColor:[NSColor grayColor]];
        [dataAlignmentField     setTextColor:[NSColor grayColor]];
        [interleavedFormatField setTextColor:[NSColor grayColor]];
        }
    else 
        {
        [dataTypeButton1         setEnabled:YES];
        [dataFormatButton        setEnabled:YES];
        [dataAlignmentButton     setEnabled:YES];
        [interleavedFormatButton setEnabled:YES];
        [dataFormatField         setEnabled:YES];
        [dataTypeField           setEnabled:YES];
        [dataAlignmentField      setEnabled:YES]; 
        [interleavedFormatField  setEnabled:YES];
        
        [dataFormatField setTextColor:[NSColor blackColor]];
        [dataAlignmentButton setEnabled:NO];
        [dataAlignmentField setTextColor:[NSColor grayColor]];
        
        NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
        if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
            {
            if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"NEXUS"] == YES)
                {
                [dataTypeButton1 setEnabled:NO];
                [interleavedFormatButton setEnabled:NO];
                [dataTypeField setTextColor:[NSColor grayColor]];
                [interleavedFormatField setTextColor:[NSColor grayColor]];
                }
            else if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"FASTA"] == YES)
                {
                [dataTypeButton1 setEnabled:YES];
                [interleavedFormatButton setEnabled:NO];
                [dataTypeField setTextColor:[NSColor blackColor]];
                [interleavedFormatField setTextColor:[NSColor grayColor]];
                [dataAlignmentButton setEnabled:YES];
                [dataAlignmentField setTextColor:[NSColor blackColor]];
                }
            else if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"Unknown"] == YES)
                {
                [dataTypeButton1 setEnabled:NO];
                [interleavedFormatButton setEnabled:NO];
                [dataTypeField setTextColor:[NSColor grayColor]];
                [interleavedFormatField setTextColor:[NSColor grayColor]];
                }
            else 
                {
                [dataTypeButton1 setEnabled:YES];
                [interleavedFormatButton setEnabled:YES];
                [dataTypeField setTextColor:[NSColor blackColor]];
                [interleavedFormatField setTextColor:[NSColor blackColor]];
                }
            }
        }
}

- (void)setToolValues {

	[myTool setDataAlignment:[self dataAlignment]];
	[myTool setDataFormat:[self dataFormat]];
	[myTool setDataType:[self dataType]];
	[myTool setDataTypeSimulated:[self dataTypeSimulated]];
	[myTool setDataInterleaved:[self dataInterleaved]];
	[myTool setNumberOfTaxa:[self numberOfTaxa]];
	[myTool setNumberOfCharacters:[self numberOfCharacters]];
	[myTool setFileName:[self fileName]];
	[myTool setPathName:[self pathName]];
	[myTool setMatrixType:(int)[matrixTypeTab indexOfTabViewItem:[matrixTypeTab selectedTabViewItem]]];
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {

	NSString* tabViewLabel = [NSString stringWithString:[tabViewItem label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
        {
        [okButton setTitle:(@"Import")];
        }
	else if ( [tabViewLabel isEqualToString:@"Blank Matrix"] == YES )
        {
        [okButton setTitle:(@"OK")];
        }
	[self setControlWindowSize];
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
    
	if ( [[item title] isEqualToString:@"Standard"] == YES )
		{
		if ( [[dataFormatButton titleOfSelectedItem] isEqualToString:@"FASTA"] == YES )
			return NO;
		}
	return YES;
}

- (void)windowDidLoad {

}

@end
