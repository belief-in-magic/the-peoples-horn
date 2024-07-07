include <math.scad>
include <screws.scad>
include <dimensions.scad>


/*

  Dimensions and negative for a simple cylindrical speaker. This speaker should have a front "ridge"
  that can be used to hold the speaker in place. 

*/

speakerRidgeR = 20;

*speaker(negative=true);

module speaker(negative=false) {
    radiuses = [speakerRidgeR, speakerRidgeR, 18, 32.3/2, 21.9/2, 21.9/2];
    dists = [0, 3.4, 3.4, 13, 0, 19.9];

    for (i = [1:5]) {

        hull() {

            cylinder(r=radiuses[i-1], h=eps);

            translate(v=[0,0, -(dists[i])])
                cylinder(r=radiuses[i], h=eps);
        }
    }

    color(c = [1,0,0])        
        if (negative) {
            translate(v=[0,0, -20])
                cylinder(r=radiuses[2], h=20);

            mirror(v=[0,0,1])
            translate(v=[0,0, -20])
                cylinder(r=radiuses[2], h=20);
        }
        
}
