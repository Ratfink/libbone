libbone
=======

A library for working with the BeagleBone in Bash

Goals
-----

To create a useful library capable of manipulating GPIOs, reading and changing
mux settings, and perhaps in the future using other interfaces such as UART,
SPI, and I2C. It may contain functionality for working with various
special-purpose hardware such as graphic displays and thermal printers
eventually.  The API will _not_ be copied from Arduino.  I don't really know
why people are so in love with that particular API, so plan on making libbone
use a new API of its own.  Arduino doesn't have all these crazy pin mux
settings (does it?  It can't have this many), so if I didn't make my own API it
would probably be like trying to fit a square peg into a round hole.
