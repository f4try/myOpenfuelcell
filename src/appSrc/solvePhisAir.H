{
    Info << nl << "Solving phisAir" << endl;

    fvScalarMatrix phisAirEqn
    (
        - fvm::laplacian(sigmaS,phisAir)
    );

    // Apply under-relaxation: not allowed
    // Under-relaxation in solid conduction (which dominates) creates a large
    // thermal inertia.  HJ, 25/May/2011
    // TcellEqn.relax();

    // Solve
    phisAirEqn.solve();


    Info << "phisAir min mean max     = "
         << Foam::gMin(phisAir.primitiveFieldRef()) << "    " //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
         << Foam::gAverage(phisAir.primitiveFieldRef()) << "    "
         << Foam::gMax(phisAir.primitiveFieldRef())
         << nl <<endl;

    Info<< " sum phisAirEqn.source() =   " << gSum(phisAirEqn.source()) << nl;
    Info<< endl;
}
