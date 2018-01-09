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

void init_settings(epch)
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
        eyr_a = 11;   // Intercalation index
        ril_a = 6; // OK
        ril_b = 75; // OK
        spz_b = 50;   // 64 fraction - OK
        spz_c = 66;   // 707 fraction - OK
        spz_f = 5;    // OK
        spz_j = 1355847;  // OK
        rahupart = 93; // 230 fraction for Rahu cycle // OK
        meradd = 1977; // OK
        venadd = 268; // OK
        maradd = 3; // OK
        jupadd = 512; // OK
        satadd = 2988; // OK
        dragkadd = 83343;
        zlapure = 1;
        tsurlug = 1;
    }
}

void prn_cal(void)
{
    set_lang();
}
