// Air side
// --------
    Info << nl << "Creating air diffusivity models" << endl;

    // scalarField passed to diff model to return calculated diffusivity
    // --------------------------------
    scalarField airDiff(airMesh.nCells());

    // pointerlist for all air diffusivity models
    // ------------------------------------------
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        airDiffModels(1 + cathodeZones.size());


    // Diffusivity: whole air zone
    // ---------------------------
    label airZnId = airMesh.cellZones().findZoneID("air");
    Info<< "zone " << airZnId << " air" << nl;
    const labelList& airCells = airMesh.cellZones()[airZnId];
    const dictionary& airDiffDict = airProperties.subDict("diffusivity");

    airDiffModels.set
    (
        0,
        new autoPtr<diffusivityModels::diffusivityModel>
        (
            diffusivityModels::diffusivityModel::New
                (airMesh, airDiff, airCells, airDiffDict)
        )
    );


    // air porous zones
    // ----------------
    label nzAir = cathodeZones.size();

    // initialize pointer lists
    PtrList<const labelList> airZnCells(nzAir);
    PtrList<const dictionary> airZnDiffDict(nzAir);
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        airZnDiffusivity(nzAir);

    forAll(cathodeZones, iz)
    {
        label znId =
            airMesh.cellZones().findZoneID(cathodeZones[iz].zoneName());

        Info<< "cathodeZone " << iz << ", airZone " << znId
            << ": " << cathodeZones[iz].zoneName() << endl;
        Info<< "size = " << airMesh.cellZones()[znId].size() << nl;

        airZnCells.set
        (
            iz,
            new labelList(airMesh.cellZones()[znId])
        );

        airZnDiffDict.set
        (
            iz,
            new dictionary(cathodeZones[iz].dict().subDict("diffusivity"))
        );

        airDiffModels.set
        (
            iz + 1,
            new autoPtr<diffusivityModels::diffusivityModel>
            (
                diffusivityModels::diffusivityModel::New
                    (airMesh, airDiff, airZnCells[iz], airZnDiffDict[iz])
            )
        );
    }



// Fuel side
// ---------
    Info << nl << "Creating fuel diffusivity models" << endl;

    // scalarField passed to diff model to return calculated diffusivity
    // --------------------------------
    scalarField fuelDiff(fuelMesh.nCells());

    // pointerlist for all fuel diffusivity models
    // ------------------------------------------
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        fuelDiffModels(1 + cathodeZones.size());


    // Diffusivity: whole fuel zone
    // ----------------------------
    label fuelZnId = fuelMesh.cellZones().findZoneID("fuel");
    Info<< "zone " << fuelZnId << " fuel" << nl;
    const labelList& fuelCells = fuelMesh.cellZones()[fuelZnId];
    const dictionary& fuelDiffDict = fuelProperties.subDict("diffusivity");

    fuelDiffModels.set
    (
        0,
        new autoPtr<diffusivityModels::diffusivityModel>
        (
            diffusivityModels::diffusivityModel::New
                (fuelMesh, fuelDiff, fuelCells, fuelDiffDict)
        )
    );


    // fuel porous zones
    // -----------------
    label nzFuel = anodeZones.size();

    // initialize pointer lists
    PtrList<const labelList> fuelZnCells(nzFuel);
    PtrList<const dictionary> fuelZnDiffDict(nzFuel);
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        fuelZnDiffusivity(nzFuel);

    forAll(anodeZones, iz)
    {
        label znId =
            fuelMesh.cellZones().findZoneID(anodeZones[iz].zoneName());

        Info<< "anodeZone " << iz << ", fuelZone " << znId
            << ": " << anodeZones[iz].zoneName() << endl;
        Info<< "size = " << fuelMesh.cellZones()[znId].size() << nl;

        fuelZnCells.set
        (
            iz,
            new labelList(fuelMesh.cellZones()[znId])
        );

        fuelZnDiffDict.set
        (
            iz,
            new dictionary(anodeZones[iz].dict().subDict("diffusivity"))
        );

        fuelDiffModels.set
        (
            iz + 1,
            new autoPtr<diffusivityModels::diffusivityModel>
            (
                diffusivityModels::diffusivityModel::New
                    (fuelMesh, fuelDiff, fuelZnCells[iz], fuelZnDiffDict[iz])
            )
        );
    }
