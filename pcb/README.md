# PCB

KiCAD (6.0) schematic and PCB files.

There are technically two KiCAD projects here. One for the "main" board and one for the
IO board. 

Supporting multiple PCBs in one KiCAD project is kinda janky right now. So it was decided 
to use different projects. This allows us to iterate separately on these boards. It also seems to be
easier for fab houses to process and manufacture, without dramatically increasing the price. 

We might have to eventually reconsider using two separate projects for scale and cost purposes, but for dev work this seems to work fine.


## Board TODO

- Power usage testing. Test low power mode for the rp2040. 
  - How long does it take to fully charge? 
  - How long does a charge last idle?
  - Does using the rp2040 lower power mode make sense?
- Separate 3.3V and 5V converters from battery. Currently we use a 3.3V LDO regulator on the boosted 5V source, this isn't the most
efficient. If we have a separate 3.3V SMPS from battery, we could also disable the 5V converter during lower power situations, allowing for
even more energy savings.
- Better charge handling. Maybe connect an rp2040 ADC pin to monitor it? and a regular GPIO pin to disable/enable charging? 
- Move charge LED to side of PCB, so we can it see from outside the case.
- Depending on power usage testing, we might need to have a manual on-off switch.
- BUG: current main board seems to not work directly on USB power. If the battery is not connected but the USB port is,
the board should power on.
- BUG (maybe): the charge IC gets pretty hot to the touch. Maybe reduce charge current? Increase trace widths?


## Preparing boards for manufacturing

TODO
