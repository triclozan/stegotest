#include "rsccode.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

RscCode::RscCode(int p) {
    NPAR = p;
    POLY = 0x1D;
    MAXDEG = NPAR * 2;
    pBytes = new int[MAXDEG];
    synBytes = new int[MAXDEG];
    genPoly = new int[MAXDEG*2];
    Lambda = new int[MAXDEG];
    Omega = new int[MAXDEG];
    DEBUG = 0;
}

int RscCode::getParity() {
    return NPAR;
}

void RscCode::setParity(int p) {
    if (p != NPAR) {
        NPAR = p;
        POLY = 0x1D;
        MAXDEG = NPAR * 2;
        delete pBytes;
        delete synBytes;
        delete genPoly;
        delete Lambda;
        delete Omega;
        pBytes = new int[MAXDEG];
        synBytes = new int[MAXDEG];
        for (int i=0; i<MAXDEG; i++) {
            synBytes[i] = 0;
        }
        genPoly = new int[MAXDEG*2];
        Lambda = new int[MAXDEG];
        Omega = new int[MAXDEG];
        DEBUG = 0;
    }
}

unsigned int RscCode::getPolynom() {
    return POLY;
}

void RscCode::setPolynom(unsigned int p) {
    POLY = p;
}

int RscCode::getBlockSize() {
    return blockSize;
}

void RscCode::setBlockSize(int s) {
    blockSize = s;
}

void RscCode::initialize_ecc()
{
    init_galois_tables();
    compute_genpoly(NPAR, genPoly);
}

void RscCode::zero_fill_from (unsigned char buf[], int from, int to)
{
  int i;
  for (i = from; i < to; i++) buf[i] = 0;
}

void RscCode::print_parity (void)
{
  int i;
  printf("Parity Bytes: ");
  for (i = 0; i < NPAR; i++)
    printf("[%d]:%x, ",i,pBytes[i]);
  printf("\n");
}


void RscCode::print_syndrome (void)
{
  int i;
  printf("Syndrome Bytes: ");
  for (i = 0; i < NPAR; i++)
    printf("[%d]:%x, ",i,synBytes[i]);
  printf("\n");
}

void RscCode::build_codeword (unsigned char msg[], int nbytes, unsigned char dst[])
{
  int i;

  for (i = 0; i < nbytes; i++) dst[i] = msg[i];

  for (i = 0; i < NPAR; i++) {
    dst[i+nbytes] = pBytes[NPAR-1-i];
  }
}

void RscCode::decode_block(unsigned char data[], int nbytes)
{
  int i, j, sum;
  for (int i=0; i<MAXDEG; i++) {
      synBytes[i] = 0;
  }
  for (j = 0; j < NPAR;  j++) {
    sum	= 0;
    for (i = 0; i < nbytes; i++) {
      sum = data[i] ^ gmult(gexp[j+1], sum);
    }
    synBytes[j]  = sum;
  }
}

int RscCode::check_syndrome (void)
{
 int i, nz = 0;
 for (i =0 ; i < NPAR; i++) {
  if (synBytes[i] != 0) {
      nz = 1;
      break;
  }
 }
 return nz;
}


void RscCode::debug_check_syndrome (void)
{
  int i;

  for (i = 0; i < 3; i++) {
    printf(" inv log S[%d]/S[%d] = %d\n", i, i+1,
       glog[gmult(synBytes[i], ginv(synBytes[i+1]))]);
  }
}

void RscCode::compute_genpoly (int nbytes, int genpoly[])
{
  int i, tp[256], tp1[256];

  zero_poly(tp1);
  tp1[0] = 1;

  for (i = 1; i <= nbytes; i++) {
    zero_poly(tp);
    tp[0] = gexp[i];
    tp[1] = 1;

    mult_polys(genpoly, tp, tp1);
    copy_poly(tp1, genpoly);
  }
}

void RscCode::encode_block (unsigned char msg[], int nbytes, unsigned char dst[])
{
  int i, LFSR[NPAR+1],dbyte, j;

  for(i=0; i < NPAR+1; i++) LFSR[i]=0;

  for (i = 0; i < nbytes; i++) {
    dbyte = msg[i] ^ LFSR[NPAR-1];
    for (j = NPAR-1; j > 0; j--) {
      LFSR[j] = LFSR[j-1] ^ gmult(genPoly[j], dbyte);
    }
    LFSR[0] = gmult(genPoly[0], dbyte);
  }

  for (i = 0; i < NPAR; i++)
    pBytes[i] = LFSR[i];

  build_codeword(msg, nbytes, dst);
}

//-----------------------------------------------------------------------------

void RscCode::Modified_Berlekamp_Massey (void)
{
  int n, L, L2, k, d, i;
  int psi[MAXDEG], psi2[MAXDEG], D[MAXDEG];
  int gamma[MAXDEG];

  /* initialize Gamma, the erasure locator polynomial */
  init_gamma(gamma);

  /* initialize to z */
  copy_poly(D, gamma);
  mul_z_poly(D);

  copy_poly(psi, gamma);
  k = -1; L = NErasures;

  for (n = NErasures; n < NPAR; n++) {

    d = compute_discrepancy(psi, synBytes, L, n);

    if (d != 0) {

      /* psi2 = psi - d*D */
      for (i = 0; i < MAXDEG; i++) psi2[i] = psi[i] ^ gmult(d, D[i]);


      if (L < (n-k)) {
    L2 = n-k;
    k = n-L;
    /* D = scale_poly(ginv(d), psi); */
    for (i = 0; i < MAXDEG; i++) D[i] = gmult(psi[i], ginv(d));
    L = L2;
      }

      /* psi = psi2 */
      for (i = 0; i < MAXDEG; i++) psi[i] = psi2[i];
    }

    mul_z_poly(D);
  }

  for(i = 0; i < MAXDEG; i++) Lambda[i] = psi[i];
  compute_modified_omega();


}

void RscCode::compute_modified_omega ()
{
  int i;
  int product[MAXDEG*2];

  mult_polys(product, Lambda, synBytes);
  zero_poly(Omega);
  for(i = 0; i < NPAR; i++) Omega[i] = product[i];

}


void RscCode::mult_polys (int dst[], int p1[], int p2[])
{
  int i, j;
  int tmp1[MAXDEG*2];

  for (i=0; i < (MAXDEG*2); i++) dst[i] = 0;

  for (i = 0; i < MAXDEG; i++) {
    for(j=MAXDEG; j<(MAXDEG*2); j++) tmp1[j]=0;

    /* scale tmp1 by p1[i] */
    for(j=0; j<MAXDEG; j++) tmp1[j]=gmult(p2[j], p1[i]);
    /* and mult (shift) tmp1 right by i */
    for (j = (MAXDEG*2)-1; j >= i; j--) tmp1[j] = tmp1[j-i];
    for (j = 0; j < i; j++) tmp1[j] = 0;

    /* add into partial product */
    for(j=0; j < (MAXDEG*2); j++) dst[j] ^= tmp1[j];
  }
}

void RscCode::init_gamma (int gamma[])
{
  int e, tmp[MAXDEG];

  zero_poly(gamma);
  zero_poly(tmp);
  gamma[0] = 1;

  for (e = 0; e < NErasures; e++) {
    copy_poly(tmp, gamma);
    scale_poly(gexp[ErasureLocs[e]], tmp);
    mul_z_poly(tmp);
    add_polys(gamma, tmp);
  }
}

void RscCode::compute_next_omega (int d, int A[], int dst[], int src[])
{
  int i;
  for ( i = 0; i < MAXDEG;  i++) {
    dst[i] = src[i] ^ gmult(d, A[i]);
  }
}



int RscCode::compute_discrepancy (int lambda[], int S[], int L, int n)
{
  int i, sum=0;

  for (i = 0; i <= L; i++)
    sum ^= gmult(lambda[i], S[n-i]);
  return (sum);
}

void RscCode::add_polys (int dst[], int src[])
{
  int i;
  for (i = 0; i < MAXDEG; i++) dst[i] ^= src[i];
}

void RscCode::copy_poly (int dst[], int src[])
{
  int i;
  for (i = 0; i < MAXDEG; i++) dst[i] = src[i];
}

void RscCode::scale_poly (int k, int poly[])
{
  int i;
  for (i = 0; i < MAXDEG; i++) poly[i] = gmult(k, poly[i]);
}

void RscCode::zero_poly (int poly[])
{
  int i;
  for (i = 0; i < MAXDEG; i++) poly[i] = 0;
}

void RscCode::mul_z_poly (int src[])
{
  int i;
  for (i = MAXDEG-1; i > 0; i--) src[i] = src[i-1];
  src[0] = 0;
}

void RscCode::Find_Roots (void)
{
  int sum, r, k;
  NErrors = 0;

  for (r = 1; r < 256; r++) {
    sum = 0;
    /* evaluate lambda at r */
    for (k = 0; k < NPAR+1; k++) {
      sum ^= gmult(gexp[(k*r)%255], Lambda[k]);
    }
    if (sum == 0)
      {
    ErrorLocs[NErrors] = (255-r); NErrors++;
    if (DEBUG) fprintf(stderr, "Root found at r = %d, (255-r) = %d\n", r, (255-r));
      }
  }
}

int RscCode::correct_errors_erasures (unsigned char codeword[],
             int csize,
             int nerasures,
             int erasures[])
{
  int r, i, j, err;

  /* If you want to take advantage of erasure correction, be sure to
     set NErasures and ErasureLocs[] with the locations of erasures.
     */
  NErasures = nerasures;
  for (i = 0; i < NErasures; i++) ErasureLocs[i] = erasures[i];

  Modified_Berlekamp_Massey();
  Find_Roots();


  if ((NErrors <= NPAR) && NErrors > 0) {

    /* first check for illegal error locs */
    for (r = 0; r < NErrors; r++) {
      if (ErrorLocs[r] >= csize) {
    if (DEBUG) fprintf(stderr, "Error loc i=%d outside of codeword length %d\n", i, csize);
    return(0);
      }
    }

    for (r = 0; r < NErrors; r++) {
      int num, denom;
      i = ErrorLocs[r];
      /* evaluate Omega at alpha^(-i) */

      num = 0;
      for (j = 0; j < MAXDEG; j++)
    num ^= gmult(Omega[j], gexp[((255-i)*j)%255]);

      /* evaluate Lambda' (derivative) at alpha^(-i) ; all odd powers disappear */
      denom = 0;
      for (j = 1; j < MAXDEG; j += 2) {
    denom ^= gmult(Lambda[j], gexp[((255-i)*(j-1)) % 255]);
      }

      err = gmult(num, ginv(denom));
      if (DEBUG) fprintf(stderr, "Error magnitude %#x at loc %d\n", err, csize-i);

      codeword[csize-i-1] ^= err;
    }
    return(1);
  }
  else {
    if (DEBUG && NErrors) fprintf(stderr, "Uncorrectable codeword\n");
    return(0);
  }
}
//----------------------------------------------------------------------
void RscCode::init_galois_tables (void)
{
  /* initialize the table of powers of alpha */
  init_exp_table();
}

void RscCode::init_exp_table (void)
{
  int i, z;
  int pinit,p1,p2,p3,p4,p5,p6,p7,p8;

  pinit = p2 = p3 = p4 = p5 = p6 = p7 = p8 = 0;
  p1 = 1;

  gexp[0] = 1;
  gexp[255] = gexp[0];
  glog[0] = 0;			/* shouldn't log[0] be an error? */

  for (i = 1; i < 256; i++) {
    pinit = p8;
    p8 = p7;
    p7 = p6;
    p6 = p5;
    p5 = p4 ^ pinit;
    p4 = p3 ^ pinit;
    p3 = p2 ^ pinit;
    p2 = p1;
    p1 = pinit;
    gexp[i] = p1 + p2*2 + p3*4 + p4*8 + p5*16 + p6*32 + p7*64 + p8*128;
    gexp[i+255] = gexp[i];
  }

  for (i = 1; i < 256; i++) {
    for (z = 0; z < 256; z++) {
      if (gexp[z] == i) {
    glog[i] = z;
    break;
      }
    }
  }
}

int RscCode::gmult(int a, int b)
{
  int i,j;
  if (a==0 || b == 0) return (0);
  i = glog[a];
  j = glog[b];
  return (gexp[i+j]);
}


int RscCode::ginv (int elt)
{
  return (gexp[255-glog[elt]]);
}
//----------------------------------------------------------
BIT16 RscCode::crc_ccitt(unsigned char *msg, int len)
{
    int i;
    BIT16 acc = 0;

    for (i = 0; i < len; i++) {
        acc = crchware((BIT16) msg[i], (BIT16) 0x1021, acc);
    }

    return(acc);
}

BIT16 RscCode::crchware(BIT16 data, BIT16 genpoly, BIT16 accum)
{
    static BIT16 i;
    data <<= 8;
    for (i = 8; i > 0; i--) {
        if ((data ^ accum) & 0x8000)
            accum = ((accum << 1) ^ genpoly) & 0xFFFF;
        else
            accum = (accum<<1) & 0xFFFF;
        data = (data<<1) & 0xFFFF;
    }
    return (accum);
}

//-----------------------------------------------------------
void RscCode::encode(unsigned char msg[], int nbytes, unsigned char dst[])
{
    int k = 0;
    for (int i=0; i<nbytes; i+=blockSize) {
        encode_block(msg + i, blockSize, dst + (i + k*NPAR));
        k++;
    }
}
//------------------------------------------------------------
void RscCode::decode(unsigned char codeword[], int nbytes, unsigned char res[])
{
    int k = 0;
    for (int i=0; i<nbytes; i+=blockSize) {
        decode_block(codeword + (i + k*NPAR), blockSize + NPAR);
        if (check_syndrome () != 0) {
            correct_errors_erasures (codeword + (i + k*NPAR), blockSize + NPAR, 0, 0);
        }
        memcpy(res + i, codeword + (i + k*NPAR), (nbytes-i <= blockSize) ? blockSize : nbytes - i);
        k++;
    }
}

