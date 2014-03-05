#include "../odd.h"

//Animation: Lights up a clump of LEDs and then travels back and forth across the row
//Follows a basic sin curve (moves faster in the middle, slower at the ends)
void cylonEye(double *params, double totalTime, odd_led_t *color, double *storage) {
    double speed = params[0];
    double radius = params[1];

    //scale the time by our speed to alter the rate of tf the animation
    double time = totalTime * speed;
    //double to keep track of the location of the center
    double center = 0.0;
    //we want the center to go between 0 and NUM_LEDS - 1, due to 0 based indexing of leds.
    int numLeds = NUM_LEDS - 1;
    //clear the tempLeds array so we can use it
    for(int i = 0; i < NUM_LEDS; i++)
    {
        setTempLED(i, 'r', 0);
        setTempLED(i, 'g', 0);
        setTempLED(i, 'b', 0);
    }
    
    //Calculate the center
    if((int)time % 2 == 1)
        center = odd_remainder(time, 1);
    else
        center = 1 - odd_remainder(time, 1);
    center = odd_sin(center);
    center *= numLeds;
    
    
    //Calculate the distance of each LED from the center, and do some math to figure out each LED's brightness
    double ledDistances[NUM_LEDS];
    for(int i = 0; i < NUM_LEDS; i++)
    {
        if(center - i > 0)
            ledDistances[i] = center - i;
        else
            ledDistances[i] = i - center;
        ledDistances[i] -= numLeds;
        ledDistances[i] *= -1;
        ledDistances[i] -= numLeds - radius;
        ledDistances[i] *= 1 / radius;
    }
    odd_led_t *color2 = color->next;
    if(color2 == NULL)
    {
        //If an LED has a positive brightness, set it.
        for(int i = 0; i < NUM_LEDS; i++)
        {
            if(ledDistances[i] > 0)
            {
                setTempLED(i, 'r', color->R * ledDistances[i]);
                setTempLED(i, 'g', color->G * ledDistances[i]);
                setTempLED(i, 'b', color->B * ledDistances[i]);
            }
        }
    }
    else
    {
        //If an LED has a positive brightness, set it.
        for(int i = 0; i < NUM_LEDS; i++)
        {
            if(ledDistances[i] > 0)
            {
                double colorDistance = pow(ledDistances[i], 1);
                odd_led_t colorTemp;
                colorTemp.R = color->R * colorDistance + color2->R * (0.1 / colorDistance);
                colorTemp.G = color->G * colorDistance + color2->G * (0.1 / colorDistance);
                colorTemp.B = color->B * colorDistance + color2->B * (0.1 / colorDistance);

                double colorStrength = (16/3) * (pow(((1.0 - ledDistances[i]) * 2) + 2, -2) - (1.0 / 15.5));
                setTempLED(i, 'r', colorTemp.R * colorStrength);
                setTempLED(i, 'g', colorTemp.G * colorStrength);
                setTempLED(i, 'b', colorTemp.B * colorStrength);
            }
        }
    }
}
