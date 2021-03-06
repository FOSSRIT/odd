#ifndef ODD
#define ODD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
//hardware
#include "tlc5947.h"
//audio
//#include <jack/jack.h>
#include <alsa/asoundlib.h>
#include <fftw3.h>
//threads
#include <pthread.h>
//networking
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <jansson.h>

#define NUM_LEDS (NUM_TLCS * 8)
#define PI 3.141592653

//Data Types
typedef struct odd_led_t {
	int R;
	int G;
	int B;
    struct odd_led_t *next;
} odd_led_t;

typedef void (*animation_f)(double*, double, odd_led_t*, double *);
typedef void (*modifier_f)( odd_led_t *[NUM_LEDS], odd_led_t *[NUM_LEDS] );

typedef struct animation_type {
	animation_f function;   //The function to call
	modifier_f modifier;    //The modifier to be used
	double* params;         //The params to be used
	odd_led_t* color;       //The color(s) the animation will be
    struct animation_type *next; //The next animation in the list
    double *storage;        //The pointer to the beginning of the storage for this animation
} animation_t;

//Driver Stuff
#define INCPORT 10001
#define MAXRCVLEN 500
#define LISTENQ (1024)

int getLED(int i, char color);
int getTempLED(int i, char color);
void setLED(int i, char color, int value);
void setTempLED(int i, char color, int value);
void addAnimation( animation_f function, double* params, odd_led_t* color, modifier_f modifier);
void updateAnimation( int index, double* params, odd_led_t* color);
void removeAnimation(int index);
int getNumAnimations();
char *getAnimationsInJson();

void thread_test();

//Networking
void *networkListen();
void *handleConnection(void *num);

//Animation Functions
typedef struct Animation {
    const char *name;
    animation_f function;
    char *paramDescriptions;
    int numParams;
    int numColors;
    int storageSize;
} Animation;

#define ANIMATION(func, name, paramDescs, num1, num2, num3)\
    void func(double *params, double totalTime, odd_led_t* color, double *storage);
#include "animations.def"
#undef ANIMATION

//Animation Modifier Functions
void invertTempLeds();
#define MODIFIER(func, name) void func(odd_led_t* leds[NUM_LEDS], odd_led_t* tempLeds[NUM_LEDS]);
#include "modifiers.def"
#undef MODIFIER

//Math Functions. No, I didn't need to write some of these. Shut up.
double odd_pow(double x, double y);
double odd_sin(double x);
float hann_window(int sample, int num_samples);
double formatTime(long int seconds, long int useconds);

//Audio related stuff
#define BUFSIZE (1024)
typedef float SAMPLE;

#define FFT_INPUT_SIZE (1024)
#define FFT_OUTPUT_SIZE (1024)

void getSoundBuffer(SAMPLE* buf);
void runFFT(SAMPLE* buf);
int audioInitialization();

//Socket stuff
#define INCPORT 10001
#define MAXRCVLEN 500
#define LISTENQ (1024)

//Device stuff
#define DEV "/dev/ttyUSB0"

#endif
