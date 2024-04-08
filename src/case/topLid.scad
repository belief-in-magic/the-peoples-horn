include <math.scad>
include <printConfig.scad>
include <dimensions.scad>

// TEMP design. It's just a lid, we just use the bottom case dimensions here.


radius = 2;

outerX = bottomCaseX + wallThickness*2;
outerY = bottomCaseY + wallThickness*2;


module topLid() {

    difference() {
        union() {
            translate(v=[radius,radius,0]) {
                minkowski() {
                    cube(size=[outerX-2*radius, outerY-2*radius, wallThickness]);

                    cylinder(r=radius, h=eps);
                }
            }

            difference() {
                translate(v=[wallThickness+xySlack/2,wallThickness+xySlack/2,wallThickness])
                    cube(size=[bottomCaseX-xySlack, bottomCaseY-xySlack, wallThickness]);

                translate(v=[wallThickness*2,wallThickness*2,wallThickness])
                    cube(size=[bottomCaseX-wallThickness*2, bottomCaseY-wallThickness*2, wallThickness]);
            }
        }


        // apply mounting holes
        mirror4XY(p=[0,0,0], dx=mainBoardScrewHoleDx, dy=mainBoardScrewHoleDy)
            cylinder(r=3, h=inf);
    }
}

topLid();
