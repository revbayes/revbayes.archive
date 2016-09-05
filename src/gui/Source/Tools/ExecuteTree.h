#import <Foundation/Foundation.h>
@class ExecuteNode;
@class Tool;



@interface ExecuteTree : NSObject {

    NSMutableArray*         nodes;
    NSMutableArray*         downPassSequence;
    ExecuteNode*            root;
    Tool*                   badTool;
}

@property (nonatomic,strong) ExecuteNode* root;

- (void)addLoop:(NSArray*)loopTools repeated:(int)nTimes;
- (ExecuteNode*)addNode;
- (Tool*)executeTree;
- (void)executeTreeFromNode:(ExecuteNode*)p;
- (ExecuteNode*)findNodeWithTool:(Tool*)t;
- (id)init;
- (id)initWithTools:(NSArray*)toolArray;
- (void)initializeDownPassSequence;
- (void)listNodes;
- (void)passDown:(ExecuteNode*)n;
- (void)print;

@end
