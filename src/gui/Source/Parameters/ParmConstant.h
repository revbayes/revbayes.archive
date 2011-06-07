#import <Foundation/Foundation.h>
#import "Parm.h"
@class ToolModel;
@class Value;
@class ViewControllerConstant;



@interface ParmConstant : Parm <NSCoding> {
    
    NSPopUpButton*                            partitionSelector;
    NSSegmentedControl*                       nodeBranchSelector;
    NSComboBox*                               valueEntry;
    NSScrollView*                             valueScroll;
    NSPopUpButton*                            valueShortcutSelector;
    NSStepper*                                vectorSizeStepper;
    NSTextField*                              vectorSizeField;
    NSTextField*                              vectorSizeLabel;

    IBOutlet NSBox*                           box;
    IBOutlet ViewControllerConstant*          treePlateView;
    IBOutlet ViewControllerConstant*          parameterView;
    IBOutlet ViewControllerConstant*          treePlateVectorView;
    IBOutlet ViewControllerConstant*          parameterVectorView;

    BOOL                                      appliesToNode;
    int                                       indexOfSelectedPartitionModel;
    int                                       valueSource;
    int                                       whichView;
    NSMutableArray*                           constantValues;
    
    BOOL                                      savedAppliesToNode;
    int                                       savedIndexOfSelectedPartitionModel;
    int                                       savedValueSource;
    NSMutableArray*                           savedConstantValues;
}

@property (readwrite) BOOL appliesToNode;

- (Value*)allocateValue;
- (IBAction)changeDimensionsActionField:(id)sender;
- (IBAction)changeDimensionsActionStepper:(id)sender;
- (IBAction)changeNodeOrBranchSelection:(id)sender;
- (IBAction)changePartitionSelectionAction:(id)sender;
- (void)chooseView;
- (Value*)getValue;
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;
- (void)initializeValue;
- (IBAction)changeValueEntry:(id)sender;
- (BOOL)hasPartitionConstants;
- (BOOL)isValidEntry:(NSString*)str;
- (void)restoreWindowState;
- (void)setValueFromString:(NSString*)str forValue:(Value*)v;

@end
