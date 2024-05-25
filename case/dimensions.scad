/*

  Contains some the main case variables. These all exist within a single file, as most of these variables can have
  pretty complicated dependencies on other variables. For example, the size of the main board and thickness of the
  IO board directly affects the dimensions of the case.

  All dimensions are in millimetres, unless otherwise stated.
  
*/

// General PCB settings

pcbThickness = 1.6;

pcbMountHoleType = "m3";
pcbMountHoleDist = 4;  // X (or Y) distance between the midpoint of the pcb mount hole and the edge of the PCB.



// Main Board

mainBoardX = 62.5; // What "X" and "Y" values are are local to the main board module
mainBoardY = 97.0;

mainBoardBottomClearance = 7; // Here, the "bottom" is the bottom side where you solder the through hole components.

mainBoardScrewHoleDx = mainBoardX - 2*pcbMountHoleDist;
mainBoardScrewHoleDy = mainBoardY - 2*pcbMountHoleDist;

// IO Board

ioBoardX = 29.0;
ioBoardY = 97.0;

ioBoardBottomClearance = 5;

ioBoardScrewHoleDx = ioBoardX - 2*pcbMountHoleDist;
ioBoardScrewHoleDy = ioBoardY - 2*pcbMountHoleDist;


// Speaker values -  assumes a rectangular-ish speaker
speakerX = 70;
speakerY = 31;
speakerZ = 16;

speakerScrewHoleDx = 63;
speakerScrewHoleDy = 24;

speakerBackClearance = 2;


// Case values

wallThickness = 2;
pcbSpacing = 3;

bottomCaseX =
    pcbThickness // from the IO board
    + ioBoardBottomClearance
    + mainBoardX
    + pcbSpacing;

bottomCaseY =
    pcbSpacing
    + speakerZ
    + speakerBackClearance
    + mainBoardY
    + pcbSpacing;

// assumes that the speaker z value is not the largest
bottomCaseZ =
    mainBoardBottomClearance
    + pcbThickness // from main board
    + 30; // TODO: this is from the headers that mount on top of the mainBoard, maybe figure out a better way to do this?
    
