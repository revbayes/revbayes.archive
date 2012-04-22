#import "MatrixController.h"
#import "MatrixRowInformation.h"




@implementation MatrixRowInformation

- (void)addEntryToEnd {

    [values addObject:@"?"];
    NSLog(@"adding entry to end of %@", self);
}

- (void)dealloc {

    [values dealloc];
    [super dealloc];
}

- (void)deleteCharacterWithIndex:(int)idx {

    [values removeObjectAtIndex:idx];
}

- (id)init {

    return [self initWithController:nil];
}

- (id)initWithController:(MatrixController*)c {

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
