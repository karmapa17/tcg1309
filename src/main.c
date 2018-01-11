#include <stdio.h>
#include <string.h>
#include "tc.def"
#include "bcd.h"
#include "bcd.ext"
#include "bcd.c"
#include "utils.c"

int main()
{
    set_epoch();
    prn_cal();
}
