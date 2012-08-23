    Info<< nl << "Reading rxnProperties" << nl;

    // rxnSpecies info
    //-----------------
    IOdictionary rxnDict
    (
        IOobject
        (
            "rxnProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    HashTable<scalar,word> rxnSpCoef(rxnDict.lookup("rxnSpecies"));

    Info << "rxnSpCoef contents: " << rxnSpCoef.toc() << nl;

    Info << "rxnSpecies stoichiometric coefficients: " << nl;
    //Info << "stoichCoeff O2  = " << rxnSpCoef["O2"] << nl;
    //Info << "stoichCoeff H2  = " << rxnSpCoef["H2"] << nl;
    //Info << "stoichCoeff H2O = " << rxnSpCoef["H2O"] << nl;
    //Info << "stoichCoeff e   = " << rxnSpCoef["e"] << nl;
    //Info << nl;


    forAll(airSpecies, i)
    {
        if(rxnSpCoef.found(airSpecies[i].name()))
        {
            Info << "stoichCoeff "
                 << airSpecies[i].name()
                 << " = " 
                 << rxnSpCoef[airSpecies[i].name()]
            << nl;
        }
    }

    forAll(fuelSpecies, i)
    {
        if(rxnSpCoef.found(fuelSpecies[i].name()))
        {
            Info << "stoichCoeff "
                 << fuelSpecies[i].name()
                 << " = " 
                 << rxnSpCoef[fuelSpecies[i].name()]
            << nl;
        }
    }

    if(rxnSpCoef.found("e"))
    {
        Info << "stoichCoeff e = " << rxnSpCoef["e"] << nl;
    }

    Info << endl;

