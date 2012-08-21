#import <Cocoa/Cocoa.h>
#import "Tool.h"
#include <vector>
@class GuiTree;
@class RbData;
@class WindowControllerParsimony;




@interface ToolParsimony : Tool <NSCoding> {

    WindowControllerParsimony*          controlWindow;
    int                                 numTreesVisited;
    unsigned**                          stateSets;
    unsigned**                          stateSetsPtr;
    int                                 numCharacters;
    int                                 scoreOfBestTree;
    NSMutableArray*                     bestTrees;
}

@property (readwrite) int numTreesVisited;

- (void)addTaxonFromList:(NSMutableArray*)availableTaxa toTree:(NSMutableArray*)nodes usingSpareNodes:(NSMutableArray*)spares treeObject:(GuiTree*)t;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (void)exhaustiveSearch;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)showControlPanel;
- (RbData*)unconditionallyMergeData:(NSMutableArray*)a;

@end
