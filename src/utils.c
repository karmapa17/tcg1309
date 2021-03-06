#include "getch.c"

/**
 * 0. Generalised Phugpa, -1000.",
 * 1. Generalised Tsurphu, -1000.",
 * 2. Generalised Error Correction, -2000.",
 * 3. mkhas pa'i snying nor, 1796.",
 * 4. New Genden Calculations, 1747 CE."
 */
int epch = 0;

char sadag_str[45];

void set_lang(void)
{
    int i;
    for (i = 0; i < 27; ++i) {
      lunmanP[i] = lunmanT[i];
    }
    for (i = 0; i < 27; ++i) {
      yogaP[i] = yogaT[i];
    }
    for (i = 0; i < 11; ++i) {
      byedP[i] = byedT[i];
    }
}

void set_grub(void)
{
    int  i;
    for (i = 0; i < 6; ++i) {
      nyidm[i] = nyidmg[i];
    }
    for (i = 0; i < 6; ++i) {
      gzadm[i] = gzadmg[i];
    }
    for (i = 0; i < 6; ++i) {
      tsedm[i] = tsedmg[i];
    }
    for (i = 0; i < 6; ++i) {
      nyilm[i] = nyilmg[i];
    }
    sun_f = 67;
    gza_f = 707;
}

void add_gen(int *a1, int *a2, int *a3, int n1, int n2)
{
    int r;
    r = a2[4] + a3[4];
    if (r < 0) {
        printf("\n\nERROR IN ADD_GEN:\n");
        printf("A2[4] = %d\n", a2[4]);
        printf("A3[4] = %d\n\n", a3[4]);
    }
    a1[4] = r % n2;
    r = a2[3] + a3[3] + r / n2;
    a1[3] = r % 6;
    r = a2[2] + a3[2] + r / 6;
    a1[2] = r % 60;
    r = a2[1] + a3[1] + r / 60;
    a1[1] = r % 60;
    r = a2[0] + a3[0] + r / 60;
    a1[0] = r % n1;
}

// Function to subtract two arrays
// a1 = a2 - a3
void sub_gen(int *a1, int *a2, int *a3, int n1, int n2)
{
    int a4[5];
    int i;

    for (i = 0; i < 5; ++i) {
        a4[i] = a2[i];
    }
    a1[4] = a4[4] - a3[4];

    if (a1[4] < 0) {
        a1[4] = a1[4] + n2;
        a4[3] = a4[3] - 1;
    }
    a1[3] = a4[3] - a3[3];

    if (a1[3] < 0) {
        a1[3] = a1[3] + 6;
        a4[2] = a4[2] - 1;
    }
    a1[2] = a4[2] - a3[2];

    if (a1[2] < 0) {
        a1[2] = a1[2] + 60;
        a4[1] = a4[1] - 1;
    }
    a1[1] = a4[1] - a3[1];

    if (a1[1] < 0) {
        a1[1] = a1[1] + 60;
        a4[0] = a4[0] - 1;
    }

    a1[0] = a4[0] - a3[0];
    if (a1[0] < 0) {
        a1[0] = a1[0] + n1;
    }
}

void clrlst(int *l)
{
    int n;
    for (n = 0; n < 5; ++n) {
        l[n] = 0;
    }
}

void clear_a_b(void)
{
    clrlst(lista);
    clrlst(listb);
}

// Routine to check for special days, festivals, anniversaries, etc.
// Many are currently commented out. Uncomment if needed.
// Month numbers would currently be wrong for Error Correction system.
int chk_spec(int m, int t) {

    switch (m) {
        case 1:
            if (t == 1) {
                sprintf(outbuf, "%s", "From 1st to 15th, Demonstration of Miracles.");
                return 1;
            }
            break;
        case 3:
            if (t == 15) {
                sprintf(outbuf, "%s", "Revelation of the Kalacakra Tantra.");
                return 1;
            }
            break;
        case 4:
            // 7 for Phugpa, 8 for Tsurphu
            if (t == 7 && tsurlug == 0) {
                sprintf(outbuf, "%s", "Birth of the Buddha.");
                return 1;
            }
            if (t == 8 && tsurlug == 1) {
                sprintf(outbuf, "%s", "Birth of the Buddha.");
                return 1;
            }
            if (t == 15) {
                sprintf(outbuf, "%s", "Enlightenment and Parinirvana of the Buddha.");
                return 1;
            }
            break;
        case 6:
            if (t == 4) {
                sprintf(outbuf, "%s", "Turning of the Wheel of the Dharma.");
                return 1;
            }
            if (t == 15) {
                sprintf(outbuf, "%s", "The Buddha's entry into the womb of his mother.");
                return 1;
            }
            break;
        case 9:
            if (t == 22) {
                sprintf(outbuf, "%s", "Descent of the Buddha from the realm of the gods.");
                return 1;
            }
            break;
    }
    return 0;
}

// Function to calculate true month, "zla ba rnam par dag pa"
void zla_dag(int y, int m) // KTC 15
{
    int yr, a, b;    // c;
    yr = y - epch_yr;

    // 'khrul sel
    if (epch == 2) {
      a = 12 * yr + m - 2;
    }
    else {
      a = 12 * yr + m - 3;
    }

    if (a >= 0) {
        b = 2 * a + eyr_a;
        zladag[1] = b % 65;
        zladag[0] = a + b / 65;
    }
    else {
        zladag[1] = 0;
        zladag[0] = 0;
    }
}

void jul2date(int jd) {

    int l, n, j, k, i;

    // This algorithm is from photcopied notes, from James Neely.
    // Also, checked with ESAA, 1992, p. 604
    // Calculates date, at noon on which the Julian date starts.
    // Julian 0 starts Greenwich mean noon on 1st Jan 4713 BC, Julian proleptic
    // calendar.
    // In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
    // by Oct 15th
    // First, get day of week:

    doweek = jd - 7 * ((jd + 1) / 7) + 2;
    if (doweek == 7) {
        doweek = 0;
    }
    if (doweek > 7) {
        printf("ERROR IN DAY OF WEEK ROUTINE:\n");
        return;
    }

    if (jd >= 2299161) {  // Gregorian calendar:

        // This has been tested between March 1, 1600 and Jan 31, 2100
        l = jd + 68569;
        n = (4 * l) / 146097;
        l = l - (146097 * n + 3) / 4;

        //  wy = 4000 * (l + 1) / 1461001;

        l2bcd(bcda, 4000);
        mulbcdl(bcda, bcda, l + 1);
        divbcdl(bcda, bcda, 1461001);
        wy = bcd2l(bcda);

        l = l - (1461 * wy) / 4 + 31;
        wm = (80 * l) / 2447;
        wd = l - (2447 * wm) / 80;
        l = wm / 11;
        wm = wm + 2 - 12 * l;
        wy = 100 * (n - 49) + wy + l;
        // j = month, k = day, i = year
    }
    else {    // Julian calendar
        j = jd + 1402;
        k = (j - 1) / 1461;
        l = j - 1461 * k;
        n = (l - 1) / 365 - l / 1461;
        i = l - 365 * n + 30;
        j = (80 * i) / 2447;
        wd = i - (2447 * j) / 80;
        i = j / 11;
        wm = j + 2 - 12 * i;
        wy = 4 * k + n + i - 4716;
    }
}

// Function to calculate general day, "spyi zhag"
// Calculates Julian for the integer Julian day count at noon UT
// on weekday of "gza' dag". Put into juldat.
void spi_zagf(void) // KTC 46
{
    int b, c;
    spizag = cur_mth * 30 + tt;
    c = spizag + spz_c;
    spz_frac_c = c % 707;
    b = spizag + spz_b;
    b = b + c / 707;
    spz_frac_b = b % 64;
    spizag = spizag - b / 64;
    c = ( spizag + spz_f ) % 7;
    b = gzadag[0];
    if (c != b) {
        if ( c > 4 && b < 2 ) {
            b += 7;
        }
        else if ( b > 4 && c < 2 ) {
            c += 7;
        }
        spizag = spizag + b - c;
    }
    if (b - c > 2) {
        printf ("\nERROR IN GENERAL DAY ROUTINE: %d\n", b - c);
    }
    juldat = spizag + spz_j;
    jul2date(juldat);
}

void div_g6(int * a, int x, int frac4, int frac5) {

    l2bcd(bcdx0, a[0]);
    l2bcd(bcdx1, a[1]);
    l2bcd(bcdx2, a[2]);
    l2bcd(bcdx3, a[3]);
    l2bcd(bcdx4, a[4]);
    l2bcd(bcdx5, a[5]);

    modbcdl(bcary, bcdx0, x);
    divbcdl(bcdx0, bcdx0, x);
    mulbcdl(bcary, bcary, 60);
    addbcd(bcdx1, bcdx1, bcary);

    modbcdl(bcary, bcdx1, x);
    divbcdl(bcdx1, bcdx1, x);
    mulbcdl(bcary, bcary, 60);
    addbcd(bcdx2, bcdx2, bcary);

    modbcdl(bcary, bcdx2, x);
    divbcdl(bcdx2, bcdx2, x);
    mulbcdl(bcary, bcary, 6);
    addbcd(bcdx3, bcdx3, bcary);

    modbcdl(bcary, bcdx3, x);
    divbcdl(bcdx3, bcdx3, x);
    mulbcdl(bcary, bcary, frac4);
    addbcd(bcdx4, bcdx4, bcary);

    modbcdl(bcary, bcdx4, x);
    divbcdl(bcdx4, bcdx4, x);
    mulbcdl(bcary, bcary, frac5);
    addbcd(bcdx5, bcdx5, bcary);

    divbcdl(bcdx5, bcdx5, x);

    a[0] = bcd2l(bcdx0);
    a[1] = bcd2l(bcdx1);
    a[2] = bcd2l(bcdx2);
    a[3] = bcd2l(bcdx3);
    a[4] = bcd2l(bcdx4);
    a[5] = bcd2l(bcdx5);
}

// Function to calculate true solar longitude, "nyi dag".
void nyi_dag(int *a1) {

    static int nyidom[6] = { 6, 10, 11, 10, 6, 0 };
    static int nyibye[6] = { 4, 1, 1, 4, 6, 6 };
    static int nyihaf[6] = { 13, 30, 0, 0, 0, 0 };
    static int nyifac[6] = { 6, 45, 0, 0, 0, 0 };

    int test, tquo, trem;

    sub_gen(nyiwor, a1, nyifac, 27, sun_f);    // KTC 31
    test = 60 * nyiwor[0] + nyiwor[1];
    if (test < 810) {
        nyidor = 0;
    }
    else {
        nyidor = 1;
        sub_gen(nyiwor, nyiwor, nyihaf, 27, sun_f);
        test = 60 * nyiwor[0] + nyiwor[1];
    }
    trem = test % 135;
    tquo = test / 135;

    if (tquo == 0) {
        tquo = 6;
    }

    clear_a_b();

    lista[2] = (trem * 60 + nyiwor[2]) * nyibye[tquo - 1];
    lista[3] = nyiwor[3] * nyibye[tquo - 1];
    lista[4] = nyiwor[4] * nyibye[tquo - 1];

    div_g6(lista, 135, sun_f, 1);

    clrlst(zerlst);
    add_gen(lista, zerlst, lista, 27, sun_f);

    listb[1] = nyidom[tquo - 1];

    if (tquo == 3 || tquo == 4 || tquo == 5) {    // Then, subtract:
        sub_gen(listc, listb, lista, 27, sun_f);
    }
    else {
        add_gen(listc, listb, lista, 27, sun_f);
    }

    if (nyidor == 0) {
        sub_gen(nyidag, a1, listc, 27, sun_f);
    }
    else {
        add_gen(nyidag, a1, listc, 27, sun_f);
    }
}

// Check for main Earth-lords, "sa bdag"
int chk_sadag(int m, int t)
{
    int yan_kwong_flg = 0;
    int klu_bzlog_flg = 0;
    int zin_phung_flg = 0;
    int klu_thebs_flg = 0;
    int nyi_nag_flg = 0;
    int sadag_str_len = 0;
    int i = 0;
    int n;

    // First, "yan kwong": // Data from Kongleg.
    yan_kwong_flg = 0;

    if (m == 1 && t == 13) {
        yan_kwong_flg = 1;
    }
    else if (m == 2 && t == 11) {
        yan_kwong_flg = 1;
    }
    else if (m == 3 && t == 9) {
        yan_kwong_flg = 1;
    }
    else if (m == 4 && t == 7) {
        yan_kwong_flg = 1;
    }
    else if (m == 5 && t == 5) {
        yan_kwong_flg = 1;
    }
    else if (m == 6 && t == 3) {
        yan_kwong_flg = 1;
    }
    else if (m == 7 && t == 1) {
        yan_kwong_flg = 1;
    }
    else if (m == 7 && t == 29) {
        yan_kwong_flg = 1;
    }
    else if (m == 8 && t == 27) {
        yan_kwong_flg = 1;
    }
    else if (m == 9 && t == 25) {
        yan_kwong_flg = 1;
    }
    else if (m == 10 && t == 23) {
        yan_kwong_flg = 1;
    }
    else if (m == 11 && t == 21) {
        yan_kwong_flg = 1;
    }
    else if (m == 12 && t == 19) {
        yan_kwong_flg = 1;
    }

    // Now, "zin phung":
    zin_phung_flg = 0;

    // These data are taken from Mongolian data in VKP2.DOC

    if (m == 1 || m == 2 || m == 3) {    // dpyid gsum, etc.
        if (t == 1 || t == 7 || t == 13 || t == 19 || t == 25) {
            zin_phung_flg = 1;
        }
    }
    else if (m == 4 || m == 5 || m == 6) {
        if (t == 6 || t == 12 || t == 18 || t == 24 || t == 30) {
          zin_phung_flg = 1;
        }
    }
    else if (m == 7 || m == 8 || m == 9) {
        if (t == 3 || t == 9 || t == 15 || t == 21 || t == 27) {
             zin_phung_flg = 1;
        }
    }
    else if (m == 10 || m == 11 || m == 12) {
        if (t == 4 || t == 10 || t == 16 || t == 22 || t == 28) {
            zin_phung_flg = 1;
        }
    }

    //  zin_phung_flg = 0; // Take it out for now. Need to check data.
    // Now, "klu bzlog":
    klu_bzlog_flg = 0;

    if (m == 1) {
        if (t == 5 || t == 10 || t == 15) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 2) {
        if (t == 8 || t == 18 || t == 20 || t == 22 || t == 28) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 4) {
        if (t == 20 || t == 25) {
            klu_bzlog_flg = 1;
        }
      }
    else if (m == 5) {
        if (t == 8 || t == 15) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 6) {
        if (t == 11 || t == 13 || t == 15 || t == 23) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 7) {
        if (t == 5 || t == 6) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 8) {
        if (t == 3 || t == 6 || t == 9 || t == 13 || t == 16) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 9) {
        if (t == 9 || t == 10 || t == 19) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 10) {
        if (t == 9 || t == 10 || t == 19 || t == 26) {
            klu_bzlog_flg = 1;
        }
    }
    else if (m == 11) {
        if (t == 2 || t == 6 || t == 16 || t == 20 || t == 26) {
            klu_bzlog_flg = 1;
        }
    }

    // Now, "klu thebs":
    klu_thebs_flg = 0;

    if (m == 1) {
        if (t == 14) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 2) {
        if (t == 10) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 3) {
        if (t == 25) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 4) {
        if (t == 8 || t == 15) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 5) {
        if (t == 20 || t == 22) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 6) {
        if (t == 5 || t == 20 || t == 25) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 7) {
        if (t == 9 || t == 19) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 8) {
        if (t == 5 || t == 15) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 9) {
        if (t == 1 || t == 11 || t == 21 || t == 22 || t == 23) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 10) {
        if (t == 8 || t == 15 || t == 18) {
            klu_thebs_flg = 1;
        }
    }
    else if (m == 11) {
        if (t == 7 || t == 15 || t == 21) {
            klu_thebs_flg = 1;
        }
    }

    // Now, "nyi nag": // Data from Vaidurya dkar po.
    nyi_nag_flg = 0;

    if (m == 1 && t == 7) {
        nyi_nag_flg = 1;
    }
    else if (m == 2 && t == 14) {
        nyi_nag_flg = 1;
    }
    else if (m == 3 && t == 21) {
        nyi_nag_flg = 1;
    }
    else if (m == 4 && t == 8) {
        nyi_nag_flg = 1;
    }
    else if (m == 5 && t == 16) {
        nyi_nag_flg = 1;
    }
    else if (m == 6 && t == 24) {
        nyi_nag_flg = 1;
    }
    else if (m == 7 && t == 9) {
        nyi_nag_flg = 1;
    }
    else if (m == 8 && t == 18) {
        nyi_nag_flg = 1;
    }
    else if (m == 9 && t == 27) {
        nyi_nag_flg = 1;
    }
    else if (m == 10 && t == 10) {
        nyi_nag_flg = 1;
    }
    else if (m == 11 && t == 20) {
        nyi_nag_flg = 1;
    }
    else if (m == 12 && t == 30) {
        nyi_nag_flg = 1;
    }

    // Now, build string
    n = yan_kwong_flg + klu_bzlog_flg + klu_thebs_flg + zin_phung_flg + nyi_nag_flg;

    if (n == 0) {
        return 0;
    }
    sadag_str[0] = '\0';

    if (yan_kwong_flg) {
        strcat(sadag_str, "yan kwong");
        i++;
    }
    if (klu_bzlog_flg) {
        if (i) {
          strcat(sadag_str, ", ");
        }
        strcat(sadag_str, "klu bzlog");
        i++;
    }
    if (klu_thebs_flg) {
        if (i) {
            strcat(sadag_str, ", ");
        }
        strcat(sadag_str, "klu thebs");
        i++;
    }
    if (zin_phung_flg) {
        if (i) {
            strcat(sadag_str, ", ");
        }
        strcat(sadag_str, "zin phung");
        i++;
    }
    if (nyi_nag_flg) {
        if (i) {
            strcat(sadag_str, ", ");
        }
        if (m == 1) {
            strcat(sadag_str, "nyi nag ngan pa dgu 'dzom");
        }
        else {
            strcat(sadag_str, "nyi nag");
        }
        i++;
    }
    i = strlen(sadag_str);

    if (i > sadag_str_len) {    // Max 35 in testing.
        sadag_str_len = i;
    }
    return 1;
}

void mul_gen (int *res, int *lst, int x, int n1, int n2)
{
    if (x >= 0) {
        l2bcd(bcdx0, lst[0]);
        l2bcd(bcdx1, lst[1]);
        l2bcd(bcdx2, lst[2]);
        l2bcd(bcdx3, lst[3]);
        l2bcd(bcdx4, lst[4]);

        mulbcdl(bcdx0, bcdx0, x);
        mulbcdl(bcdx1, bcdx1, x);
        mulbcdl(bcdx2, bcdx2, x);
        mulbcdl(bcdx3, bcdx3, x);
        mulbcdl(bcdx4, bcdx4, x);

        divbcdl(bcary, bcdx4, n2);
        modbcdl(bcdx4, bcdx4, n2);
        addbcd(bcdx3, bcdx3, bcary);

        divbcdl(bcary, bcdx3, 6);
        modbcdl(bcdx3, bcdx3, 6);
        addbcd(bcdx2, bcdx2, bcary);

        divbcdl(bcary, bcdx2, 60);
        modbcdl(bcdx2, bcdx2, 60);
        addbcd(bcdx1, bcdx1, bcary);

        divbcdl(bcary, bcdx1, 60);
        modbcdl(bcdx1, bcdx1, 60);
        addbcd(bcdx0, bcdx0, bcary);

        modbcdl(bcdx0, bcdx0, n1);

        res[0] = bcd2l (bcdx0);
        res[1] = bcd2l (bcdx1);
        res[2] = bcd2l (bcdx2);
        res[3] = bcd2l (bcdx3);
        res[4] = bcd2l (bcdx4);
    }
    else {
        x = -x;
        mul_gen(res, lst, x, n1, n2);
        clrlst(zerlst);
        sub_gen(res, zerlst, res, n1, n2);
    }
}

// Function to calculate lunar day mean weekday, "tshes kyi dhru ba".
void tse_dru(int x)
{
    mul_gen(tsedru, tsedm, x, 7, gza_f);
}

// Function to calculate lunar anomaly, "ril cha"
void rilchaf (int x)    // KTC 21
{
    int a, b;
    b = x + ril_b;
    a = 2 * x + ril_a + b / 126;
    rilcha[1] = b % 126;
    rilcha[0] = a % 28;
}

// Function to calculate lunar day solar longitude, "nyi ma'i longs spyod".
void nyi_lon(int x)
{
    mul_gen(nyilon, nyilm, x, 27, sun_f);
}

// Function to calculate month mean Sun, "nyi ma'i dhru ba".
void nyi_dru(int x)
{
    mul_gen(nyidru, nyidm, x, 27, sun_f);
    add_gen(nyidru, nyidru, nyida, 27, sun_f);
}

void gza_dru(int x)
{
    mul_gen(gzadru, gzadm, x, 7, gza_f);
    add_gen(gzadru, gzadru, gzada, 7, gza_f);
}

// Adjust month number, if needed
void adj_zla (void)
{
    if (epch == 2) {    // This is for Error Correction system.

        if (zladag[1] == 0 || zladag[1] == 1) {

            if (! zeromthfg) {    // First of two months
                zeromthfg = 1;
                adj_mth = tm - 1;
                if (adj_mth == 0) {
                    adj_mth = 12;
                }
                adj_mth = -adj_mth;
                zladag[0] = zladag[0] - 1; // It has advanced 2.
              }
            else {   // Second
                zeromthfg = 0;
                adj_mth = tm;
                zladag[0] = zladag[0] + 1; // Put it back
            }
        }
        else {
            zeromthfg = 0;
            adj_mth = tm;
            if (adj_mth == 0) {
                adj_mth = 12;
            }
        }
        return;
    }
    if (zlapure) {
        if (zladag[1] == 0 || zladag[1] == 1) {
            if (! zeromthfg) {
                zeromthfg = 1;
                adj_mth = -tm;
                zladag[0] = zladag[0] - 1;
            }
            else {
                zeromthfg = 0;
                adj_mth = tm;
                zladag[0] = zladag[0] + 1;
            }
        }
        else {
            adj_mth = tm;
            if (adj_mth == 0) {
                adj_mth = 12;
            }
        }
        return;
    }

    // ELSE, DO THIS:
    if (zladag[1] == zlasho1 || zladag[1] == zlasho2) {
        adj_mth = -tm;
    }
    else if (zladag[1] > zlasho2) {
        adj_mth = tm - 1;
    }
    else if (zladag[1] == 0 || zladag[1] == 1) {

        if (! zeromthfg) {
            zeromthfg = 1;
            adj_mth = tm - 1;
            zladag[0] = zladag[0] - 1;
        }
        else {
            zeromthfg = 0;
            adj_mth = tm;
            zladag[0] = zladag[0] + 1;
        }
    }
    else {    // Arrive here if 1 < zladag[1] < zlasho1
        adj_mth = tm;
    }
    if (adj_mth == 0) {
        adj_mth = 12;
    }
}

void set_epoch(void)
{
    // Figures for Generalised phug lugs, year -1000 (1
    static int gda1[6] = { 5, 35, 36, 4, 160, 0 };
    static int nda1[6] = { 26, 45, 53, 4, 26, 0 };

    // Figures for Generalised "'khrul sel", year -1000 (3)
    static int gda3[6] = { 6, 13, 6, 0, 288, 0 };
    static int nda3[6] = { 26, 31, 47, 2, 52, 0 };

    // Figures for "mkhas pa'i snying nor", year 1796 (4)
    static int gda4[6] = { 5, 24, 44, 1, 565, 0 };
    static int nda4[6] = { 26, 27, 45, 4, 2, 0 };

    // Figures for "dge ldan rtsis gsar, year 1747 (5)
    static int gda5[6] = { 1, 55, 13, 3, 333, 0 };
    static int nda5[6] = { 26, 39, 51, 0, 18, 0 };

    char chr;
    int i;

    printf("\n\nSelect from the following:\n\n");

    for (i = 0; i < 5; ++i) {
        printf("%s\n", e_str[i]);
    }

    do {
        chr = getch();
    }
    while (chr < '1' || chr > '5');

    if (chr >= '1' && chr <= '9') {
        epch = chr - '0' - 1;
    }

    // Generalised phug, year -1000
    if (chr == '1') {
        set_grub();
        for (i = 0; i < 6; ++i) {
            gzada[i] = gda1[i];
        }
        for (i = 0; i < 6; ++i) {
            nyida[i] = nda1[i];
        }
        epch_yr = -1000;

        // Intercalation index
        eyr_a = 7;
        ril_a = 6;
        ril_b = 61;
        spz_c = 10;
        spz_b = 26;
        spz_f = 5;
        spz_j = 1355847;

        // 230 fraction for Rahu cycle
        rahupart = 93;

        // This is added for "drag po'i rkang 'dzin".
        dragkadd = 6663418;

        // Figures to be added for planets for the
        meradd = 2080;

        // nyin zhag dal ba. These are days of cycle.
        venadd = 277;
        maradd = 4;
        jupadd = 511;
        satadd = 2995;
        zlasho1 = 48;
        zlasho2 = 49;
        zlapure = 0;
        tsurlug = 0;

    }
    else if (chr == '2') {    // Generalised Tshurphu, year -1000

        set_grub();

        for (i = 0; i < 6; ++i) {
            gzada[i] = gda2[i];
        }
        for (i = 0; i < 6; ++i) {
            nyida[i] = nda2[i];
        }
        epch_yr = -1000;

        // Intercalation index
        eyr_a = 11;
        ril_a = 6;    // OK
        ril_b = 75;    // OK
        spz_b = 50;    // 64 fraction - OK
        spz_c = 66;    // 707 fraction - OK
        spz_f = 5;    // OK
        spz_j = 1355847;    // OK
        rahupart = 93;    // 230 fraction for Rahu cycle // OK
        meradd = 1977;    // OK
        venadd = 268;    // OK
        maradd = 3;    // OK
        jupadd = 512;    // OK
        satadd = 2988;    // OK
        dragkadd = 83343;
        zlapure = 1;
        tsurlug = 1;

    }
    else if (chr == '3') {    // Generalised Error Correction, "'khrul sel", year -2000
        set_grub();
        for (i = 0; i < 6; ++i) {
            gzada[i] = gda3[i];
        }
        for (i = 0; i < 6; ++i) {
            nyida[i] = nda3[i];
        }
        epch_yr = -2000;
        eyr_a = 14;    // Intercalation index
        ril_a = 4;    // OK
        ril_b = 50;    // OK
        spz_b = 33;    // 64 fraction - not given in current texts; calculated
        spz_c = 565;    // 707 fraction - not given in current texts; calculated
        spz_f = 6;    // OK
        spz_j = 990553;    // OK
        rahupart = 144;    // 230 fraction for Rahu cycle // OK

        // According to "nor bzang rgya mtsho", the figures for planets at
        // Error Correction epoch are those given in the tantra.
        // The following are calculated on that basis:
        // Should be checked against recent Drepung finds
        meradd = 26;
        venadd = 2421;
        maradd = 62;
        jupadd = 403;
        satadd = 1727;
        dragkadd = 6607043;    // Not given in current texts; calculated
        zlapure = 1;
    }
    else if (chr == '4') {    // mkhas pa'i snying nor
        set_grub();
        for (i = 0; i < 5; ++i) {
            gzada[i] = gda4[i];
        }
        for (i = 0; i < 5; ++i) {
            nyida[i] = nda4[i];
        }
        epch_yr = 1796;
        eyr_a = 16;    // Intercalation index, OK
        ril_a = 8;    // OK
        ril_b = 52;   // OK
        spz_b = 37;    // ??
        spz_c = 433;    // ??
        spz_f = 5;    // OK
        spz_j = 2377133;    // OK
        rahupart = 178;    // 230 fraction for Rahu cycle OK
        meradd = 6303;    // Check
        venadd = 522;    // Check
        maradd = 408;    // OK
        jupadd = 3780;    // OK
        satadd = 1518;    // OK
        dragkadd = 6591588;    // Calculated from longitude in text. OK
        zlasho1 = 48;    // Assumed due to solar terms used.
        zlasho2 = 49;
    }
    else if (chr == '5') {    // dge ldan rtsis gsar

        set_grub();

        for (i = 0; i < 5; ++i) {
            gzada[i] = gda5[i];
        }
        for (i = 0; i < 5; ++i) {
            nyida[i] = nda5[i];
        }
        epch_yr = 1747;
        eyr_a = 10;    // Intercalation index, OK
        ril_a = 24;    // OK
        ril_b = 22;    // OK
        spz_b = 5;    // ??
        spz_c = 65;    // ??
        spz_f = 1;    // OK
        spz_j = 2359237;    // OK
        rahupart = 32;    // 230 fraction for Rahu cycle OK
        meradd = 2518;    // Check
        venadd = 1329;    // Check
        maradd = 375;    // OK
        jupadd = 3213;    // OK
        satadd = 5147;    // OK
        dragkadd = 6632355;    // Calculated from longitude in text. OK
        zlasho1 = 46;
        zlasho2 = 47;
    }
}

// Function to calculate true weekday, "gza' dag".
void gza_dag(int *a1)
{
    static int gzabye[14] = { 5, 5, 4, 3, 2, 1, -1, -2, -3, -4, -5, -5, -5, 5 };
    static int gzadom[14] = { 5, 10, 15, 19, 22, 24, 25, 24, 22, 19, 15, 10, 5, 0 };
    int tot, trem, chasha, rilpo;
    int gzawor[5];

    clrlst (list1);
    clrlst (list2);
    clrlst (zerlst);
    rilpo = rilcha[0] + tt;
    trem = rilpo % 14;
    if (trem == 0) {
      trem = 14;
    }
    list2[1] = gzadom[trem - 1];
    gza_short_flg = 0; // KTC 26  --- debug
    if (gza_short_flg) {    // Flag for less accurate weekday adjustment
        chasha = rilcha[1] * gzabye[trem - 1] * 254520; // 60 * 6 * 707
        // Maximum value = 125 * 5 * 254520 = 159075000 - OK
        tot = chasha / 126;
    }
    else {
        l2bcd(bcda, (30 * rilcha[1] + tt) * gzabye[trem - 1]);
        mulbcdl(bcda, bcda, 360 * gza_f);
        divbcdl(bcda, bcda, 3780);
        tot = bcd2l (bcda);
    }
    if (tot < 0) {    // KTC 24
        list1[4] = -tot;
        add_gen(list1, list1, zerlst, 7, gza_f);
        sub_gen(list3, list2, list1, 7, gza_f);
    }
    else {
        list1[4] = tot;
        add_gen(list1, list1, zerlst, 7, gza_f);
        add_gen(list3, list2, list1, 7, gza_f);
    }
    if ((rilpo / 14) % 2 == 0) {    // For the semi-true weekday
      add_gen(gzawor, a1, list3, 7, gza_f);
    }
    else {
      sub_gen(gzawor, a1, list3, 7, gza_f);
    }

    l2bcd(bcda, gzawor[4]);
    mulbcdl(bcda, bcda, sun_f);
    divbcdl(bcda, bcda, gza_f);
    gzawor[4] = bcd2l(bcda);

    //  The above replaces:
    //  gzawor[4] = (sun_f * gzawor[4]) / gza_f;
    if (nyidor == 0) {
      sub_gen(gzadag, gzawor, listc, 7, sun_f);
    }
    else {
      add_gen(gzadag, gzawor, listc, 7, sun_f);
    }

    // Convert back the lowest fractional part:
    l2bcd(bcda, gzadag[4]);
    mulbcdl(bcda, bcda, gza_f);
    divbcdl(bcda, bcda, sun_f);
    gzadag[4] = bcd2l(bcda);
}

void prn_cal(void)
{
    int cur_year, last_year, new_year, zla0, zla1, intercal, delaymth,
        yr_elemx, mt_elemx, sd_elemx, e, g, i, nextlhag, nextchad, lunmanx,
        sbyorba, byedpa, byedpax, yr_animx, mt_animx, ld_animx, sd_animx,
        chad, lhag, sd_clunmanx, ld_parx, sd_smex, ld_smex, ZS_s,
        ZS_d, ZS_m, newmth, more, n, prv_byedpa, prv_byedpax, prv_sbyorba,
        Cmonth, prv_lunmanx;

    // 1 = female, odd; 0 = male, even
    int yr_gender;

    // 1 = female, odd; 0 = male, even
    int mt_gender;

    // Adjusted, without negative.
    int mthnum;

    // Last and next JD calculations.
    int curjd, nextjd;

    int lastjd = 0;

    set_lang();

    printf("\n\n%s\n", e_str[epch]);
    printf("Enter the starting year: ");
    scanf("%d", &ty);
    printf("\n");

    cur_year = ty;

    printf("Enter the finish year: ");
    scanf("%d", &last_year);
    printf("\n");

donextyear:

    new_year = 1;
    ty = cur_year;
    tm = 1;
    zeromthfg = 0;

    if (tsurlug == 1) {
      sprintf(filenam, "ts_%d.txt", ty);
    }
    else if (epch == 0) {
      sprintf(filenam, "pl_%d.txt", ty);
    }
    else if (epch == 2) {
      sprintf(filenam, "ec_%d.txt", ty);
    }

    fptgt = fopen (filenam, "w");

    if (fptgt == NULL) {
        printf("Error opening target file!\n");
        return;
    }

    // tt not yet set
    // First, check to see if we are not at adjusted 12th month:
    zla_dag(ty, tm); // But only if moving one month at a time!

    zla0 = zladag[0];
    zla1 = zladag[1];

    adj_zla();

    if (adj_mth == 12) {
        if (! zeromthfg) { // Should always work
          ++tm;
        }
    }
    else if (adj_mth == -1) {
        zladag[0] = zla0;
        zladag[1] = zla1;
        zeromthfg = 0;
    }
    n = 0;
    more = 1;
    newmth = 1;

    // START OF MONTH LOOP
    do {
        intercal = 0;
        if (adj_mth == -1) {
          intercal = 1;
        }
        delaymth = 0;
        if (newmth) {
            if (! zeromthfg) {   // We need to use the same data, twice.
              zla_dag (ty, tm); // But only if moving one month at a time!
            }
            adj_zla();
            cur_mth = zladag[0];  // Needed for general day
            gza_dru(cur_mth);
            nyi_dru(cur_mth);
            rilchaf(cur_mth); // MOVED, from gza_dag, NEW, 11/9/94

            // DO WE HAVE NEW YEAR?
            if ((adj_mth == -1 || adj_mth == 1) && new_year) {
                if (ty < 7) {
                    yr_animx = (ty + 1200 - 7) % 12;
                    yr_elemx = ((ty + 1200) / 2 - 3) % 5;
                    yr_gender = (ty + 1200) % 2; // 1 = female, odd; 0
                }
                else {
                    yr_animx = (ty - 7) % 12;
                    yr_elemx = (ty / 2 - 3) % 5;
                    yr_gender = ty % 2; // 1 = female, odd; 0 = male
                }
                sprintf(outbuf, "\nNew Year: %d, %s-%s-%s\n", ty, cycelem1[yr_elemx], cycgendE[yr_gender], cycanim[yr_animx]);
                fprintf(fptgt, "%s", outbuf);
                new_year = 0;
            }
        }
         // Phugpa grub rtsis
        if (zlapure == 0) {

            // Intercalary
            if (adj_mth < 0) {
                intercal = 1;
                delaymth = 0;
            }
            // Changed, 1/9/2010
            else if (zladag[1] == zlasho1+2 || zladag[1] == zlasho2+2) {
                intercal = 0;
                delaymth = 1;
            }
        }
        else {
            // Intercalary
            if (adj_mth < 0) {
                intercal = 1;
                delaymth = 0;
            }
            else if (adj_mth > 0 && (zladag[1] == 0 || zladag[1] == 1)) {
              intercal = 0;
              delaymth = 1;
            }
        }

        if (adj_mth < 0) {
          mthnum = -adj_mth;
        }
        else {
          mthnum = adj_mth;
        }

        if (tsurlug == 1) {
            mt_animx = (mthnum + 10) % 12;
            Cmonth = mthnum;
        }
        else {
            mt_animx = mthnum % 12;
            Cmonth = mthnum + 2;
            if (Cmonth > 12) {
                Cmonth -= 12;
            }
        }

        if (tsurlug == 1) {
            // yr_gender - 1 = female, odd; 0 = male, even
            // ORDER IS: "Fire", "Earth", "Iron", "Water", "Wood"
            //             0        1       2        3       4
            e = yr_elemx; g = yr_gender;
            if ((e == 4 && g == 0) || (e == 1 && g == 1)) {
              mt_elemx = 0 + (mthnum - 1) / 2;
            }
            if ((e == 4 && g == 1) || (e == 2 && g == 0)) {
              mt_elemx = 1 + (mthnum - 1) / 2;
            }
            if ((e == 0 && g == 0) || (e == 2 && g == 1)) {
              mt_elemx = 2 + (mthnum - 1) / 2;
            }
            if ((e == 1 && g == 0) || (e == 3 && g == 1)) {
              mt_elemx = 4 + (mthnum - 1) / 2;
            }
            if ((e == 0 && g == 1) || (e == 3 && g == 0)) {
              mt_elemx = 3 + (mthnum - 1) / 2;
            }
        }
        else {
          mt_elemx = yr_elemx + 1 + (mthnum + 1) / 2;
        }

        // Male year, even CE number
        if (yr_gender == 0) {
            if (mthnum == 11 || mthnum == 12) {
              mt_elemx = yr_elemx + (mthnum + 1) / 2;
            }
        }

        mt_elemx = mt_elemx % 5;
        mt_gender = (mthnum + 1) % 2; // There is no month zero

        if (intercal) {
          sprintf(outbuf, "Tibetan Lunar Month: %d (Intercalary) - %s-%s-%s",
              -adj_mth, cycelem1[mt_elemx], cycgendE[mt_gender], cycanim[mt_animx]);

        }
        else if (epch != 2 && delaymth) {
          sprintf(outbuf, "Tibetan Lunar Month: %d (Delayed) - %s-%s-%s",
              adj_mth, cycelem1[mt_elemx], cycgendE[mt_gender], cycanim[mt_animx]);
        }
        else {
          sprintf(outbuf, "Tibetan Lunar Month: %d - %s-%s-%s",
              adj_mth, cycelem1[mt_elemx], cycgendE[mt_gender], cycanim[mt_animx]);
        }

        printf("%s\n", outbuf);
        fprintf(fptgt, "\n%s\n\n", outbuf);
        tt = 1;
        ltt = 1;

        // START OF DAY LOOP - START OF DAY LOOP - START OF DAY LOOP **********************
        do {
            tt = ltt;
            nextlhag = 0;
            lhag = 0;
            chad = 0;
            nextchad = 0;
            tse_dru (tt);
            nyi_lon (tt);
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f);
            nyi_dag (nyibar); // Must be done before gza_dag
            gza_dag (tsebar);
            spi_zagf ();

            // Work out previous and next days to determine CHAD or LHAG
            curjd = juldat;
            tt = ltt - 1;
            tse_dru (tt);
            nyi_lon (tt);
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f);
            nyi_dag (nyibar); // Must be done before gza_dag
            gza_dag (tsebar);
            spi_zagf ();
            lastjd = juldat;

            for (i = 0; i < 6; ++i) {
                prv_nyidag[i] = nyidag[i];
                prv_gzadag[i] = gzadag[i];
            }

            tt = ltt + 1;
            tse_dru(tt);
            nyi_lon(tt);
            add_gen(tsebar, gzadru, tsedru, 7, gza_f);
            add_gen(nyibar, nyidru, nyilon, 27, sun_f);
            nyi_dag(nyibar); // Must be done before gza_dag
            gza_dag(tsebar);
            spi_zagf();
            nextjd = juldat;

            // Save for later, in case we have a LHAG day:
            for (i = 0; i < 6; ++i) {
                nxt_nyidag[i] = nyidag[i];
                nxt_gzadag[i] = gzadag[i];
            }

            // Recalculate for current tithi:
            tt = ltt;
            tse_dru (tt);
            nyi_lon (tt);
            add_gen (tsebar, gzadru, tsedru, 7, gza_f);
            add_gen (nyibar, nyidru, nyilon, 27, sun_f);
            nyi_dag (nyibar); // Must be done before gza_dag
            gza_dag (tsebar);
            spi_zagf ();
            if (curjd == lastjd + 2) {
              lhag = 1;
            }
            if (nextjd == curjd + 2) {
              nextlhag = 1;
            }
            if (curjd == lastjd) {
              chad = 1;
            }
            if (curjd == nextjd) {
              nextchad = 1;
            }

            // Calculate lunar mansion at daybreak:
            // add solar longitude to tithi * factor. Then subtract hours, etc., from gzadag
            clear_a_b();
            for (i = 0; i < 5; ++i) {
                monlong[i] = 0;
            }

            monlong[1] = 54;  // 1/30th of a revolution
            mul_gen(monlong, monlong, tt, 27, sun_f);
            add_gen(monlong, monlong, nyidag, 27, sun_f);
            listb[0] = 0;

            for (i = 1; i < 5; ++i) {
              listb[i] = gzadag[i];
            }
            listb[4] = (sun_f * listb[4]) / gza_f;
            // This gives longitude at daybreak:
            sub_gen(monlong, monlong, listb, 27, sun_f);

            lunmanx = monlong[0];

            // Now calculate yoga, sbyor ba:

            // This is strictly wrong, we should use the Sun's longitude at daybreak,
            // but in the Tibetan tradition such an adjustment is not made

            add_gen(lista, monlong, nyidag, 27, sun_f);
            sbyorba = lista[0];

            for (i = 0; i < 6; ++i) {
              sbyorlong[i] = lista[i];
            }

            // Now calculate karana, byed pa:
            clear_a_b();

            // At daybreak for moon
            sub_gen(lista, monlong, nyidag, 27, sun_f);
            byedpa = lista[0] * 60 + lista[1];
            byedpa = byedpa / 27;     // OK

            if (byedpa == 0) {
              byedpax = 7;
            }
            else if (byedpa == 57) {
              byedpax = 8;
            }
            else if (byedpa == 58) {
              byedpax = 9;
            }
            else if (byedpa == 59) {
              byedpax = 10;
            }
            else {
              byedpax = (byedpa - 1) % 7;
            }

            // Solar day data:
            sd_animx = (juldat - 2) % 12;
            sd_elemx = ((juldat - 3) / 2) % 5;
            sd_clunmanx = (juldat - 17) % 28;
            sd_smex = (juldat - 2) % 9 + 1;

            // Lunar day data:
            // If Chinese month is number 1, Trigram is Li, index = 1
            ld_parx = (Cmonth - 1) * 30 + tt;
            ld_parx = ld_parx % 8;
            // If Chinese month is number 1, "sme ba" is 1
            ld_smex = (Cmonth - 1) * 30 + tt;
            ld_smex = ld_smex % 9;

            if (ld_smex == 0) {
              ld_smex = 9;
            }

            // If Chinese month is number 1, Animal is Tiger, index = 11
            ld_animx = (Cmonth - 1) * 30 + tt + 10;
            ld_animx = ld_animx % 12;

            // This is for omitted lunar day
            if (chad) {
                sprintf(outbuf, "%d. Omitted: %s %s %d", tt, cycanim[ld_animx], cycpar[ld_parx], ld_smex);
                // Should include "gza'" for end of lunar day, plus solar longitude.
                fprintf(fptgt, "%s\n", outbuf);
            }
            // If normal or duplicated:
            else {
                // NYIDAG is not changed after here, so we can now calculate zodiacal sign:
                // Zodiac signs, degrees, minutes -  ZS_s, ZS_d, ZS_m
                ZS_m = ((nyidag[0] * 60 + nyidag[1]) * 60 + nyidag[2]) * 6 + nyidag[3];
                ZS_m = ((nyibar[0] * 60 + nyibar[1]) * 60 + nyibar[2]) * 6 + nyibar[3];
                l2bcd(bcdx0, ZS_m);
                mulbcdl(bcdx0, bcdx0, 21600);
                divbcdl(bcdx1, bcdx0, 583200);
                ZS_m = bcd2l(bcdx1);
                ZS_d = ZS_m / 60;
                ZS_m = ZS_m % 60;
                ZS_s = ZS_d / 30;
                ZS_d = ZS_d % 30;

                // If this is the first of two weekdays.
                if (lhag) {
                    // Put here any special calculations for extra day
                    // At this point, the gzadag, nyidag & monlong are correct for
                    // the second of the two days with same tithi. We need for the
                    // first. Previous tithi ended just before start of day we need.
                    clear_a_b();
                    for (i = 0; i < 5; ++i) {
                        prv_monlong[i] = 0;
                    }
                    prv_monlong[1] = 54;

                    mul_gen(prv_monlong, prv_monlong, tt, 27, sun_f);
                    add_gen(prv_monlong, prv_monlong, nyidag, 27, sun_f);
                    listb[0] = 1;
                    sub_gen(prv_monlong, prv_monlong, listb, 27, sun_f);

                    prv_lunmanx = prv_monlong[0];

                    // Now calculate yoga, sbyor ba:
                    add_gen(lista, prv_monlong, nyidag, 27, sun_f);
                    prv_sbyorba = lista[0];

                    for (i = 0; i < 6; ++i) {
                        prv_sbyorlong[i] = lista[i];
                    }

                    // Now calculate karana, sbyed pa:
                    clear_a_b ();
                    sub_gen(lista, prv_monlong, prv_nyidag, 27, sun_f);
                    prv_byedpa = lista[0] * 60 + lista[1];
                    prv_byedpa = prv_byedpa / 27;

                    if (prv_byedpa == 0) {
                        prv_byedpax = 7;
                    }
                    else if (prv_byedpa == 57) {
                        prv_byedpax = 8;
                    }
                    else if (prv_byedpa == 58) {
                        prv_byedpax = 9;
                    }
                    else if (prv_byedpa == 59) {
                        prv_byedpax = 10;
                    }
                    else {
                        prv_byedpax = (prv_byedpa - 1) % 7;
                    }

                    juldat = juldat - 1;

                    // This is normally done by spi_zagf
                    jul2date(juldat);

                    // Solar day data:
                    sd_animx = (juldat - 2) % 12;
                    sd_elemx = ((juldat - 3) / 2) % 5;
                    sd_clunmanx = (juldat - 17) % 28;
                    sd_smex = (juldat - 2) % 9 + 1;

                    // LINE 1. - First of duplicated day
                    sprintf(outbuf, "%d: %s. %s. %s-%s; %d %s %d", tt, dayoweek[doweek],
                        lunmanP[prv_lunmanx], fourelem[dowkelem[doweek]], fourelem[lunmanelem[prv_lunmanx]],
                        wd, wmonths[(wm-1)], wy);

                    fprintf(fptgt, "%s\n", outbuf);

                    // LINE 2. First of duplicated day
                    sprintf(outbuf, "%s, %s, %s, %s %d", yogaP[prv_sbyorba], byedP[prv_byedpax], cycanim[ld_animx],
                        cycpar[ld_parx], ld_smex);
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // LINE 3. First of duplicated day
                    if (tsurlug == 1) {
                        sprintf(outbuf, "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                            (prv_gzadag[0] + 1) % 7, 60, 0, prv_monlong[0], prv_monlong[1], prv_monlong[2],
                            nyidag[0], nyidag[1], nyidag[2], prv_sbyorlong[0], prv_sbyorlong[1], prv_sbyorlong[2]);
                    }
                    else {
                        sprintf(outbuf, "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d",
                           (prv_gzadag[0] + 1) % 7, 60, 0, prv_monlong[0], prv_monlong[1], prv_monlong[2], nyidag[0],
                           nyidag[1], nyidag[2], prv_sbyorlong[0], prv_sbyorlong[1], prv_sbyorlong[2], ZS_s, ZS_d, ZS_m);
                    }
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // LINE 4. First of duplicated day
                    if (tsurlug == 1) {
                        sprintf(outbuf, "Solar: %s-%s. %s" , cycelem1[sd_elemx], cycanim[sd_animx], Clunman[sd_clunmanx]);
                    }
                    else {
                        sprintf(outbuf, "Solar: %s-%s. %s %d", cycelem1[sd_elemx], cycanim[sd_animx], Clunman[sd_clunmanx], sd_smex);
                    }
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // Now do second of duplicated lunar day
                    juldat = juldat + 1;  // This is readjustment!
                    jul2date(juldat);

                    // Solar day data:
                    sd_animx = (juldat - 2) % 12;
                    sd_elemx = ((juldat - 3) / 2) % 5;
                    sd_clunmanx = (juldat - 17) % 28;
                    sd_smex = (juldat - 2) % 9 + 1;

                    // LINE 1. second of duplicated
                    sprintf(outbuf, "%d: %s. %s. %s-%s; %d %s %d", tt, dayoweek[doweek], lunmanP[lunmanx],
                        fourelem[dowkelem[doweek]], fourelem[lunmanelem[lunmanx]],
                              wd, wmonths[(wm-1)], wy);
                    fprintf(fptgt, "%s\n", outbuf);

                    // LINE 2.  Second of duplicated day
                    sprintf(outbuf, "%s, %s, %s, %s %d",
                              yogaP[sbyorba],
                              byedP[byedpax],
                              cycanim[ld_animx],
                              cycpar[ld_parx], ld_smex);
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // LINE 3. Second of duplicated
                    if (tsurlug == 1) {
                        sprintf(outbuf, "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d", gzadag[0], gzadag[1], gzadag[2], monlong[0],
                            monlong[1], monlong[2], nyidag[0], nyidag[1], nyidag[2], sbyorlong[0], sbyorlong[1], sbyorlong[2]);
                    }
                    else {
                        sprintf(outbuf, "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d", gzadag[0], gzadag[1], gzadag[2], monlong[0],
                            monlong[1], monlong[2], nyidag[0], nyidag[1], nyidag[2], sbyorlong[0], sbyorlong[1], sbyorlong[2], ZS_s, ZS_d, ZS_m);
                    }
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // LINE 4. Second of duplicated
                    if (tsurlug == 1) {
                        sprintf(outbuf, "Solar: %s-%s. %s" , cycelem1[sd_elemx], cycanim[sd_animx], Clunman[sd_clunmanx]);
                    }
                    else {
                        sprintf(outbuf, "Solar: %s-%s. %s %d" , cycelem1[sd_elemx], cycanim[sd_animx], Clunman[sd_clunmanx], sd_smex);
                    }
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                // END OF if (lhag)
                }
                else {
                    // No lhag - normal day}
                    // LINE 1.
                    sprintf(outbuf, "%d: %s. %s. %s-%s; %d %s %d", tt, dayoweek[doweek], lunmanP[lunmanx], fourelem[dowkelem[doweek]],
                    fourelem[lunmanelem[lunmanx]], wd, wmonths[(wm-1L)], wy);
                    fprintf(fptgt, "%s\n", outbuf);

                    // LINE 2. NORMAL DAY
                    sprintf(outbuf, "%s, %s, %s, %s %d", yogaP[sbyorba], byedP[byedpax], cycanim[ld_animx], cycpar[ld_parx], ld_smex);
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // LINE 3. NORMAL DAY
                    if (tsurlug == 1) {
                        sprintf(outbuf, "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d", gzadag[0], gzadag[1], gzadag[2], monlong[0], monlong[1],
                        monlong[2], nyidag[0], nyidag[1], nyidag[2], sbyorlong[0], sbyorlong[1], sbyorlong[2]);
                    }
                    else {
                        sprintf(outbuf, "%d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d %d;%d,%d", gzadag[0], gzadag[1], gzadag[2], monlong[0],
                        monlong[1], monlong[2], nyidag[0], nyidag[1], nyidag[2], sbyorlong[0], sbyorlong[1], sbyorlong[2], ZS_s, ZS_d, ZS_m);
                    }
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);

                    // LINE 4. Normal day
                    if (tsurlug == 1) {
                        sprintf(outbuf, "Solar: %s-%s. %s" , cycelem1[sd_elemx], cycanim[sd_animx], Clunman[sd_clunmanx]);
                    }
                    else {
                        sprintf(outbuf, "Solar: %s-%s. %s %d" , cycelem1[sd_elemx], cycanim[sd_animx], Clunman[sd_clunmanx], sd_smex);
                    }
                    fprintf(fptgt, "\x20\x20%s\n", outbuf);
                // End of normal day, no "lhag"
                }

                // LINE 5, Normal day or duplicated
                if (tsurlug == 1) {
                    // (1) - 13 & (8) - 27 has both
                    if (chk_sadag (mthnum, tt)) {
                      fprintf(fptgt, "\x20\x20\x20\x20%s\n", sadag_str);
                    }
                }

                if (! delaymth && chk_spec(mthnum, tt)) {
                    fprintf(fptgt, "  %s\n", outbuf);
                }
            }
            // End of normal or duplicated; ie. not omitted.
            ++ltt;
            newmth = 0;
            lastjd = juldat;
        }
        while (ltt <= 30); // End of day loop
        // End of day loop
        newmth = 1;
        if (! zeromthfg) {
            ++tm;
        }
        if (tm > 12) {
            ++ty;
            tm = 1;
        }
        if (adj_mth == 12) {
            // First check for following intercalary:
            if (epch == 2 && (zladag[1] == 63 || zladag[1] == 64)) {
                more = 1;
            }
            else {
                // We have finished.
                more = 0;
            }
        }
    }
    while (more);
    // End of month loop

    fclose(fptgt);

    if (cur_year < last_year) {
        ++cur_year;
        goto donextyear;
    }
}
