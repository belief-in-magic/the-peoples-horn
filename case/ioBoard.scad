include <dimensions.scad>
include <math.scad>
include <screws.scad>


ioBoardSwitchSpace = 5;

module ioBoard(negative=false) {

    difference() {

        union() {
            pcb();

            if (negative) cutouts();
        }

        for (hole = ioBoardHoles) {
            translate(v=[hole[0],hole[1],-inf/2])
            cylinder(r=m3Radius, h=inf);
        }

    }

    module pcb() {
        translate(v=[pcbR, pcbR, 0])
            color(c=pcbColor) // pcb green
            minkowski() {
            cube(size=[ioBoardX-pcbR*2, ioBoardY-pcbR*2, pcbThickness]);
            cylinder(h=eps, r=pcbR);
        }
    }

    module cutouts() {

        knobCutoutX = 21.00;
        knobCutoutZ = 12.00;

        knobCutoutDx = (ioBoardX-knobCutoutX)/2.00;
        knobCutoutDz = -15.00;

        keySwitchCutoutX = 25.00;
        keySwitchCutoutY = 78.50;

        keySwitchCutoutDx = 5;
        keySwitchCutoutDy = 14;


        color(c=[1,0,0]) {
            // knob
            translate(v=[knobCutoutDx, -inf, knobCutoutDz])
            cube(size=[knobCutoutX, inf, knobCutoutZ]);

            // key switches
            translate(v=[keySwitchCutoutDx, keySwitchCutoutDy, pcbThickness])
            cube(size=[keySwitchCutoutX, keySwitchCutoutY, 20]);
    
            translate(v=[keySwitchCutoutDx, keySwitchCutoutDy, pcbThickness + ioBoardSwitchSpace])
            cube(size=[keySwitchCutoutX, keySwitchCutoutY+8, 20]);
            
            
        }
    }
}

*ioBoard(negative=true);
