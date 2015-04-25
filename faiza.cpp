
/**  randVlo() a method by Faïza Harbi , CS50x session 2014   **/

/**   write a function let us say, `randVlo(char s)` that takes
  *   an x or y as argument and returns the required numbers.
  *   {-6, -5, -4,-3, 3, 4, 5, 6} where x would be in the list
  *   above and y the positives from the list only
  ***************************************************************/

#include "faiza.h"

/*  based on reading the char x or y other excluded , read the char, if x
    use the if x part else the y part                                       */

int randVlo(char v)
{
    const int range_arr[8] = {-6, -5, -4, -3, 3, 4, 5, 6};

    // TODO (ebob): this is not really needed anymore
    /** srand must be called from outside this function else
        the sequence starts over again with a small shift. **/
    // srand(GetTickCount());

    int index = 0;

    if(v != 'x' && v!= 'y')
        return 0;    // exit(0);
    else
    {
        // handles vy aka no negatives
        if(v == 'y')
        {
            do
            {
                index = rand() % 7;
            }
            while(range_arr[index] < 0);
        }
        //  handles vx all possible values in the array
        else
        {
            index = rand() % 7;
        }
    }
    // return the index element of our array
    return range_arr[index];
}
