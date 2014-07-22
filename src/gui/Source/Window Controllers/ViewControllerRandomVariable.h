#import <Foundation/Foundation.h>



@interface ViewControllerRandomVariable : NSViewController {
    
    IBOutlet NSPopUpButton*                    partitionSelector;
    IBOutlet NSSegmentedControl*               nodeBranchSelector;
	IBOutlet NSTextField*                      distributionLabel;
	IBOutlet NSPopUpButton*                    distributionSelector;
	IBOutlet NSTextField*                      parameter1Label;
	IBOutlet NSPopUpButton*                    parameter1Selector;
	IBOutlet NSTextField*                      parameter2Label;
	IBOutlet NSPopUpButton*                    parameter2Selector;
	IBOutlet NSTextField*                      parameter3Label;
	IBOutlet NSPopUpButton*                    parameter3Selector;
	IBOutlet NSTextField*                      parameter4Label;
	IBOutlet NSPopUpButton*                    parameter4Selector;
    IBOutlet NSButton*                         clampedValueButton;
    IBOutlet NSPopUpButton*                    clampedValueSelector;
	IBOutlet NSTextField*                      nameField;
	IBOutlet NSTextField*                      nameLabel;
    IBOutlet NSButton*                         monitorVariableButton;
    IBOutlet NSStepper*                        vectorSizeStepper;
    IBOutlet NSTextField*                      vectorSizeField;
    IBOutlet NSTextField*                      vectorSizeLabel;
}

@property (nonatomic,weak) NSPopUpButton*      partitionSelector;
@property (nonatomic,weak) NSSegmentedControl* nodeBranchSelector;
@property (nonatomic,weak) NSTextField*        distributionLabel;
@property (nonatomic,weak) NSPopUpButton*      distributionSelector;
@property (nonatomic,weak) NSTextField*        parameter1Label;
@property (nonatomic,weak) NSPopUpButton*      parameter1Selector;
@property (nonatomic,weak) NSTextField*        parameter2Label;
@property (nonatomic,weak) NSPopUpButton*      parameter2Selector;
@property (nonatomic,weak) NSTextField*        parameter3Label;
@property (nonatomic,weak) NSPopUpButton*      parameter3Selector;
@property (nonatomic,weak) NSTextField*        parameter4Label;
@property (nonatomic,weak) NSPopUpButton*      parameter4Selector;
@property (nonatomic,weak) NSButton*           clampedValueButton;
@property (nonatomic,weak) NSPopUpButton*      clampedValueSelector;
@property (nonatomic,weak) NSTextField*        nameField;
@property (nonatomic,weak) NSTextField*        nameLabel;
@property (nonatomic,weak) NSButton*           monitorVariableButton;
@property (nonatomic,weak) NSStepper*          vectorSizeStepper;
@property (nonatomic,weak) NSTextField*        vectorSizeField;
@property (nonatomic,weak) NSTextField*        vectorSizeLabel;

@end
