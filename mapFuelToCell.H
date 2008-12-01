    // Mapping fuel subdomain.  Memory management
    {
        // Create constant fields from fuel side
        scalarField rhoCpFld = rhoFuel*CpFuel;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpFld, fuelCellMap);
        kCellIn.rmap(kFuel, fuelCellMap);


        // Map fuel fluxes
        labelList fuelInternalFaceMap
        (
            SubList<label>(fuelFaceMap, fuelMesh.nInternalFaces())
        );

        scalarField rhoCpPhiFld =
            fvc::interpolate(rhoFuel*CpFuel)*phiFuel.internalField();

//        Info << "rhoF CpF rhoF*CpF"
//             << rhoF.value() << " " << CpF.value()
//             << " " << rhoF.value()*CpF.value() << endl;

//        Info<< "phiFuel.internalField" << phiFuel.internalField() << endl;

//        Info<< "rhoCpPhiF old-new" 
//            << rhoF.value()*CpF.value()*phiFuel.internalField()
//               - rhoCpPhiF
//            << endl;

        phiCellIn.rmap(phiFuel.internalField(), fuelInternalFaceMap);
        rhoCpPhiCellIn.rmap(rhoCpPhiFld, fuelInternalFaceMap);

        // Do flux boundary conditions

        forAll (fuelPatchesMap, patchI)
        {
            if (fuelPatchesMap[patchI] > -1)
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        fuelFaceMap,
                        fuelMesh.boundary()[patchI].size(),
                        fuelMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [fuelPatchesMap[patchI]].patch().start();

                phiCell.boundaryField()[fuelPatchesMap[patchI]].rmap
                (
                    phiFuel.boundaryField()[patchI],
                    curFpm
                );

                rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]].rmap
                (
                    phiFuel.boundaryField()[patchI],
                    curFpm
                );

                rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]] *=
                    //rhoF.value()*CpF.value();
                    rhoFuel.boundaryField()[patchI]
                    *CpFuel.boundaryField()[patchI];
            }
        }
    }
