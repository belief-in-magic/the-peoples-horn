// include <screws.scad>
include <math.scad>
include <dimensions.scad>
include <speaker.scad>

include <pcbFrame.scad>
use <mainBoard.scad>
include <ioBoard.scad>


// Distance between the inner faces of the main pcb and io pcb.
pcbFaceDist = 30;

bodyWallThickness = 2;

bodyWidth = 45;
assert(bodyWidth >= bodyWallThickness*2 + pcbX);

// special wall thickness for the wall with the usb and sd card
backWallThickness = 0.5; // TODO wtf?


ioBoardExtraSpace = 8;


bodyHeight = ioBoardSwitchSpace + 2 * pcbThickness + pcbFaceDist + batteryExtraSpace + ioBoardExtraSpace;

backWallToPcbDist = 2; // really just extra space
backToForwardPlaneDistance = pcbY + backWallThickness+backWallToPcbDist;

mainBoard_to_bodyShape = 
    rotate(a=[0,90,0]) *
    translate(v=[-pcbX,-1,-pcbThickness]);

ioBoard_to_bodyShape =
    translate(v=[pcbFaceDist+pcbThickness, 0, 0]) *
    rotate(a=[0,90,0]) *
    translate(v=[-pcbX,-1,-pcbThickness]);

speaker_to_bodyShape =
    translate(v=[pcbFaceDist/2, 130, pcbX/2]) *
    rotate(a=[-90,0,0]) *
    identity;

mainBoardFrame_to_bodyShape = mainBoard_to_bodyShape * inv4x4(mainBoard_to_pcbFrame);
ioBoardFrame_to_bodyShape = ioBoard_to_bodyShape *  inv4x4(ioBoard_to_pcbFrame);



module backConstructionPlane(shrink=0) {
    translate(v=[-(batteryExtraSpace+pcbThickness+bodyWallThickness)+shrink/2, -(backWallToPcbDist+backWallThickness), shrink/2 - 3.5])
        rotate(a=[90,0,0])
        cube(size=[bodyHeight-shrink, bodyWidth-shrink, eps]);
}

module forwardConstructionPlane(shrink=0) {
    translate(v=[-(batteryExtraSpace+pcbThickness+bodyWallThickness)+shrink/2, pcbY + 25, shrink/2-3.5])
        rotate(a=[90,0,0])
        cube(size=[bodyHeight-shrink, bodyWidth-shrink, eps]);
}
module speakerConstructionPlane() {
    cube(size = [1,1,1]);
}


intersection () {

    // shave off bits pointing out
    filledShape(shrink=20, r=10);

    difference() {
    
        union () {
            multmatrix(mainBoardFrame_to_bodyShape)
                guideRail(expansion=18);
            multmatrix(ioBoardFrame_to_bodyShape)
                guideRail(expansion=10);
        

            difference() {


                shell();
                    
                union() {
                    multmatrix(mainBoardFrame_to_bodyShape)
                        guideRail(negative=true, expansion=18);
                    multmatrix(ioBoardFrame_to_bodyShape)
                        guideRail(negative=true, expansion=10);

                    multmatrix(mainBoard_to_bodyShape)
                        mainBoard(negative=true);

                    multmatrix(ioBoard_to_bodyShape)
                        ioBoard(negative=true);

                    multmatrix(speaker_to_bodyShape)
                        speaker(negative=true);
                }
            }

        
        }
        multmatrix(mainBoard_to_bodyShape)
            mainBoard(negative=true);

        multmatrix(ioBoard_to_bodyShape)
            ioBoard(negative=true);
    }
}



module filledShape(shrink, r) {

    minkowski() {
        hull() {
            translate(v=[0,r,-bodyWallThickness])
                backConstructionPlane(shrink=shrink);
            translate(v=[0,0,-bodyWallThickness])
                forwardConstructionPlane(shrink=shrink);

        }
    
        sphere(r=r);
    }
}

module shell() {

    r = 10;

    // temp cutaway

    translate(v=[0,0,0])
        difference() {
        filledShape(shrink=2*r, r=r);

        union() {
            scale(v=[0.93,0.91,1])
                translate(v=[1,1,2])
                filledShape(shrink=2*r, r=r);

            translate(v=[-inf/2,-inf/2,20])
                cube(size=[inf, inf, inf]);
        }
    }

    

}


