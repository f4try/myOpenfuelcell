    Info<< nl << "Reading fuel sofcSpeciesProperties" << nl;

    // sofcSpecies info
    //-----------------
    IOdictionary fuelSpeciesDict
    (
        IOobject
        (
            "sofcSpeciesProperties",
            runTime.constant(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // species names
    // -------------
    speciesTable fuelSpeciesNames(fuelSpeciesDict.lookup("speciesList"));

    //forAll(fuelSpeciesNames, i)
    //{
    //    Info<< "fuelSpeciesNames[" << i << "] " << fuelSpeciesNames[i] << nl;
    //}


    // species data
    // ------------
    PtrList<sofcSpecie> fuelSpecies(fuelSpeciesNames.size());

    forAll(fuelSpeciesNames, i)
    {
        sofcSpecie tmp(fuelSpeciesDict.lookup(fuelSpeciesNames[i]));
        fuelSpecies.set
        (
            i,
            new sofcSpecie(tmp)
        );
    }


if (Pstream::myProcNo() == 0)
{
    // header
          //fuelSpecies[i] H2O        18.01534        2        1        -241834
    cout<< "               "
        << std::setiosflags(std::ios::left)
        << std::setw(8)
        << "name"
        << std::setw(16)
        << "MW[kg/kmol]"
        << std::setw(8)
        << "ne"
        << std::setw(8)
        << "rSign"
        << std::setw(12)
        << "hForm"
        << std::setw(8)
        << "sForm"
        << nl;

    forAll(fuelSpeciesNames, i)
    {
        cout<< "fuelSpecies[" << i    << "] "
            << std::setiosflags(std::ios::left)
            << std::setw(8)
            << fuelSpecies[i].name()
            << std::setw(16)
            << fuelSpecies[i].MW()
            << std::setw(8)
            << fuelSpecies[i].ne()
            << std::setw(8)
            << fuelSpecies[i].rSign()
            << std::setw(12)
            << fuelSpecies[i].hForm()
            << std::setw(8)
            << fuelSpecies[i].sForm()
            << nl;
    }
    Info << nl;
}


    // background specie (not necessarily chemically inert)
    // -----------------
    word fuelInertSpecie(fuelSpeciesDict.lookup("inertSofcSpecie"));
    Info << "fuelInertSpecie = " << fuelInertSpecie << nl << endl;

    if (!fuelSpeciesNames.contains(fuelInertSpecie))
    {
        FatalErrorIn(args.executable())
            << "Specified inert specie '"
            << fuelInertSpecie << "' not found in "
            << "species list. Available species:" << fuelSpeciesNames
            << exit(FatalError);
    }


    // toddYoung polynomials
    // from Todd&Young, J. Power Sources 110 (2002) pp 186-200

    // molar Cp polynomials
    // --------------------
    dictionary& tyDictFuelCp = fuelSpeciesDict.subDict("CpCoeffs");
    PtrList<polyToddYoung> molarCpFuel(fuelSpecies.size());
    forAll(fuelSpecies, i)
    {
        polyToddYoung tmp(tyDictFuelCp.lookup(fuelSpecies[i].name()));
        molarCpFuel.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "CpCoeffs(" << fuelSpecies[i].name() << ") ";
        molarCpFuel[i].writeData();
    }
    Info<< endl;

    // mu polynomials
    // --------------
    dictionary& tyDictFuelMu = fuelSpeciesDict.subDict("muCoeffs");
    PtrList<polyToddYoung> muFuelL(fuelSpecies.size());
    forAll(fuelSpecies, i)
    {
        polyToddYoung tmp(tyDictFuelMu.lookup(fuelSpecies[i].name()));
        muFuelL.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "muCoeffs(" << fuelSpecies[i].name() << ") ";
        muFuelL[i].writeData();
    }
    Info<< endl;

    // k polynomials
    // --------------
    dictionary& tyDictFuelK = fuelSpeciesDict.subDict("kCoeffs");
    PtrList<polyToddYoung> kFuelL(fuelSpecies.size());
    forAll(fuelSpecies, i)
    {
        polyToddYoung tmp(tyDictFuelK.lookup(fuelSpecies[i].name()));
        kFuelL.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "kCoeffs(" << fuelSpecies[i].name() << ") ";
        kFuelL[i].writeData();
    }
    Info<< endl;

    Info<< "ToddYoung checks" << nl;
    forAll(fuelSpecies, i)
    {
        Info<< fuelSpecies[i].name() << nl;
        Info<< "    Cp(1473) = " << molarCpFuel[i].polyVal(1473) << nl;
        Info<< "    mu(1473) = " << muFuelL[i].polyVal(1473) << nl;
        Info<< "    k(1473)  = " << kFuelL[i].polyVal(1473) << nl;
    }
    Info<< nl;


    // mass fraction fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fuelYs;
    PtrList<volScalarField> Yfuel(fuelSpecies.size());

    forAll(fuelSpecies, i)
    {
        Yfuel.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Y" + fuelSpecies[i].name(),
                    runTime.timeName(),
                    fuelMesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fuelMesh
            )
        );
        fuelYs.add(Yfuel[i]);
    }

    // initialize sum Y[i]/MW[i] field for X[i] calclations
    // ----------------------------------------------------
    volScalarField sumFuelYIonMI ("YoM", 0.0*Yfuel[0]);


    // mole fraction fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fuelXs;
    PtrList<volScalarField> Xfuel(fuelSpecies.size());

    forAll(fuelSpecies, i)
    {
        Xfuel.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "X" + fuelSpecies[i].name(),
                    runTime.timeName(),
                    fuelMesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                fuelMesh,
                dimensionedScalar("x", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
                zeroGradientFvPatchScalarField::typeName
            )
        );
        fuelXs.add(Xfuel[i]);
    }

    // calculate X fields
    // -----------------
    #include "getXfuel.H"


    // diffusivity fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fuelDiffs;
    PtrList<volScalarField> diffSpFuel(fuelSpecies.size());

    forAll(fuelSpecies, i)
    {
        diffSpFuel.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "diff" + fuelSpecies[i].name(),
                    runTime.timeName(),
                    fuelMesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                fuelMesh,
                dimensionedScalar("d", dimensionSet(0, 2, -1, 0, 0, 0, 0), 1),
                zeroGradientFvPatchScalarField::typeName
            )
        );
        fuelDiffs.add(diffSpFuel[i]);
    }


    // activation terms
    // ------------------------------
    autoPtr<activationOverpotentialModel> fuelActivationOverpotential
    (
        activationOverpotentialModel::New(fuelSpeciesNames, fuelSpeciesDict.subDict("activationParameters"))
    );
