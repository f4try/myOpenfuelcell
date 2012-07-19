// ------------------------------------------------------------------
// compute thickness of electrolyte as
//
//     (electrolyte volume)/(average electrode interface area)
//
// Using average area of both electrode interfaces accommodates both
// planar and cylindrical cells.
// ------------------------------------------------------------------

dimensionedScalar hE("hE", dimLength, 0);    //construct dimensioned thickness

{
    // anode side area
    scalar anodeSide = Foam::gSum
    (
        electrolyteMesh.magSf().boundaryField()[electrolyteAnodeID]
    );

    // cathode side area
    scalar cathodeSide = Foam::gSum
    (
        electrolyteMesh.magSf().boundaryField()[electrolyteCathodeID]
    );

    // volume
    scalar electrolyteVolume = Foam::gSum
    (
        electrolyteMesh.V()
    );

    // thickness value
    hE.value() = electrolyteVolume/(0.5*(anodeSide + cathodeSide));
}

//Info<< "    electrolyte thickness (calculated) hE = " << hE << nl << endl;
