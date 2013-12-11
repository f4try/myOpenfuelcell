    // calculate volumetric electrochemical heating

    Info << nl << "Calculating electrochemical heating" << endl;

    // interpolate anode current density i from fuel side to electrolyte side
    scalarField iEA =
        fuelAnodeToElectrolyteAnode.faceInterpolate(i);

    iEA = Foam::max(0e0, iEA);


    //scalar Tr = 298.15;  //reference Temperature [K] for enthalpy calculations


    // Note (hkr 2012.04.17):
    // Following Cp(T) are 6th degree (7th order) polynomials in T[K]/1000, from
    // Todd & Young, Thermodynamic and transport properties of gases for use in
    // solid oxide fuel cell modelling, J. Power Sources 110 (2002) pp186-200.
    // Cp(T) = sum(a[k]*(T/1000)^k), k = 0,..,6
    // Enthalpies are definite integrals from reference to ambient temperature.


    /***********************************************************************
    scalarField Tsource =
        -(H2Oform + hH2O - hH2 -0.5*hO2)*iEA/(2*F)/hE.value()
        - iEA*V.value()/hE.value();
    ***********************************************************************/


    scalar hFormSum = 0;    //accumulator for enthalpy of formation
                            //normalized by n electrons assoc'd with ionization


    // air side accumulators
    scalarField hReactSrcAir(anodeT.size(),0);
    scalarField hProdSrcAir(anodeT.size(),0);

    PtrList<scalarField> hSpAir(airSpecies.size());
    forAll(airSpecies, i)
    {
        if(airSpecies[i].ne() != 0)
        {
            polyToddYoung& CpI = molarCpAir[i];        // [J/mol]
            scalarField hSpI = CpI.polyInt(Tr, anodeT);
            hSpAir.set(i, hSpI);

            hFormSum += airSpecies[i].hForm()/airSpecies[i].ne();
            // allow reactions on airside, S.Keuler (Dec. 2013)
         

            if(airSpecies[i].rSign() > 0)
            {
                // product
                hProdSrcAir += hSpI/airSpecies[i].ne();
            }
            else if(airSpecies[i].rSign() < 0)
            {
                // reactant
                hReactSrcAir += hSpI/airSpecies[i].ne();
            }
            else
            {
                Info<< "ERROR: rSign = 0 but ne != 0" << nl;
            }
        }
    }
    //Info<< "sum(hReactSrcAir)= " << gSum(hReactSrcAir) << nl;
    //Info<< "sum(hProdSrcAir)= " << gSum(hProdSrcAir) << nl;
    //Info<< "sum(prod - react)=" << gSum(hProdSrcAir - hReactSrcAir) << nl;

/* -------------------------------------------------------------------------- */

    //fuel side accumulators
    scalarField hReactSrcFuel(anodeT.size(),0);
    scalarField hProdSrcFuel(anodeT.size(),0);


    PtrList<scalarField> hSpFuel(fuelSpecies.size());  //species enthalpies
    forAll(fuelSpecies, i)
    {
        if(fuelSpecies[i].ne() != 0)
        {
            polyToddYoung& CpI = molarCpFuel[i];          // [J/mol]
            scalarField hSpI = CpI.polyInt(Tr, anodeT);
            hSpFuel.set(i, hSpI);

            hFormSum += fuelSpecies[i].hForm()/fuelSpecies[i].ne();

            if(fuelSpecies[i].rSign() > 0)
            {
                // product
                hProdSrcFuel += hSpI/fuelSpecies[i].ne();
            }
            else if(fuelSpecies[i].rSign() < 0)
            {
                // reactant
                hReactSrcFuel += hSpI/fuelSpecies[i].ne();
            }
            else
            {
                Info<< "ERROR: rSign = 0 but ne != 0" << nl;
            }
        }
    }
    //Info<< "sum(hReactSrcFuel)= " << gSum(hReactSrcFuel) << nl;
    //Info<< "sum(hProdSrcFuel)= " << gSum(hProdSrcFuel) << nl;
    //Info<< "sum(prod - react)=" << gSum(hProdSrcFuel - hReactSrcFuel) << nl;


    scalarField volMolRate = iEA/F/hE.value();    // [mol/(m^3 s)]

    scalarField hSource =
    (hProdSrcFuel - hReactSrcFuel + hProdSrcAir - hReactSrcAir)*volMolRate;

    //Info<< "hSource  min max sum: "
    //    << gMin(hSource) << " "
    //    << gMax(hSource) << " " << gSum(hSource) << nl;
    //Info<< nl;

//
//    scalarField Tsource =
//        -(H2Oform + hH2O - hH2 -0.5*hO2)*iEA/(2*F)/hE.value()
//        - iEA*V.value()/hE.value();
//

    scalarField Tsource =
    (
        -hFormSum*volMolRate - hSource - iEA*V.value()/hE.value()
    );


    // smear the surface heating field into electrolyte volume
    smearPatchToMesh smearInterpolation
    (
        electrolyteMesh,
        electrolyteAnodeName
    );
    electrochemicalHeating.internalField() = smearInterpolation.smear(Tsource);

    Info << "min,mean,max(electrochemicalHeating): "
         << gMin(electrochemicalHeating.internalField()) << "  "
         << gAverage(electrochemicalHeating.internalField()) << "  "
         << gMax(electrochemicalHeating.internalField()) << endl;

