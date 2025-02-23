# HERE LIES

This project is no longer actively developed. 

I've decided to open-source all the components (pcb, case design, and firmware) for anyone interested. There are a couple of bits that may be useful
for anyone trying to do something similar.

Please see [EPILOGUE.md](/EPILOGUE.md) for more details



# The People's Horn

One word: WHIMSY.

The People's Horn is a portable soundboard that can be operated with a single hand. 
The chorded input interface allows for 15 (2^4-1) sounds to be played. 


## DEMO

![demo video](/media/demo.mp4)

## Basic usage

- On the main board (the one with the RP2040, uSD slot, and USB-C connector), there should be a power jumper (POWER_EN)
This needs to be shorted for the board to power on.
- Slot in the microSD card with sound files stored inside. The format and naming of said sound files are explained below.
- Insert the speaker leads into the green terminal block. Note the polarity of the leads. ([PCB/v0.2 only] There aren't any signifiers for the polarity of the leads on the PCB. The one 
soldered to the square pad on the PCB is the positive one.)
- Connect the IO board with the main board using the JST connector.
- Compile and upload the firmware. This is described in the firmware directory README.

## Loading files

Sound files are stored in an external uSD card. Current limitations in the hardware require that the sound files to be in a certain format. Notably they need to be in `.wav` format, with a mono 44.1kHz sampling rate.


Here is a nice ffmpeg command to convert an example file `0001.mp3` into a compliant `wav` file:

```
ffmpeg -i 0001.mp3 -f wav  -flags +bitexact -map_metadata -1 -ar 44100 -ac 1 -af "highpass=f=120,highpass=f=100,lowpass=f=19000,lowpass=f=20000" 0001.wav
```

The names of the final sound files also need to follow a specific format: `[0|1][0|1][0|1][0|1].wav` (e.g. `1001.wav`, `0111.wav`). The number in the filename represents the buttons to
press in order to play it. The lowest order bit represents the button closest to the rotary encoder. The highest order bit is the one furthest away.


## Current Bugs:

- [PCB/v0.3]: Current main board seems to not work directly on USB power. If the battery is not connected but the USB port is,
the board should power on. This is not the case with v0.2. This might be caused by the main inductor backdriving (?). This is hopefully fixed with the new diode placement.
  - Update: this is still the case with v0.3 :( 


## TODO + Possible Improvements:

- [PCB]: Power usage testing. Test low power mode for the rp2040. 
  - How long does it take to fully charge? 
  - How long does a charge last idle?
  - Does using the rp2040 lower power mode make sense?
- [PCB]: Separate 3.3V and 5V converters from battery. Currently we use a 3.3V LDO regulator on the boosted 5V source, this isn't the most
efficient. If we have a separate 3.3V SMPS from battery, we could also disable the 5V converter during lower power situations, allowing for
even more energy savings.
