{
    Info << nl << "Solving electrochemistry" << endl;


    // Interpolate the temperature
    scalarField anodeT(anodePatch.size());
    {
        surfaceScalarField Ts = fvc::interpolate(Tcell);

        forAll (anodeT, faceI)
        {
            anodeT[faceI] = Ts[anodeMask[faceI]];
        }
    }

    Info << "min,max(anodeT): "
         << Foam::min(anodeT) <<" , "
         << Foam::max(anodeT) << endl;

    // Interpolate cathode molar fraction of oxygen from the air side
    scalarField xO2 =
        cathodeToAnode.faceInterpolate
        (
            yO2air.boundaryField()[cathodeID]/MO2/
            (
                yO2air.boundaryField()[cathodeID]/MO2
              + yN2air.boundaryField()[cathodeID]/MN2
            )
        );

    // Calculate molar fractions of hydrogen and water on the fuel side
    scalarField xH2 =
        yH2fuel.boundaryField()[anodeID]/MH2/
        (
            yH2fuel.boundaryField()[anodeID]/MH2
          + yH2Ofuel.boundaryField()[anodeID]/MH2O
        );

    scalarField xH2O =
        yH2Ofuel.boundaryField()[anodeID]/MH2O/
        (
            yH2fuel.boundaryField()[anodeID]/MH2
          + yH2Ofuel.boundaryField()[anodeID]/MH2O
        );

    Info << "min,max(xO2): "
         << Foam::min(xO2) <<" , "
         << Foam::max(xO2) << endl;

    Info << "min,max(xH2): "
         << Foam::min(xH2) <<" , "
         << Foam::max(xH2) << endl;

    Info << "min,max(xH2O): "
         << Foam::min(xH2O) <<" , "
         << Foam::max(xH2O) << endl;

    // ensure positivity
    xO2 = Foam::max(xO2, Foam::doubleScalarSMALL);
    xH2 = Foam::max(xH2, Foam::doubleScalarSMALL);
    xH2O = Foam::max(xH2O, Foam::doubleScalarSMALL);


    // Calculate Nernst potential
    // Note (hkr 2010.10.20):
    //     -deltaG(T) is approximated by (-deltaH + T*deltaS), where
    //     -deltaH=247340 and deltaS=-54.85 are consistent with T=925K using
    //     Hernandez-Pacheco & Mann, The rational approximation method in the
    //     prediction of thermodynamic properties for SOFCs, J. Power Sources
    //     128 (2004) 25-33.

    scalarField E = (247340 - 54.85*anodeT)/(2*F)
        + Rgas*anodeT/(2*F)*Foam::log(xH2*pow(xO2, 0.5)/xH2O);


    // area specific resistance, R
    // scalarField R is based on anodeT
    #include "ASRfunction.H"


    // Calculate current density
    scalarField i = (E - V.value())/R;

    // ensure non-negativity
    i = Foam::max(i, 0.0);

    scalarField& idensityIn = idensity.internalField();
    smearPatchToMesh smearIdensity(fuelMesh, anodeName);
    idensityIn = smearIdensity.smear(i);
    idensity.correctBoundaryConditions();
    idensity.boundaryField()[anodeID] == i;

    // mean current density
    dimensionedScalar ibar ("ibar", dimensionSet(0, -2, 0, 0, 0, 1, 0), 0.0);
    ibar.value() = Foam::average(i);

    Info << "min,max(E): "
         << Foam::min(E) <<" , "
         << Foam::max(E) << endl;

    Info << "min,mean,max(i): "
         << Foam::min(i) <<" , " << ibar.value() << " , "
         << Foam::max(i) << endl;

    // voltage correction
    V += Rhat*(ibar - ibar0);

    Info << "ibar0 = " << ibar0.value()
         << "    ibar = " << ibar.value()
         << "    V = " << V.value() << endl;

    // Calculate electrochemical heating
    #include "electrochemicalHeating.H"

    // Calculate species sources and sinks and set
    // boundary conditions for scalars

    // Air side
    {
        scalarField mO2 = -MO2*i/(vO2*F);                         // (3)

        #include "O2utilization.H"

        scalarField gammaCathode =
        (
            diffAir.boundaryField()[cathodeID]
            *rhoAir.boundaryField()[cathodeID]
        );

        // yN2 bc
        fixedGradientFvPatchScalarField& N2bc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yN2air.boundaryField()[cathodeID]
            );

        // N2 source
        scalarField N2source =
            anodeToCathode.faceInterpolate(-mO2*N2bc);            // (5)

        // Set the N2 gradient boundary conditon
        N2bc.gradient() = N2source/gammaCathode;

        // yO2 bc
        fixedGradientFvPatchScalarField& O2bc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yO2air.boundaryField()[cathodeID]
            );

        // O2 sink
        scalarField O2sink =
            anodeToCathode.faceInterpolate(mO2*(1.- O2bc));           // (4)

        // Set the gradient boundary conditon
        O2bc.gradient() = O2sink/gammaCathode;

	// Set the interface velocity condition
        Uair.boundaryField()[cathodeID] ==
        (
            -anodeToCathode.faceInterpolate(mO2)
            /rhoAir.boundaryField()[cathodeID]
            *(airMesh.Sf().boundaryField()[cathodeID])
            /(airMesh.magSf().boundaryField()[cathodeID])
        );
    }

    // Fuel side
    {
        scalarField mH2 = -MH2*i/(vH2*F);
        scalarField mH2O = MH2O*i/(vH2O*F);

        #include "H2utilization.H"

        scalarField gammaAnode =
        (
            diffFuel.boundaryField()[anodeID]
            *rhoFuel.boundaryField()[anodeID]
        );

        // yH2 bc
        fixedGradientFvPatchScalarField& H2bc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yH2fuel.boundaryField()[anodeID]
            );

        // yH2O bc
        fixedGradientFvPatchScalarField& H2Obc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yH2Ofuel.boundaryField()[anodeID]
            );

        // Set the gradient boundary conditons
        H2bc.gradient() =
            (mH2*(1 - H2bc) - mH2O*H2bc)/gammaAnode;     // (7,12)

        H2Obc.gradient() =
            (-mH2*H2Obc + mH2O*(1 - H2Obc))/gammaAnode;  //(8,11)

	// Set the interface velocity condition
        Ufuel.boundaryField()[anodeID] ==
        (
            -(mH2O + mH2)
            /rhoFuel.boundaryField()[anodeID]
            *(fuelMesh.Sf().boundaryField()[anodeID])
            /(fuelMesh.magSf().boundaryField()[anodeID])
        );
    }
}

// O2 (air side) and H2 (fuel side) utilization based on flow rate
{
    #include "utilization.H"
    #include "ReynoldsNumber.H"
}
