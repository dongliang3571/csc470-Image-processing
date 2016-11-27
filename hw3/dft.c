#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415927

typedef struct {
    int len;
    float *real;
    float *imag;
} complexS, *complexP;

void dft(FILE *in, int dir, FILE *out);
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
        dft(in, dir, out);
        return 0;
    } else {
        printf("Expected 3 argument\n");
        return 0;
    }

    fclose(in);
    fclose(out);

    return 0;
}

void dft(FILE *in, int dir, FILE *out) {
    // get width and height
    int width, height;
    fscanf(in, "%d %d", &width, &height);

    int n, k, N = height, done;
    float c, s, real, imag;
    complexS f, F;
    f.len = height;
    f.real = malloc(sizeof(float)*N);
    f.imag = malloc(sizeof(float)*N);

    F.len = height;
    F.real = malloc(sizeof(float)*N);
    F.imag = malloc(sizeof(float)*N);

    // DFT or IDFT
    switch (dir) {
        case 0:
            for (int i = 0; i < N; i++) {
                fscanf(in, "%f %f", &f.real[i], &f.imag[i]);
            }

            for(n = 0; n < N; n++) {        /* compute spectrum over all freq u */
                real = imag = 0;    /* reset real, imag component of F(u) */
                for(k = 0; k < N; k++) {    /* visit each input pixel */
                    c =  cos(2.*PI*n*k/N);
                    s = -sin(2.*PI*n*k/N);
                    real += (f.real[k]*c - f.imag[k]*s);
                    imag += (f.real[k]*s + f.imag[k]*c);
                }
                F.real[n] = real / N;
                F.imag[n] = imag / N;
            }

            done = write_output(F, out);
            if(done > 0) {
                printf("succeed!\n");
            } else {
                printf("failed to write to output file!\n");
            }
            break;
        case 1:
            for (int i = 0; i < N; i++) {
                fscanf(in, "%f %f", &F.real[i], &F.imag[i]);
            }

            for(k = 0; k < N; k++) {        /* compute each output pixel */
                real = imag = 0;
                for(n=0; n<N; n++) {
                    c = cos(2.*PI*n*k/N);
                    s = sin(2.*PI*n*k/N);
                    real += (F.real[n]*c - F.imag[n]*s);
                    imag += (F.real[n]*s + F.imag[n]*c);
                }
                f.real[k] = real;
                f.imag[k] = imag;
            }
            done = write_output(f, out);
            if(done > 0) {
                printf("succeed!\n");
            } else {
                printf("failed to write to output file!\n");
            }
            break;
        default:
        printf("invalid option");
        return;
        break;
    }

    free(f.real);
    free(f.imag);
    free(F.real);
    free(F.imag);
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
