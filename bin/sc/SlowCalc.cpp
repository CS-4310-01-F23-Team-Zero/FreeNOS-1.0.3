/*
 * Copyright (C) 2015 Niek Linnenbank
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <String.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include "SlowCalc.h"

SlowCalc::SlowCalc(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Compute prime numbers using the Sieve of Eratosthenes algorithm");
    parser().registerPositional("NUMBER", "Maximum number to search for prime numbers");
    //parser().registerFlag('o', "stdout", "Write results to standard output if set");
}

SlowCalc::~SlowCalc()
{
}

SlowCalc::Result SlowCalc::exec()
{
    int n, i, j, k;
    double l;

    // Read max number
    n = atoi(arguments().get("NUMBER"));

    // Slow it down
    for (i = 1; i < n; i++)
        for (j = 1; j < n; j++)
            for (k = 1; k < n; k++){
                l = sqrt(i+j+k)+i+j+k;
                while (l>0){
                    l--;
                }
    }

    // Done
    return Success;
}