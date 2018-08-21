{
    // Map to cell.  In practice, fields for rho, Cp and k will come from
    // components (air, fuel, electrolyte) but currently they are constant

    scalarField& rhoCpCellIn = rhoCpCell.primitiveFieldRef();
    rhoCpCellIn = 0.0;

    scalarField& kCellIn = kCell.primitiveFieldRef();
    kCellIn = 0.0;

    scalarField& TsourceCellIn = TsourceCell.primitiveFieldRef();
    TsourceCellIn = 0.0;

    scalarField& phiCellIn = phiCell.primitiveFieldRef();
    phiCellIn = 0.0;

    scalarField& rhoCpPhiCellIn = rhoCpPhiCell.primitiveFieldRef();
    rhoCpPhiCellIn = 0.0;

#   include "mapAirToCell.H"
#   include "mapFuelToCell.H"

#   include "mapElectrolyteToCell.H"
#   include "mapInterconnectToCell.H"

    // Correct boundary conditions
    rhoCpCell.correctBoundaryConditions();
    kCell.correctBoundaryConditions();

    // Check continuity of cell fluxes, as they appear in the energy equation
    continuityErrs(phiCell);
}

