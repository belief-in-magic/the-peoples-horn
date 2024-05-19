# The People's Horn

A whimsical device, for whimsical people. Make sounds with buttons. Pollute your environment with noise.



## Loading files

Sound files are stored in an external uSD card. Current limitations in the hardware require that the sound files to be in a certain format. Notably they need to be in `.wav` format, with a mono 44.1kHz sampling rate.


Here is a nice ffmpeg command to convert an example file `0001.mp3` into a compliant `wav` file:

```
ffmpeg -i 0001.mp3 -f wav  -flags +bitexact -map_metadata -1 -ar 44100 -ac 1 -af "highpass=f=120,highpass=f=100,lowpass=f=19000,lowpass=f=20000" 0001.wav
```

The names of the final sound files also need to follow a specific format: `[0|1][0|1][0|1][0|1].wav` (e.g. `1001.wav`, `0111.wav`). The number in the filename represents the buttons to
press in order to play it. The lowest order bit represents the button closest to the rotary encoder. The highest order bit is the one furthest away.


