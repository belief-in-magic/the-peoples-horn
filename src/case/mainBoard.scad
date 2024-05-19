include <dimensions.scad>
include <math.scad>
include <screws.scad>

module mainBoard(negative=false) {

    difference() {

        union() {
            color(c=[0.1,0.6,0.1]) // pcb green
                cube(size=[mainBoardX, mainBoardY, pcbThickness]);

            color(c=[1,0,0])
                if (negative) {

                    usbCutoutDx = 16;
                    usbCutoutX = 16;
                    usbCutoutZ = 10;

                    sdCutoutDx = 38;
                    sdCutoutX = 15;
                    sdCutoutZ = 10;

                    translate(v=[usbCutoutDx, mainBoardY, pcbThickness])
                        cube(size=[usbCutoutX, 25, usbCutoutZ]);

                    translate(v=[sdCutoutDx, mainBoardY, pcbThickness])
                        cube(size=[sdCutoutX, 25, sdCutoutZ]);
                
                }
        }

        mirror4XY(p=[pcbMountHoleDist,pcbMountHoleDist,0], dx=mainBoardScrewHoleDx, dy=mainBoardScrewHoleDy)
            cylinder(r=m3Radius, h=inf);

    }
    
}


mainBoard(negative=true);
