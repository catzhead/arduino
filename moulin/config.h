#ifndef __CONFIG__
#define __CONFIG__

/* Mettre ce define si c'est une nouvelle carte, executer et puis l'enlever */
//#define NEW_BOARD

/* Debug sur la liaison série */
#define DEBUG_ENABLED
#define DEBUG_BUTTON
//#define DEBUG_ULTRASONIC

/* Activer le GSM */
#define GSM_ENABLED

/* Activer le LCD */
#define LCD_ENABLED

/* Configuration pour le capteur ultrason */
#define TRIG_PIN 10
#define ECHO_PIN 6

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
#define DEBOUNCE_DELAY 100

/* Période d'échantillonage en ms pour le calcul de la vitesse */
#define PERIOD 120000

/* Durée minimum entre deux détections de bras de roue en ms */
#define DETECTION_MIN_DELAY 1000

/* Distances min/max de detection en mm */
#define DETECTION_MIN_LIMIT 0
#define DETECTION_MAX_LIMIT 500

/* Pins du module GSM */
#define GSM_PIN1 3
#define GSM_PIN2 4
#define GSM_PIN3 5

/* Temporisations pour le module GSM */
#define GSM_START_DELAY 15000
#define GSM_SEND_DELAY 10000
#define GSM_FINISH_DELAY 1000

/* Delai d'envoi du message de test en heures */
#define HEARTBEAT_MESSAGE_DELAY 24 /* 168h = 1 semaine */

/* Numéros de téléphone pour les SMS */
//#define TEL1 "0630291418"
#define NB_TEL_NUMBERS 2
#define TEL1 "0781428861"
#define TEL2 "0781428861"

#endif
