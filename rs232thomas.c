/*
-- RS232-THOMAS :: INTERFACE :: IMPLEMENTIERUNG --
*/


/* INCLUDES */

#include "rs232thomas.h"

#include <stdio.h> // Standard-IO-Funktionen
#include <unistd.h> // UNIX-Standard-Funktionen
#include <fcntl.h> // Datei-Funktionen
#include <termios.h> // POSIX Terminal-Funktionen
#include <math.h> // Mathematische Funktionen


/* VARIABLEN */

// Beide Motoren stehen.
int lastSpeed[2] = {0, 0};


/* FUNKTIONEN */

int connect()
{
	// Anschlussvariable (hier auch Zeiger genannt) erstellen
	// RS232-Anschluss (ttyS0) laden
	int f = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
	
	// Fehler abfangen
	if(f == -1)
	{
		// TODO: Fehlerbehandlung
	}
	else
	{
		// Anschluss-Flags leeren
		fcntl(f, F_SETFL, 0);
	}
	
	// Anschluss-Optionen
	struct termios fOpt;
	
	// Aktuelle Anschluss-Optionen abrufen
	tcgetattr(f, &fOpt);
	
	// Baudrate setzen (9600); TODO: Zu Testzwecken auf 1200
	cfsetispeed(&fOpt, B9600); // Input
	cfsetospeed(&fOpt, B9600); // Output
	
	// TODO: Daten empfangen??
	/*
	// Flags setzen
	fOpt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	fOpt.c_cflag &= ~PARENB;
    fOpt.c_cflag &= ~CSTOPB;
    fOpt.c_cflag &= ~CSIZE;
    fOpt.c_cflag |= CS8;
	fOpt.c_cc[VMIN] = 1;
	fOpt.c_cc[VTIME] = 0;
	fOpt.c_cflag |= (CLOCAL | CREAD);
	*/
	
	// Neue Port-Optionen einsetzen
	tcsetattr(f, TCSANOW, &fOpt);
	
	// Anschlussvariable zur�ckgeben
	return f;
}

int disconnect(int f)
{
	// Verbindung schlie�en und Status zur�ckgeben
	return close(f);
}

int send(int f, char com, char params[])
{

	int len = strlen(params);
	
	if((com == 5 || com == 2) && params[1] == 0) {
		len++;
	}

	// Befehlsarray erstellen (3 Sonderbytes + Kommandobyte + Parameter)
	char *data;
	data = malloc((len + 4) * sizeof(char)); // malloc() erzeugt hier eine Warnung. Keine Ahnung, warum. Funktioniert aber.

	// Sicherheitshalber auf Fehler pr�fen
	if(data == NULL)
		return FALSE; // Nix gut
	
	// Die beiden Steuerbytes setzen
	data[0] = 35;
	data[1] = 35;
	
	// L�ngenbyte setzen
	data[2] = (len + 1);
	
	// Kommandobyte setzen
	data[3] = com;
	
	// Parameterbytes zuweisen
	int i;
	for(i = 0; i < len; i++)
	{
		data[i + 4] = params[i];
	}
	
	
	// Anzahl der gesendeten Daten merken
	int n = write(f, data, len + 4);
	//printf("%s\n", data);

	// Anzahl der gesendeten Daten pr�fen
	if(n < (len + 4))
	{
		// Mist
		printf("Fehler beim Senden eines Befehls!\n");
		return FALSE;
	}
	
	// Alles in Ordnung
	return TRUE;
}

int setMotorSpeed(int f, int motor, int speed)
{
	
	// Speed auf 255-Werte umrechnen
	speed = (int) (speed * 2.55);
	
	
	char param[2] = {};
	
	
	// Rechter Motor oder beide Motoren
	if(motor == MRIGHT)
	{
		
		// Neue Rotation? Rotation bestimmen
		if(lastSpeed[MRIGHT_ARR] <= 0 && speed > 0)
		{
			
			param[0] = MRIGHT;
			param[1] = FORWARDS;
			send(f, 5, param);
		}
		
		if(lastSpeed[MRIGHT_ARR] >= 0 && speed < 0)
		{
			
			param[0] = MRIGHT;
			param[1] = BACKWARDS;
			send(f, 5, param);
			
		}
		
		if(lastSpeed[MRIGHT_ARR] != speed) {
			// Geschwindigkeit senden (ohne Vorzeichen)
			param[0] = MRIGHT;
			param[1] = abs(speed);
			send(f, 2, param);
			
		}
		lastSpeed[MRIGHT_ARR] = speed;
		
	}
	
	// Linker Motor oder beide Motoren
	if(motor == MLEFT)
	{
	
		// Neue Rotation? Rotation bestimmen
		if(lastSpeed[MLEFT_ARR] <= 0 && speed > 0)
		{
			
			param[0] = MLEFT;
			param[1] = FORWARDS;
			send(f, 5, param);
		}
		
		if(lastSpeed[MLEFT_ARR] >= 0 && speed < 0)
		{
			
			param[0] = MLEFT;
			param[1] = BACKWARDS;
			send(f, 5, param);
		}
		
		if(lastSpeed[MLEFT_ARR] != speed) {
			// Geschwindigkeit senden (ohne Vorzeichen)
			param[0] = MLEFT;
			param[1] = abs(speed);
			send(f, 2, param);
			
		}
		lastSpeed[MLEFT_ARR] = speed;
		
	}
	// Alles toll
	return TRUE;
}
