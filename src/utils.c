/**
 * 1. Generalised Phugpa, -1000.",
 * 2. Generalised Tsurphu, -1000.",
 * 3. Generalised Error Correction, -2000.",
 * 4. mkhas pa'i snying nor, 1796.",
 * 5. New Genden Calculations, 1747 CE."
 */
int epch = 1;

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
        ZS_d, ZS_m, newmth, more, n, prv_byedpa, prv_byedpax, prv_sbyorba;

    // 1 = female, odd; 0 = male, even
    int yr_gender;

    // 1 = female, odd; 0 = male, even
    int mt_gender;

    // Adjusted, without negative.
    int mthnum;

    // Last and next JD calculations.
    int curjd, nextjd;

    int lastjd = 0;
    int sadag_str = 0;

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
            clear_a_b ();
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
