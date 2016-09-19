//#import "CharacterData.h"
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


- (void)awakeFromNib {
	
	[matrixTypeTab selectTabViewItemAtIndex:[myTool matrixType]];
    [fileFormatDeterminationMethod selectItemWithTag:isDataFormatAutomaticallyDetermined];
	[self setControlsEnabledState];
	[self setControlWindowSize];
}

- (IBAction)cancelAction:(id)sender {
	
	[self setToolValues];
	[myTool closeControlPanelWithCancel];
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

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolReadData*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowReadData"]) )
        {
		// allocate objects
        fileName = @"";
        pathName = @"";

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
        [self setIsDataFormatAutomaticallyDetermined:YES];
        }
	return self;
}

- (BOOL)isBlankMatrixOfDataType:(NSString*)dt {

    if ( [[dataTypeButton2 titleOfSelectedItem] isEqualToString:dt] == YES )
        return YES;
    return NO;
}

- (BOOL)makeBlankMatrix {

	NSString* tabViewLabel = [NSString stringWithString:[[matrixTypeTab selectedTabViewItem] label]];
	if ( [tabViewLabel isEqualToString:@"Data Matrix"] == YES )
        return NO;
    return YES;
}

- (IBAction)okButtonAction:(id)sender {

	// remember the state of the control panel
	[self setToolValues];
    
	// perform the action
    [myTool closeControlPanelWithOK];
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
