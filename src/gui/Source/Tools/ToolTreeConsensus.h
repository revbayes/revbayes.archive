#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class GuiTree;
@class TreeTaxonBipartitions;
@class WindowControllerTreeConsensusViewer;




@interface ToolTreeConsensus : Tool <NSCoding> {

    TreeTaxonBipartitions*                  myParts;
    WindowControllerTreeConsensusViewer*    treeInspector;
    GuiTree*                                consensusTree;
    BOOL                                    isConsensusTreeWindowOpen;
}

@property (nonatomic)        BOOL       isConsensusTreeWindowOpen;
@property (nonatomic,strong) GuiTree*   consensusTree;

- (void)addTree:(GuiTree*)t;
- (GuiTree*)getConsensusTree;
- (void)closeInspectorPanel;

@end
