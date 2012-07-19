    //volumetric electrochemical Heating

    volScalarField electrochemicalHeating
    (
        IOobject
        (
            "electrochemicalHeating",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        HsrcElectrolyte,
        zeroGradientFvPatchScalarField::typeName
    );
