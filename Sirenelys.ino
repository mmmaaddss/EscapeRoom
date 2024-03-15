#include <FastLED.h>

#define LED_PIN     2  // D4 on Wemos D1 Mini maps to GPIO2
#define NUM_LEDS    180
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Parameters for the siren effect
#define SIREN_SPEED 2 // Controls the speed of the siren effect
#define MIN_BRIGHTNESS 50 // Minimum brightness for the siren effect
#define MAX_BRIGHTNESS 100 // Maximum brightness for the siren effect #255 er maks

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  static uint8_t hue = 0;
  // Siren effect - a simple sine wave to modulate the brightness
  uint8_t brightness = map(sin8(hue), 0, 255, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  CRGB color = CHSV(25 /* orange hue */, 255 /* full saturation */, brightness /* brightness */);

  fill_solid(leds, NUM_LEDS, color);

  FastLED.show();
  hue += SIREN_SPEED;
  delay(33); // Adjust for smoother or faster animation
}
