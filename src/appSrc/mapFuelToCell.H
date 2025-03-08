    // Mapping fuel subdomain.  Memory management
    {
        // Create fields from fuel side

        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]

        scalarField& cp = cpFuel.primitiveFieldRef();
        cp = 0;

        forAll(fuelSpecies, i)
        {
            // molarCp[i].polyVal() returns [J/(mol K)]
            // species[i].MW() returns {kg/kmol]
            // we require cp [J/(kg K] = [J/(mol K)]/[kg/mol]

            scalar rMWi = 1.0/(fuelSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpi =
            (
                 molarCpFuel[i].polyVal(Tfuel.primitiveFieldRef())*rMWi
            );
            cp += Yfuel[i].primitiveFieldRef()*cpi;
        }
        cpFuel.correctBoundaryConditions();

        scalarField rhoCpF = cp*rhoFuel.primitiveFieldRef();

        Info<< "min mean max rhoCpF(fuel) = " << gMin(rhoCpF) << " "
            << gAverage(rhoCpF) << " " << gMax(rhoCpF) << nl;
        Info << endl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, fuelCellMap);


        //scalarField kIn(fuelMesh.nCells(), kF.value());
        scalarField& kIn = kFuel.primitiveFieldRef();

        forAll(anodeZones, iz)
        {
            label znId =
                fuelMesh.cellZones().findZoneID(anodeZones[iz].zoneName());

            dimensionedScalar kZn(anodeZones[iz].dict().lookup("k"));
            scalar porZn = anodeZones[iz].porosity();

            labelList znCells(fuelMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                //kIn[znCells[cellI]] =
                //    kZn.value()*(1 - porZn) + kF.value()*porZn;

                kIn[znCells[cellI]] *= porZn;
                kIn[znCells[cellI]] += (1 - porZn)*kZn.value();
            }
        }

        // Perform reverse mapping
        kCellIn.rmap(kIn, fuelCellMap);


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
            //CpFuel.value()*phiFuel.primitiveFieldRef();
            linearInterpolate(cpFuel)*phiFuel.primitiveFieldRef();

        phiCellIn.rmap
        (
            phiFuel.primitiveFieldRef()*fuelInternalFaceMask,
            fuelInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*fuelInternalFaceMask,
            fuelInternalFaceMap
        );


        // Do electrolyte interface    (hkr: 2010 Dec 03)
        scalarField anodePhi = phiFuel.boundaryFieldRef()[anodeID];
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

                phiCell.boundaryFieldRef()[fuelPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        phiFuel.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryFieldRef()[fuelPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        (
                            cpFuel.boundaryFieldRef()[patchI]
                            *phiFuel.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );

                //rhoCpPhiCell.boundaryFieldRef()[fuelPatchesMap[patchI]] *=
                //(
                //    CpFuel.value()
                //);
            }
        }
    }
