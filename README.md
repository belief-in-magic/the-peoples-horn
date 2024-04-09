# tarhorn

example ffmpeg command:

```
ffmpeg -i 0001.mp3 -f wav  -flags +bitexact -map_metadata -1 -ar 44100 -ac 1 -af "highpass=f=120,highpass=f=100,lowpass=f=19000,lowpass=f=20000" boom.wav
```



test addition
