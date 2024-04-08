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

                translate(v=[wallThickness,wallThickness*2,wallThickness])
                    cube(size=[bottomCaseX-wallThickness*2, bottomCaseY-wallThickness*2, wallThickness]);
            }
        }

        for (i = [0:8]) {
            translate(v=[13,i*8 + 30,0])
            hull(){
                cylinder(h=inf, r=2);

                translate(v=[50,0, -2])
                cylinder(h=inf, r=2);
                
            }
        }

    }
}

topLid();
