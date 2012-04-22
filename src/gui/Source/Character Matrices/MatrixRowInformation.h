#import <Foundation/Foundation.h>
@class MatrixController;



@interface MatrixRowInformation : NSObject {

    NSMutableArray*             values;
    MatrixController*           myController;
}

- (void)addEntryToEnd;
- (void)deleteCharacterWithIndex:(int)idx;
- (id)initWithController:(MatrixController*)c;
- (int)numberOfColumns;
- (id)valueForKey:(NSString *)key;
- (void)setValue:(id)val forKey:(NSString *)key;

@end
