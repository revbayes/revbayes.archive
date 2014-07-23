#import <Cocoa/Cocoa.h>



@interface ViewControllerConstant : NSViewController {

    IBOutlet NSPopUpButton*                    partitionSelector;
    IBOutlet NSSegmentedControl*               nodeBranchSelector;
    IBOutlet NSComboBox*                       valueEntry;
    IBOutlet NSScrollView*                     valueScroll;
    IBOutlet NSPopUpButton*                    valueShortcutSelector;
	IBOutlet NSTextField*                      nameField;
	IBOutlet NSTextField*                      nameLabel;
    IBOutlet NSStepper*                        vectorSizeStepper;
    IBOutlet NSTextField*                      vectorSizeField;
    IBOutlet NSTextField*                      vectorSizeLabel;
}

@property (nonatomic,weak) NSPopUpButton*      partitionSelector;
@property (nonatomic,weak) NSSegmentedControl* nodeBranchSelector;
@property (nonatomic,weak) NSComboBox*         valueEntry;
@property (nonatomic,weak) NSScrollView*       valueScroll;
@property (nonatomic,weak) NSPopUpButton*      valueShortcutSelector;
@property (nonatomic,weak) NSTextField*        nameField;
@property (nonatomic,weak) NSTextField*        nameLabel;
@property (nonatomic,weak) NSStepper*          vectorSizeStepper;
@property (nonatomic,weak) NSTextField*        vectorSizeField;
@property (nonatomic,weak) NSTextField*        vectorSizeLabel;

@end
