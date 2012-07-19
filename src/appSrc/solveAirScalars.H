{
    Info << nl << "Solving air scalars" << endl;

    tmp<fv::convectionScheme<scalar> > mvConvection
    (
        fv::convectionScheme<scalar>::New
        (
            airMesh,
            airFields,
            phiAir,
            airMesh.divScheme("div(phi,y)")
        )
    );

    // ddt(rho y) + div(rho U y) - div(gamma grad y) = S = 0; gamma = rho*diff
    // steady state; phi already incorporates rho:
    // div(phi, y) - laplacian(gamma, y) = 0

    fvScalarMatrix yO2AirEqn
    (
        mvConvection->fvmDiv(phiAir, yO2air)
      - fvm::laplacian(rhoAir*diffAir, yO2air, "laplacian(diff,y)")
    );

    // under-relax and solve
    yO2AirEqn.relax();
    yO2AirEqn.solve();

    Info << "min,max(yO2Air): "
         << (Foam::min(yO2air)).value() <<" , "
         << (Foam::max(yO2air)).value() << endl;


    // ensure positivity
    yO2air = Foam::max(yO2air, Foam::doubleScalarSMALL);
    yO2air = Foam::min(yO2air, 1e0);

    yO2air.correctBoundaryConditions();


    yN2air = 1 - yO2air;
    yN2air.correctBoundaryConditions();

    Info << "min,max(yN2Air): "
         << (Foam::min(yN2air)).value() <<" , "
         << (Foam::max(yN2air)).value() << endl;



    // mole fractions
    moleO2air = yO2air/MO2/(yO2air/MO2+yN2air/MN2);
    moleO2air.correctBoundaryConditions();
    
    moleN2air = 1 - moleO2air;
    moleN2air.correctBoundaryConditions();
}
