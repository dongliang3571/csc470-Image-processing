#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415927

typedef struct {
    int len;
    float *real;
    float *imag;
} complexS, *complexP;

void fft(FILE *in, int dir, FILE *out);
void _fft(complexP I1, int dir, complexP I2);
int write_output(complexS in, FILE *out);


int main(int argc, char *argv[]) {

    // file I/O
    FILE *in, *out;

    if(argc == 4) {
        char *in_name = argv[1];
        char *out_name = argv[3];
        int dir = atoi(argv[2]);

        // open input and output file
        in = fopen(in_name, "r");
        out = fopen(out_name, "w");
        fft(in, dir, out);
        return 0;
    } else {
        printf("Expected 3 argument\n");
        return 0;
    }

    fclose(in);
    fclose(out);

    return 0;
}

void fft(FILE *in, int dir, FILE *out) {
    // get width and height
    int width, height;
    fscanf(in, "%d %d", &width, &height);

    int n, k, N = height, done;
    float c, s, real, imag;
    complexP I1, I2;
    I1->len = height;
    I1->real = malloc(sizeof(float)*N);
    I1->imag = malloc(sizeof(float)*N);

    I2->len = height;
    I2->real = malloc(sizeof(float)*N);
    I2->imag = malloc(sizeof(float)*N);

    for (int i = 0; i < N; i++) {
        fscanf(in, "%f %f", &I1->real[i], &I2->imag[i]);
    }

    // do FFT
    _fft(I1, dir, I2);
    // write_output(*I2, out);

    // Free memory
    free(I1->real);
    free(I1->imag);
    free(I2->real);
    free(I2->imag);
    return;
}


int write_output(complexS in, FILE *out) {
    int done = fprintf(out, "%d %d\n", 2, in.len);
    if(done < 0) {
        return done;
    }

    for(int i = 0; i < in.len; i++) {
        done = fprintf(out, "%f %f\n", in.real[i], in.imag[i]);
        if(done < 0) {
            return done;
        }
    }
    return 1;
}

void _fft(complexP I1, int dir, complexP I2) {

        int i, N, N2;
        float *r1, *i1, *r2, *i2, *ra, *ia, *rb, *ib;
        double FCTR, fctr, a, b, c, s;
        complexP Ia, Ib;

        N = I1->len;
        r1 = I1->real;
        i1 = I1->imag;
        r2 = I2 ->real;
        i2 = I2 ->imag;

        if(N == 2) {	// F(0)=f(0)+f(1); F(1)=f(0)-f(1)
    		a = r1[0] + r1[1];	// a, b needed when r1=r2
    		b = i1[0] + i1[1];
    		r2[1] = r1[0] - r1[1];
    		i2[1] = i1[0] - i1[1];
    		r2[0] = a;
    		i2[0] = b;
    	} else {
    		N2 = N / 2;
            Ia->len = N2;
            Ia->real = malloc(sizeof(float)*N2);
            Ia->imag = malloc(sizeof(float)*N2);

            Ib->len = N2;
            Ib->real = malloc(sizeof(float)*N2);
            Ib->imag = malloc(sizeof(float)*N2);

    		ra = Ia->real;
            ia = Ia->imag;
    		rb = Ib->real;
            ib = Ib->imag;

    		// split list into 2 halves: even and odd
    		for(i=0; i<N2; i++) {
    			ra[i] = *r1++;		ia[i] = *i1++;
    			rb[i] = *r1++;		ib[i] = *i1++;
    		}

    		// compute fft on both lists
    		_fft(Ia, dir, Ia);
    		_fft(Ib, dir, Ib);

    		// build up coefficients
    		if(!dir)	// forward
    			FCTR = -PI / N;
    		else	FCTR =  PI / N;
    		for(fctr=i=0; i<N2; i++, fctr+=FCTR) {
    			c = cos(fctr);
    			s = sin(fctr);
    			a = c*rb[i] - s*ib[i];
    			r2[i]	 = ra[i] + a;
    			r2[i+N2] = ra[i] - a;

    			a = s*rb[i] + c*ib[i];
    			i2[i]	 = ia[i] + a;
    			i2[i+N2] = ia[i] - a;
    		}
            free(Ia->real);
            free(Ia->imag);
            free(Ib->real);
            free(Ib->imag);
    	}
    	// if(dir) {	// inverse: divide by log N
        //     for(i = 0; i < N; i++) {
        //         I2->real[i] /= 2;
        //     }
    	// }
}
