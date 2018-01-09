#include <stdio.h>
#include "tc.def"
#include "utils.c"

/**
 * 1. Generalised Phugpa, -1000.",
 * 2. Generalised Tsurphu, -1000.",
 * 3. Generalised Error Correction, -2000.",
 * 4. mkhas pa'i snying nor, 1796.",
 * 5. New Genden Calculations, 1747 CE."
 */
int epch = 1;

int main()
{
    printf("Hello World");
    init_settings(epch);
    prn_cal();
    printf("Good Night");
}
