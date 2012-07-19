
    // Diffusivity: air region
    // -----------------------

    airRegionDiffusivity->evaluate();
    diffAir.internalField() = airDiff;
    if(firstTime)
    {
        Info<< nl << "airRegionDiffusivity" << nl;
        airRegionDiffusivity->writeData();
    }

    // air porous zones
    // ----------------

    forAll(cathodeZones, iz)
    {
        airZnDiffusivity[iz]->evaluate();
        if(firstTime)
        {
	    Info<< cathodeZones[iz].zoneName() << nl;
            airZnDiffusivity[iz]->writeData();
        }
    }

    diffAir.internalField() = airDiff;
    diffAir.correctBoundaryConditions();

    Info << nl;
    Info << "diff(air) [min mean max] = ["
         << Foam::min(diffAir.internalField()) << "  "
         << Foam::average(diffAir.internalField()) << "  "
         << Foam::max(diffAir.internalField()) << "]" << nl;



    // Diffusivity: fuel region
    // ------------------------

    fuelRegionDiffusivity->evaluate();
    diffFuel.internalField() = fuelDiff;
    if(firstTime)
    {
        Info<< nl << "fuelRegionDiffusivity" << nl;
        fuelRegionDiffusivity->writeData();
    }

    // fuel porous zones
    // -----------------

    forAll(anodeZones, iz)
    {
        fuelZnDiffusivity[iz]->evaluate();
        if(firstTime)
        {
	    Info<< anodeZones[iz].zoneName() << nl;
            fuelZnDiffusivity[iz]->writeData();
        }
    }

    diffFuel.internalField() = fuelDiff;
    diffFuel.correctBoundaryConditions();

    Info << "diff(fuel) [min mean max] = ["
         << Foam::min(diffFuel.internalField()) << "  "
         << Foam::average(diffFuel.internalField()) << "  "
         << Foam::max(diffFuel.internalField()) << "]" << nl;
    Info << endl;
