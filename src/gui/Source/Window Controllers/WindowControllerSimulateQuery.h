#import <Cocoa/Cocoa.h>
@class ToolSimulate;

#define TREE_FROM_SIM   0
#define TREE_FROM_FILE  1
#define TREE_FROM_TOOL  2


@interface WindowControllerSimulateQuery : NSWindowController <NSCoding> {

    IBOutlet NSButton*        cancelButton;
    IBOutlet NSButton*        okButton;
    IBOutlet NSButton*        helpButton;
    IBOutlet NSPopUpButton*   treeGenSelector;
    IBOutlet NSMatrix*        treeType;
    IBOutlet NSTextField*     numTaxaLabelField;
    IBOutlet NSTextField*     numTaxaField;
    IBOutlet NSTextField*     parm1Field;
    IBOutlet NSTextField*     parm2Field;
    IBOutlet NSTextField*     parm3Field;
    IBOutlet NSTextField*     parm1LabelField;
    IBOutlet NSTextField*     parm2LabelField;
    IBOutlet NSTextField*     parm3LabelField;
    IBOutlet NSButton*        newickFinderButton;
    
    ToolSimulate*             myTool;
    int                       numTaxa;
    double                    lambda;
    double                    mu;
    double                    theta;
    int                       whichTreeSourceMethod;
    int                       whichTreeGenerationMethod;
    BOOL                      closedWithCancel;
}

@property (nonatomic) int    numTaxa;
@property (nonatomic) double lambda;
@property (nonatomic) double mu;
@property (nonatomic) double theta;
@property (nonatomic) int    whichTreeSourceMethod;
@property (nonatomic) int    whichTreeGenerationMethod;
@property (nonatomic) BOOL   closedWithCancel;

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)changeTreeSourceMethod:(id)sender;
- (IBAction)changeTreeGenerationMethod:(id)sender;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)init;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithTool:(ToolSimulate*)t;
- (IBAction)okButtonAction:(id)sender;
- (void)setWindowSize;

@end
