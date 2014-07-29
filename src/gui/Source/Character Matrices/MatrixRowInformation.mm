#import "MatrixRowInformation.h"
#import "WindowControllerDataEntry.h"




@implementation MatrixRowInformation

- (void)addEntryToEnd {

    [values addObject:@"?"];
}

- (void)deleteCharacterWithIndex:(int)idx {

    [values removeObjectAtIndex:idx];
}

- (id)init {

    return [self initWithController:nil];
}

- (id)initWithController:(WindowControllerDataEntry*)c {

    if ( (self = [super init]) ) 
        {
        myController = c;
        values = [[NSMutableArray alloc] init];
        [values addObject:@"New Taxon"];
        for (int i=0; i<[myController numCharacters]; i++)
            [values addObject:@"?"];
        }
    return self;
}

- (int)integerRepresentationForCharacter:(int)idx {

    id obj = [values objectAtIndex:(idx+1)];
    NSString* str = [NSString stringWithString:obj];
    char c = [str characterAtIndex:0];
    if ( c == '?' )
        return -1;
    int x = [str intValue];
    return x;
}

- (int)numberOfColumns {

    return (int)[values count];
}

- (void)setValue:(id)val forKey:(NSString*)key {

    if ( [key isEqualToString:@"Taxon Name"] == YES )
        [values replaceObjectAtIndex:0 withObject:val];
    else 
        {
        int idx = [key intValue];
        [values replaceObjectAtIndex:idx withObject:val];
        }
}

- (id)valueForKey:(NSString*)key {

    if ( [key isEqualToString:@"Taxon Name"] == YES )
        return [values objectAtIndex:0];
    int idx = [key intValue];
    return [values objectAtIndex:idx];
}

@end
