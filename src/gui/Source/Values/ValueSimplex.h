#import <Cocoa/Cocoa.h>
#import "Value.h"



@interface ValueSimplex : Value <NSCoding> {
    
    NSArray*            value;
}

- (id)initWithVector:(NSArray*)v;
- (void)setValue:(NSArray*)x;
- (NSArray*)value;

@end
