#import <Cocoa/Cocoa.h>
@class ToolTreeSet;



@interface WindowControllerTreeSet : NSWindowController <NSTabViewDelegate> {

	IBOutlet NSTabView*             treeSourceTab;
    IBOutlet NSButton*              okButton;
    IBOutlet NSButton*              cancelButton;
    IBOutlet NSButton*              exportButton;
    IBOutlet NSButton*              helpButton;
    IBOutlet NSPopUpButton*         numberOfInletsSelector;
    ToolTreeSet*                    myTool;
    int                             numberOfInlets;
}

@property (readwrite) int numberOfInlets;

- (IBAction)cancelAction:(id)sender;
- (IBAction)changeTreeSource:(id)sender;
- (IBAction)changeNumberOfInlets:(id)sender;
- (IBAction)exportAction:(id)sender;
- (IBAction)helpAction:(id)sender;
- (id)initWithTool:(ToolTreeSet*)t;
- (IBAction)okAction:(id)sender;
- (void)setButtonTitles;

@end
