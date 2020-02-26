//
// Created by daran on 1/12/2017 to be used in ECE420 Sp17 for the first time.
// Modified by dwang49 on 1/1/2018 to adapt to Android 7.0 and Shield Tablet updates.
//
// ProcessFrame implemented by Shaobo Li

#include "ece420_main.h"

// Student Variables
#define FRAME_SIZE 128

// FIR Filter Function Defined here located at the bottom
int16_t firFilter(int16_t sample);

void ece420ProcessFrame(sample_buf *dataBuf) {
    // Keep in mind, we only have a small amount of time to process each buffer!
    struct timeval start;
    gettimeofday(&start, NULL);

    // Using {} initializes all values in the array to zero
    int16_t bufferIn[FRAME_SIZE] = {};
    int16_t bufferOut[FRAME_SIZE] = {};

    // Your buffer conversion (unpacking) here
    // Fetch data sample from dataBuf->buf_[], unpack and put into bufferIn[]
    // ******************** START YOUR CODE HERE ******************** //

    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        int16_t sample = 0;
        sample |= dataBuf->buf_[2*i];
        sample = sample<<8;
        sample |= dataBuf->buf_[2*i+1] ;
        bufferIn[i] = sample;
    }


    // ********************* END YOUR CODE HERE ********************* //

    // Loop code provided as a suggestion. This loop simulates sample-by-sample processing.
    for (int sampleIdx = 0; sampleIdx < FRAME_SIZE; sampleIdx++) {
        // Grab one sample from bufferIn[]
        int16_t sample = bufferIn[sampleIdx];
        // Call your filFilter funcion
        int16_t output = firFilter(sample);
        // Grab result and put into bufferOut[]
        bufferOut[sampleIdx] = output;
    }

    // Your buffer conversion (packing) here
    // Fetch data sample from bufferOut[], pack them and put back into dataBuf->buf_[]
    // ******************** START YOUR CODE HERE ******************** //
    for(uint32_t i = 0; i < FRAME_SIZE; i++){
        uint8_t hbyte, lbyte;
        hbyte = (uint8_t) bufferOut[i] >> 8;
        lbyte = (uint8_t) bufferOut[i] & 0x00FF;
        dataBuf->buf_[2*i+1] = lbyte;
        dataBuf->buf_[2*i] = hbyte;
    }


    // ********************* END YOUR CODE HERE ********************* //

	// Log the processing time to Android Monitor or Logcat window at the bottom
    struct timeval end;
    gettimeofday(&end, NULL);
    LOGD("Loop timer: %ld us",  ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

}

// TODO: Change N_TAPS to match your filter design
#define N_TAPS 7

// TODO: Change myfilter to contain the coefficients of your designed filter.
double myfilter[N_TAPS] = {-0.006216437840355568, -0.007627372360667975, -0.008515480349417954, 0.9911814076240004, -0.008515480349417954, -0.007627372360667975, -0.006216437840355568};

// Circular Buffer
int16_t circBuf[N_TAPS] = {};
int16_t circBufIdx = 0;

// FirFilter Function
int16_t firFilter(int16_t sample) {
    // This function simulates sample-by-sample processing. Here you will
    // implement an FIR filter such as:
    //
    // y[n] = a x[n] + b x[n-1] + c x[n-2] + ...
    //
    // You will maintain a circular buffer to store your prior samples
    // x[n-1], x[n-2], ..., x[n-k]. Suggested initializations circBuf
    // and circBufIdx are given.
    //
    // Input 'sample' is the current sample x[n].
    // ******************** START YOUR CODE HERE ******************** //
    int16_t output = 0;

    circBuf[circBufIdx] = sample;
    double out= 0;


    for (int i = 0; i < N_TAPS; i++) {
        int k = (N_TAPS + circBufIdx - i)%N_TAPS;
        double  inc = (circBuf[ k] * myfilter[i]);
        out += inc;
    }

    circBufIdx = (circBufIdx+1)%N_TAPS;

    output = out;

    // ********************* END YOUR CODE HERE ********************* //
    return output;

}
