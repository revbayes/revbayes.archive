#import <Cocoa/Cocoa.h>
@class ToolParsimony;

#define EXHAUSTIVE  0
#define BANDB       1
#define HEURISTIC   2


@interface WindowControllerParsimony : NSWindowController {

    ToolParsimony*                  myTool;
    int                             searchMethod;
    
    IBOutlet NSTabView*             searchMethodSelectorTab;
    IBOutlet NSPopUpButton*         hsSwapSelector;
    IBOutlet NSComboBox*            hsKeepSelector;
    IBOutlet NSPopUpButton*         hsMulTreesSelector;
    IBOutlet NSComboBox*            hsRearrLimitSelector;
    IBOutlet NSComboBox*            hsReconLimitSelector;
    IBOutlet NSComboBox*            hsNBestSelector;
    IBOutlet NSTextField*           hsRetainSelector;
    IBOutlet NSPopUpButton*         hsAllSwapSelector;
    IBOutlet NSPopUpButton*         hsUseNonMinSelector;
    IBOutlet NSPopUpButton*         hsSteepestSelector;
    IBOutlet NSTextField*           hsNChuckSelector;
    IBOutlet NSComboBox*            hsChuckScoreSelector;
    IBOutlet NSPopUpButton*         hsAbortRepSelector;
    IBOutlet NSPopUpButton*         hsRandomizeSelector;
    IBOutlet NSPopUpButton*         hsAddSeqSelector;
    IBOutlet NSTextField*           hsNRepsSelector;
    IBOutlet NSComboBox*            hsHoldSelector;
    IBOutlet NSComboBox*            bbKeepSelector;
    IBOutlet NSPopUpButton*         bbMulTreesSelector;
    IBOutlet NSTextField*           bbUpBoundSelector;
    IBOutlet NSPopUpButton*         bbAddSeqSelector;
    IBOutlet NSComboBox*            exKeepSelector;

    NSString*                       hsSwap;
    NSString*                       hsKeep;
    NSString*                       hsMulTrees;
    NSString*                       hsRearrLimit;
    NSString*                       hsReconLimit;
    NSString*                       hsNBest;
    NSString*                       hsRetain;
    NSString*                       hsAllSwap;
    NSString*                       hsUseNonMin;
    NSString*                       hsSteepest;
    int                             hsNChuck;
    NSString*                       hsChuckScore;
    NSString*                       hsAbortRep;
    NSString*                       hsRandomize;
    NSString*                       hsAddSeq;
    int                             hsNReps;
    NSString*                       hsHold;
    NSString*                       bbKeep;
    NSString*                       bbMulTrees;
    double                          bbUpBound;
    NSString*                       bbAddSeq;
    NSString*                       exKeep;

    NSString*                       old_hsKeep;
    NSString*                       old_hsRearrLimit;
    NSString*                       old_hsReconLimit;
    NSString*                       old_hsNBest;
    NSString*                       old_hsChuckScore;
    NSString*                       old_hsHold;
    NSString*                       old_bbKeep;
    NSString*                       old_exKeep;
}

@property (nonatomic)        int       searchMethod;
@property (nonatomic,strong) NSString* hsSwap;
@property (nonatomic,strong) NSString* hsKeep;
@property (nonatomic,strong) NSString* hsMulTrees;
@property (nonatomic,strong) NSString* hsRearrLimit;
@property (nonatomic,strong) NSString* hsReconLimit;
@property (nonatomic,strong) NSString* hsNBest;
@property (nonatomic,strong) NSString* hsRetain;
@property (nonatomic,strong) NSString* hsAllSwap;
@property (nonatomic,strong) NSString* hsUseNonMin;
@property (nonatomic,strong) NSString* hsSteepest;
@property (nonatomic)        int       hsNChuck;
@property (nonatomic,strong) NSString* hsChuckScore;
@property (nonatomic,strong) NSString* hsAbortRep;
@property (nonatomic,strong) NSString* hsRandomize;
@property (nonatomic,strong) NSString* hsAddSeq;
@property (nonatomic)        int       hsNReps;
@property (nonatomic,strong) NSString* hsHold;
@property (nonatomic,strong) NSString* bbKeep;
@property (nonatomic,strong) NSString* bbMulTrees;
@property (nonatomic)        double    bbUpBound;
@property (nonatomic,strong) NSString* bbAddSeq;
@property (nonatomic,strong) NSString* exKeep;

- (IBAction)cancelButtonAction:(id)sender;
- (BOOL)checkEntry:(NSString*)val forValidPossibilities:(NSArray*)possibilities;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolParsimony*)t;
- (IBAction)okButtonAction:(id)sender;

@end
