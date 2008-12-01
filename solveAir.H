{
    Info << nl << "Solving air flow" << endl;

    pAir.storePrevIter();

    // Read controls
    const dictionary& piso = airMesh.solutionDict().subDict("PISO");
    int nCorr(readInt(piso.lookup("nCorrectors")));
    int nNonOrthCorr(readInt(piso.lookup("nNonOrthogonalCorrectors")));

    fvVectorMatrix UEqn
    (
        fvm::div(phiAir, Uair)
      - fvm::laplacian(nuAir, Uair)
    );
    
    UEqn.relax();

    solve(UEqn == -fvc::grad(pAir));

    // --- PISO loop

    for (int corr = 0; corr < nCorr; corr++)
    {
        pAir.boundaryField().updateCoeffs();
        Uair = UEqn.H()/UEqn.A();
        Uair.correctBoundaryConditions();

        phiAir = fvc::interpolate(Uair) & airMesh.Sf();

        for (int nonOrth = 0; nonOrth <= nNonOrthCorr; nonOrth++)
        {
            fvScalarMatrix pEqn
            (
                fvm::laplacian(1.0/UEqn.A(), pAir) == fvc::div(phiAir)
            );

            pEqn.setReference(pAirRefCell, pAirRefValue);
            pEqn.solve();

            if (nonOrth == nNonOrthCorr)
            {
                phiAir -= pEqn.flux();
            }
        }

        continuityErrs(phiAir);

        // Explicitly relax pressure for momentum corrector
        pAir.relax();

        Uair -= fvc::grad(pAir)/UEqn.A();
        Uair.correctBoundaryConditions();
    }
}
