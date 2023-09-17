#include <math.h>

#define MATH_TOL_GRADIENT 0.000001  // Lines with a gradient smaller this - are straight

double LinHorz(double Y, double spX, double spY, double epX, double epY){
    // Calculates intersection X of a !diagonal! line (spX,spY - epX,epY) with a horizontal line on Y
    // !! calling functions must deliver a non-parallel line !!

    double deltaX = spX - epX;
    double deltaY = spY - epY;

    double gradient = deltaY / deltaX;

    // normalize Y (shift on 0)
    Y -= spY;

    double X = Y / gradient;

    // de-normalize X
    X += spX;

    return X;

}

double LinVert(double X, double spX, double spY, double epX, double epY){
    // Calculates intersection Y of a !diagonal! line (spX,spY - epX,epY) with a vertical line on X
    // !! calling functions must deliver a non-parallel line !!
    
    double deltaX = spX - epX;
    double deltaY = spY - epY;

    double gradient = deltaX / deltaY;

    // normalize Y (shift on 0)
    X -= spX;

    double Y = X / gradient;

    // de-normalize Y
    Y += spY;

    return Y;

}

int PointInRect(double pX, double pY, double reX1, double reY1, double reX2, double reY2){

    // normalize re
    double temp = 0;

    if (reX2 < reX1){
        temp = reX1;
        reX1 = reX2;
        reX2 = temp;
    }
    if (reY2 < reY1){
        temp = reY1;
        reY1 = reY2;
        reY2 = temp;
    }

    return (pX >= reX1 && pX <= reX2 && pY >= reY1 && pY <= reY2);
        
}

double LenLine(spX, spY, epX, epY){
    // Len of a Line
    double deltaX = spX - epX;
    double deltaY = spY - epY;
    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}

int LineInRect(double *startX, double *startY, double *stopX, double *stopY, double reX1, double reY1, double reX2, double reY2){

    // return 0 = Line has nothing to do with the rectangle
    //        1 = horizontal line
    //        2 = vertical line
    //        3 = diagonal line
    // the valid line itself is returned within startXY & stopXY 

    // normalize re
    double temp = 0;

    if (reX2 < reX1){
        temp = reX1;
        reX1 = reX2;
        reX2 = temp;
    }
    if (reY2 < reY1){
        temp = reY1;
        reY1 = reY2;
        reY2 = temp;
    }

	double deltaX = *stopX - *startX;
	double deltaY = *stopY - *startY;
	int noLine = 0;

//printf("IN\n");

	if (abs(deltaX) < MATH_TOL_GRADIENT){
//printf("Vert\n");

    	// Vertical line
		if (*startX >= reX1 && *startX <= reX2){
            // X is on screen....
			if (*startY > reY2){
				// Out of Screen (bottom)...
				*startY = reY2;
			}
			else if (*startY < reY1){
				// Out of Screen (top)...
				*startY = reY1;
			}
			if (*stopY > reY2){
				// Out of Screen (bottom)...
				*stopY = reY2;
			}
			else if (*stopY < reY1){
				// Out of Screen (top)...
				*stopY = reY1;
			}
		}
		else{
			// line right or left out of screen
			return 0;
		}
        if (abs(startY - stopY) > MATH_TOL_GRADIENT){
            // line inside
            return 2;
        }
        else{
            // line above or under out of screen
            return 0;
        }
	}
	else if (abs(deltaY) < MATH_TOL_GRADIENT){
//printf("Horz\n");

		// Horizontal line
		if (*startY >= reY1 && *startY <= reY2){
			// Y is on screen
			if (*startX < reX1){
				// out of screen left
				*startX = reX1;
			}
			else if (*startX > reX2){
				// out of screen right
				*startX = reX2;
			}
			if (*stopX < reX1){
				// out of screen left
				*stopX = reX1;
			}
			else if (*stopX > reX2){
				// out of screen right
				*stopX = reX2;
			}
		}
		else{
			// line above or under out of screen
//printf("Horz1\n");
			return 0;
		}
        if (abs(startX - stopX) > MATH_TOL_GRADIENT){
            // line inside
//printf("Horz2\n");
            return 1;
        }
        else{
            // line right or left out of screen
//printf("Horz3\n");
            return 0;
        }
	}
	else{
//printf("DIAG\n");

		// Diagonal line
		int spInRect = PointInRect(*startX, *startY, reX1, reY1, reX2, reY2);
		int epInRect = PointInRect(*stopX, *stopY, reX1, reY1, reX2, reY2);
		double interX = 0;
		double interY = 0;
		int lower = 0;
		int left = 0;
		int upper = 0;
		int right = 0;

		if (spInRect && epInRect){
			// start- and end-point on screen
//printf("Both in Rect\n");
		}
		else if (spInRect || epInRect){
			// start- or end-point on screen - find possible intersection edge
			if(spInRect){
				// ep not on screen
				if (stopX < startX && stopY < startY){
					// Lower Left
					lower = 1;
					left = 1;
				}
				else if (stopX > startX && stopY > startY){
					// Upper Right
					upper = 1;
					right = 1;
				}
				else if (stopX < startX && stopY > startY){
					// Upper Left
					upper = 1;
					left = 1;
				}
				else{
					// Lower Right
					lower = 1;
					right = 1;
				}
			}
			else{
				// sp not on screen
				if (stopX > startX && stopY > startY){
					// Lower Left
					lower = 1;
					left = 1;
				}
				else if (stopX < startX && stopY < startY){
					// Upper Right
					upper = 1;
					right = 1;
				}
				else if (stopX > startX && stopY < startY){
					// Upper Left
					upper = 1;
					left = 1;
				}
				else{
					// Lower Right
					lower = 1;
					right = 1;
				}
			}
			// Check on intersection with
			if (lower){
				// intersection with bottom
				interY = reY1;
			}
			else{
				// intersection with top
				interY = reY2;
			}
			interX = LinHorz(interY, *startX, *startY, *stopX, *stopY);
			if (!PointInRect(interX, interY, reX1, reY1, reX2, reY2)){
				if (left){
					// intersection with left
					interX = reX1;
				}
				else{
					// intersect with right
					interX = reX2;
				}
				interY = LinVert(interX, *startX, *startY, *stopX, *stopY);				
			}
			if (spInRect){
				// missing end-point
                *stopX = interX;
				*stopY = interY;
			}
			else{
                // missing start-point
				*startX = interX;
				*startY = interY;
			}
		}
		else{
			// all points out of screen - get all theoretical intersections
			double spX1, spY1, spX2, spY2, spX3, spY3, spX4, spY4; 
			spY1 = reY1;
			spX1 = LinHorz(spY1, *startX, *startY, *stopX, *stopY);
			if (PointInRect(spX1, spY1, reX1, reY1, reX2, reY2)){
				upper = 1;
			}
			spY2 = reY2;
			spX2 = LinHorz(spY2, *startX, *startY, *stopX, *stopY);
			if (PointInRect(spX2, spY2, reX1, reY1, reX2, reY2)){
				lower = 1;
			}
			spX3 = reX1;
			spY3 = LinVert(spX3, *startX, *startY, *stopX, *stopY);
			if (PointInRect(spX3, spY3, reX1, reY1, reX2, reY2)){
				left = 1;
			}
			spX4 = reX2;
			spY4 = LinVert(spX4, *startX, *startY, *stopX, *stopY);
			if (PointInRect(spX4, spY4, reX1, reY1, reX2, reY2)){
				right = 1;
			}
            // find the valid intersections
			if (upper && lower){
				// spXY1 & spXY2 already OK
			}
			else if(upper && left){
				spX2 = spX3;
				spY2 = spY3;
			}
			else if(upper && right){
				spX2 = spX4;
				spY2 = spY4;
			}
			else if(lower && right){
				spX1 = spX4;
				spY1 = spY4;
			}
			else if(lower && left){
				spX1 = spX3;
				spY1 = spY3;
			}
			else{
				// line out of screen
				return 0;
			}
            // we have a valid line (spXY1 - spXY2) - sort direction like original
            if (LenLine(startX, startY, spX1, spY1) < LenLine(startX, startY, spX2, spY2)){
                // direction fits
            }
            else{
                // switch start- and end-point
                spX3 = spX1; spY3 = spY1;
                spX1 = spX2; spY1 = spY2;
                spX2 = spX3; spY2 = spY3;
            }
            // Return line
            *startX = spX1; *startY = spY1;
            *stopX = spX2; *stopY = spY2;
		}
	}	
    return 3;
}