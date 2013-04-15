#ifndef RSCCODE_H
#define RSCCODE_H

typedef unsigned long BIT32;
typedef unsigned short BIT16;

class RscCode
{
public:
    RscCode();
    RscCode(int p);

    int getParity();
    void setParity(int p);
    int getBlockSize();
    void setBlockSize(int s);
    unsigned int getPolynom();
    void setPolynom(unsigned int p);

    void initialize_ecc();
    void zero_fill_from (unsigned char buf[], int from, int to);
    void print_parity (void);
    void print_syndrome (void);
    void build_codeword (unsigned char msg[], int nbytes, unsigned char dst[]);
    void decode_block(unsigned char data[], int nbytes);
    void decode(unsigned char codeword[], int nbytes, unsigned char res[]);
    int check_syndrome (void);
    void debug_check_syndrome (void);
    void compute_genpoly (int nbytes, int genpoly[]);
    void encode (unsigned char msg[], int nbytes, unsigned char dst[]);
    void encode_block (unsigned char msg[], int nbytes, unsigned char dst[]);
    void Modified_Berlekamp_Massey();
    void compute_modified_omega ();
    void mult_polys (int dst[], int p1[], int p2[]);
    void init_gamma (int gamma[]);
    void compute_next_omega (int d, int A[], int dst[], int src[]);
    int compute_discrepancy (int lambda[], int S[], int L, int n);
    void add_polys (int dst[], int src[]);
    void copy_poly (int dst[], int src[]);
    void scale_poly (int k, int poly[]);
    void zero_poly (int poly[]);
    void mul_z_poly (int src[]);
    void Find_Roots (void);
    int correct_errors_erasures (unsigned char codeword[],
                 int csize,
                 int nerasures,
                 int erasures[]);

    void init_galois_tables (void);
    void init_exp_table (void);
    int gmult(int a, int b);
    int ginv (int elt);

    BIT16 crc_ccitt(unsigned char *msg, int len);
    BIT16 crchware(BIT16 data, BIT16 genpoly, BIT16 accum);

private:
    int NPAR;
    unsigned int POLY;
    int blockSize;
    int MAXDEG;
    int DEBUG;
    int *pBytes;
    int *synBytes;
    int gexp[512];
    int glog[256];
    int *genPoly;

    int *Lambda;
    int *Omega;
    int ErrorLocs[256];
    int NErrors;
    int ErasureLocs[256];
    int NErasures;
};

#endif // RSCCODE_H
