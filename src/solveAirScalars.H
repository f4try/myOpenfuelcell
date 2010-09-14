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

    // Note: solving all species is not too clever: one should be considered
    // as "background phase".  HJ, 16/Mar/2007

    fvScalarMatrix yO2AirEqn
    (
        mvConvection->fvmDiv(phiAir, yO2air)
      - fvm::laplacian(gammaAir/rhoAir, yO2air, "laplacian(gamma,y)")
    );

    // Apply under-relaxation
    yO2AirEqn.relax();

    // Solve
    yO2AirEqn.solve();


    fvScalarMatrix yN2AirEqn
    (
        mvConvection->fvmDiv(phiAir, yN2air)
      - fvm::laplacian(gammaAir/rhoAir, yN2air, "laplacian(gamma,y)")
    );

    // Apply under-relaxation
    yN2AirEqn.relax();

    // Solve
    yN2AirEqn.solve();
}
