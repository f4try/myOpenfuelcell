{
    // Map to cell.  In practice, fields for rho, Cp and k will come from
    // components (air, fuel, electrolyte) but currently they are constant

    scalarField& rhoCpCellIn = rhoCpCell.internalField();
    rhoCpCellIn = 0.0;

    scalarField& kCellIn = kCell.internalField();
    kCellIn = 0.0;

    scalarField& TsourceCellIn = TsourceCell.internalField();
    TsourceCellIn = 0.0;

    scalarField& phiCellIn = phiCell.internalField();
    phiCellIn = 0.0;

    scalarField& rhoCpPhiCellIn = rhoCpPhiCell.internalField();
    rhoCpPhiCellIn = 0.0;

#   include "mapAirToCell.H"
#   include "mapFuelToCell.H"

#   include "mapElectrolyteToCell.H"
#   include "mapInterconnectBottomToCell.H"
#   include "mapInterconnectTopToCell.H"

    // Correct boundary conditions
    rhoCpCell.correctBoundaryConditions();
    kCell.correctBoundaryConditions();

    // Check continuity of cell fluxes, as they appear in the energy equation
    continuityErrs(phiCell);
}

