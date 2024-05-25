# Firmware source code

This builds the firmware binaries. It uses the arduino-pico core and some related libraries.

## Install:
Run this within the containing directory:
```
# Install arduino-cli in local directory
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=$PWD sh

# Install arduino-pico core 
./arduino-cli config add board_manager.additional_urls  https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
./arduino-cli core update-index
./arduino-cli core install rp2040:rp2040
```

### Compiling:

```
./arduino-cli compile --fqbn rp2040:rp2040:rpipico sketch
```

### Uploading:

```
./arduino-cli upload -p /dev/ttyACM0 --fqbn rp2040:rp2040:rpipico sketch
```

## Serial debugging:

You can start a serial connection with minicom:

```
minicom -D /dev/ttyACM0 -b 115200
```


## Language Server

TODO: I can't get this fucking thing to work

```
./arduino-language-server \
 -clangd /usr/bin/clangd \
 -cli ../arduino-cli \
 -cli-config $HOME/.arduino15/arduino-cli.yaml \
 -fqbn rp2040:rp2040:rpipico sketch
```
