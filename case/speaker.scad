include <math.scad>
include <screws.scad>
include <dimensions.scad>


speaker(negative=true);

module speaker(negative=false) {

    speakerScrewSpacingX = (speakerX - speakerScrewHoleDx)/2;
    speakerScrewSpacingY = (speakerY - speakerScrewHoleDy)/2;


    translate(v=[0,0,-speakerZ])
    union() {
        difference() {

            cube(size=[speakerX, speakerY, speakerZ]);
        
            mirror4XY(p=[speakerScrewSpacingX,speakerScrewSpacingY,0], dx=speakerScrewHoleDx, dy=speakerScrewHoleDy)
                cylinder(r=m3Radius, h=inf);
        }

        color([1,0,0])
            if (negative) {

                mirror4XY(p=[speakerScrewSpacingX,speakerScrewSpacingY,0], dx=speakerScrewHoleDx, dy=speakerScrewHoleDy)
                    cylinder(r=m3RadiusSlacked, h=inf);

                speakerHoleDx = 12.5; // 10-15?
                speakerHoleDy = 2;
                speakerHoleR = 13.5;
                speakerHoleRdiff = 12;

                translate(v=[speakerHoleDx+speakerHoleR, speakerHoleDy+speakerHoleR, 0])
                    hull() {
                    cylinder(r=speakerHoleR, h=inf);

                    translate(v=[speakerHoleRdiff, 0, 0])
                        cylinder(r=speakerHoleR, h=inf);
                }
        
            }
    }

}
