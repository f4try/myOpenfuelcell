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

    // Note: solving all species is not too clever: one should be considered
    // as "background phase".  HJ, 16/Mar/2007

    fvScalarMatrix yH2FuelEqn
    (
        mvConvection->fvmDiv(phiFuel, yH2fuel)
      - fvm::laplacian(gammaFuel/rhoFuel, yH2fuel, "laplacian(gamma,y)")
    );

    // Apply under-relaxation
    yH2FuelEqn.relax();

    // Solve
    yH2FuelEqn.solve();


    fvScalarMatrix yN2FuelEqn
    (
        mvConvection->fvmDiv(phiFuel, yN2fuel)
      - fvm::laplacian(gammaFuel/rhoFuel, yN2fuel, "laplacian(gamma,y)")
    );

    // Apply under-relaxation
    yN2FuelEqn.relax();

    // Solve
    yN2FuelEqn.solve();


    fvScalarMatrix yH2OFuelEqn
    (
        mvConvection->fvmDiv(phiFuel, yH2Ofuel)
      - fvm::laplacian(gammaFuel/rhoFuel, yH2Ofuel, "laplacian(gamma,y)")
    );

    // Apply under-relaxation
    yH2OFuelEqn.relax();

    // Solve
    yH2OFuelEqn.solve();
}
