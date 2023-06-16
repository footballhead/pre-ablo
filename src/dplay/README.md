# Custom dplay.dll 

We make our own "dplay.dll" so we can hijack our way into the process memory space.

## What's with fake ordinals?

`DIABLO.EXE` imports two functions from the real dplay.dll:

  * DirectPlayCreate (ordinal 1)
  * DirectPlayEnumerateA (ordinal 2)

The game will complain if these ordinals are missing. However, we don't actually implement the DirectPlay API. So we provide fake functions.
