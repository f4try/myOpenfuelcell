    // Mapping interconnect0 subdomain.  Memory management
    {
        // Create constant fields from interconnect0 side
        scalarField rhoCpF
        (
            interconnect0Mesh.nCells(),
            rhoInterconnect0.value()*CpInterconnect0.value()
        );

        scalarField kF
        (
            interconnect0Mesh.nCells(),
            kInterconnect0.value()
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, interconnect0CellMap);
        kCellIn.rmap(kF, interconnect0CellMap);
    }
