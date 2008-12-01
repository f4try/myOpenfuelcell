    // Mapping interconnect subdomain.  Memory management
    {
        // Create constant fields from interconnect side
        scalarField rhoCpF
        (
            interconnectMesh.nCells(),
            rhoInterconnect.value()*CpInterconnect.value()
        );

        scalarField kF(interconnectMesh.nCells(), kInterconnect.value());

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, interconnectCellMap);
        kCellIn.rmap(kF, interconnectCellMap);
    }
