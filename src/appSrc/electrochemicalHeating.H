    // calculate volumetric electrochemical heating

    Info << nl << "Calculating electrochemical heating" << endl;

    // interpolate anode current density i from fuel side to electrolyte side
    scalarField iEA =
        fuelAnodeToElectrolyteAnode.faceInterpolate(i);

    iEA = Foam::max(0e0, iEA);


    scalar Tr = 298.15;  //reference Temperature [K] for enthalpy calculations
    scalarField Tref (anodeT.size(), Tr);
    //
    // Note: Tref field avoids ambiguous "-" overload in enthalpy calculations


    // Note (hkr 2012.04.17):
    // Following Cp(T) are 6th degree (7th order) polynomials in T[K]/1000, from
    // Todd & Young, Thermodynamic and transport properties of gases for use in
    // solid oxide fuel cell modelling, J. Power Sources 110 (2002) pp186-200.
    // Cp(T) = sum(a[k]*(T/1000)^k), k = 0,..,6
    // Enthalpies are definite integrals from reference to ambient temperature.
    // integral(Cp(T)dT) = sum(a[k]/(k+1)*(T^k+1)/(1000^k)), k=0,..6
/***********************************************************************
    polyToddYoung CpH2O
        (37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015);

    polyToddYoung CpH2
        (21.157, 56.036, -150.55, 199.29, -136.15, 46.903, -6.4725);

    polyToddYoung CpO2
    (    34.850, -57.975, 203.68, -300.37, 231.72, -91.821, 14.776);
***********************************************************************/


    // enthalpy H2O
    scalarField hH2O = CpH2O.polyInt(anodeT) - CpH2O.polyInt(Tref);

    //enthalpy H2
    scalarField hH2 = CpH2.polyInt(anodeT) - CpH2.polyInt(Tref);

    //enthapy O2
    scalarField hO2 = CpO2.polyInt(anodeT) - CpO2.polyInt(Tref);


    //Heat source
    // hE is thickness of electrolyte

    // Note (hkr 2010.10.20):
    // Enthalpy of formation of H20 at 298.15K (-241.8349e3 J/mol) comes from
    // Table 15, Hernandez-Pacheco and Mann, The rational approximation method
    // in the prediction of thermodynamic properties for SOFCs,
    // J. Power Sources 128 (2004) pp25-33

    scalar H2Oform = -241.8349e3;    // [J/mol] Hernandez-Pacheco and Mann 2004

    scalarField Tsource =
        -(H2Oform + hH2O - hH2 -0.5*hO2)*iEA/(2*F)/hE.value()
        - iEA*V.value()/hE.value();


    // smear the surface heating field into electrolyte volume
    smearPatchToMesh smearInterpolation
    (
        electrolyteMesh,
        electrolyteAnodeName
    );
    electrochemicalHeating.internalField() = smearInterpolation.smear(Tsource);

    Info << "min,mean,max(electrochemicalHeating): "
         << gMin(electrochemicalHeating.internalField()) << " , "
         << gAverage(electrochemicalHeating.internalField()) << " , "
         << gMax(electrochemicalHeating.internalField()) << endl;

