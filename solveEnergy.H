{
    Info << nl << "Solving energy" << endl;

    // Solve energy equation in temperature
    solve
    (
        fvm::div(rhoCpPhiCell, Tcell)
      - fvm::laplacian(kCell, Tcell)
     ==
        TsourceCell
    );

    continuityErrs(phiCell);

    Info << "T min: " << Foam::min(Tcell.internalField()) << "    "
         << "T max: " << Foam::max(Tcell.internalField()) << endl;
}
