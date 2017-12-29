#ifndef __CONFIG__
#define __CONFIG__

/* Debug sur la liaison série */
#define DEBUG_ENABLED

/* Activer le GSM */
//#define GSM_ENABLED

/* Activer le LCD */
#define LCD_ENABLED

/* Si le GSM est activé, on ne peut plus utiliser la liaison série pour autre chose */
#ifndef GSM_ENABLED
  #ifdef DEBUG_ENABLED
    #define DEBUG_BUTTON
  #endif
#endif

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
#define DEBOUNCE_DELAY 100

/* Période d'échantillonage en ms pour le calcul de la vitesse */
#define PERIOD 60000

/* Durée minimum entre deux détections de bras de roue en ms */
#define DETECTION_MIN_DELAY 1000

/* Distances min/max de detection en cm */
#define DETECTION_MIN_LIMIT 0
#define DETECTION_MAX_LIMIT 50

/* Pins du module GSM */
#define GSM_PIN1 3
#define GSM_PIN2 4
#define GSM_PIN3 5

/* Numéros de téléphone pour les SMS */
#define TEL1 "0630291418"

#endif
