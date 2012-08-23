    Info<< nl << "Reading air sofcSpeciesProperties" << nl;

    // sofcSpecies info
    //-----------------
    IOdictionary airSpeciesDict
    (
        IOobject
        (
            "sofcSpeciesProperties",
            runTime.constant(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // species names
    // -------------
    speciesTable airSpeciesNames(airSpeciesDict.lookup("speciesList"));
    forAll(airSpeciesNames, i)
    {
        Info<< "airSpeciesNames[" << i << "] " << airSpeciesNames[i] << nl;
    }


    // species data
    // ------------
          //airSpecies[0] O2        31.9988        4        -1        0
    cout<< "              "
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

    PtrList<sofcSpecie> airSpecies(airSpeciesNames.size());

    forAll(airSpeciesNames, i)
    {
        sofcSpecie tmp(airSpeciesDict.lookup(airSpeciesNames[i]));
        airSpecies.set
        (
            i,
            new sofcSpecie(tmp)
        );

        cout<< "airSpecies[" << i    << "] "
            << std::setiosflags(std::ios::left)
            << std::setw(8)
            << airSpecies[i].name()
            << std::setw(16)
            << airSpecies[i].MW()
            << std::setw(8)
            << airSpecies[i].ne()
            << std::setw(8)
            << airSpecies[i].rSign()
            << std::setw(12)
            << airSpecies[i].hForm()
            << std::setw(8)
            << airSpecies[i].sForm()
            << nl;
    }
    Info << nl;


    // background specie for mass fraction calculations
    // -----------------
    // labelled inertSpecie but NOT necessarily chemically inert)
    //
    word airInertSpecie(airSpeciesDict.lookup("inertSofcSpecie"));
    Info << "airInertSpecie = " << airInertSpecie << nl << endl;

    if (!airSpeciesNames.contains(airInertSpecie))
    {
        FatalErrorIn(args.executable())
            << "Specified inert specie '" << airInertSpecie << "' not found in "
            << "species list. Available species:" << airSpeciesNames
            << exit(FatalError);
    }


    // toddYoung molar Cp polynomials
    // ------------------------------
    dictionary& tyDictAir = airSpeciesDict.subDict("toddYoung");
    PtrList<polyToddYoung> molarCpAir(airSpecies.size());
    forAll(airSpecies, i)
    {
        polyToddYoung tmp(tyDictAir.lookup(airSpecies[i].name()));
        molarCpAir.set
        (
            i,
            new polyToddYoung(tmp)
        );
        Info<< "CpCoeffs(" << airSpecies[i].name() << ") ";
        molarCpAir[i].writeData();
    }
    Info<< endl;


    // mass fraction fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airYs;
    PtrList<volScalarField> Yair(airSpecies.size());

    forAll(airSpecies, i)
    {
        Yair.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Y" + airSpecies[i].name(),
                    runTime.timeName(),
                    airMesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                airMesh
            )
        );
        airYs.add(Yair[i]);
    }

    // initialize sum Y[i]/MW[i] field for X[i] calclations
    // ----------------------------------------------------
    volScalarField sumAirYIonMI ("YoM", 0.0*Yair[0]);


    // mole fraction fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airXs;
    PtrList<volScalarField> Xair(airSpecies.size());

    forAll(airSpecies, i)
    {
        Xair.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "X" + airSpecies[i].name(),
                    runTime.timeName(),
                    airMesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                airMesh,
                dimensionedScalar("x", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
                zeroGradientFvPatchScalarField::typeName
            )
        );
        airXs.add(Xair[i]);
    }

    // calculate X fields
    // -----------------
    #include "getXair.H"


    // diffusivity fields and files
    // ------------------------------
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airDiffs;
    PtrList<volScalarField> diffSpAir(airSpecies.size());

    forAll(airSpecies, i)
    {
        diffSpAir.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "diff" + airSpecies[i].name(),
                    runTime.timeName(),
                    airMesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                airMesh,
                dimensionedScalar("d", dimensionSet(0, 2, -1, 0, 0, 0, 0), 1),
                zeroGradientFvPatchScalarField::typeName
            )
        );
        airDiffs.add(diffSpAir[i]);
    }

