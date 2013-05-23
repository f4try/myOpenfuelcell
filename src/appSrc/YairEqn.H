{
    Info << nl << "Solving air scalars" << endl;

    tmp<fv::convectionScheme<scalar> > yConvection
    (
        fv::convectionScheme<scalar>::New
        (
            airMesh,
            airYs,
            phiAir,
            airMesh.divScheme("div(phi,y)")
        )
    );


    label inertIndex = -1;
    volScalarField Yt("Yt", 0.0*Yair[0]);

    for (label i = 0; i < Yair.size(); i++)
    {
        Info << " Yair[i].name() = " << Yair[i].name() << nl;

        if (airSpecies[i].name() != airInertSpecie)
        {
            volScalarField& Yi = Yair[i];
            volScalarField& diffAirS = diffSpAir[i];

            solve
            (
                fvm::ddt(rhoAir, Yi)
              + yConvection->fvmDiv(phiAir, Yi)
              - fvm::laplacian(rhoAir*diffAirS, Yi, "laplacian(diff,y)"),
                airMesh.solver("Yi")
            );

            //Yi.max(0.0);
            Yi.max(Foam::doubleScalarSMALL);
            Yt += Yi;
        }
        else
        {
            inertIndex = i;
        }
    }

    Info<< "inertIndex = " << inertIndex << nl;
    Yair[inertIndex] = scalar(1) - Yt;
    //Yair[inertIndex].max(0.0);
    Yair[inertIndex].max(Foam::doubleScalarSMALL);

    forAll(Yair, i)
    {
        Info << "min,mean,max(" << Yair[i].name() << "): "
             << gMin(Yair[i]) <<" , "
             << gAverage(Yair[i]) <<" , "
             << gMax(Yair[i]) << endl;
    }
}

// calculate mole fractions
#include "getXair.H"

