#import <Cocoa/Cocoa.h>



@interface ViewControllerConstant : NSViewController {

    IBOutlet NSPopUpButton*        partitionSelector;
    IBOutlet NSSegmentedControl*   nodeBranchSelector;
    IBOutlet NSComboBox*           valueEntry;
    IBOutlet NSScrollView*         valueScroll;
    IBOutlet NSPopUpButton*        valueShortcutSelector;
	IBOutlet NSTextField*          nameField;
	IBOutlet NSTextField*          nameLabel;
    IBOutlet NSStepper*            vectorSizeStepper;
    IBOutlet NSTextField*          vectorSizeField;
    IBOutlet NSTextField*          vectorSizeLabel;
}

@property (nonatomic,strong) NSPopUpButton*      partitionSelector;
@property (nonatomic,strong) NSSegmentedControl* nodeBranchSelector;
@property (nonatomic,strong) NSComboBox*         valueEntry;
@property (nonatomic,strong) NSScrollView*       valueScroll;
@property (nonatomic,strong) NSPopUpButton*      valueShortcutSelector;
@property (nonatomic,strong) NSTextField*        nameField;
@property (nonatomic,strong) NSTextField*        nameLabel;
@property (nonatomic,strong) NSStepper*          vectorSizeStepper;
@property (nonatomic,strong) NSTextField*        vectorSizeField;
@property (nonatomic,strong) NSTextField*        vectorSizeLabel;

@end
