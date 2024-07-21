/**
 **************************************************
 * @file main.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 19.06.2023
 * @brief: main.c source file to implement the stopwatch functionality and a button to start the timer and
 * pressing it again will save the current time, print it one line below and continue counting the time.
 *
 * @usage: In order to use this module, you need to exclude the module "fan_control" from
* 		  the build.
==================================================
### Resources used ###
	(see stopwatch.c)
==================================================
*/

/* Includes */
#include <stopwatch.h>

/**
 * @brief Main function.
 *
 * This is the entry point of the program. It initializes the stopwatch and starts it in an infinite loop.
 * The stopwatch_init() function is called to initialize the stopwatch and all the peripherals. For more info
 * see stopwatch.c
 * The stopwatch_start() function is then called in an infinite loop to start the stopwatch functionality.
 *
 * @return none
 */
int main(void) {
    stopwatch_init(); // Initialize the stopwatch.

    while (1) {
        stopwatch_start(); // Start the stopwatch.
    }
}

