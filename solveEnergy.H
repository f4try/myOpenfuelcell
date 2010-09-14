{
    Info << nl << "Solving energy" << endl;

    // couple patches
#   include "attachPatches.H"

    // Solve energy equation in temperature

    dimensionedScalar rhoCpAir = rhoAir*CpAir;
    dimensionedScalar rhoCpFuel = rhoFuel*CpFuel;

    Info<< "rhoCpAir=" << rhoCpAir << nl;
    Info<< "rhoCpFuel=" << rhoCpFuel << nl << endl;


    const dictionary& piso = airMesh.solutionDict().subDict("PISO");
    int nNonOrthCorr(readInt(piso.lookup("nNonOrthogonalCorrectors")));

    for (int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++)
    {
        Tinterconnect.storePrevIter();
        Tair.storePrevIter();
        Telectrolyte.storePrevIter();
        Tfuel.storePrevIter();

        coupledFvScalarMatrix TEqns(4);

        // Air
        TEqns.set
        (
            0,
            new fvScalarMatrix
            (
                 //fvm::div(rhoCpPhiAir, Tair)
                 rhoCpAir*fvm::div(phiAir, Tair)
               - fvm::laplacian(kAir, Tair)
            )
        );

        // Electrolyte
        TEqns.set
        (
            1,
            new fvScalarMatrix
            (
               -fvm::laplacian(kElectrolyte, Telectrolyte)
               ==
               //ohmicHeating
               Qsrc
            )
        );

        // Fuel
        TEqns.set
        (
            2,
            new fvScalarMatrix
            (
                 //fvm::div(rhoCpPhiFuel, Tfuel)
                 rhoCpFuel*fvm::div(phiFuel, Tfuel)
               - fvm::laplacian(kFuel, Tfuel)
            )
        );

        // Interconnect
        TEqns.set
        (
            3,
            new fvScalarMatrix
            (
               -fvm::laplacian(kInterconnect, Tinterconnect)
            )
        );

        Tinterconnect.relax();
        Tair.relax();
        Telectrolyte.relax();
        Tfuel.relax();

        TEqns.solve();
    }

    continuityErrs(phiAir);
    continuityErrs(phiFuel);

    Info << "Tair min mean max:  "
         << Foam::min(Tair) << "  "
         << Foam::average(Tair) << "  "
         << Foam::max(Tair) << endl;

    Info << "Tfuel min mean max:  "
         << Foam::min(Tfuel) << "  "
         << Foam::average(Tfuel) << "  "
         << Foam::max(Tfuel) << endl;

    Info << "Telectrolyte min mean max:  "
         << Foam::min(Telectrolyte) << "  "
         << Foam::average(Telectrolyte) << "  "
         << Foam::max(Telectrolyte) << endl;

    Info << "Tinterconnect min mean max:  "
         << Foam::min(Tinterconnect) << "  "
         << Foam::average(Tinterconnect) << "  "
         << Foam::max(Tinterconnect) << endl;
}
