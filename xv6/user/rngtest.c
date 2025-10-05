#include "types.h"
#include "user.h"
#include "stat.h"

/* The following code is added by Robert Reece | rwr230002 */
/* This file can be used to test the random number generator system call  */

int main(void)
{
 int i = rng(10);
 printf(1,"%d\n",i);
 exit();
}

/* End of code added */
