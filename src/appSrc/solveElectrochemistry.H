{
    Info << nl << "Solving electrochemistry" << endl;

    const scalar F = physicalConstant::F.value();         // C/mol
    const scalar FF = physicalConstant::FF.value();       // C/kmol
    const scalar Rgas = physicalConstant::Rgas.value();   // J/(mol K)
    const scalar pAtm = physicalConstant::pAtm.value();   // Pa - added SBB 06-12-2013

    const scalar Tr = 298.15;  //reference Temperature [K] for thermodynamics
    scalarField i(anodePatch.size());    
    scalarField iOld(cathodePatch.size());    
    scalarField mfluxOld(cathodePatch.size());    
    scalarField iNew(cathodePatch.size());    

    // Interpolate global temperature to electrolyte/fuel interface
    // ------------------------------------------------------------
    scalarField anodeT(electrolyteAnodePatch.size());
    {
        surfaceScalarField Ts = fvc::interpolate(Tcell);

        forAll (anodeT, faceI)
        {
            anodeT[faceI] = Ts[electrolyteAnodeMask[faceI]];
        }
    }

    Info << "min,max(anodeT): "
         << Foam::gMin(anodeT) <<" , "  //S.Keuler, Bugfix for consistent output (parallel simulation) [two lines]
         << Foam::gMax(anodeT) << endl;

    // Interpolate global temperature to electrolyte/air interface
    // ------------------------------------------------------------
    scalarField cathodeT(electrolyteCathodePatch.size());
    {
        surfaceScalarField Ts = fvc::interpolate(Tcell);

        forAll (cathodeT, faceI)
        {
            cathodeT[faceI] = Ts[electrolyteCathodeMask[faceI]];
        }
    }

    Info << "min,max(cathodeT): "
         << Foam::gMin(cathodeT) <<" , " 
         << Foam::gMax(cathodeT) << endl;


    // Interpolate cathode mole fractions of oxidant species to anode
    // --------------------------------------------------------------
    scalarField sumAYjOnMj    //sum(Yj/MWj)
    (
        Yair[0].boundaryFieldRef()[cathodeID].size(),
        0
    );

    forAll(Yair, j)
    {
        sumAYjOnMj += Yair[j].boundaryFieldRef()[cathodeID]/airSpecies[j].MW();
    }

    PtrList<scalarField> xAir(airSpecies.size());
    const scalarField& pAirPatch = pAir.boundaryFieldRef()[cathodeID]; //added Qing 04.06.2014
    forAll(airSpecies, s)
    {
        if (airSpecies[s].rSign() != 0)    //reactant or product (S.Keuler, corrected 30.01.2014)
        {
            xAir.set
            (
                s,
                new scalarField(anodeT.size(), 0)
            );

            xAir[s] =
                cathodeToAnode.faceInterpolate
                (
                    Yair[s].boundaryFieldRef()[cathodeID]/airSpecies[s].MW()
                    /sumAYjOnMj
                );

            // ensure positivity
            xAir[s] = Foam::max(xAir[s], Foam::doubleScalarSmall);

            Info << "min,mean,max(X" << airSpecies[s].name() << "): "
                 << Foam::gMin(xAir[s]) <<" , "
                 << Foam::gAverage(xAir[s]) <<" , "
                 << Foam::gMax(xAir[s]) << endl;
        }
    }


    // Calculate anode mole fractions of fuel reactant and product species
    // -------------------------------------------------------------------
    scalarField sumFYjOnMj    //sum(Yj/MWj)
    (
        Yfuel[0].boundaryFieldRef()[anodeID].size(),
        0
    );

    forAll(Yfuel, j)
    {
      sumFYjOnMj += Yfuel[j].boundaryFieldRef()[anodeID]/fuelSpecies[j].MW();
    }

    PtrList<scalarField> xFuel(fuelSpecies.size());
    const scalarField& pFuelPatch = pFuel.boundaryFieldRef()[anodeID]; //added hj 03-09-2013
    forAll(fuelSpecies,s)
    {
        if(fuelSpecies[s].rSign() != 0)    //reactant or product
        {
            xFuel.set
            (
                s,
                new scalarField(anodeT.size(), 0)
            );

            xFuel[s] =
            (
                Yfuel[s].boundaryFieldRef()[cathodeID]/fuelSpecies[s].MW()
                /sumFYjOnMj
            );

            // ensure positivity
            xFuel[s] = Foam::max(xFuel[s], Foam::doubleScalarSmall);

            Info << "min,mean,max(X" << fuelSpecies[s].name() << "): "
                 << Foam::gMin(xFuel[s]) <<" , "
                 << Foam::gAverage(xFuel[s]) <<" , "
                 << Foam::gMax(xFuel[s]) << endl;
        }
    }


    // Calculate Nernst potential
    #include "NernstEqn.H"

    // area specific resistance, R
    // scalarField R is based on anodeT
    #include "ASRfunction.H"

    // Takes voltage from cell center of cell adjacent to boundary face and applies it at the face   
    forAll(electrolyteAnodePatch, facei)
    {
	voltage.boundaryFieldRef()[electrolyteAnodeID][facei] = voltage[electrolyteAnodePatch.faceCells()[facei]];
    }
        // SBB Get the interface velocity condition and set the mass flux and the "old" current density
      mfluxOld =
//     (phiAir.boundaryField()[cathodeID]) SBB: Careful with the [cathodeID]
//     /(airMesh.magSf().boundaryField()[cathodeID])
      -rhoAir.boundaryField()[cathodeID]*(
      (Uair.boundaryField()[cathodeID]
      & airMesh.Sf().boundaryField()[cathodeID])
      /airMesh.magSf().boundaryField()[cathodeID]
        );
    iOld = -4.*FF*mfluxOld/32;//Magic numbers - Total mass flow is that of O2
//    iOld = 2*FF*mfluxOld/2.01594;//Magic numbers - Total mass flow is that of H2
    if (runTime.value() <= 1) iOld = ibar0.value();//First time step


    // Solve Current
    // Add etaA and etaC into the current function after the pre-iteration, Qing, 22.06.2014
//    scalarField i(anodePatch.size());    

        if (runTime.value() > PreIterAct.value())
	{
	    iNew = (Nernst - voltage.boundaryField()[electrolyteAnodeID] - etaA.boundaryField()[electrolyteAnodeID] - etaC.boundaryField()[electrolyteAnodeID])/R;
	}
	else
	{
	 iNew = (Nernst - voltage.boundaryField()[electrolyteAnodeID])/R;
	}
            Info << "min,mean,max(iNew" << "): "
                  << Foam::gMin(iNew) <<" , "
                  << Foam::gAverage(iNew) <<" , "
                 << Foam::gMax(iNew) << endl;        
            Info << "min,mean,max(iOld" << "): "
                  << Foam::gMin(iOld) <<" , "
                  << Foam::gAverage(iOld) <<" , "
                 << Foam::gMax(iNew) << endl;        
    i = relax.value()*iNew + (1-relax.value())*iOld;//Relax current density	
    iOld = i;
	
    // ensure i > 0
    i = Foam::max(i, Foam::doubleScalarSmall);

    // Activation overpotential - added SBB 04-09-2013
    // scalarField etaAnode is computed from an inverse Butler-Volmer eqn.
  	
    if (runTime.value() > PreIterAct.value()){
    #include "activationOverpotential.H"
}

    // Smear boundary current and Nernst potential over electrolyte volume
    #include "idensity.H"
    
    // Calculate mean current density
    #include "solveCurrent.H"

    // mean current density
    Info << "min,mean,max(current): "
         << gMin(idensity.primitiveFieldRef()) << " , "
         << gAverage(idensity.primitiveFieldRef()) << " , "
         << gMax(idensity.primitiveFieldRef()) << endl;



    // Calculate electrochemical heating
    #include "electrochemicalHeating.H"


    // Calculate species sources and sinks and set
    // boundary conditions for mass fractions and velocity
    // at fluid/electrode interfaces

    // Air side
    {
        Info<< nl << "air species electrochemical fluxes and YEqn BCs" << nl;

        label nSpec = airSpecies.size();       //number of species
        List<scalarField> mflux(nSpec);        //mass flux by species
        List<Switch> isFlux(nSpec, "false");   //true if species contributes
        scalarField mfluxSum(i.size(),0);      //sum over species

        // mass fluxes for electrochemically active species
        forAll(airSpecies, s)
        {
            if(airSpecies[s].ne() != 0)
            {
                mflux[s] =
                (
                    airSpecies[s].rSign()*airSpecies[s].MW()
                    *i/(FF*airSpecies[s].ne())
                );
                mfluxSum += mflux[s];
                isFlux[s] = "true";
                //Info<< s << " mflux[s] " << Foam::gSum(mflux[s]) << nl;
            }
            Info<< "s species[] isFlux[] mflux[] = "
                << s << " " << airSpecies[s].name() << " " << isFlux[s] << " "
                << Foam::gSum(mflux[s]) << nl;
        }
        Info<< endl;


        // mass fraction BCs for all species except the "inert" specie
        // (inert specie mass fraction is 1-complement of all others)
        forAll(Yair, s)
        {
            //Info << airSpecies[s].name() << nl;

            if(airSpecies[s].name() != airInertSpecie)
            {
                scalarField gammaCathodeS =
                (
                    diffSpAir[s].boundaryFieldRef()[cathodeID]
                    *rhoAir.boundaryFieldRef()[cathodeID]
                );

                // interface patchField
                volScalarField& Ys = Yair[s];
                fixedGradientFvPatchScalarField& YsBC =
                    refCast<fixedGradientFvPatchScalarField>
                    (
                        //Ys.boundaryFieldRef()[anodeID]
                        Ys.boundaryFieldRef()[cathodeID]
                    );

                // gradient boundary condition
                // note:  air mass fluxes defined on anode patch
                //        -> we interpolate to cathode patch

                // initialize gradient
                YsBC.gradient() =
                    anodeToCathode.faceInterpolate(mflux[s])*(1.0 - YsBC);

                // add changes due to other species
                forAll(Yair, t)
                {
                    //Info<< "s, t, isFlux[t] = " << s << t << isFlux[t] << nl;

                    if((t != s) && isFlux[t])
                    {
                        //Info<< t << " mflux[t] "
                        //    << Foam::gSum(mflux[t]) << nl;
                        YsBC.gradient() -=
                        anodeToCathode.faceInterpolate(mflux[t])*YsBC;
                    }
                }
                YsBC.gradient() /= gammaCathodeS;
            }
        }
        Info<< endl;


        // Set the interface velocity condition
        Uair.boundaryFieldRef()[cathodeID] ==
        (
            -anodeToCathode.faceInterpolate(mfluxSum)
            /rhoAir.boundaryFieldRef()[cathodeID]
            *(airMesh.Sf().boundaryField()[cathodeID])
            /(airMesh.magSf().boundaryField()[cathodeID])
        );

        #include "O2utilization.H"
    }


    // Fuel side
    {
        Info<< "fuel species electrochemical fluxes and YEqn BCs" << nl;

        label nSpec = fuelSpecies.size();
        List<scalarField> mflux(nSpec);
        List<Switch> isFlux(nSpec, "false");
        scalarField mfluxSum(i.size(),0);

        // mass fluxes for electrochemically active species
        forAll(fuelSpecies, s)
        {
            if(fuelSpecies[s].ne() != 0)
            {
                mflux[s] =
                (
                    fuelSpecies[s].rSign()*fuelSpecies[s].MW()
                    *i/(FF*fuelSpecies[s].ne())
                );
                mfluxSum += mflux[s];
                isFlux[s] = "true";
                //Info<< s << " mflux[s] " << Foam::gSum(mflux[s]) << nl;
            }
            Info<< "s species[] isFlux[] mflux[] = "
                << s << " " << fuelSpecies[s].name() << " " << isFlux[s] << " "
                << Foam::gSum(mflux[s]) << nl;
        }
        Info<< endl;


        // mass fraction BCs for all species except the "inert" specie
        // (inert specie mass fraction is 1-complement of all others)
        forAll(Yfuel, s)
        {
            //Info<< fuelSpecies[s].name() << nl;

            if(fuelSpecies[s].name() != fuelInertSpecie)
            {
                scalarField gammaAnodeS =
                (
                    diffSpFuel[s].boundaryFieldRef()[anodeID]
                    *rhoFuel.boundaryFieldRef()[anodeID]
                );

                // interface patchField
                volScalarField& Ys = Yfuel[s];
                fixedGradientFvPatchScalarField& YsBC =
                    refCast<fixedGradientFvPatchScalarField>
                    (
                        Ys.boundaryFieldRef()[anodeID]
                    );

                // gradient boundary condition
                // initialize gradient
                YsBC.gradient() = mflux[s]*(1.0 - YsBC);

                // add changes due to other species
                forAll(Yfuel, t)
                {
                    //Info<< "s, t, isFlux[t] = " << s << t << isFlux[t] << nl;
                    if((t != s) && isFlux[t])
                    {
                        //Info<< t << " mflux[t] "
                        //    << Foam::gSum(mflux[t]) << nl;
                        YsBC.gradient() -= YsBC*mflux[t];
                    }
                }
                YsBC.gradient() /= gammaAnodeS;
            }
        }
        Info<< endl;


        // Set the interface velocity condition
        Ufuel.boundaryFieldRef()[anodeID] ==
        (
            -(mfluxSum)
            /rhoFuel.boundaryFieldRef()[anodeID]
            *(fuelMesh.Sf().boundaryField()[anodeID])
            /(fuelMesh.magSf().boundaryField()[anodeID])
        );

        #include "H2utilization.H"
    }
}

