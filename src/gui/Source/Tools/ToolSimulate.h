#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class GuiTree;
@class WindowControllerSimulate;
@class WindowControllerSimulateQuery;



@interface ToolSimulate : ToolData <NSCoding> {

    WindowControllerSimulateQuery*  queryWindow;
    WindowControllerSimulate*       controlWindow;
    GuiTree*                        myTree;
    double                          treeLength;
    double                          alpha;
    double                          rAC;
    double                          rAG;
    double                          rAT;
    double                          rCG;
    double                          rCT;
    double                          rGT;
    double                          piA;
    double                          piC;
    double                          piG;
    double                          piT;
    int                             sequenceLength;
}

@property (readwrite) double treeLength;
@property (readwrite) double alpha;
@property (readwrite) double rAC;
@property (readwrite) double rAG;
@property (readwrite) double rAT;
@property (readwrite) double rCG;
@property (readwrite) double rCT;
@property (readwrite) double rGT;
@property (readwrite) double piA;
@property (readwrite) double piC;
@property (readwrite) double piG;
@property (readwrite) double piT;
@property (readwrite) int    sequenceLength;

- (char)charState:(int)x;
- (void)closeControlPanel;
- (void)closeQueryPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (GuiTree*)exposeTreePtr;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)makeTreeOfSize:(int)nt;
- (NSMutableAttributedString*)sendTip;
- (void)simulate;

@end
