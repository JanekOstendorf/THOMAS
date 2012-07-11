/*
 * THOMAS
 */


/** INCLUDES **/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/ioctl.h>
#include <linux/joystick.h>
#include "rs232thomas.h"

/** DEFINES **/
#define JOY_DEV "/dev/input/js0"
#define NO_POWER_ZONE 10
#define DELTA_V 4

int left = 0, right = 0, end = 0;
int rs232 = 0;
int axis[4] = {};

/**
 * Thread zum Senden der PWM Werte und zur langsamen Beschleunigung
 */

void *send_pwm() {
    int last_left = 0, last_right = 0, new_left = 0, new_right = 0;

    while(!end) {
        
        if(left < (last_left - DELTA_V))
        new_left -= DELTA_V;
        else if(left >= (last_left - DELTA_V) && left <= (last_left + DELTA_V))
        new_left = left;
        else
        new_left += DELTA_V;

        if(right < (last_right - DELTA_V))
        new_right -= DELTA_V;
        else if(right >= (last_right - DELTA_V) && right <= (last_right + DELTA_V))
        new_right = right;
        else
        new_right += DELTA_V;

        last_left = new_left;
        last_right = new_right;

        // Motorbefehle senden
        // Einzeln ansteuern
        // TODO: Abbremszeitr�ume bestimmen und probieren, wie beim einzelnen Ansteuern vorgegangen werden soll
        // TODO: Beeinflusst die Anpassungszeit der R�der die sichere Fahrt, d.h. THOMAS f�hrt nicht sofort in die gew�nschte Richtung?
        // TODO: Ggf. Beschleunigungsregulierungsschritte (SAFE_ACC_STEPS) in rs232_thomas.h �ndern
        setMotorSpeed(rs232, MLEFT, new_left);
        setMotorSpeed(rs232, MRIGHT, new_right);

        usleep(20000);
    }
    
    return;
     
}

/**
 * Hauptfunktion
 * @return 
 */
int main(void) {
    
	int power_x = 0, power_z = 0, power_r = 0; // X = axis[0] ; Y = axis[1] ; R = axis[3]

    	double tmp = 0;

	
	// Mit RS232-Port verbinden
	//rs232 = connect();
	
	//pthread_t t_send_pwm;
        //pthread_create(&t_send_pwm, NULL, send_pwm, NULL);
    
        
        socket_server_start();
        
        for(int i = 0; i < 4; i++) {
            
            printf("%d\n", axis[i]);
            
        }
        
        fflush(stdout);
        
        return 0;
	
	/*while(!end)
	{

        power_x = (int)(axis[0]/327);

        if(power_x < NO_POWER_ZONE && power_x > NO_POWER_ZONE*(-1))
            power_x = 0;

        power_z = (int)(axis[1]/(-327));

        if(power_z < NO_POWER_ZONE && power_z > NO_POWER_ZONE*(-1))
            power_z = 0;

        power_r = (int)(axis[2]/327);

        if(power_r < NO_POWER_ZONE && power_r > NO_POWER_ZONE*(-1))
            power_r = 0;

        tmp = (-1) * (power_z < 0);
        if(tmp == 0)
            tmp = 1;
        tmp = ((((float) power_r * tmp) + (float) power_x) / 2);
        power_x = (int) tmp;

        if(power_x <= 0)
            right = power_z;
        else if(power_z <= 0) {
            right = power_z + abs(power_x);
            if(right > 0)
                right = 0;
        }
        else if(power_z > 0) {
            right = ((100 - power_z + abs(power_x)) * (-1)) + 100;
            if(right < 0)
                right = 0;
        }

        if(power_x >= 0)
            left = power_z;
        else if(power_z <= 0) {
            left = power_z + abs(power_x);
            if(left > 0)
                left = 0;
        }
        else if(power_z > 0) {
            left = ((100 - power_z + abs(power_x)) * (-1)) + 100;
            if(left < 0)
                left = 0;
        }

        if(!(power_x > 50 || power_z) && power_r) {
            left = (power_r/2);
            right = (power_r/2) * (-1);
        }

		// print the results 
		//printf("Links: %d   Rechts: %d\n", left, right);
		
		usleep(2000);
		//printf("\r");
		fflush(stdout);
        
	} // while
    
	end = 1;
    printf("\n\nClosing threads ...\n");
    usleep(1000000);
	setMotorSpeed(rs232, MLEFT, 0);
	setMotorSpeed(rs232, MRIGHT, 0);
    
	// Verbindung zum RS232-Port trennen
	disconnect(rs232);
    */
} // main

