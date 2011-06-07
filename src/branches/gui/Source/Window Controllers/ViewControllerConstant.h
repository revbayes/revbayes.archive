#import <Cocoa/Cocoa.h>



@interface ViewControllerConstant : NSViewController {

    IBOutlet NSPopUpButton*       partitionSelector;
    IBOutlet NSSegmentedControl*  nodeBranchSelector;
    IBOutlet NSComboBox*          valueEntry;
    IBOutlet NSScrollView*        valueScroll;
    IBOutlet NSPopUpButton*       valueShortcutSelector;
	IBOutlet NSTextField*         nameField;
	IBOutlet NSTextField*         nameLabel;
    IBOutlet NSStepper*           vectorSizeStepper;
    IBOutlet NSTextField*         vectorSizeField;
    IBOutlet NSTextField*         vectorSizeLabel;
}

@property (assign) NSPopUpButton*      partitionSelector;
@property (assign) NSSegmentedControl* nodeBranchSelector;
@property (assign) NSComboBox*         valueEntry;
@property (assign) NSScrollView*       valueScroll;
@property (assign) NSPopUpButton*      valueShortcutSelector;
@property (assign) NSTextField*        nameField;
@property (assign) NSTextField*        nameLabel;
@property (assign) NSStepper*          vectorSizeStepper;
@property (assign) NSTextField*        vectorSizeField;
@property (assign) NSTextField*        vectorSizeLabel;

@end
