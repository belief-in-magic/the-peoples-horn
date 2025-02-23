# Epilogue

*This project is no longer actively developed!!!*

I've decided to open-source all the components (PCB, case design, and firmware) for anyone interested. There are a couple of bits that may be useful
for anyone trying to do something similar.

Firmware:
- How to read wav files from an sd interface and play multiple at once
- Control logic for a chorded input type-thing
- Using software fifo-queues for orchestrating the two RP2040 cores 

PCB:
- Reading from an SD card (using SPI) using an RP2040 chip 
- NiMh battery charging circuitry (honestly this is probably kinda buggy)
- Probably more...

## Writeup

I started this project back in November 2023 (roughly 1.5 years ago) and worked on it (on and off) for almost a year.
Development stopped when my personal priorities shifted and I started looking for a new job. The new job I landed
requires more of my energy and I don't see myself working on this project in the future. 

It wasn't for nothing though! I learnt a lot. I even took some prototypes out with me during hangouts with friends. This thing really is a tomfoolery device. 

I would say this project is probably around 90% complete; the core functionality is there, and what's left is mainly ironing out some hardware bugs
and polishing. (I say 90% done, but the last 10% is probably another 90% of work!) It's unfortunate that I'm not neatly tying everything up here, but I
feel like I've personally squeezed out everything I want from this project. 

This project was definitely a learning experience:
- Designing a custom PCB and shipping its various iterations for manufacturing was really fun. I've acquired a new appreciation for how fast
iteration cycles are for software. It was also really expensive! Canadian tariffs on PCBs really add up!
- Writing software for hardware that you've personally designed is also fun. I've never had to think about whether a feature should be represented in hardware or
software. I've also never really debugged issues where I was unsure if the problem came from the hardware or software! These were all very novel 
experiences for me.
- Designing the case presented me with some unfamiliar design challenges. The intent was to be able to use the horn with just one hand. I had to consider 
seemingly obvious design-y things: Can this be comfortably held? Is the mute button easily accesible? Is the case handle the occasional drop? 
I had to do this all while factoring in the physical constraints brought on by the PCBs. 

I had also originally intended to sell a couple of these. (I think the personal target I set for myself was 100?) Looking back, this was a pretty misguided
endeavor and I'm glad I didn't really continue there. The unit economics just didn't add up. I'd have to sell an individual unit for like $100, which is
way too much for a toy like this. There's also incorporating into a company, setting up manufacturing contracts, QA and insurance, marketing, setting up a website,
handling storage, handling returns, etc etc. Maybe for a different product in the future, but not this one.


## Technical stuff

All the components of this project (the case, PCB, software) went through at least one redesign, each. (A pattern I've noticed in nearly all my personal projects.)

### Firmware: 

The V1 firmware used shared memory and locks to communicate between the two RP2040 cores. One would look for user input chords, and the other would read from the sd card interface
and drive the speakers. Debugging this setup was a *complete* nightmare. This was at least partly due to my inexperience with lower level multiprocessing. The dance between
reading sd contents to memory buffers, getting "interrupted" by the IO core, and driving the audio chip became really clumsy, really fast. That was also exhasperbated by fact that multiple
sounds could be played at the same time (4 sounds to be exact, so 4 different sound buffers). The max number of sounds was mainly constrained
by how fast the RP2040 could read from the SD card (in SPI mode, not SDIO, which probably has higher throughput but is also harder to implement). If the max number of sounds was playing and
the user triggered another one, the least recently used sound would be evicted. The LRU eviction strategy seemed like the most intuitive for users.

At some point I hit a wall with V1 and decided to try something different.
V2 of the firmware replaced the shared memory and locks with some thread-safe FIFO queues. Two to be exact, one for core 1 to send messages to core 2, and another for the opposite direction. 
The cores would send messages to each other like "TRIGGER SOUND X", "STOP BUFFER Y", "READ SD SECTOR 2 FOR BUFFER Z" as their main coordination mechanism. This incurs a theoretical performance 
hit, but makes it easy to linearize what's going on in an individual buffer. This makes debugging a lot more pleasant.

Multicore processing is just hard. Even in the two core scenario, I struggled to wrap my head around all the possible states the system could be in. 
Always look into ways (e.g linearizing your state) to make reasoning simpler.


### PCB:

I felt most out of my element here. The PCBs went through 2 main designs, the latter mainly to shrink the footprints and reorient how they're placed within the case:


PCB images, left is V1, right is V2:

Fronts:
![fronts](/media/pcbfront.jpeg)

Backs:
![backs](/media/pcbback.jpeg)

Most of the circuitry was hacked together by looking at online tutorials. Oftentimes, if I was choosing between two components for something (like an amp IC), I would end
up choosing the one with the Adadruit or Sparkfun breakout board and tutorial. Whether the PCB assembly manufacturer had it in stock would also sometimes be a deciding factor. 

I also had to dive into the occasional datasheet for the various ICs I chose. I still don't think my
power+charging circuitry is without bugs. I chose to use NiMH batteries for this project because it seemed to me to be the most accessible rechargeble batteries that wouldn't set the house on fire
if something was wrong. Turns out NiMH charging ICs are a bit more rare (lithium ion charging ICs are way more common), and the only resource online I had to go off of
for the chip I chose was the online datasheet. Maybe if I was a more skilled hardware designer I would have gotten it right on the first iteration, but the charging logic turned out to be surprisingly
difficult. (For example, making sure the device still runs if the battery is removed, but the device is plugged in.)

Getting the PCB footprint right was also really difficult. I tried to use as many SMD components as possible, and have them all placed on one side (for both pcbs). This was so I could use the PCBA service provided from
JLC. I HATE soldering.

Sending my designed parts to a PCB manufacturer was a fun experience. I used JLCPCB for my design (I also used their PCBA service for SMD parts) and I have no complaints. It's honestly suprising 
how fast they can manufacture a design and ship it out. I think it would normally take 2 weeks, from sending the design, to the parts reaching me in Toronto. Very nice service. 


### Builds

##### v1 (very boxy):
![V1 test build (missing speaker)](/media/v1.jpeg)


##### v2 (rounder, more compact):
![V2 test build](/media/v2.jpeg)

##### side by side (note how v2 is much smaller and streamlined):
![side by side](/media/sidebyside.jpeg)


## FIN

