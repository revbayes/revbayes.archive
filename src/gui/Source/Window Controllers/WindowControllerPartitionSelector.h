#import <Cocoa/Cocoa.h>
@class ParmTree;
@class RbData;
@class ToolModel;



@interface WindowControllerPartitionSelector : NSWindowController {

    IBOutlet NSScrollView*             firstTaxaView;
    IBOutlet NSScrollView*             secondTaxaView;
    IBOutlet NSPopUpButton*            firstTaxaSetsButton;
    IBOutlet NSPopUpButton*            secondTaxaSetsButton;
    IBOutlet NSButton*                 moveDownTaxaButton;
    IBOutlet NSButton*                 moveUpTaxaButton;
    IBOutlet NSTextField*              nameField;

	NSMatrix*                          firstMatrix;
	NSMatrix*                          secondMatrix;
    NSMutableArray*                    taxonList;
    NSMutableArray*                    firstTaxonList;
    NSMutableArray*                    secondTaxonList;
	int                                numberOfOutlets;
	float                              cellHeight;
    float                              longestNameLength;
    NSString*                          bpName;
    
    ParmTree*                          myParm;
    ToolModel*                         myTool;
}

@property (nonatomic,strong) NSString* bpName;

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)changeNameAction:(id)sender;
- (IBAction)firstTaxaSetsButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithParm:(ParmTree*)p andTool:(ToolModel*)t;
- (void)initializeTaxonLists;
- (IBAction)moveDownButtonAction:(id)sender;
- (IBAction)moveUpButtonAction:(id)sender;
- (float)lengthOfLongestName;
- (IBAction)okButtonAction:(id)sender;
- (IBAction)secondTaxaSetsButtonAction:(id)sender;
- (void)setMatrixList;
- (BOOL)setMatrixInfo:(RbData*)matrix;
- (void)showControlPanel;

@end
