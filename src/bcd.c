signed char bcda[BCDMAX];
signed char bcdb[BCDMAX];
signed char bcdc[BCDMAX];
signed char bcdd[BCDMAX];

// These are for intermediate calculations only:
signed char bcdx0[BCDMAX];
signed char bcdx1[BCDMAX];
signed char bcdx2[BCDMAX];
signed char bcdx3[BCDMAX];
signed char bcdx4[BCDMAX];
signed char bcdx5[BCDMAX];
signed char bcdx6[BCDMAX];
signed char bcary[BCDMAX];

// Highest place used
int bcd_top = BCDMAX;

// Routine to print to screen maximum number of significant figures used
// in a BCD integer. Highest value encountered so far is 12
void chk_bcd(void)
{
    printf("Highest figures used: %d\n", BCDMAX - bcd_top);
}

void l2bcd(signed char * b, int n)
{
    int i;
    int x = n;

    for (i = 0; i < BCDMAX; ++i) {
      *(b + i) = '\0';
    }
    if (x == 0) {
      return;
    }
    if (x < 0) {
        x = -n;
        *b = -1;
    }
    else {
        *b = 1;
    }
    i = BCDMAX - 1;
    do {
        *(b + i) = x % 10;
        x = x / 10;
        if (i < bcd_top) {
            bcd_top = i;
        }
        --i;
        if (i < 1) {
            printf("OVERFLOW IN L2BCD\n");
            return;
        }
    }
    while (x);
}

int bcd2l(signed char * b)
{
    int i;
    int x = 0;
    for (i = 1; i < BCDMAX; ++i) {
        x = x * 10 + *(b + i);
    }
    if (*b < 0) {
      x = -x;
    }
    return x;
}

void addbcd(signed char * a1, signed char * b1, signed char * b2)
{
    int i, x;
    signed char bx1[BCDMAX];
    signed char bx2[BCDMAX];

    if (*b1 == 0) {
        for (i = 0; i < BCDMAX; ++i) {
          *(a1 + i) = *(b2 + i);
        }
        return;
    }

    if (*b2 == 0) {
        for (i = 0; i < BCDMAX; ++i) {
          *(a1 + i) = *(b1 + i);
        }
        return;
    }

    for (i = 0; i < BCDMAX; ++i) {
      *(bx1 + i) = *(b1 + i);
    }
    for (i = 0; i < BCDMAX; ++i) {
      *(bx2 + i) = *(b2 + i);
    }

    if (*b1 > 0 && *b2 < 0) {
        *bx2 = 1;
        subbcd(a1, b1, bx2);
        return;
    }

    if (*b1 < 0 && *b2 > 0) {
        *bx1 = 1;
        subbcd(a1, b2, bx1);
        return;
    }

    if (*b1 < 0 && *b2 < 0) {
        for (i = 1; i < BCDMAX; ++i) {
          *(bx1 + i) = *(b1 + i);
        }
        *bx1 = 1;
        for (i = 1; i < BCDMAX; ++i) {
          *(bx2 + i) = *(b2 + i);
        }
        *bx2 = 1;
        addbcd(a1, bx1, bx2);
        *a1 = -1;
        return;
    }

    // Both positive:
    for (i = 0; i < BCDMAX; ++i) {
        *(a1 + i) = '\0';
    }
    x = 0;
    for (i = BCDMAX - 1; i > 0; --i) {
        x = x + *(bx1 + i) + *(bx2 + i);
        *(a1 + i) = x % 10;
        x = x / 10;
    }
    *a1 = 1;
}

// Compare absolute values
int compabsbcd(signed char * x1, signed char * x2)    // +1 if x1 > x2
{                                                     // -1 if x1 < x2
    int x = 0;                                        // 0  if x1 = x2
    int ind1 = 1;
    int ind2 = 1;

    do {
        if (*(x1 + ind1) > *(x2 + ind2)) {
            x = 1;
        }
        else if (*(x1 + ind1) < *(x2 + ind2)) {
            x = -1;
        }
        if (x) {
          break;
        }
        ++ind1;
        ++ind2;
    } while (ind2 < BCDMAX);

    return x;
}

void subbcd(signed char * a1, signed char * b1, signed char * b2)
{
    // a1 = b1 - b2
    int i, x, c;
    signed char bx1[BCDMAX];
    signed char bx2[BCDMAX];

    if (*b2 == 0) {
        for (i = 0; i < BCDMAX; ++i) {
          *(a1 + i) = *(b1 + i);
        }
        return;
    }

    if (*b1 == 0) {
        for (i = 0; i < BCDMAX; ++i) {
          *(a1 + i) = *(b2 + i);
        }
        if (*b2 == -1) {
          *a1 = 1;
        }
        else {
          *a1 = -1;
        }
        return;
    }

    for (i = 0; i < BCDMAX; ++i) {
        *(bx1 + i) = *(b1 + i);
    }
    for (i = 0; i < BCDMAX; ++i) {
        *(bx2 + i) = *(b2 + i);
    }

    if (*b1 > 0 && *b2 < 0) {
        *bx2 = 1;
        addbcd(a1, b1, bx2);
        return;
    }

    if (*b1 < 0 && *b2 > 0) {
        *bx1 = 1;
        addbcd(a1, b2, bx1);
        *a1 = -1;
        return;
    }

    if (*b1 < 0 && *b2 < 0) {
        *bx1 = 1;
        *bx2 = 1;
        subbcd(a1, bx2, bx1);
        return;
    }

    // Normal, both positive
    // First, we must sort out the larger absolute value
    c = compabsbcd(b1, b2);   // Compare absolute values

    if (c > 0) {    // b1 > b2
        for (i = 0; i < BCDMAX; ++i) {
            *(a1 + i) = '\0';
        }
        for (i = BCDMAX - 1; i > 0; --i) {
            x = *(bx1 + i);
            if (x < *(b2 + i))
              {
                x = x + 10;
                *(bx1 + i - 1) = *(bx1 + i - 1) - 1;
              }
            *(a1 + i) = x - *(b2 + i);
        }
        *a1 = 1;
    }
    else if (c < 0) {   // b1 < b2
        subbcd(a1, bx2, bx1); // a1 = b2 - b1
        *a1 = -1;
    }
    else {    // b1 = b2
        for (i = 0; i < BCDMAX; ++i) {
          *(a1 + i) = '\0';
        }
    }
}

void mulbcdl(signed char * a1, signed char * b1, int n)    // a1 = b1 * n
{
    int i, r, c, j, sgn, x;

    signed char bx1[BCDMAX];
    signed char bx2[BCDMAX];

    for (i = 0; i < BCDMAX; ++i) {
        *(bx1 + i) = *(b1 + i);
    }

    for (i = 0; i < BCDMAX; ++i) {
        *(a1 + i) = '\0';
    }
    if (*bx1 == '\0' || n == 0) {    // Result will be zero
        return;
    }
    if (n < 0) {
        i = -1;
    }
    else {
        i = 1;
    }
    sgn = *bx1 * i;    // This gets the sign right

    // Now, we multiply:
    x = n;
    if (x < 0) {    // Use positive only, sign is already right
        x = -n;
    }

    j = 0;    // Pointer difference in array

    do {
        r = x % 10;
        x = x / 10;
        for (i = 1; i < BCDMAX; ++i) {
          *(bx2 + i) = '\0';
        }
        *bx2 = 1;
        c = 0;
        for (i = BCDMAX - 1; i > 0; --i) {
            if (i - j < 1) {
                break;
            }
            *(bx2 + i - j) = c + *(bx1 + i) * r;
            c = *(bx2 + i - j) / 10;
            *(bx2 + i - j) = *(bx2 + i - j) % 10;
        }
        addbcd(a1, a1, bx2);
        ++j;
    }
    while (x);
    *a1 = sgn;    // This gets the sign right
    i = 0;

    do {
        ++i;
    }
    while (*(a1 + i) == '\0' && i < BCDMAX);

    if (i < bcd_top) {
        bcd_top = i;
    }
}

void divbcdl(signed char * a1, signed char * b1, int n)    // a1 = b1 / n
{
    int  i, j, sgn, numdigs, strtdig, x, y, res, rem;
    signed char bx1[BCDMAX];
    signed char dx1[BCDMAX];

    for (i = 0; i < BCDMAX; ++i) {
        *(bx1 + i) = *(b1 + i);
    }
    for (i = 0; i < BCDMAX; ++i) {
        *(a1 + i) = '\0';
    }

    if (*bx1 == '\0') {    // Result will be zero
        return;
    }

    // If n > b1, result will be zero, so:
    l2bcd(dx1, n);

    if (compabsbcd(dx1, bx1) > 0) {    // +1 if x1 > x2
      return;
    }

    if (n < 0) {
      i = -1;
    }
    else {
      i = 1;
    }
    sgn = *bx1 * i; // This gets the sign right

    // Now, we divide:
    x = n;
    if (x < 0) {    // Use positve only, sign is already right
      x = -n;
    }

    numdigs = 0;
    y = x;

    do {
        ++numdigs;
        y = y / 10;
    }
    while (y);

    strtdig = 1;

    do {
        ++strtdig;
    }
    while (*(bx1 + strtdig) == '\0');

    strtdig = strtdig + numdigs - 1;

    if (strtdig > BCDMAX - 1) {
        printf("STRTDIG OVERFLOW\n");
        return;
    }

    do {
        // We are going to try dividing from strtdig, dx1 / x
        for (i = 1; i < BCDMAX; ++i) {
            *(dx1 + i) = '\0';
        }
        *dx1 = 1;

        j = BCDMAX - 1;
        for (i = strtdig; i > 0; --i) {
            *(dx1 + j) = *(bx1 + i);
            --j;
        }

        y = bcd2l(dx1);
        res = y / x;
        if (res) {    // Does it go?
            rem = y % x;
            *(a1 + strtdig) = res;
            l2bcd(dx1, rem);
            j = BCDMAX - 1;
            for (i = strtdig; i > 0; --i) {
                *(bx1 + i) = *(dx1 + j);
                --j;
            }
        }
        else {    // If it doesn't go...
          *(a1 + strtdig) = '\0';
        }
        ++strtdig;
    }
    while (strtdig < BCDMAX);
    *a1 = sgn;    // THIS GETS THE SIGN RIGHT
}

void modbcdl(signed char * a1, signed char * b1, int x)
{
    signed char dx1[BCDMAX];
    signed char dx2[BCDMAX];

    divbcdl(dx1, b1, x);
    mulbcdl(dx2, dx1, x);
    subbcd(a1, b1, dx2);
}
