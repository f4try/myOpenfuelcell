    // Mapping electrolyte subdomain.  Memory management
    {
        // Create constant fields from electrolyte side
        scalarField rhoCpF
        (
            electrolyteMesh.nCells(),
            rhoElectrolyte.value()*CpElectrolyte.value()
        );

        scalarField kF(electrolyteMesh.nCells(), kElectrolyte.value());

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, electrolyteCellMap);
        kCellIn.rmap(kF, electrolyteCellMap);


        // Add Ohmic heating from electrolyte
        TsourceCellIn.rmap(ohmicHeating, electrolyteCellMap);
    }
