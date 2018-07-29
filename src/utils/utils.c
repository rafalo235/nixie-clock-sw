/*
 utils.c

  Created on: Jul 29, 2018
      Author: Rafał Olejniczak
 */

#include "utils/utils.h"

int fast_atoi( const char * str )
{
    int val = 0;
    int multiplier = 1;

    if (*str == '-')
    {
      multiplier = -1;
    }

    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val * multiplier;
}

