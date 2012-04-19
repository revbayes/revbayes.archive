#import "CharacterData.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolDataEntry.h"
#import "WindowControllerDataEntry.h"



@implementation WindowControllerDataEntry

@synthesize numberOfCharacters;
@synthesize numberOfTaxa;


- (void)awakeFromNib {
	
}

- (IBAction)cancelAction:(id)sender {
	
	[myTool closeControlPanel];
}

- (IBAction)changeBlankDataType:(id)sender {

}

- (void)dealloc {

	[super dealloc];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolDataEntry*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowReadData"]) )
        {
		// initialize the address of the tool associated with this control window
        myTool = t;
		
		// initialize the data
		[self setNumberOfTaxa:[myTool numberOfTaxa]];
		[self setNumberOfCharacters:[myTool numberOfCharacters]];
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

	// perform the action

}

- (void)setControlsEnabledState {


}

- (void)setToolValues {

	[myTool setNumberOfTaxa:[self numberOfTaxa]];
	[myTool setNumberOfCharacters:[self numberOfCharacters]];
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

- (void)windowDidLoad {

}

@end
