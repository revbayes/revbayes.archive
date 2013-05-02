#import <Cocoa/Cocoa.h>

#define DNA         0
#define RNA         1
#define AA          2
#define STANDARD    3
#define CONTINUOUS  4
#define MIXED       5



@interface RbDataCell : NSObject <NSCoding> {

	BOOL                 isDiscrete;
	size_t               row;
	size_t               column;
	int                  dataType;
	size_t               numStates;
	BOOL                 isAmbig;
    BOOL                 isGapState;
	NSNumber*            val;
}

@property (readwrite)        BOOL      isDiscrete;
@property (readwrite)        size_t    row;
@property (readwrite)        size_t    column;
@property (readwrite)        int       dataType;
@property (readwrite)        size_t    numStates;
@property (readwrite)        BOOL      isAmbig;
@property (readwrite)        BOOL      isGapState;
@property (readwrite,retain) NSNumber* val;

- (float)getContinuousState;
- (char)getDiscreteState;
- (char)interpretAsAminoAcid:(unsigned)x;
- (char)interpretAsDna:(unsigned)x;
- (char)interpretAsStandard:(unsigned)x;
- (id)initWithCell:(RbDataCell*)c;
- (void)setDiscreteStateTo:(int)x; 
- (unsigned)unsignedRepresentation;

@end
