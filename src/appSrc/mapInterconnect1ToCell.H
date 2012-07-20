    // Mapping interconnect1 subdomain.  Memory management
    {
        // Create constant fields from interconnect1 side
        scalarField rhoCpF
        (
            interconnect1Mesh.nCells(),
            rhoInterconnect1.value()*CpInterconnect1.value()
        );

        scalarField kF
        (
            interconnect1Mesh.nCells(),
            kInterconnect1.value()
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, interconnect1CellMap);
        kCellIn.rmap(kF, interconnect1CellMap);
    }
