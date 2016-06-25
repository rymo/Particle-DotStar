/*------------------------------------------------------------------------
  Particle library to control Adafruit DotStar addressable RGB LEDs.

  Ported by Technobly for Particle Core, Photon, P1 and Electron.

  ------------------------------------------------------------------------
  -- original header follows ---------------------------------------------
  ------------------------------------------------------------------------

  This file is part of the Adafruit Dot Star library.

  Adafruit Dot Star is free software: you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  Adafruit Dot Star is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see <http://www.gnu.org/licenses/>.
  ------------------------------------------------------------------------*/

#ifndef _ADAFRUIT_DOT_STAR_H_
#define _ADAFRUIT_DOT_STAR_H_

#include "application.h"

// Color-order flag for LED pixels (optional extra parameter to constructor):
// Bits 0,1 = R index (0-2), bits 2,3 = G index, bits 4,5 = B index
#define DOTSTAR_RGB (0 | (1 << 2) | (2 << 4))
#define DOTSTAR_RBG (0 | (2 << 2) | (1 << 4))
#define DOTSTAR_GRB (1 | (0 << 2) | (2 << 4))
#define DOTSTAR_GBR (2 | (0 << 2) | (1 << 4))
#define DOTSTAR_BRG (1 | (2 << 2) | (0 << 4))
#define DOTSTAR_BGR (2 | (1 << 2) | (0 << 4))

class Adafruit_DotStar {

 public:

    Adafruit_DotStar(uint16_t n, uint8_t o=DOTSTAR_GBR);
    Adafruit_DotStar(uint16_t n, uint8_t d, uint8_t c, uint8_t o=DOTSTAR_GBR);
   ~Adafruit_DotStar(void);                 // Destructor
  void
    begin(void),                            // Prime pins/SPI for output
    clear(),                                // Set all pixel data to zero
    setBrightness(uint8_t),                 // Set global brightness 0-255
    setPixelColor(uint16_t n, uint32_t c),
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
    show(void),                             // Issue color data to strip
    updatePins(void),                       // Change pin assignments (HW)
    updatePins(uint8_t d, uint8_t c),       // Change pin assignments (SW)
    updateLength(uint16_t n);               // Change length
  uint32_t
    Color(uint8_t r, uint8_t g, uint8_t b), // R,G,B to 32-bit color
    getPixelColor(uint16_t n) const;        // Return 32-bit pixel color
  uint16_t
    numPixels(void);                        // Return number of pixels
  uint8_t
    getBrightness(void) const,              // Return global brightness
   *getPixels(void) const;                  // Return pixel data pointer

 private:

  uint16_t
    numLEDs;                                // Number of pixels
  uint8_t
    dataPin,                                // If soft SPI, data pin #
    clockPin,                               // If soft SPI, clock pin #
    brightness,                             // Global brightness setting
   *pixels,                                 // LED RGB values (3 bytes ea.)
    rOffset,                                // Index of red in 3-byte pixel
    gOffset,                                // Index of green byte
    bOffset;                                // Index of blue byte
  void
    hw_spi_init(void),                      // Start hardware SPI
    hw_spi_end(void),                       // Stop hardware SPI
    sw_spi_init(void),                      // Start bitbang SPI
    sw_spi_out(uint8_t n),                  // Bitbang SPI write
    sw_spi_end(void);                       // Stop bitbang SPI

};

inline void Adafruit_DotStar::clear() { // Write 0s (off) to full pixel buffer
  memset(pixels, 0, numLEDs * 3);
}

// Set global strip brightness.  This does not have an immediate effect;
// must be followed by a call to show().  Not a fan of this...for various
// reasons I think it's better handled in one's sketch, but it's here for
// parity with the NeoPixel library.  Good news is that brightness setting
// in this library is 'non destructive' -- it's applied as color data is
// being issued to the strip, not during setPixel(), and also means that
// getPixelColor() returns the exact value originally stored.
inline void Adafruit_DotStar::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.  This
  // optimizes the actual scaling math later, allowing a fast 8x8-bit
  // multiply and taking the MSB.  'brightness' is a uint8_t, adding 1
  // here may (intentionally) roll over...so 0 = max brightness (color
  // values are interpreted literally; no scaling), 1 = min brightness
  // (off), 255 = just below max brightness.
  brightness = b + 1;
}

inline uint8_t Adafruit_DotStar::getBrightness(void) const {
  return brightness - 1; // Reverse above operation
}

// Return pointer to the library's pixel data buffer.  Use carefully,
// much opportunity for mayhem.  It's mostly for code that needs fast
// transfers, e.g. SD card to LEDs.  Color data is in BGR order.
inline uint8_t *Adafruit_DotStar::getPixels(void) const {
  return pixels;
}

#endif // _ADAFRUIT_DOT_STAR_H_
