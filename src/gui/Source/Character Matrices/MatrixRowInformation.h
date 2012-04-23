#import <Foundation/Foundation.h>
@class WindowControllerDataEntry;



@interface MatrixRowInformation : NSObject {

    NSMutableArray*             values;
    WindowControllerDataEntry*  myController;
}

- (void)addEntryToEnd;
- (void)deleteCharacterWithIndex:(int)idx;
- (id)initWithController:(WindowControllerDataEntry*)c;
- (int)integerRepresentationForCharacter:(int)idx;
- (int)numberOfColumns;
- (id)valueForKey:(NSString*)key;
- (void)setValue:(id)val forKey:(NSString *)key;

@end
