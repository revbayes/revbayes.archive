//
//  YRKSpinningProgressIndicator.h
//
//  Copyright 2009 Kelan Champagne. All rights reserved.
//

#define NUM_FINS     12

@interface YRKSpinningProgressIndicator : NSView {

    int        _position;
    int        _numFins;
    NSColor*   _finColors[NUM_FINS];
    
    BOOL       _isAnimating;
    BOOL       _isFadingOut;
    NSTimer*   _animationTimer;
	NSThread*  _animationThread;
    
    NSColor*   _foreColor;
    NSColor*   _backColor;
    BOOL       _drawsBackground;
    
    BOOL       _displayedWhenStopped;
    BOOL       _usesThreadedAnimation;

    BOOL       _isIndeterminate;
    double     _currentValue;
    double     _maxValue;
}

@property (atomic,strong)                                 NSColor* color;
@property (atomic,strong)                                 NSColor* backgroundColor;
@property (atomic,readonly)                               BOOL     drawsBackground;
@property (atomic,getter=isDisplayedWhenStopped,readonly) BOOL     displayedWhenStopped;
@property (atomic,readonly)                               BOOL     usesThreadedAnimation;
@property (atomic,getter=isIndeterminate,readonly)        BOOL     indeterminate;
@property (atomic,readonly)                               double   doubleValue;
@property (atomic,readonly)                               double   maxValue;

- (void)stopAnimation:(id)sender;
- (void)startAnimation:(id)sender;
- (void)setIndeterminate:(BOOL)isIndeterminate;
- (void)setUsesThreadedAnimation:(BOOL)useThreaded;

@end
