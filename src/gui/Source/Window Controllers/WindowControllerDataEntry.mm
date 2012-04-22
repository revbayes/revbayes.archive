#import "CharacterData.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolDataEntry.h"
#import "WindowControllerDataEntry.h"



@implementation WindowControllerDataEntry

- (void)awakeFromNib {
	
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

	if ( (self = [super initWithWindowNibName:@"ControlWindowDataEntry"]) )
        {
        myTool = t;
        }
    return self;
}

- (IBAction)okButtonAction:(id)sender {

    [myTool closeControlPanel];
}

@end
