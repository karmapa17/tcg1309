#define BCDMAX 20    // was 24

void l2bcd(signed char *, int);
int bcd2l(signed char *);
void addbcd(signed char *, signed char *, signed char *);
int compabsbcd(signed char *, signed char *);
void subbcd(signed char *, signed char *, signed char *);
void mulbcdl(signed char *, signed char *, int);
void divbcdl(signed char *, signed char *, int);
void modbcdl(signed char *, signed char *, int);
