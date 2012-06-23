libbone API
===========

How pins are represented
------------------------

This is the first decision I will have to make.  An easy method would be to
define two constants P8 = 0 and P9 = 46, then just add to them for, say, P8 pin
3 = P8 + 3 = 3, or P9 pin 11 = P9 + 11 = 57.  This might be a fine method for
some things, but it could very well complicate some other tasks such as polling
inputs.  Perhaps that system would be good for initializing a pin variable,
then using more advanced techniques after that.

There may very well have to be several internal representations of pins,
depending on the requirements of the particular libbone subsystem they're
needed for.  That would be ugly, however, and I'll try to get everything as
close to a single structure as I can.  For the mux subsystem, the pins will
need to be searchable by their mode 0 names, and for the gpio subsystem, by
their gpio numbers.  It would probably make most sense to keep structures for
different subsystems separate, contrary to what I said above, now that I think
about it more.

All subsystems must have something in common, though, and that would be the
simple Pn+pin representation mentioned above.  Using 46 for the value of P9
might not be the best idea, though, as that would put the value of P9 on a real
pin on P8.  Conflict!  P9 should equal something like 50 then?  That would work
just as well, and error checking can make sure that no numbers between 46 and
50 get used as pins.

Okay, so the Pn+pin standard is the following:
    P8 = 0
    P9 = 50
This leaves enough room to remove collisions, puts P9 on a round number, and
doesn't put too many unused indices between Port 8 and Port 9.

The mux subsystem will have an array of strings, each of which is the mode 0
name of the pin.  The array will use Pn+pin for its indexing.  I don't believe
anything else will need to be stored for the gpio subsystem, as the program
author can figure out what modes do what on his own.  In fact, I've noticed
that several functions can be brought out to multiple pins by changing mux
settings, such as spi chip selects.  I don't know what effect this would have,
and I don't feel like finding out the hard way and having to spend $90 on a new
BeagleBone, so perhaps I should read up on this more some time.

The gpio subsystem will need to figure out a gpio number from Pn+pin notation,
which seems to have no simple way to be determined algorithmically, and so will
probably need to be hard-coded.  It does seem to be that mode 7, if available,
is always GPIO.  This will make determining what mux setting to make a pin a
GPIO much simpler.  I suppose it could simply be an array of ints, and for
example, gpio[P8+3] = 38.  Basically the value that needs to be written to the
gpio exports file.  And there'll need to be more structures for the gpio
subsystem, but those don't have much to do with this directly.

Other subsystems which are less pin-based might have less to do with this
Pn+pin notation than the two mentioned above.  For example, the SPI subsystem
will probably have very little to do with Pn+pin, as there are only two SPI
controllers on the BeagleBone and each has only two cs pins (though as
mentioned above they can be brought out to different pins by using different
mux settings; this needs investigation eventually).  The uart subsystem would
be able to use a similar mechanism, as would the i2c subsystem.  And the led
subsystem would be a completely different animal; attachable hardware like
802.11 adapters can create new controllable leds which would need new nodes and
constant scanning to find when they start and stop existing.  More on this to
come in the future.
