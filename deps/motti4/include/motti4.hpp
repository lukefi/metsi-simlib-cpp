namespace Motti4 {
    typedef struct Motti4Spev9 {
        float ma;        // 1    mänty
        float ku;        // 2    kuusi
        float ra;        // 3    rauduskoivu
        float hi;        // 4    hieskoivu
        float ha;        // 5    haapa
        float hl;        // 6    harmaaleppä
        float tl;        // 7    tervaleppä
        float mh;        // 8    muu havupuu
        float ml;        // 9    muu lehtipuu
    } Motti4Spev9;

    typedef struct Motti4Spev10 {
        float total;     // 1
        float ma;        // 2
        float ku;        // 3
        float ra;        // 4
        float hi;        // 5
        float ha;        // 6
        float hl;        // 7
        float tl;        // 8
        float mh;        // 9
        float ml;        // 10
    } Motti4Spev10;

    typedef struct Motti4Biomass {
        float trunk;       // 1
        float waste;       // 2
        float branch_live; // 3
        float branch_dead; // 4
        float leaf;        // 5
        float base;        // 6
        float root_dense;  // 7
        float root_thin;   // 8
    } Motti4Biomass;

    typedef struct Motti4Tree {
        float id;            // 1
        float f;             // 2         1/ha
        float spe;           // 3
        float age;           // 4         vuosi
        float age13;         // 5         vuosi
        float d13;           // 6         cm
        float h;             // 7         m
        float cr;            // 8
        float snt;           // 9         syntytapa
        Motti4Spev10 ccftop; // 10..19
        Motti4Spev10 bal;    // 20..29
        float vol;           // 30        tilavuus           m^3
        float vol_t;         // 31        tukkitilavuus      m^3
        float vol_s;         // 32        pikkutukkitilavuus m^3
        float vol_f;         // 33        kuitutilavuus      m^3
        float waste;         // 34        hukkapuu
        float destr;         // 35        tuhon aste
        float crfix;         // 36
        float keh;           // 37
        float storie;        // 38
        float latraj;        // 39        m
        float crerror;       // 40
        float h0;            // 41        m
        float cr0;           // 42
        float crt;           // 43        vuosi
        float d13_0;         // 44        cm
        float sid;           // 45
        float fdead;         // 46        1/ha
        float xd;            // 47        cm/step
        float xg;            // 48        cm^2/step
        float xh;            // 49        m/step
        float xvol;          // 50
        float xvol_dead;     // 51
        float ba;            // 52        cm^2
        float _53;           // 53
        float thin1;         // 54
        float thin2;         // 55
        float _56[25];       // 56..87
        Motti4Biomass bm;    // 88        kg
        float _89[2];        // 89..90
    } Motti4Tree;

    typedef Motti4Tree Motti4Trees[1000];

    typedef struct Motti4Stratum {
        float spe;           // 1
        float age;           // 2
        float ba;            // 3
        float f;             // 4
        float h;             // 5
        float hw;            // 6
        float d;             // 7
        float dg;            // 8
        float storey;        // 9
        float st;            // 10
        float sid;           // 11
    } Motti4Stratum;

    typedef Motti4Stratum Motti4Strata[10];

    typedef struct Motti4SaplingStratum {
        float year;                            // 1
        float age;                             // 2
        float hdom;                            // 3
        float f_kkp, f_klv, f_vlj;             // 4, 5, 6
        float crfix_kkp, crfix_klv, crfix_vlj; // 7, 8, 9
        float N_kkp, N_klv, N_vlj;             // 10, 11, 12
        float h_kkp, h_klv, h_vlj;             // 13, 14, 15
        float d_kkp, d_klv, d_vlj;             // 16, 17, 18
        float osid_kkp, osid_klv, osid_vlj;    // 19, 20, 21
        float age_kkp, age_klv, age_vlj;       // 22, 23, 24
        float age13_kkp, age13_klv, age13_vlj; // 25, 26, 27
        float g_kkp, g_klv, g_vlj;             // 28, 29, 30
        float v_kkp, v_klv, v_vlj;             // 31, 32, 33
        float hg_kkp, hg_klv, hg_vlj;          // 34, 35, 36
        float dg_kkp, dg_klv, dg_vlj;          // 37, 38, 39
        float _40;                             // 40
    } Motti4SaplingStratum;

    typedef struct Motti4SaplingsSpev {
        Motti4SaplingStratum ma;        // 1
        Motti4SaplingStratum ku;        // 2
        Motti4SaplingStratum ra;        // 3
        Motti4SaplingStratum hi;        // 4
        Motti4SaplingStratum ha;        // 5
        Motti4SaplingStratum hl;        // 6
        Motti4SaplingStratum tl;        // 7
        Motti4SaplingStratum mh;        // 8
        Motti4SaplingStratum ml;        // 9
        Motti4SaplingStratum _10;       // 10
    } Motti4SaplingsSpev;

    typedef Motti4SaplingsSpev Motti4Saplings[10];

    typedef struct Motti4Fertilization {
        float year;         // 1   lannoituksen simulointivuosi
        float V0;           // 2   lannoitushetken puuston tilavuus
        float N0;           // 3   lannoitushetken puuston rulu
        float alr;          // 4   kuvion alaryhmä (ei käytetä)
        float _5;           // 5   simulointivuosi uudestaan (ei käytetä)
        float type;         // 6   lannoitetyyppi (ks. pymotti)
        float amount;       // 7   lannoitteen määrä
        float p;            // 8   laitettiinko fosforia? 1/0 (vain kangasmailla)
        float phos;         // 9   fosforin määrä (kg, vain turvemailla)
        float _10;          // 10
    } Motti4Fertilization;

    typedef float Motti4VcrArray[270];
    typedef float Motti4KorArray[2160];
    typedef Motti4Fertilization Motti4FerArray[10];

    typedef struct Motti4Ctrl {
        int death_forest;   // 1
        int death_tree;     // 2
        int _3;             // 3
        int _4;             // 4
        int _5;             // 5
        int _6;             // 6
        int _7;             // 7
        int _8;             // 8
        int calibrate;      // 9
        int _10;            // 10
    } Motti4Ctrl;

    typedef struct Motti4StoreyInfo {
        float age100;       // 1
        float h100;         // 2
        float g;            // 3
        float f;            // 4
        float dg;           // 5
        float spe;          // 6
    } Motti4StoreyInfo;

    typedef struct Motti4Site {
        float Y;            // 1     EUREF FIN km
        float X;            // 2     EUREF FIN km
        float Z;            // 3     m
        float lake;         // 4
        float sea;          // 5
        float dd;           // 6
        float _7;           // 7
        float _8;           // 8
        float _9;           // 9
        float _10;          // 10
        float rgn_nat_spe;  // 11     luontaisen uudistumisen puulaji     ar.spe  (3)        pl
        float rgn_seedratio; // 12    reunametsän suhteellinen siemennysala                  0-1
        float rgn_vlj_spe;  // 13     viljelty puulaji                    ar.spe  (3)        pl
        float rgn_f;        // 14     viljelytiheys                       ar.f    (4)        1/ha
        float rgn_surv;     // 15     viljelyn onnistumissuhde            ar.surv (2)        0-1
        float _16;          // 16
        float xt_regen;     // 17     vuosi
        float xt_muok;      // 18     vuosi                               huom: ar.mm nollaa uudistaessa
        float xt_raiv;      // 19     vuosi                               huom: ar.raiv nollaa uudistaessa
        float xt_fert_prev; // 20     vuosi
        float mal;          // 21
        float mty;          // 22
        float verl;         // 23
        float verlt;        // 24
        float alr;          // 25
        float pd;           // 26
        float _27;          // 27
        float muok;         // 28     maanmuokkaustyyppi                  ar.mm   (5)
        float _29;          // 29
        float _30;          // 30
        Motti4Spev9 si;     // 31..39
        float tkg;          // 40
        Motti4Spev9 hd50;   // 41..49
        float year;         // 50       vuosi
        float step;         // 51       vuosi
        float sid;          // 52
        float _53;          // 53
        float xt_perk;      // 54       vuosi
        float prt;          // 55
        float fthin;        // 56
        float xt_thin;      // 57       vuosi
        float xt_fert;      // 58       vuosi
        float fert_peat;    // 59
        float xt_thoit;     // 60       aika taimikonhoidosta         vuosi
        float drain;        // 61
        float xt_ndrain;    // 62       aika kunnostusojituksesta     vuosi
        float xt_rdrain;    // 63       aika uudisojituksesta         vuosi
        float _64;          // 64
        float _65;          // 65
        float _66;          // 66
        float _67;          // 67
        float _68;          // 68
        float xt_kar;       // 69       aika karsinnasta              vuosi
        float xt_fthin;     // 70       aika ensiharvennuksesta       vuosi
        float agedom;       // 71       vuosi
        float agedom13;     // 72       vuosi
        float ndom;         // 73
        float spedom;       // 74
        float spedom2;      // 75
        float dcond;        // 76       ojien kunto 1  hyvä,  <= 0  huono
        float kehl;         // 77
        float nstorey;      // 78
        float gstorey;      // 79
        Motti4StoreyInfo st1;       // 80..85      lo
        Motti4StoreyInfo st2;       // 86..91      hi
        Motti4StoreyInfo st3;       // 92..97      over
        Motti4StoreyInfo st4;       // 98..103     retain
        Motti4StoreyInfo st12;      // 104..109    12, ks. mottidok
        Motti4Spev10 hdom100;       // 110..119
        Motti4Spev10 hdom_j;        // 120..129
        Motti4Spev10 hg;            // 130..139
        Motti4Spev10 hf;            // 140..149
        Motti4Spev10 ddom100;       // 150..159
        Motti4Spev10 ddom_latv;     // 160..169
        Motti4Spev10 dg;            // 170..179
        Motti4Spev10 df;            // 180..189
        float h100_perk;    // 190
        float crdom;        // 191
        float crerror;      // 192
        float rimp;         // 193
        float vg;           // 194
        float v1;           // 195
        float v2;           // 196
        float v3;           // 197
        float v4;           // 198
        float v12;          // 199
        Motti4Spev10 f;     // 200..209    rl, taimet mukana                 1/ha
        Motti4Spev10 f13;   // 210..219    rl, vain isot puut                1/ha
        Motti4Spev10 G;     // 220..229
        Motti4Spev10 ccf;   // 230..239
        float _240[10];     // 240..249
        Motti4Spev10 ccfi;  // 250..259
        Motti4Spev10 V;     // 260..269
        float f_dead;       // 270
        float ba_dead;      // 271
        float v_dead;       // 272
        float _273;         // 273
        float _274;         // 274
        float _275;         // 275
        float _276;         // 276
        float _277;         // 277
        float jh;           // 278
        float jd;           // 279
        Motti4Spev10 xhdom; // 280..289
        float _290;         // 290
        float ddomg0;       // 291
        float dgdom0;       // 292
        float ba0;          // 293
        float h100_0;       // 294
        float cr100_0;      // 295
        float v0;           // 296
        float dg0;          // 297
        float _298;         // 298
        float dgM;          // 299
        float _300;         // 300
        // ------- yy2 ----------
        float _301;         // 1
        float _302;         // 2
        float _303;         // 3
        float _304;         // 4
        float _305;         // 5
        float _306;         // 6
        float itpl;         // 7          synt itpl
        float yev;          // 8          synt yer
        float plerc;        // 9          synt plerc
        float las;          // 10         synt las        tämä ei taida tehdä mitään?
        float rtvanha;      // 11         synt rtvanha    kasvatuskelpoisten määrä kun synt kutsuttiin?
        float rkvanha;      // 12         synt rkvanha    kehityskelvottomien määrä kun synt kutsuttiin?
        float _313;         // 13
        float _314;         // 14
        float _315;         // 15
        float _316;         // 16
        float _317;         // 17
        float _318;         // 18
        float _319;         // 19
        float _320;         // 20
        float _321;         // 21
        float _322;         // 22
        float xt_synt;      // 23         aika taimien syntymisestä
        float rt_synt;      // 24         syntyneiden taimien runkoluku (synt)
        float _325;         // 25
        float _326;         // 26
        float _327;         // 27
        float _328;         // 28
        float _329;         // 29
        float _330;         // 30
        float _331;         // 31
        float _332;         // 32
        float _333;         // 33
        float _334;         // 34
        float _335;         // 35
        float _336;         // 36
        float _337;         // 37
        float _338;         // 38
        float _339;         // 39
        float _340;         // 40
        float _341;         // 41
        float _342;         // 42
        float _343;         // 43
        float _344;         // 44
        float _345;         // 45
        float _346;         // 46
        float _347;         // 47
        float _348;         // 48
        float _349;         // 49
        float phak;         // 50          poimintahakkuu            1/0
        float xt_phak;      // 51          aika poimintahakkuusta    vuosi
        float _352;         // 52
        float _353;         // 53
        float _354;         // 54
        float skip_gstorey; // 55          ohita jaksopäättely       1/0
        float _356;         // 56
        float _357;         // 57
        float _358;         // 58
        float _359;         // 59
        float _360;         // 60
        float _361;         // 61
        float _362;         // 62
        float _363;         // 63
        float _364;         // 64
        float _365;         // 65
        float _366;         // 66
        float _367;         // 67
        float _368;         // 68
        float _369;         // 69
        float _370;         // 70
        float year_prev;    // 71          edellinen simulointivuosi kun taydyy kutsuttiin
        float _372;         // 72
        float _373;         // 73
        float _374;         // 74
        float _375;         // 75
        float _376;         // 76
        float _377;         // 77
        float _378;         // 78
        float _379;         // 79
        float _380;         // 80
        float _381;         // 81
        float _382;         // 82
        float _383;         // 83
        float _384;         // 84
        float _385;         // 85
        float _386;         // 86
        float _387;         // 87
        float _388;         // 88
        float _389;         // 89
        float _390;         // 90
        float _391;         // 91
        float _392;         // 92
        float _393;         // 93
        float _394;         // 94
        float _395;         // 95
        float _396;         // 96
        float _397;         // 97
        float _398;         // 98
        float _399;         // 99
        float _400;         // 100
        float _401[200];    // 101..300
    } Motti4Site;

    typedef struct Motti4RegenParm {
        float meth;        // 1  menetelmä (1=luonnollinen   2,3=istutuksia)
        float elop;        // 2  onnistumissuhde                0-100
        float spe;         // 3  viljelty puulaji               pl
        float f;           // 4  viljelytiheys                  1/ha
        float mm;          // 5  maanmuokkaustyyppi
        float raiv;        // 6  raivataanko?                   1/0
        float spe_seed;    // 7  siementävä puulaji luontaisessa uudistamisessa
    } Motti4RegenParm;

    typedef struct Motti4EarlyCareInfo {
        float f;           // 1
        float fdel;        // 2
        float d13;         // 3
        float h;           // 4
        float _5[5];       // 5..9
    } Motti4EarlyCareInfo;

    void Motti4SiteInit(Motti4Site *yy, float *Y, float *X, float *Z, int *rv);

    void Motti4Init(
            Motti4Strata *yo, Motti4Site *yy, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, Motti4Trees *yp, Motti4Ctrl *o,
            int *numtrees, int *err, int *rv
    );

    void Motti4UpdateAfterImport(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, int *rv
    );

    void Motti4Growth(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, Motti4FerArray *fer,
            int *numfer, Motti4Ctrl *o, int *step, int *rv
    );

    void Motti4Regenerate(
            Motti4RegenParm *meth, Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut,
            Motti4KorArray *kor, Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees,
            int *step, int *rv
    );

    void Motti4TaimiViive(Motti4Site *yy, Motti4Saplings *ut, int *step, int *spe, int *rv);

    void Motti4FillinPlanting(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, int *rspe, float *num,
            int *osid, int *rv
    );

    void Motti4EarlyCare(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, Motti4EarlyCareInfo *ar,
            int *mode, int *rv
    );

    void Motti4PCTGuidelines(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, Motti4Spev9 *rem,
            int *rv
    );

    void Motti4PCT(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, Motti4Spev9 *rem,
            int *rv
    );

    void Motti4MineralSoilsFertilization(
            int *ftype, float *N, int *phosphorus,
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, Motti4FerArray *fer,
            int *numfer, int *rv
    );

    void Motti4PeatlandFertilization(
            int *ftype,
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, Motti4FerArray *fer,
            int *numfer, int *ecode, int *rv
    );

    void Motti4AfterSeedtreeCutting(
            Motti4Site *yy, Motti4Trees *yp, Motti4Saplings *ut, Motti4KorArray *kor,
            Motti4VcrArray *vcr, Motti4KorArray *apv, int *numtrees, int *ecode, int *rv
    );

    void Motti4CheckYY(Motti4Site *yy, int *nerr, int *err);

    void CheckStrata(Motti4Site *yy, Motti4Strata *yo, int *res);
}