#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class GuiTree;
@class WindowControllerSimulate;
@class WindowControllerSimulateQuery;



@interface ToolSimulate : ToolData <NSCoding> {

    WindowControllerSimulateQuery*  queryWindow;
    WindowControllerSimulate*       controlWindow;
    GuiTree*                        simTree;
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

@property (nonatomic,strong) GuiTree*      simTree;
@property (nonatomic)        double        treeLength;
@property (nonatomic)        double        alpha;
@property (nonatomic)        double        rAC;
@property (nonatomic)        double        rAG;
@property (nonatomic)        double        rAT;
@property (nonatomic)        double        rCG;
@property (nonatomic)        double        rCT;
@property (nonatomic)        double        rGT;
@property (nonatomic)        double        piA;
@property (nonatomic)        double        piC;
@property (nonatomic)        double        piG;
@property (nonatomic)        double        piT;
@property (nonatomic)        int           sequenceLength;

- (char)charState:(int)x;
- (void)closeControlPanel;
- (void)closeQueryPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (GuiTree*)exposeTreePtr;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)makeTreeOfSize:(int)nt;
- (NSMutableAttributedString*)sendTip;
- (void)simulate;

@end
