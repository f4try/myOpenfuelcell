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
          + yN2fuel.boundaryField()[anodeID]/MN2
          + yH2Ofuel.boundaryField()[anodeID]/MH2O
        );

    scalarField xH2O =
        yH2Ofuel.boundaryField()[anodeID]/MH2O/
        (
            yH2fuel.boundaryField()[anodeID]/MH2
          + yN2fuel.boundaryField()[anodeID]/MN2
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

    // Calculate Nernst potential
    scalarField E =
        E0.value()
        + physicalConstant::Rgas.value()*anodeT/(2*F)
        *Foam::log(xH2*pow(xO2, 0.5)/xH2O);

    // Calculate lumped resistance
    //    range of validity = 550 to 1200 degrees Celsius = 823 to 1473 K
    //    original polynomial in r delivers R in units of ohm*cm^2
    //    we multiply by 1.0e-4 to convert to ohm*m^2

    scalarField r = 1000.0/(anodeT-273.15) - 1.1463;

    scalarField R = 1.0e-4
        *(0.3044 + 0.408*r + 0.8687*sqr(r) + 2.7861*pow(r,3) + 2.9285*pow(r,4));

    // Calculate current density
    scalarField i = (E - V.value())/R;

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

    // Calculate ohmic heating
    #include "ohmicHeating.H"

    // Calculate species sources and sinks and set
    // boundary conditions for scalars

    // Air side
    {
        scalarField mO2 = -MO2*i/(vO2*F);                         // (3)

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
        N2bc.gradient() = N2source/(gammaAir/rhoAir).value();


        // yO2 bc
        fixedGradientFvPatchScalarField& O2bc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yO2air.boundaryField()[cathodeID]
            );

        // O2 sink
        scalarField O2sink =
            anodeToCathode.faceInterpolate(mO2*(N2bc));           // (4)

        // Set the gradient boundary conditon
        O2bc.gradient() = O2sink/(gammaAir/rhoAir).value();
    }


    // Fuel side
    {
        scalarField mH2 = -MH2*i/(vH2*F);
        scalarField mH2O = MH2O*i/(vH2O*F);
        
        // yH2 bc
        fixedGradientFvPatchScalarField& H2bc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yH2fuel.boundaryField()[anodeID]
            );

        // yN2 bc
        fixedGradientFvPatchScalarField& N2bc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yN2fuel.boundaryField()[anodeID]
            );

        // yH2O bc
        fixedGradientFvPatchScalarField& H2Obc =
            refCast<fixedGradientFvPatchScalarField>
            (
                yH2Ofuel.boundaryField()[anodeID]
            );

        H2bc.gradient() =
            (mH2*(1 - H2bc) - mH2O*H2bc)/(gammaFuel/rhoFuel).value();  // (7,12)

        N2bc.gradient() =
            (-mH2*N2bc - mH2O*N2bc)/(gammaFuel/rhoFuel).value();       // (9,13)

        H2Obc.gradient() =
            (-mH2*H2Obc + mH2O*(1 - H2Obc))/(gammaFuel/rhoFuel).value(); //(8,11
    }
}
