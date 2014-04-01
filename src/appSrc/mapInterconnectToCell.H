    // Mapping interconnect subdomain.  Memory management
    {
        // Create constant fields from interconnect side
        scalarField rhoCpF
        (
            interconnectMesh.nCells(),
            rhointerconnect.value()*Cpinterconnect.value()
        );

        scalarField kF
        (
            interconnectMesh.nCells(),
            kinterconnect.value()
        );

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, interconnectCellMap);
        kCellIn.rmap(kF, interconnectCellMap);
    }
