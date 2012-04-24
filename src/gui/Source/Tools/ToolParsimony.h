#import <Cocoa/Cocoa.h>
#import "Tool.h"
#include <vector>
@class WindowControllerParsimony;




@interface ToolParsimony : Tool <NSCoding> {

    WindowControllerParsimony*          controlWindow;
    int                                 numTreesVisited;
    std::vector<std::vector<unsigned> > stateSets;
}

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (void)exhaustiveSearch;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)showControlPanel;

@end
