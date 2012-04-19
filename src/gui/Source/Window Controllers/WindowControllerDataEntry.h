#import <Cocoa/Cocoa.h>
#include <vector>
@class ToolDataEntry;
class CharacterData;



@interface WindowControllerDataEntry : NSWindowController <NSTabViewDelegate> {

    ToolDataEntry*                     myTool;
	IBOutlet NSButton*                 okButton;
	int                                numberOfTaxa;
	int                                numberOfCharacters;
}

@property (readwrite) int numberOfCharacters;
@property (readwrite) int numberOfTaxa;

- (IBAction)cancelAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolDataEntry*)t;
- (IBAction)okButtonAction:(id)sender;

@end
