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

mainBoardBottomClearance = 6; // Here, the "bottom" is the bottom side where you solder the through hole components.

mainBoardScrewHoleDx = mainBoardX - 2*pcbMountHoleDist;
mainBoardScrewHoleDy = mainBoardY - 2*pcbMountHoleDist;

// IO Board

ioBoardX = 29.0;
ioBoardY = 97.0;

ioBoardBottomClearance = 5;

ioBoardScrewHoleDx = ioBoardX - 2*pcbMountHoleDist;
ioBoardScrewHoleDy = ioBoardY - 2*pcbMountHoleDist;


// Case values

wallThickness = 3;
pcbSpacing = 3;


