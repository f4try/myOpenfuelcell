    // Mapping electrolyte subdomain.  Memory management
    {
        scalarField rhoCpF
        (
            electrolyteMesh.nCells(),
            rhoElectrolyte.value()*CpElectrolyte.value()
        );

        scalarField kF(electrolyteMesh.nCells(), kElectrolyte.value());

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, electrolyteCellMap);
        kCellIn.rmap(kF, electrolyteCellMap);

        // Add electrochemical heating from electrolyte
        TsourceCellIn.rmap
        (
            electrochemicalHeating.internalField(),
            electrolyteCellMap
        );

        // Set all fluxes touching the electrolyte to zero


        // Map electrolyte fluxes
        labelList electrolyteInternalFaceMap
        (
            SubList<label>
            (
                electrolyteFaceMap,
                electrolyteMesh.nInternalFaces()
            )
        );
        Info<< "ELECTROLYTE FLUX MAPPING ACTIVATED" << endl;
        //
        // ** recall phi already incorporates rho **
        //

        scalarField zf(electrolyteInternalFaceMap.size(), 0);

        phiCellIn.rmap
        (
            zf,
//             scalarField(electrolyteInternalFaceMap.size(), 0),
            electrolyteInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            zf,
//             scalarField(electrolyteInternalFaceMap.size(), 0),
            electrolyteInternalFaceMap
        );

        // Mapping makes no difference with SuSp term in TEqn.
        // HJ, 12/Dec/2011
        // Do flux boundary conditions
//         forAll (electrolytePatchesMap, patchI)
//         {
//         }
    }
