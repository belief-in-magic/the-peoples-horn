include <math.scad>
include <dimensions.scad>

use <mainBoard.scad>
use <ioBoard.scad>
use <speaker.scad>

mainBoardAssemblyTransform = identity;

ioBoardAssemblyTransform =
    translate(v=[-ioBoardBottomClearance,0,pcbThickness])
    * rotatey(-90);

speakerAssemblyTransform =
    translate(v=[-(pcbSpacing+ioBoardBottomClearance)+2.5,-(speakerZ+speakerBackClearance+pcbSpacing),-2]) 
    * rotate(a=[90,0,0]);

bottomCaseAssemblyTransform =
    translate(v=[-(ioBoardBottomClearance+pcbThickness+wallThickness),
                 -(wallThickness + pcbSpacing + speakerZ + speakerBackClearance),
                 -(wallThickness+mainBoardBottomClearance)
                ]);


multmatrix(mainBoardAssemblyTransform)
*mainBoard(negative=true);

multmatrix(ioBoardAssemblyTransform)
*ioBoard(negative=true);

multmatrix(speakerAssemblyTransform)
*speaker(negative=true);

multmatrix(bottomCaseAssemblyTransform)
bottomCase();


module bottomCase() {


    module fullShell() {

        outerX = bottomCaseX + wallThickness*2;
        outerY = bottomCaseY + wallThickness*2;
        outerZ = bottomCaseZ + wallThickness;

        difference() {

            minkowski() {
                radius = 2;
                translate(v=[radius, radius, 0])
                    cube(size=[outerX-2*radius, outerY-2*radius, outerZ]);
                cylinder(r=radius, h=eps);
            }

            union() {

                translate(v=[wallThickness, wallThickness, wallThickness])
                    cube(size=[bottomCaseX, bottomCaseY, bottomCaseZ+2*eps]);

                multmatrix(inv4x4(bottomCaseAssemblyTransform)*ioBoardAssemblyTransform)
                    ioBoard(negative=true);

                multmatrix(inv4x4(bottomCaseAssemblyTransform)*mainBoardAssemblyTransform)
                    mainBoard(negative=true);


                multmatrix(inv4x4(bottomCaseAssemblyTransform)*speakerAssemblyTransform)
                    speaker(negative=true);

                
                    
            }
        }
        
    }


    module mainBoardMount() {
        difference() {
            cylinder(r=5, h=mainBoardBottomClearance);

            translate(v=[0,0,mainBoardBottomClearance-3])
                hexNutPocket_N("m3", bridgeFront=true);
        }
        
    }


    
    fullShell();


    translate(v=[0,0,-(pcbThickness+mainBoardBottomClearance)])
        multmatrix(inv4x4(bottomCaseAssemblyTransform))
        multmatrix(translate(v=[pcbMountHoleDist, pcbMountHoleDist,0])) // TODO adhere to proper space transformations
        rotate(a=[0,0,90])
        mainBoardMount();


    translate(v=[0,0,-(pcbThickness+mainBoardBottomClearance)])
        multmatrix(inv4x4(bottomCaseAssemblyTransform))
        multmatrix(translate(v=[pcbMountHoleDist+mainBoardScrewHoleDx, pcbMountHoleDist,0])) // TODO adhere to proper space transformations
        rotate(a=[0,0,90])
        mainBoardMount();

    translate(v=[0,0,-(pcbThickness+mainBoardBottomClearance)])
        multmatrix(inv4x4(bottomCaseAssemblyTransform))
        multmatrix(translate(v=[pcbMountHoleDist, pcbMountHoleDist+mainBoardScrewHoleDy,0])) // TODO adhere to proper space transformations
        rotate(a=[0,0,-90])
        mainBoardMount();

    translate(v=[0,0,-(pcbThickness+mainBoardBottomClearance)])
        multmatrix(inv4x4(bottomCaseAssemblyTransform))
        multmatrix(translate(v=[pcbMountHoleDist+mainBoardScrewHoleDx, pcbMountHoleDist+mainBoardScrewHoleDy,0])) // TODO adhere to proper space transformations
        rotate(a=[0,0,-90])
        mainBoardMount();
}










