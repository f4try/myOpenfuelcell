    Info<< "coupling patches" << endl;

#   include "attachPatches.H"

    kAir.correctBoundaryConditions();
    kFuel.correctBoundaryConditions();
    kElectrolyte.correctBoundaryConditions();
    kInterconnect.correctBoundaryConditions();

//  include "detachPatches.H"
