#import <Cocoa/Cocoa.h>
#import "Value.h"



@interface ValueRealPos : Value <NSCoding> {

    NSNumber*            value;
}

- (id)initWithNumber:(float)v;
- (void)setValue:(float)x;
- (float)value;

@end
