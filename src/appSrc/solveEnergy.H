{
    Info << nl << "Solving energy" << endl;

    fvScalarMatrix TcellEqn
    (
        fvm::div(rhoCpPhiCell, Tcell)
      + fvm::SuSp(-fvc::div(rhoCpPhiCell), Tcell)
      - fvm::laplacian(kCell, Tcell)
      - TsourceCell
    );

    // Apply under-relaxation: not allowed
    // Under-relaxation in solid conduction (which dominates) creates a large
    // thermal inertia.  HJ, 25/May/2011
    // TcellEqn.relax();

    // Solve
    TcellEqn.solve();


    Info << "T min mean max     = "
         << Foam::gMin(Tcell.primitiveFieldRef()) << "    " //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
         << Foam::gAverage(Tcell.primitiveFieldRef()) << "    "
         << Foam::gMax(Tcell.primitiveFieldRef())
         << nl <<endl;

    Info<< " sum TcellEqn.source() =   " << gSum(TcellEqn.source()) << nl;
    Info<< endl;
}
