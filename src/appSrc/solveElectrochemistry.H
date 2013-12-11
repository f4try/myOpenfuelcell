{
    Info << nl << "Solving electrochemistry" << endl;

    const scalar F = physicalConstant::F.value();         // C/mol
    const scalar FF = physicalConstant::FF.value();       // C/kmol
    const scalar Rgas = physicalConstant::Rgas.value();   // J/(mol K)

    const scalar Tr = 298.15;  //reference Temperature [K] for thermodynamics


// hkr 2013 May 24
// ---------------
    // initialize current density field
    static scalarField i(anodePatch.size());
    if(firstTime)
    {
        //i = ibar0.value();
        // idensity field is initialized with ibar0.
        // anode patch is later updated by i
        i = idensity.boundaryField()[anodeID];
    }


    // Interpolate global temperature to electrolyte/fuel interface
    // ------------------------------------------------------------
    scalarField anodeT(anodePatch.size());
    {
        surfaceScalarField Ts = fvc::interpolate(Tcell);

        forAll (anodeT, faceI)
        {
            anodeT[faceI] = Ts[anodeMask[faceI]];
        }
    }

    Info << "min,max(anodeT): "
         << Foam::gMin(anodeT) <<" , "  //S.Keuler, Bugfix for consistent output (parallel simulation) [two lines]
         << Foam::gMax(anodeT) << endl;


    // Interpolate cathode mole fractions of oxidant species to anode
    // --------------------------------------------------------------
    scalarField sumAYjOnMj    //sum(Yj/MWj)
    (
        Yair[0].boundaryField()[cathodeID].size(),
        0
    );

    forAll(Yair, j)
    {
        sumAYjOnMj += Yair[j].boundaryField()[cathodeID]/airSpecies[j].MW();
    }

    PtrList<scalarField> xAir(airSpecies.size());
    forAll(airSpecies, s)
    {
        if (airSpecies[s].rSign() == 0)    //reactant or product
        {
            xAir.set
            (
                s,
                new scalarField(anodeT.size(), 0)
            );

            xAir[s] =
                cathodeToAnode.faceInterpolate
                (
                    Yair[s].boundaryField()[cathodeID]/airSpecies[s].MW()
                    /sumAYjOnMj
                );

            // ensure positivity
            xAir[s] = Foam::max(xAir[s], Foam::doubleScalarSMALL);

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
        Yfuel[0].boundaryField()[anodeID].size(),
        0
    );

    forAll(Yfuel, j)
    {
        sumFYjOnMj += Yfuel[j].boundaryField()[anodeID]/fuelSpecies[j].MW();
    }

    PtrList<scalarField> xFuel(fuelSpecies.size());
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
                Yfuel[s].boundaryField()[cathodeID]/fuelSpecies[s].MW()
                /sumFYjOnMj
            );

            // ensure positivity
            xFuel[s] = Foam::max(xFuel[s], Foam::doubleScalarSMALL);

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


    // Calculate current density
    //scalarField i = (Nernst - V.value())/R;
    i = (Nernst - V.value())/R;

    // ensure i > 0
    i = Foam::max(i, Foam::doubleScalarSMALL);

    // idensity field provides visualization
    // and is used only for initialization
    #include "idensity.H"

    // mean current density
    dimensionedScalar ibar ("ibar", dimensionSet(0, -2, 0, 0, 0, 1, 0), 0.0);
    ibar.value() = Foam::gAverage(i); //S.Keuler, Bugfix: global average needed for parallel simulation

    Info << "min,mean,max(i): "
         << Foam::gMin(i) <<" , " << ibar.value() << " , " //S.Keuler, Bugfix for consistent output (parallel simulation) [two lines]
         << Foam::gMax(i) << endl;


    // voltage correction
    if (galvanostatic)
    {
        V += Rhat*(ibar - ibar0);

        Info << "ibar0 = " << ibar0.value()
             << "    ibar = " << ibar.value()
             << "    V = " << V.value() << endl;
    }
    else
    {
        Info << "    ibar = " << ibar.value()
             << "    V = " << V.value() << endl;
    }


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
                    diffSpAir[s].boundaryField()[cathodeID]
                    *rhoAir.boundaryField()[cathodeID]
                );

                // interface patchField
                volScalarField& Ys = Yair[s];
                fixedGradientFvPatchScalarField& YsBC =
                    refCast<fixedGradientFvPatchScalarField>
                    (
                        //Ys.boundaryField()[anodeID]
                        Ys.boundaryField()[cathodeID]
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
        Uair.boundaryField()[cathodeID] ==
        (
            -anodeToCathode.faceInterpolate(mfluxSum)
            /rhoAir.boundaryField()[cathodeID]
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
                    diffSpFuel[s].boundaryField()[anodeID]
                    *rhoFuel.boundaryField()[anodeID]
                );

                // interface patchField
                volScalarField& Ys = Yfuel[s];
                fixedGradientFvPatchScalarField& YsBC =
                    refCast<fixedGradientFvPatchScalarField>
                    (
                        Ys.boundaryField()[anodeID]
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
        Ufuel.boundaryField()[anodeID] ==
        (
            -(mfluxSum)
            /rhoFuel.boundaryField()[anodeID]
            *(fuelMesh.Sf().boundaryField()[anodeID])
            /(fuelMesh.magSf().boundaryField()[anodeID])
        );

        #include "H2utilization.H"
    }
}

