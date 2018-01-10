/**
 * 1. Generalised Phugpa, -1000.",
 * 2. Generalised Tsurphu, -1000.",
 * 3. Generalised Error Correction, -2000.",
 * 4. mkhas pa'i snying nor, 1796.",
 * 5. New Genden Calculations, 1747 CE."
 */
int epch = 1;

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
void zla_dag (int y, int m) // KTC 15
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

void clrlst(int *l)
{
    int n;
    for (n = 0; n < 5; ++n) {
        l[n] = 0;
    }
}

// Check for main Earth-lords, "sa bdag"
int chk_sadag (int m, int t)
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

void init_settings()
{
    if (1 == epch) {
        set_grub();
        int i;
        for (i = 0; i < 6; ++i) {
          gzada[i] = gda2[i];
        }
        for (i = 0; i < 6; ++i) {
          nyida[i] = nda2[i];
        }
        epch_yr = -1000;
        eyr_a = 11;    // Intercalation index
        ril_a = 6;    // OK
        ril_b = 75;    // OK
        spz_b = 50;    // 64 fraction - OK
        spz_c = 66;    // 707 fraction - OK
        spz_f = 5;    // OK
        spz_j = 1355847;    // OK
        rahupart = 93;   // 230 fraction for Rahu cycle // OK
        meradd = 1977;    // OK
        venadd = 268;    // OK
        maradd = 3;    // OK
        jupadd = 512;    // OK
        satadd = 2988;   // OK
        dragkadd = 83343;
        zlapure = 1;
        tsurlug = 1;
    }
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

    init_settings();
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
            adj_zla ();
            cur_mth = zladag[0];  // Needed for general day
            gza_dru (cur_mth);
            nyi_dru (cur_mth);
            rilchaf (cur_mth); // MOVED, from gza_dag, NEW, 11/9/94

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
                    fprintf(fptgt, "\0x20\0x20%s\n", outbuf);

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
