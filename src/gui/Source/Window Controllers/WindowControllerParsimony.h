#import <Cocoa/Cocoa.h>
@class ToolParsimony;



@interface WindowControllerParsimony : NSWindowController {

    IBOutlet NSPopUpButton*         tieBreakerSelector;
    IBOutlet NSSegmentedControl*    bioNjSelector;
    ToolParsimony*                  myTool;

    /*NSString*                       hsSwap;
    hsKeep
    NSString*                       hsMulTrees;
    hsRearrLimit;
    hsReconLimit;
    hsNBest;
    NSString*                       hsRetain;
    NSString*                       hsAllSwap;
    NSString*                       hsUseNonMin;
    NSString*                       hsSteepest;
    int                             hsNChuck;
    hsChuckScore;
    NSString*                       hsAbortRep;
    NSString*                       hsRandomize;
    NSString*                       hsAddSeq;
    int                             hsNReps;
    hsHold;
    bbKeep;
    NSString*                       bbMulTrees;
    double                          bbUpBound;
    NSString*                       bbAddSeq;
    exKeep;*/
}

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolParsimony*)t;
- (IBAction)okButtonAction:(id)sender;

@end
