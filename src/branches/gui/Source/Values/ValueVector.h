#import <Foundation/Foundation.h>
#import "Value.h"



@interface ValueVector : Value <NSCoding> {
    
    NSMutableArray*     vector;
}

- (int)dimensions;
- (id)initWithDimensions:(int)dim;
- (Value*)newValue;
- (void)setDimensions:(int)dim;
- (Value*)valueIndexed:(int)idx;

@end
