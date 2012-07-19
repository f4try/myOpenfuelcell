    // Mapping fuel subdomain.  Memory management
    {
        // Create fields from fuel side

        //scalarField rhoCpF_old
        //(
        //    fuelMesh.nCells(),
        //    CpFuel.value()
        //);
        //rhoCpF_old *= rhoFuel.internalField();
        //
        //Info << nl;
        //Info<< "min mean max rhoCpF_old = " << gMin(rhoCpF_old) << " "
        //    << gMin(rhoCpF_old) << " " << gMin(rhoCpF_old) << nl;


        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]
        scalarField cpH2 = CpH2.polyVal(Tfuel)/MH2;
        scalarField cpH2O = CpH2O.polyVal(Tfuel)/MH2O;

        scalarField& cp = cpFuel.internalField();
        cp = cpH2*yH2fuel + cpH2O*yH2Ofuel;
        cpFuel.correctBoundaryConditions();

        scalarField rhoCpF = cp*rhoFuel.internalField();

        Info<< "min mean max rhoCpF(fuel) = " << gMin(rhoCpF) << " "
            << gAverage(rhoCpF) << " " << gMax(rhoCpF) << nl;
        Info << endl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, fuelCellMap);


        scalarField kF(fuelMesh.nCells(), kFuel.value());

        forAll(anodeZones, iz)
        {
            label znId =
                fuelMesh.cellZones().findZoneID(anodeZones[iz].zoneName());

            dimensionedScalar CpZn(anodeZones[iz].dict().lookup("Cp"));
            dimensionedScalar kZn(anodeZones[iz].dict().lookup("k"));
            scalar porZn = anodeZones[iz].porosity();

            labelList znCells(fuelMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                kF[znCells[cellI]] =
                    kZn.value()*(1 - porZn) + kFuel.value()*porZn;
            }
        }

        // Perform reverse mapping
        kCellIn.rmap(kF, fuelCellMap);


        // Map fuel fluxes
        labelList fuelInternalFaceMap
        (
            SubList<label>(fuelFaceMap, fuelMesh.nInternalFaces())
        );

        scalarField fuelInternalFaceMask
        (
            scalarField::subField(fuelFaceMask, fuelMesh.nInternalFaces())
        );

        //
        // ** recall: phi already incorporates rho **
        //

        scalarField rhoCpPhiF =
            //CpFuel.value()*phiFuel.internalField();
            linearInterpolate(cpFuel)*phiFuel.internalField();

        phiCellIn.rmap
        (
            phiFuel.internalField()*fuelInternalFaceMask,
            fuelInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*fuelInternalFaceMask,
            fuelInternalFaceMap
        );


        // Do electrolyte interface    (hkr: 2010 Dec 03)
        scalarField anodePhi = phiFuel.boundaryField()[anodeID];
        forAll(anodePhi, faceI)
        {
            phiCell[anodeMask[faceI]] = anodePhi[faceI];
            //rhoCpPhiCell[anodeMask[faceI]] = CpFuel.value()*anodePhi[faceI];
            rhoCpPhiCell[anodeMask[faceI]] =
            (
                cpFuel[faceI]*anodePhi[faceI]
            );
        }

        // Do flux boundary conditions
        forAll (fuelPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                fuelPatchesMap[patchI] > -1
             && fuelPatchesMap[patchI] < mesh.boundary().size()
            )
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

                scalarField curMask
                (
                    scalarField::subField
                    (
                        fuelFaceMask,
                        fuelMesh.boundary()[patchI].size(),
                        fuelMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [fuelPatchesMap[patchI]].patch().start();

                phiCell.boundaryField()[fuelPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        phiFuel.boundaryField()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        (
                            cpFuel.boundaryField()[patchI]
                            *phiFuel.boundaryField()[patchI]
                        )*curMask,
                        curFpm
                    );

                //rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]] *=
                //(
                //    CpFuel.value()
                //);
            }
        }
    }
