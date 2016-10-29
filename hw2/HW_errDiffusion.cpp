void gammaCorrect(ImagePtr, double, ImagePtr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_errDiffusion:
//
// Apply error diffusion algorithm to image I1.
//
// This procedure produces a black-and-white dithered version of I1.
// Each pixel is visited and if it + any error that has been diffused to it
// is greater than the threshold, the output pixel is white, otherwise it is black.
// The difference between this new value of the pixel from what it used to be
// (somewhere in between black and white) is diffused to the surrounding pixel
// intensities using different weighting systems.
//
// Use Floyd-Steinberg     weights if method=0.
// Use Jarvis-Judice-Ninke weights if method=1.
//
// Use raster scan (left-to-right) if serpentine=0.
// Use serpentine order (alternating left-to-right and right-to-left) if serpentine=1.
// Serpentine scan prevents errors from always being diffused in the same direction.
//
// A circular buffer is used to pad the edges of the image.
// Since a pixel + its error can exceed the 255 limit of uchar, shorts are used.
//
// Apply gamma correction to I1 prior to error diffusion.
// Output is saved in I2.
//
void
HW_errDiffusion(ImagePtr I1, int method, bool serpentine, double gamma, ImagePtr I2)
{
    ImagePtr tempImage;
    IP_copyImageHeader(I1, I2);
    IP_copyImageHeader(I1, tempImage);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    int type, y, x, ch, bufferSize, i;
    ChannelPtr<uchar> p1, p2, endd;

    gammaCorrect(I1, gamma, tempImage);

    if(method == 0) {
        short* in1;
        short* in2;
        int thr = MXGRAY/2;
        int error;
        bufferSize = w+2;
        short* bufferUp   = new short[bufferSize];
        short* bufferDown = new short[bufferSize];

        for(ch = 0; IP_getChannel(tempImage, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            bufferUp[0] = *p1;
            bufferUp[bufferSize-1] = *(p1+w-1);
            for (i = 1; i < bufferSize-1; i++) {
                bufferUp[i] = *p1++;
            }
            for(y = 1; y < h; y++) {
                if (y%2 == 0) {
                    bufferUp[0] = *p1;
                    bufferUp[bufferSize-1] = *(p1+w-1);
                    for (i = 1; i < bufferSize-1; i++) {
                        bufferUp[i] = *p1++;
                    }
                } else {
                    bufferDown[0] = *p1;
                    bufferDown[bufferSize-1] = *(p1+w-1);
                    for (i = 1; i < bufferSize-1; i++) {
                        bufferDown[i] = *p1++;
                    }
                }

                if(serpentine) {
                    if (y%2 == 0) {
                        p2 = p2 + w - 1;
                        in2 = bufferUp   + bufferSize - 2;
                        in1 = bufferDown + bufferSize - 2;
                        for (x = 0; x < w; x++) {
                            *p2 = (*in1 < thr)? 0 : 255;
                            error = *in1 - *p2;
                            *(in1-1)   += (error * 7/16);
                            *(in2+1)   += (error * 3/16);
                            *(in2)     += (error * 5/16);
                            *(in2-1)   += (error * 1/16);
                            in1--;
                            in2--;
                            p2--;
                        }
                        p2 = p2 + w + 1;
                    } else {
                        in1 = bufferUp   + 1;
                        in2 = bufferDown + 1;
                        for (x = 0; x < w; x++) {
                            *p2 = (*in1 < thr)? 0 : 255;
                            error = *in1 - *p2;
                            *(in1+1)   += (error * 7/16);
                            *(in2-1)   += (error * 3/16);
                            *(in2)     += (error * 5/16);
                            *(in2+1)   += (error * 1/16);
                            in1++;
                            in2++;
                            p2++;
                        }
                    }
                } else {
                    if (y%2 == 0) {
                        in2 = bufferUp   + 1;
                        in1 = bufferDown + 1;
                    } else {
                        in1 = bufferUp   + 1;
                        in2 = bufferDown + 1;
                    }
                    for (x = 0; x < w; x++) {
                        *p2 = (*in1 < thr)? 0 : 255;
                        error = *in1 - *p2;
                        *(in1+1)   += (error * 7/16);
                        *(in2-1)   += (error * 3/16);
                        *(in2)     += (error * 5/16);
                        *(in2+1)   += (error * 1/16);
                        in1++;
                        in2++;
                        p2++;
                    }
                }
            }
        }
        delete [] bufferUp;
        delete [] bufferDown;
    } else if(method == 1) {

    } else {
        for(int ch = 0; IP_getChannel(tempImage, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd;) *p2++ = *p1++;
        }
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// gammaCorrect:
//
// Apply gamma correction to image I1.
// Save result in I2.
//
void
gammaCorrect(ImagePtr I1, double gamma, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
	int w = I1->width ();
	int h = I1->height();
	int total = w * h;

	// init lookup table
	int i, lut[MXGRAY];
	for(i=0; i<MXGRAY; ++i) {
        lut[i] = CLIP(pow((double)i/MaxGray, 1/gamma) * MaxGray, 0, MaxGray);
    }

	// evaluate output: each input pixel indexes into lut[] to eval output
	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);
		for(endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
	}
}
