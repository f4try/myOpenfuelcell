{
    Info << nl << "Solving phil" << endl;

    fvScalarMatrix philEqn
    (
        - fvm::laplacian(sigmaL,phil)
    );

    // Apply under-relaxation: not allowed
    // Under-relaxation in solid conduction (which dominates) creates a large
    // thermal inertia.  HJ, 25/May/2011
    // TcellEqn.relax();

    // Solve
    philEqn.solve();


    Info << "phis min mean max     = "
         << Foam::gMin(phil.primitiveFieldRef()) << "    " //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
         << Foam::gAverage(phil.primitiveFieldRef()) << "    "
         << Foam::gMax(phil.primitiveFieldRef())
         << nl <<endl;

    Info<< " sum philEqn.source() =   " << gSum(philEqn.source()) << nl;
    Info<< endl;
}
