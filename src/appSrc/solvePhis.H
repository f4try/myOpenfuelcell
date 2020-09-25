{
    Info << nl << "Solving phis" << endl;

    fvScalarMatrix phisEqn
    (
        - fvm::laplacian(sigmaS,phis)
    );

    // Apply under-relaxation: not allowed
    // Under-relaxation in solid conduction (which dominates) creates a large
    // thermal inertia.  HJ, 25/May/2011
    // TcellEqn.relax();

    // Solve
    phisEqn.solve();


    Info << "phis min mean max     = "
         << Foam::gMin(phis.primitiveFieldRef()) << "    " //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
         << Foam::gAverage(phis.primitiveFieldRef()) << "    "
         << Foam::gMax(phis.primitiveFieldRef())
         << nl <<endl;

    Info<< " sum phisEqn.source() =   " << gSum(phisEqn.source()) << nl;
    Info<< endl;
}
