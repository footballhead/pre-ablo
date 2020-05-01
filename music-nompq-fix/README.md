There is no music if MPQ isn't being used
===============================================================================

It's beneficial to extract the MPQ and make modifications locally without having to mess with the MPQ. However, this will cause the music to stop working.

Turns out there's an additional BOOL that needs to be changed in order for music to load from disk.

Unfortunately, it uses a new code path that crashes on exit.

Change .data:004BC17C (binary offset 0xBA97C) from 0 to 1.
