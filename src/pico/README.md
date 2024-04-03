# Raspberry Pi Pico source

This builds the binaries for a raspberry pi pico. It uses the arduino-pico core and some related libraries.

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
You might need to change the rpipicow (Raspberry PI Pico W) core to something else
```
./arduino-cli compile --fqbn rp2040:rp2040:rpipicow sketch
```

### Uploading:

Make sure that the board is discoverable. (Hold down BOOTSEL before plugging in Pico)
```
./arduino-cli upload -p /dev/ttyACM0 --fqbn rp2040:rp2040:rpipicow sketch
```

## Serial debugging:

You can start a serial connection with minicom:

```
minicom -D /dev/ttyACM0 -b 115200
```


## Language Server

```
./arduino-language-server \
 -clangd /usr/bin/clangd \
 -cli ../arduino-cli \
 -cli-config $HOME/.arduino15/arduino-cli.yaml \
 -fqbn rp2040:rp2040:rpipicow sketch
```
