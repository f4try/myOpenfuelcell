{
    Info << nl << "Solving fuel flow" << endl;

#   include "detachPatches.H"

    pFuel.storePrevIter();

    // Read controls
    const dictionary& piso = fuelMesh.solutionDict().subDict("PISO");
    int nCorr(readInt(piso.lookup("nCorrectors")));
    int nNonOrthCorr(readInt(piso.lookup("nNonOrthogonalCorrectors")));

    fvVectorMatrix UEqn
    (
        fvm::div(phiFuel, Ufuel)
      - fvm::laplacian(nuFuel, Ufuel)
    );

    UEqn.relax();

    solve(UEqn == -fvc::grad(pFuel));

    // --- PISO loop

    for (int corr = 0; corr < nCorr; corr++)
    {
        pFuel.boundaryField().updateCoeffs();
        Ufuel = UEqn.H()/UEqn.A();
        Ufuel.correctBoundaryConditions();

        phiFuel = fvc::interpolate(Ufuel) & fuelMesh.Sf();

        for (int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++)
        {
            fvScalarMatrix pEqn
            (
                fvm::laplacian(1.0/UEqn.A(), pFuel) == fvc::div(phiFuel)
            );

            pEqn.setReference(pFuelRefCell, pFuelRefValue);
            pEqn.solve();

            if (nonOrth == nNonOrthCorr)
            {
                phiFuel -= pEqn.flux();
            }
        }

        continuityErrs(phiFuel);

        // Explicitly relax pressure for momentum corrector
        pFuel.relax();

        Ufuel -= fvc::grad(pFuel)/UEqn.A();
        Ufuel.correctBoundaryConditions();
    }
}
