/*
 * Bezier path utility category
 *
 * (c) 2004 Alastair J. Houghton
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. The name of the author of this software may not be used to endorse
 *      or promote products derived from the software without specific prior
 *      written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY DIRECT, INDIRECT,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#import "AJHBezierUtils.h"

// Subdivide a Bézier (50% subdivision)
static void subdivideBezier(const NSPoint bez[4], NSPoint bez1[4], NSPoint bez2[4]) {

    NSPoint q;

    // Subdivide the Bézier further
    bez1[0].x = bez[0].x;
    bez1[0].y = bez[0].y;
    bez2[3].x = bez[3].x;
    bez2[3].y = bez[3].y;

    q.x = (bez[1].x + bez[2].x) / 2.0;
    q.y = (bez[1].y + bez[2].y) / 2.0;
    bez1[1].x = (bez[0].x + bez[1].x) / 2.0;
    bez1[1].y = (bez[0].y + bez[1].y) / 2.0;
    bez2[2].x = (bez[2].x + bez[3].x) / 2.0;
    bez2[2].y = (bez[2].y + bez[3].y) / 2.0;

    bez1[2].x = (bez1[1].x + q.x) / 2.0;
    bez1[2].y = (bez1[1].y + q.y) / 2.0;
    bez2[1].x = (q.x + bez2[2].x) / 2.0;
    bez2[1].y = (q.y + bez2[2].y) / 2.0;

    bez1[3].x = bez2[0].x = (bez1[2].x + bez2[1].x) / 2.0;
    bez1[3].y = bez2[0].y = (bez1[2].y + bez2[1].y) / 2.0;
}

// Subdivide a Bézier (specific division)
static void subdivideBezierAtT(const NSPoint bez[4], NSPoint bez1[4], NSPoint bez2[4], float t) {

    NSPoint q;
    float mt = 1 - t;

    // Subdivide the Bézier further
    bez1[0].x = bez[0].x;
    bez1[0].y = bez[0].y;
    bez2[3].x = bez[3].x;
    bez2[3].y = bez[3].y;

    q.x = mt * bez[1].x + t * bez[2].x;
    q.y = mt * bez[1].y + t * bez[2].y;
    bez1[1].x = mt * bez[0].x + t * bez[1].x;
    bez1[1].y = mt * bez[0].y + t * bez[1].y;
    bez2[2].x = mt * bez[2].x + t * bez[3].x;
    bez2[2].y = mt * bez[2].y + t * bez[3].y;

    bez1[2].x = mt * bez1[1].x + t * q.x;
    bez1[2].y = mt * bez1[1].y + t * q.y;
    bez2[1].x = mt * q.x + t * bez2[2].x;
    bez2[1].y = mt * q.y + t * bez2[2].y;

    bez1[3].x = bez2[0].x = mt * bez1[2].x + t * bez2[1].x;
    bez1[3].y = bez2[0].y = mt * bez1[2].y + t * bez2[1].y;
}

// Distance between two points
static double distanceBetween(NSPoint a, NSPoint b) {

    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt (dx * dx + dy * dy);
}

// Length of a Bézier curve
static double lengthOfBezier(const NSPoint bez[4], double acceptableError) {

    double polyLen = 0.0;
    for (int n=0; n<3; ++n)
        polyLen += distanceBetween(bez[n], bez[n + 1]);
    double chordLen = distanceBetween(bez[0], bez[3]);
    double errLen = polyLen - chordLen;

    double retLen;
    if (errLen > acceptableError) 
        {
        NSPoint left[4], right[4];
        subdivideBezier(bez, left, right);
        retLen = (lengthOfBezier(left, acceptableError) + lengthOfBezier(right, acceptableError));
        } 
    else 
        {
        retLen = 0.5 * (polyLen + chordLen);
        }
    return retLen;
}

// Split a Bézier curve at a specific length
static double subdivideBezierAtLength (const NSPoint bez[4], NSPoint bez1[4], NSPoint bez2[4], double length, double acceptableError) {
                       
    float prevT = 0.5, t = 0.5;
    for (;;) 
        {
        float top = 1.0, bottom = 0.0;

        subdivideBezierAtT(bez, bez1, bez2, t);

        double len1 = lengthOfBezier(bez1, 0.5 * acceptableError);

        if (fabs (length - len1) < acceptableError)
            return len1;

        if (length > len1) 
            {
            bottom = t;
            t = 0.5 * (t + top);
            } 
        else if (length < len1) 
            {
            top = t;
            t = 0.5 * (bottom + t);
            }

        if (t == prevT)
            return len1;

        prevT = t;
        }
}

@implementation NSBezierPath (AJHBezierUtils)

// Estimate the total length of a Bézier path
- (double)lengthWithMaximumError:(double)maxError {

    int elements          = (int)[self elementCount];
    double length         = 0.0;
    NSPoint pointForClose = NSMakePoint(0.0, 0.0);
    NSPoint lastPoint     = NSMakePoint(0.0, 0.0);

    for (int n=0; n<elements; ++n)
        {
        NSPoint points[3];
        NSBezierPathElement element = [self elementAtIndex:n associatedPoints:points];
        switch (element)
            {
            case NSMoveToBezierPathElement:
                pointForClose = lastPoint = points[0];
                break;
            case NSLineToBezierPathElement:
                length += distanceBetween (lastPoint, points[0]);
                lastPoint = points[0];
                break;
            case NSCurveToBezierPathElement:
                {
                NSPoint bezier[4] = { lastPoint, points[0], points[1], points[2] };
                length += lengthOfBezier (bezier, maxError);
                lastPoint = points[2];
                break;
                }
            case NSClosePathBezierPathElement:
                length += distanceBetween (lastPoint, pointForClose);
                lastPoint = pointForClose;
                break;
            }
        }

    return length;
}

// Convenience method
- (double)length {

    return [self lengthWithMaximumError:0.1];
}

// Return an NSBezierPath corresponding to the first trimLength units of this NSBezierPath.
- (NSBezierPath*)bezierPathByTrimmingToLength:(double)trimLength withMaximumError:(double)maxError {

    NSBezierPath* newPath = [NSBezierPath bezierPath];
    int elements          = (int)[self elementCount];
    double length         = 0.0;
    NSPoint pointForClose = NSMakePoint(0.0, 0.0);
    NSPoint lastPoint     = NSMakePoint(0.0, 0.0);

    for (int n=0; n<elements; ++n)
        {
        NSPoint		points[3];
        NSBezierPathElement element = [self elementAtIndex:n
                          associatedPoints:points];
        double		elementLength;
        double		remainingLength = trimLength - length;
        
        switch (element)
            {
            case NSMoveToBezierPathElement:
                [newPath moveToPoint:points[0]];
                pointForClose = lastPoint = points[0];
                continue;
            case NSLineToBezierPathElement:
                elementLength = distanceBetween (lastPoint, points[0]);
                if (length + elementLength <= trimLength)
                    [newPath lineToPoint:points[0]];
                else
                    {
                    double f = remainingLength / elementLength;
                    [newPath lineToPoint:NSMakePoint(lastPoint.x + f * (points[0].x - lastPoint.x),lastPoint.y + f * (points[0].y - lastPoint.y))];
                    return newPath;
                    }
                length += elementLength;
                lastPoint = points[0];
                break;
            case NSCurveToBezierPathElement:
                {
                NSPoint bezier[4] = { lastPoint, points[0], points[1], points[2] };
                elementLength = lengthOfBezier (bezier, maxError);
                if (length + elementLength <= trimLength)
                    [newPath curveToPoint:points[2] controlPoint1:points[0] controlPoint2:points[1]];
                else
                    {
                    NSPoint bez1[4], bez2[4];
                    subdivideBezierAtLength(bezier, bez1, bez2, remainingLength, maxError);
                    [newPath curveToPoint:bez1[3] controlPoint1:bez1[1] controlPoint2:bez1[2]];
                    return newPath;
                    }
                length += elementLength;
                lastPoint = points[2];
                break;
                }
            case NSClosePathBezierPathElement:
                elementLength = distanceBetween (lastPoint, pointForClose);
                if (length + elementLength <= trimLength)
                    [newPath closePath];
                else
                    {
                    double f = remainingLength / elementLength;
                    [newPath lineToPoint:NSMakePoint(lastPoint.x + f * (points[0].x - lastPoint.x),lastPoint.y + f * (points[0].y - lastPoint.y))];
                    return newPath;
                    }
                length += elementLength;
                lastPoint = pointForClose;
                break;
            }
        }
  
  return newPath;
}

// Convenience method
- (NSBezierPath *)bezierPathByTrimmingToLength:(double)trimLength {

    return [self bezierPathByTrimmingToLength:trimLength withMaximumError:0.1];
}

/* Return an NSBezierPath corresponding to the part *after* the first
   trimLength units of this NSBezierPath. */
- (NSBezierPath*)bezierPathByTrimmingFromLength:(double)trimLength withMaximumError:(double)maxError {
                
    NSBezierPath* newPath = [NSBezierPath bezierPath];
    int elements          = (int)[self elementCount];
    double length         = 0.0;
    NSPoint pointForClose = NSMakePoint(0.0, 0.0);
    NSPoint lastPoint     = NSMakePoint(0.0, 0.0);
  
    for (int n = 0; n < elements; ++n)
        {
        NSPoint points[3];
        NSBezierPathElement element = [self elementAtIndex:n associatedPoints:points];
        double elementLength;
        double remainingLength = trimLength - length;

        switch (element)
            {
            case NSMoveToBezierPathElement:
                [newPath moveToPoint:points[0]];
                pointForClose = lastPoint = points[0];
                continue;
            case NSLineToBezierPathElement:
                elementLength = distanceBetween (lastPoint, points[0]);
                if (length > trimLength)
                    [newPath lineToPoint:points[0]];
                else if (length + elementLength > trimLength)
                    {
                    double f = remainingLength / elementLength;
                    [newPath moveToPoint:NSMakePoint(lastPoint.x + f * (points[0].x - lastPoint.x),lastPoint.y + f * (points[0].y - lastPoint.y))];
                    [newPath lineToPoint:points[0]];
                    }
                length += elementLength;
                lastPoint = points[0];
                break;
            case NSCurveToBezierPathElement:
                {
                NSPoint bezier[4] = { lastPoint, points[0], points[1], points[2] };
                elementLength = lengthOfBezier (bezier, maxError);

                if (length > trimLength)
                    [newPath curveToPoint:points[2] controlPoint1:points[0] controlPoint2:points[1]];
                else if (length + elementLength > trimLength)
                    {
                    NSPoint bez1[4], bez2[4];
                    subdivideBezierAtLength(bezier, bez1, bez2, remainingLength, maxError);
                    [newPath moveToPoint:bez2[0]];
                    [newPath curveToPoint:bez2[3] controlPoint1:bez2[1] controlPoint2:bez2[2]];
                    }
                length += elementLength;
                lastPoint = points[2];
                break;
                }

            case NSClosePathBezierPathElement:
                elementLength = distanceBetween(lastPoint, pointForClose);
                if (length > trimLength)
                    [newPath closePath];
                else if (length + elementLength > trimLength)
                    {
                    double f = remainingLength / elementLength;
                    [newPath moveToPoint:NSMakePoint (lastPoint.x + f * (points[0].x - lastPoint.x),lastPoint.y + f * (points[0].y - lastPoint.y))];
                    [newPath lineToPoint:points[0]];
                    }
                length += elementLength;
                lastPoint = pointForClose;
                break;
            }
        }
  
  return newPath;
}

// Convenience method
- (NSBezierPath *)bezierPathByTrimmingFromLength:(double)trimLength {

    return [self bezierPathByTrimmingFromLength:trimLength withMaximumError:0.1];
}

// Find the first point in the path
- (NSPoint)firstPoint {

    NSPoint points[3];
    NSBezierPathElement element = [self elementAtIndex:0 associatedPoints:points];

    if (element == NSMoveToBezierPathElement)
        return points[0];
    else
        return NSMakePoint(0.0, 0.0);
}

/* Append a Bezier path, but if it starts with a -moveToPoint, then remove
   it.  This is useful when manipulating trimmed path segments. */
- (void)appendBezierPathRemovingInitialMoveToPoint:(NSBezierPath*)path {

    int elements = (int)[path elementCount];

    for (int n=0; n<elements; ++n)
        {
        NSPoint points[3];
        NSBezierPathElement element = [path elementAtIndex:n associatedPoints:points];

        switch (element)
            {
            case NSMoveToBezierPathElement:
                {
                if (n != 0)
                    [self moveToPoint:points[0]];
                break;
                }
            case NSLineToBezierPathElement:
                [self lineToPoint:points[0]];
                break;
            case NSCurveToBezierPathElement:
                [self curveToPoint:points[2] controlPoint1:points[0] controlPoint2:points[1]];
                break;
            case NSClosePathBezierPathElement:
                [self closePath];
            }
        }
}

// Create an NSBezierPath containing an arrowhead for the start of this path
- (NSBezierPath*)bezierPathWithArrowHeadForStartOfLength:(double)length angle:(double)angle {

    NSBezierPath* rightSide           = [self bezierPathByTrimmingToLength:length];
    NSBezierPath* leftSide            = [rightSide bezierPathByReversingPath];
    NSAffineTransform* rightTransform = [NSAffineTransform transform];
    NSAffineTransform* leftTransform  = [NSAffineTransform transform];
    NSPoint firstPoint                = [self firstPoint];

    // Rotate about the point of the arrowhead
    [rightTransform translateXBy:firstPoint.x yBy:firstPoint.y];
    [rightTransform rotateByDegrees:angle];
    [rightTransform translateXBy:-firstPoint.x yBy:-firstPoint.y];

    [rightSide transformUsingAffineTransform:rightTransform];

    // Same again, but for the left hand side of the arrowhead
    [leftTransform translateXBy:firstPoint.x yBy:firstPoint.y];
    [leftTransform rotateByDegrees:-angle];
    [leftTransform translateXBy:-firstPoint.x yBy:-firstPoint.y];

    [leftSide transformUsingAffineTransform:leftTransform];

    /* Careful!  We don't want to append the -moveToPoint from the right hand
    side, because then -closePath won't do what we would want it to. */
    [leftSide appendBezierPathRemovingInitialMoveToPoint:rightSide];

    return leftSide;
}

// Convenience function for obtaining arrow for the other end
- (NSBezierPath*)bezierPathWithArrowHeadForEndOfLength:(double)length angle:(double)angle {

    return [[self bezierPathByReversingPath] bezierPathWithArrowHeadForStartOfLength:length angle:angle];
}

@end
