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
    scalarField i0Anode(anodeT.size(), 1);
    scalarField etaCathode(anodeT.size(), 0);
    scalarField i0Cathode(anodeT.size(), 1);

    scalar A, B;
    if (gammaFuel.value() > 1.e-5) { // DF 9.9.2014
    // Compute local anode exchange current density
    forAll(fuelSpecies, fsi)
    {
        if(activeFuelSwitch[fsi])
        {
            i0Anode *= Foam::pow
            (
                (pFuelPatch*xFuel[fsi]/pAtm),
                fuelStoichNumbers[fsi].value()
            );
            Info<< "min,mean,max (pf) = " << gMin(pFuelPatch) << "  "
            << gAverage(pFuelPatch) << "  " << gMax(pFuelPatch) << nl;
            Info<< "min,mean,max (xProduct) = " << gMin(xFuel[fsi]) << "  "
            << gAverage(xFuel[fsi]) << "  " << gMax(xFuel[fsi]) << nl;
        }
    }

    i0Anode *= gammaFuel.value()*anodeT;
    i0Anode *= Foam::exp(-1.*EFuel.value()/(Rgas*anodeT));
    Info<< "min,mean,max (anodeT) = " << gMin(anodeT) << "  "
        << gAverage(anodeT) << "  " << gMax(anodeT) << nl;
    Info<< "min,mean,max (i0An) = " << gMin(i0Anode) << "  "
        << gAverage(i0Anode) << "  " << gMax(i0Anode) << nl;
    Info<< "min,mean,max oldBV(etaAn) = " << gMin(oldEtaA) << "  "
        << gAverage(oldEtaA) << "  " << gMax(oldEtaA) << nl;
//    Compute anodic overpotential from inverse Butler-Volmer equation

    forAll (etaAnode, j) 
    {
      A = 2*alphaFuel.value()*F/(Rgas*anodeT[j]);
      B = -2*(1-alphaFuel.value())*F/(Rgas*anodeT[j]);
      testFunction tf(i[j], i0Anode[j], A , B);
      etaAnode[j] = RiddersRoot<testFunction>(tf, 1e-5).root(0., 1.2); // eta aus (0., 1.2)
   }
    Info<< "min,mean,max calBV(etaAn) = " << gMin(etaAnode) << "  "
        << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;

    etaAnode = (1-relaxFuel.value()) * oldEtaA + relaxFuel.value() * etaAnode;	//relax the etaAnode, Qing, 22.06.2014
    Info<< "min,mean,max newBV(etaAn) = " << gMin(etaAnode) << "  "
        << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;
    } // if (gammaAnode()... DF 9.9.2014



// Compute local cathode exchange current density
// SBB: Must add the air-side pressure
// SBB: Also Cathode overpotential computed with anode temperature

    forAll(airSpecies, fsi)
    {
        if(activeAirSwitch[fsi])
        {
            i0Cathode *= Foam::pow
            (
                (pAirPatch*xAir[fsi]/pAtm),
                airStoichNumbers[fsi].value()
            );
            Info<< "min,mean,max (pf) = " << gMin(pAirPatch) << "  "
            << gAverage(pAirPatch) << "  " << gMax(pAirPatch) << nl;
            Info<< "min,mean,max (xProduct) = " << gMin(xAir[fsi]) << "  "
            << gAverage(xAir[fsi]) << "  " << gMax(xAir[fsi]) << nl;
	    }
    }

    i0Cathode *= gammaAir.value()*anodeT;
    i0Cathode *= Foam::exp(-1.*EAir.value()/(Rgas*anodeT));

    Info<< "min,mean,max (i0Ca) = " << gMin(i0Cathode) << "  "
        << gAverage(i0Cathode) << "  " << gMax(i0Cathode) << nl;
    Info<< "min,mean,max oldBV(etaCa) = " << gMin(oldEtaC) << "  "
        << gAverage(oldEtaC) << "  " << gMax(oldEtaC) << nl;

//    Compute cathodic overpotential from inverse Butler-Volmer equation

     forAll (etaCathode, j) 
   {
     A = 2*alphaAir.value()*F/(Rgas*anodeT[j]);
     B = -2*(1-alphaAir.value())*F/(Rgas*anodeT[j]);
     testFunction tf(i[j], i0Cathode[j], A , B);
     etaCathode[j] = RiddersRoot<testFunction>(tf, 1e-5).root(0., 1.2); // eta aus (0., 1.2)
     //etaCathode[j] = Foam::max(Foam::doubleScalarSmall, etaCathode[j]);
   }
    Info<< "min,mean,max calBV(etaCa) = " << gMin(etaCathode) << "  "
        << gAverage(etaCathode) << "  " << gMax(etaCathode) << nl;

    etaCathode = (1-relaxAir.value()) * oldEtaC + relaxAir.value() * etaCathode;	//relax the etaCathode, Qing, 22.06.2014
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



