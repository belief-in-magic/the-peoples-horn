// include <screws.scad>
include <math.scad>
include <dimensions.scad>

use <pcbFrame.scad>
use <mainBoard.scad>
use <ioBoard.scad>

// Distance between the inner faces of the main pcb and io pcb.
pcbFaceDist = 30;

bodyWallThickness = 2;
bodyWidth = bodyWallThickness*2 + pcbX;

// special wall thickness for the wall with the usb and sd card
backWallThickness = 1;

// Space from the top of the pcb to the lower mounting ridge in the switches. Hand-measured
ioBoardToSwitchSpace = 5; // TODO: figure out how to fasten the switches in a nice way

ioBoardExtraSpace = 8;


bodyHeight = ioBoardToSwitchSpace + 2 * pcbThickness + pcbFaceDist + batteryExtraSpace + ioBoardExtraSpace;

backWallToPcbDist = 2; // really just extra space
backToForwardPlaneDistance = pcbY + backWallThickness+backWallToPcbDist;

mainBoard_to_bodyShape = 
    rotate(a=[0,90,0]) *
    translate(v=[-pcbX,0,-pcbThickness]);

ioBoard_to_bodyShape =
    translate(v=[pcbFaceDist+pcbThickness, 0, 0]) *
    rotate(a=[0,90,0]) *
    translate(v=[-pcbX,0,-pcbThickness]);

multmatrix(mainBoard_to_bodyShape)
mainBoard(negative=false);

multmatrix(ioBoard_to_bodyShape)
ioBoard(negative=false);



module backConstructionPlane(shrink=0) {
    translate(v=[-(batteryExtraSpace+pcbThickness+bodyWallThickness)+shrink/2, -(backWallToPcbDist+backWallThickness), shrink/2])
        rotate(a=[90,0,0])
        cube(size=[bodyHeight-shrink, bodyWidth-shrink, eps]);
}

module forwardConstructionPlane(shrink=0) {
    translate(v=[-(batteryExtraSpace+pcbThickness+bodyWallThickness)+shrink/2, pcbY, shrink/2])
        rotate(a=[90,0,0])
        cube(size=[bodyHeight-shrink, bodyWidth-shrink, eps]);
}
module speakerConstructionPlane() {
    cube(size = [1,1,1]);
}

module shell() {

    r = 5;


    module filledShape() {

        minkowski() {
            hull() {
                translate(v=[0,r,0])
                    backConstructionPlane(shrink=2*r);
                translate(v=[0,-r,0])
                    forwardConstructionPlane(shrink=2*r);

            }
    
            sphere(r=r);
        }
    }

    // temp cutaway

    difference() {
        filledShape();

        filledShape();
    }

    

}




