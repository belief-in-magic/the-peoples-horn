include <dimensions.scad>
include <math.scad>
include <screws.scad>


// TODO: extract pcb shape as separate module, and use it as a negative in pcbFrame
module mainBoard(negative=false) {
    
    difference() {
        
        union() {
            pcb();
            batteryHolder();

            if (negative) cutouts();
        }

        for (hole = mainBoardHoles) {
            translate(v=[hole[0],hole[1],-inf/2])
            cylinder(r=m3Radius, h=inf);
        }
    }


    module pcb() {
        translate(v=[pcbR, pcbR, 0])
            color(c=pcbColor) // pcb green
            minkowski() {
            cube(size=[mainBoardX-pcbR*2, mainBoardY-pcbR*2, pcbThickness]);
            cylinder(h=eps, r=pcbR);
        }
    }

    module batteryHolder() {
        batteryY = 60.00;
        batteryX = mainBoardX;
        batteryZ = 16.5;

        batteryDy = 9.80;

        mirror(v=[0,0,1])
        translate(v=[0,batteryDy,0])
        cube(size=[batteryX, batteryY, batteryZ]);
    }

    module cutouts() {

        usbCutoutX = 10.00;
        usbCutoutZ = 3.50;

        usbCutoutDx = mainBoardX - 12.70; // from edge of PCB to midpoint of USB cutout
        usbCutoutDy = 0.00;

        sdCutoutX = 12.50;
        sdCutoutZ = 3.00;

        sdCutoutDx = mainBoardX - 26.70;  // from edge of PCB to midpoint of SD card cutout
        sdCutoutDy = 0.00;

        powerSwitchCutoutY = 16;
        powerSwitchCutoutZ = 10;
        powerSwitchCutoutDx = 25;
        powerSwitchCutoutDy = 75;

            

        color(c=[1,0,0]) {

            translate(v=[-usbCutoutX/2 + usbCutoutDx, -inf + usbCutoutDy, pcbThickness])
                cube(size=[usbCutoutX, inf, usbCutoutZ]);

            translate(v=[-sdCutoutX/2 + sdCutoutDx, -inf + sdCutoutDy, pcbThickness])
                cube(size=[sdCutoutX, inf, sdCutoutZ]);

            translate(v=[powerSwitchCutoutDx, powerSwitchCutoutDy, pcbThickness])
                cube(size=[inf, powerSwitchCutoutY, powerSwitchCutoutZ]);
            
        }
    }

}

mainBoard(negative=true);
