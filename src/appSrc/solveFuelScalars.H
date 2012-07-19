{
    Info << nl << "Solving fuel scalars" << endl;

    tmp<fv::convectionScheme<scalar> > mvConvection
    (
        fv::convectionScheme<scalar>::New
        (
            fuelMesh,
            fuelFields,
            phiFuel,
            fuelMesh.divScheme("div(phi,y)")
        )
    );

    // ddt(rho y) + div(rho U y) -div(gamma grad y) = S = 0; gamma = rho*diff
    // steady state, phi already incorporates rho

    fvScalarMatrix yH2FuelEqn
    (
        mvConvection->fvmDiv(phiFuel, yH2fuel)
      - fvm::laplacian(rhoFuel*diffFuel, yH2fuel, "laplacian(diff,y)")
    );

    // under-relax and solve
    yH2FuelEqn.relax();
    yH2FuelEqn.solve();

    Info << "min,max(yH2fuel): "
         << (Foam::min(yH2fuel)).value() <<" , "
         << (Foam::max(yH2fuel)).value() << endl;


    // ensure positivity
    yH2fuel = Foam::max(yH2fuel, Foam::doubleScalarSMALL);
    yH2fuel = Foam::min(yH2fuel, 1e0);

    yH2fuel.correctBoundaryConditions();


    yH2Ofuel = 1 - yH2fuel;
    yH2Ofuel.correctBoundaryConditions();

    Info << "min,max(yH2Ofuel): "
         << (Foam::min(yH2Ofuel)).value() <<" , "
         << (Foam::max(yH2Ofuel)).value() << endl;


    // mole fractions
    moleH2fuel = yH2fuel/MH2/(yH2fuel/MH2+yH2Ofuel/MH2O);
    moleH2fuel.correctBoundaryConditions();

    moleH2Ofuel = 1 - moleH2fuel;
    moleH2Ofuel.correctBoundaryConditions();
}
