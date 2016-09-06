#import <Foundation/Foundation.h>
@class AnalysisView;
@class ExecuteNode;
@class Tool;
@class ToolLoop;



@interface ExecuteTree : NSObject {

    AnalysisView*           myView;
    NSMutableArray*         nodes;
    NSMutableArray*         downPassSequence;
    ExecuteNode*            root;
    Tool*                   badTool;
}

@property (nonatomic,strong) ExecuteNode* root;

- (void)addLoop:(NSArray*)loopTools repeated:(int)nTimes loopId:(ToolLoop*)tid;
- (ExecuteNode*)addNode;
- (Tool*)executeTree;
- (void)executeTreeFromNode:(ExecuteNode*)p;
- (ExecuteNode*)findNodeWithTool:(Tool*)t;
- (id)init;
- (id)initWithTools:(NSArray*)toolArray andView:(AnalysisView*)v;
- (void)initializeDownPassSequence;
- (void)listNodes;
- (void)passDown:(ExecuteNode*)n;
- (void)print;

@end
