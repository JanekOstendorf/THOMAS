/*
-- RS232-THOMAS :: INTERFACE :: DEFINITION --
Definiert die gekapselten THOMAS-RS232-Port-Befehle.
*/


/* HEADER-HAUPTMAKRO */
#ifndef RS232THOMAS
#define RS232THOMAS


/* KONSTANTEN */

// Hilfskonstante zur Darstellung des Rückgabewerts WAHR.
#define TRUE 1

// Hilfskonstante zur Darstellung des Rückgabewerts FALSCH.
#define FALSE 0

// Definiert die Stufen, die die Motorgeschwindigkeit in 100ms geändert wird.
#define SAFE_ACC_STEPS 10

// Definiert den in Fahrtrichtung rechten Motor.
// Die Array-Konstante dient der einfacheren Angabe eines Array-Elements des jeweiligen Motors.
#define MRIGHT 1
#define MRIGHT_ARR 0

// Definiert den in Fahrtrichtung linken Motor.
// Die Array-Konstante dient der einfacheren Angabe eines Array-Elements des jeweiligen Motors.
#define MLEFT 2
#define MLEFT_ARR 1

// Definiert beide Motoren.
#define MBOTH 3

// Drehrichtungen
#define FORWARDS 1
#define BACKWARDS 0


/* MAKROS */



/* TYPEN */



/* VARIABLEN */

// Speichert die jeweilig letzten eingestellten Motorgeschwindigkeiten.
// Inhalt:
// [0]: MRIGHT.
// [1]: MLEFT.
extern int lastSpeed[2];


/* FUNKTIONEN */

// Stellt eine Verbindung mit dem RS232-Port her und gibt den Verbindungszeiger zurück.
int connect();

// Beendet die Verbindung mit dem RS232-Port mithilfe des übergebenen Zeigers und gibt den Status zurück (0 = alles OK).
// Parameter:
// -> f: Der von der connect()-Funktion erstellte Zeiger.
int disconnect(int f);

// Sendet die übergebenen Bytes an den RS232-Port und gibt bei Erfolg TRUE zurück, bei Fehlern FALSE.
// Parameter:
// -> f: Der von der connect()-Funktion erstellte Zeiger.
// -> com: Das zu übergebende Befehlsbyte.
// -> params: Die zu übergebenden Parameter.
int send(int f, char com, char params[]);

// Sendet den Geschwindigkeitsänderungsbefehl für den angegebenen Motor und gibt bei Erfolg TRUE zurück, bei Fehlern FALSE.
// Der safeAcc-Parameter sorgt durch stufenweise Beschleunigung dafür, dass der Motor nicht plötzlich bei voller Fahrt
// seine Drehrichtung ändert oder zu stark bremst / beschleunigt und dabei beschädigt wird.
// -> f: Der von der connect()-Funktion erstellte Zeiger.
// -> motor: Der betroffene Motor (MRIGHT, MLEFT oder MBOTH).
// -> speed: Die neue Motorgeschwindigkeit (-100 bis 100).
int setMotorSpeed(int f, int motor, int speed);


/* HEADER-HAUPTMAKRO ENDE */
#endif
