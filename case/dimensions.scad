/*

  Contains some the main case variables. These all exist within a single file, as most of these variables can have
  pretty complicated dependencies on other variables. For example, the size of the main board and thickness of the
  IO board directly affects the dimensions of the case.

  All dimensions are in millimetres, unless otherwise stated.
  
*/

// General PCB settings
pcbThickness = 1.6;
pcbR = 2;
pcbColor = [0.1, 0.6, 0.1];
pcbMountHoleType = "m3";
pcbMountHoleDist = 4;  // X (or Y) distance between the midpoint of the pcb mount hole and the edge of the PCB.
pcbX = 34.0;
pcbY = 100.0;
pcbHoles = [[pcbX - 3.50, 3.50],[pcbX - 17.00 , pcbY - 3.50]];

// Main Board
mainBoardX = pcbX; // What "X" and "Y" values are are local to the main board module
mainBoardY = pcbY;
mainBoardHoles = pcbHoles;

// IO Board
ioBoardX = pcbX;
ioBoardY = pcbY;
ioBoardHoles = pcbHoles;


// Case values
wallThickness = 2;
pcbSpacing = 3;

bottomCaseX =
    pcbThickness // from the IO board
    + mainBoardX
    + pcbSpacing;

bottomCaseY =
    pcbSpacing
    + mainBoardY
    + pcbSpacing;


batteryExtraSpace = 18;

// assumes that the speaker z value is not the largest
bottomCaseZ =    
    + pcbThickness // from main board
    + 30; // TODO: this is from the headers that mount on top of the mainBoard, maybe figure out a better way to do this?
    
