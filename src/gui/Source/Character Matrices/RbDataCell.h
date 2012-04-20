#import <Cocoa/Cocoa.h>

#define DNA         0
#define RNA         1
#define AA          2
#define STANDARD    3
#define CONTINUOUS  4



@interface RbDataCell : NSObject <NSCoding> {

	BOOL                 isDiscrete;
	int                  row;
	int                  column;
	int                  dataType;
	int                  numStates;
	BOOL                 isAmbig;
    BOOL                 isGapState;
	NSNumber*            val;
}

@property (readwrite) BOOL isDiscrete;
@property (readwrite) int row;
@property (readwrite) int column;
@property (readwrite) int dataType;
@property (readwrite) int numStates;
@property (readwrite) BOOL isAmbig;
@property (readwrite) BOOL isGapState;
@property (readwrite,retain) NSNumber* val;

- (float)getContinuousState;
- (char)getDiscreteState;
- (char)interpretAsAminoAcid:(unsigned)x;
- (char)interpretAsDna:(unsigned)x;
- (char)interpretAsStandard:(unsigned)x;
- (id)initWithCell:(RbDataCell*)c;
- (void)setDiscreteStateTo:(int)x; 

@end
