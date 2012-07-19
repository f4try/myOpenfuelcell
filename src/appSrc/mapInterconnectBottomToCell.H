    // Mapping interconnect0 subdomain.  Memory management
    {
        // Create constant fields from interconnect0 side
        scalarField rhoCpF
        (
            interconnectBottomMesh.nCells(),
            rhointerconnectBottom.value()*CpinterconnectBottom.value()
        );

        scalarField kF
        (
            interconnectBottomMesh.nCells(),
            kinterconnectBottom.value()
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, interconnectBottomCellMap);
        kCellIn.rmap(kF, interconnectBottomCellMap);
    }
