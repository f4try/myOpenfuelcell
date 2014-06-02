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
    if (activationOverpotential)//Begin activation calculation 
{
    scalar A, B;

    // Compute local anode exchange current density
    forAll(fuelSpecies, fsi)
    {
        if (fuelSpecies[fsi].rSign() == 1) 
        {
        i0Anode *= Foam::pow((pFuelPatch*xFuel[fsi]/pAtm), bElectrolyte.value());
        Info<< "min,mean,max (pf) = " << gMin(pFuelPatch) << "  "
        << gAverage(pFuelPatch) << "  " << gMax(pFuelPatch) << nl;
        Info<< "min,mean,max (xH2O) = " << gMin(xFuel[fsi]) << "  "
        << gAverage(xFuel[fsi]) << "  " << gMax(xFuel[fsi]) << nl;
        }
            // product ie. water.
        else if (fuelSpecies[fsi].rSign() == -1) 
        {
        i0Anode *= Foam::pow((pFuelPatch*xFuel[fsi]/pAtm), aElectrolyte.value());
        Info<< "min,mean,max (xH2) = " << gMin(xFuel[fsi]) << "  "
        << gAverage(xFuel[fsi]) << "  " << gMax(xFuel[fsi]) << nl;        
    }
            // reactant ie. hydrogen.
        }

    i0Anode *= gammaAnodeElectrolyte.value()*anodeT;
    i0Anode *= Foam::exp(-1.*EAnodeElectrolyte.value()/(Rgas*anodeT));
    Info<< "min,mean,max (anodeT) = " << gMin(anodeT) << "  "
        << gAverage(anodeT) << "  " << gMax(anodeT) << nl;
    Info<< "min,mean,max (i0An) = " << gMin(i0Anode) << "  "
        << gAverage(i0Anode) << "  " << gMax(i0Anode) << nl;

//    Compute anodic overpotential from inverse Butler-Volmer equation

    forAll (etaAnode, j) 
   {
      A = 2*alphaAnodeElectrolyte.value()*F/(Rgas*anodeT[j]);
      B = -2*(1-alphaAnodeElectrolyte.value())*F/(Rgas*anodeT[j]);
      testFunction tf(i[j], i0Anode[j], A , B);
      etaAnode[j] = RiddersRoot<testFunction>(tf, 1e-5).root(0, 1.2);
   }
    Info<< "min,mean,max BV(etaAn) = " << gMin(etaAnode) << "  "
        << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;


// Compute local cathode exchange current density
// SBB: Must add the air-side pressure
// SBB: Also Cathode overpotential computed with anode temperature

    forAll(airSpecies, fsi)
    {
        if (airSpecies[fsi].rSign() == -1) 
        i0Cathode *= Foam::pow(xAir[fsi], mElectrolyte.value());
    }

    i0Cathode *= gammaCathodeElectrolyte.value()*anodeT;
    i0Cathode *= Foam::exp(-1.*ECathodeElectrolyte.value()/(Rgas*anodeT));
    Info<< "min,mean,max (i0Ca) = " << gMin(i0Cathode) << "  "
        << gAverage(i0Cathode) << "  " << gMax(i0Cathode) << nl;

    forAll (etaCathode, j) 
   {
      A = 2*alphaCathodeElectrolyte.value()*F/(Rgas*anodeT[j]);
      B = -2*(1-alphaCathodeElectrolyte.value())*F/(Rgas*anodeT[j]);
      testFunction tf(i[j], i0Cathode[j], A , B);
      etaCathode[j] = RiddersRoot<testFunction>(tf, 1e-5).root(0, 1.2);
   }
    Info<< "min,mean,max BV(etaCa) = " << gMin(etaCathode) << "  "
        << gAverage(etaCathode) << "  " << gMax(etaCathode) << nl;
}
// End activation calculation

//  Save data for output

    scalarField& etaAIn = etaA.internalField();
    smearPatchToMesh smearetaA(electrolyteMesh, electrolyteAnodeName);
    etaAIn = smearetaA.smear(etaAnode);
    etaA.correctBoundaryConditions();
    etaA.boundaryField()[electrolyteAnodeID] == etaAnode;

    scalarField& etaCIn = etaC.internalField();
    smearPatchToMesh smearetaC(electrolyteMesh, electrolyteAnodeName);
    etaCIn = smearetaC.smear(etaCathode);
    etaC.correctBoundaryConditions();
    etaC.boundaryField()[electrolyteAnodeID] == etaCathode;



