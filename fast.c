#include "PoorTui.h"
//#include <float.h>
//#include <math.h>

// float Cycle1Hours = 0;
// float Cycle2Hours = 0;

int main(){

    ClearScreen();

    float On1 = 10.5;
    float Off1 = 9.5;
    float Weeks1 = 6;

    float On2 = 12.0;
    float Off2 = 12.0;
    float Weeks2 = 8;

    float HoursPerWeek = 7 * 24;


    float Cycle1Hours = Weeks1 * HoursPerWeek;
    float Cycle2Hours = Weeks2 * HoursPerWeek;

    float IntervalCnt1 = Cycle1Hours / (On1 + Off1);
    float IntervalCnt2 = Cycle2Hours / (On2 + Off2);

    float OnTime1 = IntervalCnt1 * On1;
    float OnTime2 = IntervalCnt2 * On2;


    printf ("\nOn1: %.2f\n", On1);
    printf ("Off1: %.2f\n", Off1);
    printf ("On2: %.2f\n", On2);
    printf ("Off2: %.2f\n\n", Off2);

    printf ("OnTime_1: %.2f\t(%.0f periods)\n", OnTime1, IntervalCnt1);
    printf ("OnTime_2: %.2f\t(%.0f periods)\n", OnTime2, IntervalCnt2);

    return 0;

}
