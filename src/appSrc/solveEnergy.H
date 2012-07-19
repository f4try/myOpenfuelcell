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
         << Foam::min(Tcell.internalField()) << "    "
         << Foam::average(Tcell.internalField()) << "    "
         << Foam::max(Tcell.internalField())
         << nl <<endl;

    Info<< " sum TcellEqn.source() =   " << gSum(TcellEqn.source()) << nl;
    Info<< endl;
}
