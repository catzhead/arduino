#ifndef __CONFIG__
#define __CONFIG__

/* Debug sur la liaison série */
#define DEBUG_BUTTON

/* Configuration pour le capteur ultrason */
#define TRIG_PIN 9
#define ECHO_PIN 8

/* Configuration pour le LCD */
#define LCD_RS     12
#define LCD_ENABLE 11
#define LCD_D4     5
#define LCD_D5     4
#define LCD_D6     3
#define LCD_D7     2

/* Entrée pour les boutons */
#define BUTTONS_PIN A0

/* Debouncing delai en ms */
#define DEBOUNCE_DELAY 200 

/* Période d'échantillonage en ms pour le calcul de la vitesse */
#define PERIOD 60000

#endif
