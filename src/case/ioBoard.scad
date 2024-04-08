include <dimensions.scad>
include <math.scad>
include <screws.scad>


module ioBoard(negative=false) {

    difference() {

        union() {
            color(c=[0.1,0.6,0.1]) // pcb green
                cube(size=[ioBoardX, ioBoardY, pcbThickness]);

            color(c=[1,0,0])
                if (negative) {

                    buttonCutoutDx = 6.5;
                    buttonCutoutX = 15.5;

                    translate(v=[buttonCutoutDx, 0, pcbThickness])
                        cube(size=[buttonCutoutX, ioBoardY, 50]);


                    mirror4XY(p=[pcbMountHoleDist,pcbMountHoleDist,0], dx=ioBoardScrewHoleDx, dy=ioBoardScrewHoleDy)
                        cylinder(r=m3RadiusSlacked, h=inf);
                }
        }

        mirror4XY(p=[pcbMountHoleDist,pcbMountHoleDist,0], dx=ioBoardScrewHoleDx, dy=ioBoardScrewHoleDy)
            cylinder(r=m3Radius, h=pcbThickness);

            
    }
}

ioBoard(negative=true);
