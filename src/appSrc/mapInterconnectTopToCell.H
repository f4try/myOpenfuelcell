    // Mapping interconnect1 subdomain.  Memory management
    {
        // Create constant fields from interconnect1 side
        scalarField rhoCpF
        (
            interconnectTopMesh.nCells(),
            rhointerconnectTop.value()*CpinterconnectTop.value()
        );

        scalarField kF
        (
            interconnectTopMesh.nCells(),
            kinterconnectTop.value()
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, interconnectTopCellMap);
        kCellIn.rmap(kF, interconnectTopCellMap);
    }
