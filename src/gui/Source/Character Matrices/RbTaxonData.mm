#import "MatrixRowInformation.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"



@implementation RbTaxonData

@synthesize dataType;
@synthesize taxonName;

- (void)addObservation:(RbDataCell*)dc {

    [data addObject:dc];
}

- (void)cleanName:(NSString*)nameStr {

    nameStr = [nameStr stringByReplacingOccurrencesOfString:@"\'" withString:@""];
    nameStr = [nameStr stringByReplacingOccurrencesOfString:@"\"" withString:@""];
    nameStr = [nameStr stringByReplacingOccurrencesOfString:@" "  withString:@"_"];
}

- (RbDataCell*)dataCellIndexed:(int)idx {

	return [data objectAtIndex:idx];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:dataType      forKey:@"dataType"];
	[aCoder encodeObject:data       forKey:@"data"];
	[aCoder encodeObject:taxonName  forKey:@"taxonName"];
}

- (id)init {

    if ( (self = [super init]) ) 
        {
		// allocate an array holding the data
		data      = [[NSMutableArray alloc] init];
		taxonName = [[NSString alloc] init];
		
        // initialize some variables
		dataType = 0;
        }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        dataType  = [aDecoder decodeIntForKey:@"dataType"];
		data      = [aDecoder decodeObjectForKey:@"data"];
		taxonName = [aDecoder decodeObjectForKey:@"taxonName"];
		}
	return self;
}

- (id)initWithRbTaxonData:(RbTaxonData*)d {

    if ( (self = [super init]) ) 
		{
		// allocate an array holding the data
		data = [[NSMutableArray alloc] init];
		
		// copy information
		if (d != nil)
			{
			dataType  = [d dataType];
			taxonName = [[NSString alloc] initWithString:[d taxonName]];
            [self cleanName:taxonName];
			for (int i=0; i<[d numCharacters]; i++)
				{
				RbDataCell* c    = [d dataCellIndexed:i]; 
				RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
				[data addObject:newC];
				}
			}
        else
            {
            taxonName = [[NSString alloc] init];
            }
		}
    return self;
}

- (id)initWithMatrixRowInformation:(MatrixRowInformation*)mr {

    if ( (self = [super init]) ) 
		{
		// allocate an array holding the data
		data = [[NSMutableArray alloc] init];
		
		// copy information
		if (mr != nil)
			{
			dataType  = STANDARD; // the MatrixRowInformation class only deals with standard data type
			taxonName = [[NSString alloc] initWithString:[mr valueForKey:@"Taxon Name"]];
            [self cleanName:taxonName];
			for (int i=0; i<[mr numberOfColumns]-1; i++)
				{
                int intVal = [mr integerRepresentationForCharacter:i];
                RbDataCell* dc = [[RbDataCell alloc] init];
                [dc setIsDiscrete:YES];
                [dc setDataType:STANDARD];
                [dc setNumStates:10];
                [dc setIsAmbig:NO];
                [dc setIsGapState:NO];
                [dc setDiscreteStateTo:intVal];
                if (intVal == -1)
                    [dc setIsDiscrete:YES];
                [data addObject:dc];
				}
			}
        else
            {
            taxonName = [[NSString alloc] init];
            }
		}
    return self;
}

- (int)numCharacters {

    return (int)[data count];
}

- (void)print {

    NSString* prtStr = [NSString stringWithString:taxonName];
    [prtStr stringByAppendingFormat:@"\nDataType=%d\n", dataType];
    for (RbDataCell* c in [data objectEnumerator])
        {
        char state = [c getDiscreteState];
        [prtStr stringByAppendingFormat:@"%c", state];
        }
    NSLog(@"%@", prtStr);
}

- (void)removeObervationFromEnd {

    [data removeLastObject];
}

@end
