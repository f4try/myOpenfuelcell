//    Activation term prescribed by Butler-Volmer Equation:
//    i = i0[exp(2*alpha*F*eta/RT-exp(-2*(1-alpha)*F*eta/RT)]
//    eta = Activation overpotential obtained by numerical iversion of BV eqn.
//    i = local current density
//    i0 = exchange current density
//    alpha = transfer coefficient 
//    R = universal gas constant
//    F = Faraday constant
//    For anode (fuel) i0 = gamma*pH2^a*pH2O^b*exp(EAct/(R*T))
//    For cathode (air) i0 = gamma*pO2^m*exp(EAct/(R*T))
//    where
//    gamma = pre-exponent
//    pH2 = partial pressure, H2
//    pH2O = partial pressure, H2O
//    a, b, m  = exponents
//    EAct = Actuation energy
//
//    initialize etaAnode

    scalarField etaAnode(anodeT.size(), 0);
   // scalarField i0Anode(anodeT.size(), 1);
    scalarField etaCathode(anodeT.size(), 0);
  //  scalarField i0Cathode(anodeT.size(), 1);

    scalar A, B;

    tmp<scalarField> i0Anode(fuelActivationOverpotential->exchangeCurrentDensity
    (
        anodeT,
        pFuelPatch,
        xFuel
    ));

    forAll (etaAnode, j) 
    {
      A = 2*fuelActivationOverpotential->alpha()*F/(Rgas*anodeT[j]);
      B = -2*(1-fuelActivationOverpotential->alpha())*F/(Rgas*anodeT[j]);
      testFunction tf(i[j], i0Anode.ref()[j], A , B);
      etaAnode[j] = RiddersRoot<testFunction>(tf, 1e-5).root(0., 1.2); // eta aus (0., 1.2)
   }

   Info<< "min,mean,max calBV(etaAn) = " << gMin(etaAnode) << "  "
       << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;

   etaAnode =
       (1-fuelActivationOverpotential->relax()) * oldEtaA
     + fuelActivationOverpotential->relax() * etaAnode; //relax the etaAnode, Qing, 22.06.2014

   Info<< "min,mean,max newBV(etaAn) = " << gMin(etaAnode) << "  "
       << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;



// Compute local cathode exchange current density
// SBB: Must add the air-side pressure
// SBB: Also Cathode overpotential computed with anode temperature

    tmp<scalarField> i0Cathode(airActivationOverpotential->exchangeCurrentDensity
    (
        cathodeT,
        pAirPatch,
        xAir
    ));

//    Compute cathodic overpotential from inverse Butler-Volmer equation

    forAll (etaCathode, j) 
    {
        A = 2*airActivationOverpotential->alpha()*F/(Rgas*cathodeT[j]);
        B = -2*(1-airActivationOverpotential->alpha())*F/(Rgas*cathodeT[j]);
        testFunction tf(i[j], i0Cathode.ref()[j], A , B);
        etaCathode[j] = RiddersRoot<testFunction>(tf, 1e-5).root(0., 1.2); // eta aus (0., 1.2)
        //etaCathode[j] = Foam::max(Foam::doubleScalarSmall, etaCathode[j]);
    }
    Info<< "min,mean,max calBV(etaCa) = " << gMin(etaCathode) << "  "
        << gAverage(etaCathode) << "  " << gMax(etaCathode) << nl;

    etaCathode = (1-airActivationOverpotential->relax()) * oldEtaC + airActivationOverpotential->relax() * etaCathode;	//relax the etaCathode, Qing, 22.06.2014
    Info<< "min,mean,max newBV(etaCa) = " << gMin(etaCathode) << "  "
        << gAverage(etaCathode) << "  " << gMax(etaCathode) << nl;
   oldEtaA = etaAnode;
   oldEtaC = etaCathode;

// End activation calculation

//  Save data for output

    scalarField& etaAIn = etaA.primitiveFieldRef();
    smearPatchToMesh smearetaA(electrolyteMesh, electrolyteAnodeName);
    etaAIn = smearetaA.smear(etaAnode);
    etaA.correctBoundaryConditions();
    etaA.boundaryFieldRef()[electrolyteAnodeID] == etaAnode;

    scalarField& etaCIn = etaC.primitiveFieldRef();
    smearPatchToMesh smearetaC(electrolyteMesh, electrolyteAnodeName);
    etaCIn = smearetaC.smear(etaCathode);
    etaC.correctBoundaryConditions();
    etaC.boundaryFieldRef()[electrolyteCathodeID] == etaCathode;



