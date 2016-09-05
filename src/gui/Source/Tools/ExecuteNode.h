#import <Foundation/Foundation.h>
@class Tool;



@interface ExecuteNode : NSObject {

    NSMutableArray*         children;
    NSMutableSet*           tipSet;
    ExecuteNode*            parent;
    __weak Tool*            tool;
    int                     numRepeats;
}

@property (nonatomic,strong) ExecuteNode* parent;
@property (nonatomic,weak  ) Tool*        tool;
@property (nonatomic)        int          numRepeats;

- (void)addChild:(ExecuteNode*)n;
- (void)replaceChild:(ExecuteNode*)n withChild:(ExecuteNode*)c;
- (void)addToTipSet:(ExecuteNode*)n;
- (void)execute;
- (NSArray*)getChildren;
- (NSMutableSet*)getNodeSet;
- (BOOL)isLeaf;
- (void)removeAllChildren;
- (void)removeChild:(ExecuteNode*)n;

@end
