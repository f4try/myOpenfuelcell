    //volumetric ohmic Heating

    scalarField ohmicHeating
    (
        electrolyteMesh.nCells(),
        HsrcElectrolyte.value()
    );

