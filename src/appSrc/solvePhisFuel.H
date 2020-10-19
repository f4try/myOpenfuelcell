{
    Info << nl << "Solving phisFuel" << endl;

    fvScalarMatrix phisFuelEqn
    (
        - fvm::laplacian(sigmaS,phisFuel)
    );

    // Apply under-relaxation: not allowed
    // Under-relaxation in solid conduction (which dominates) creates a large
    // thermal inertia.  HJ, 25/May/2011
    // TcellEqn.relax();

    // Solve
    phisFuelEqn.solve();


    Info << "phisFuel min mean max     = "
         << Foam::gMin(phisFuel.primitiveFieldRef()) << "    " //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
         << Foam::gAverage(phisFuel.primitiveFieldRef()) << "    "
         << Foam::gMax(phisFuel.primitiveFieldRef())
         << nl <<endl;

    Info<< " sum phisFuelEqn.source() =   " << gSum(phisFuelEqn.source()) << nl;
    Info<< endl;
}
