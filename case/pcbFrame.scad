include <screws.scad>
include <math.scad>
include <dimensions.scad>

/*
  Frame to hold an individual PCB. It's assumed here that both the IO board and main board
  have the same dimensions and mounting points.
*/
module guideRail() {

    guideThickness = 8.0;
    guideWidth = 7.0;
    guideTopHeight = 3.0;
    guideSideWallThickness = 3.0;

    frontGuideThickness = 3.0;

    module guide(heightToScrew = 3.5) {

        difference() {
            union() {

                // holds nut
                translate(v=[0,-guideThickness/2,0])
                    cube(size=[guideWidth, guideThickness, guideTopHeight + heightToScrew]);

                // front wall
                translate(v=[-pcbThickness - frontGuideThickness, -guideThickness/2, 0])
                    cube(size=[frontGuideThickness, guideThickness, guideTopHeight + heightToScrew]);

                // side wall
                translate(v=[-pcbThickness-guideSideWallThickness, -guideThickness/2 - guideSideWallThickness,0])
                cube(size=[pcbThickness + guideSideWallThickness + guideWidth, guideSideWallThickness , guideTopHeight+heightToScrew]);
            }

            translate(v=[guideWidth/2, 0, heightToScrew])
            rotate(a=[0,-90,0])
                hexNutPocket_N("m3");
        }
    }


    translate(v=[0, pcbHoles[1][1], 0])
        mirror(v=[0,1,0])
        guide(heightToScrew = 17.0);

    translate(v=[0, pcbHoles[0][1], 0])
        guide(heightToScrew = 3.5);
    
}


guideRail();
 


// Transformation to slot in mainBoard/ioBoard pcb into the frame
use <mainBoard.scad>

mainBoard_to_pcbFrame =
    translate(v=[-pcbThickness,0, pcbX]) *
    rotate(a=[0,90,0]);

ioBoard_to_pcbFrame =
    translate(v=[-pcbThickness,0, pcbX]) *
    rotate(a=[0,90,0]);

multmatrix(mainBoard_to_pcbFrame)
*mainBoard(negative=true);
