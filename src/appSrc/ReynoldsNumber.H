// RIB geometry
//a=2.0 mm= 2.0e-3 m
//b=1.5 mm= 1.5e-3 m
//double a=2.0e-3;
//double b=1.5e-3;

// Reynolds number @ airInlet
{ 
    label inletID  = airMesh.boundaryMesh().findPatchID("airInlet");
//    double x[3],y[3],z[3],epsx=1.e-9;
//    const pointField& bcPoint = airMesh.boundaryMesh()[inletID].localPoints();
//    x[1]= x[2]= bcPoint[0][0]; 
//    y[1]= y[2]= bcPoint[0][1];
//    z[1]= z[2]= bcPoint[0][2]; 
//    forAll(bcPoint,pointI)
//    { 
//        x[1]= min(bcPoint[pointI][0],x[1]); 
//        x[2]= max(bcPoint[pointI][0],x[2]);
//        y[1]= min(bcPoint[pointI][1],y[1]); 
//        y[2]= max(bcPoint[pointI][1],y[2]);
//        z[1]= min(bcPoint[pointI][2],z[1]); 
//        z[2]= max(bcPoint[pointI][2],z[2]);
//    }
//    x[0]= Foam::mag(x[1]-x[2]); 
//    y[0]= Foam::mag(y[1]-y[2]);
//    z[0]= Foam::mag(z[1]-z[2]);
//    if (fabs(x[0]-0.0)<epsx)
//        { a=y[0]; b=z[0]; }
//    else if (fabs(y[0]-0.0)<epsx)
//        { a=z[0]; b=x[0]; }
//    else if (fabs(z[0]-0.0)<epsx)
//        { a=x[0]; b=y[0]; }
//    scalar DH= 2.*a*b/(a+b);
//    scalar ReAir= DH*Foam::mag

    scalar ReAir= dHydAir.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoAir.boundaryField()[inletID]
            *Uair.boundaryField()[inletID]
            /muAir.boundaryField()[inletID]
        )
    );
     
    Info<< "ReAir  = "<<ReAir<<endl;
}

// Reynolds number @ fuelInlet
{
    label inletID  = fuelMesh.boundaryMesh().findPatchID("fuelInlet");
//    const pointField& bcPoint = fuelMesh.boundaryMesh()[inletID].localPoints();
//    double x[3],y[3],z[3],epsx=1.e-9;
//    x[1]= x[2]= bcPoint[0][0]; 
//    y[1]= y[2]= bcPoint[0][1];
//    z[1]= z[2]= bcPoint[0][2]; 
//    forAll(bcPoint,pointI)i
//    { 
//        x[1]= min(bcPoint[pointI][0],x[1]); 
//        x[2]= max(bcPoint[pointI][0],x[2]);
//        y[1]= min(bcPoint[pointI][1],y[1]); 
//        y[2]= max(bcPoint[pointI][1],y[2]);
//        z[1]= min(bcPoint[pointI][2],z[1]); 
//        z[2]= max(bcPoint[pointI][2],z[2]);
//    }
//    x[0]= Foam::mag(x[1]-x[2]);
//    y[0]= Foam::mag(y[1]-y[2]);
//    z[0]= Foam::mag(z[1]-z[2]);
//    if (fabs(x[0]-0.0)<epsx)
//        { a=y[0]; b=z[0]; }
//    else if (fabs(y[0]-0.0)<epsx)
//        { a=z[0]; b=x[0]; }
//    else if (fabs(z[0]-0.0)<epsx)
//        { a=x[0]; b=y[0]; }
//    scalar DH= 2.*a*b/(a+b);
//    scalar ReFuel= DH*Foam::mag

    scalar ReFuel= dHydFuel.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoFuel.boundaryField()[inletID]
            *Ufuel.boundaryField()[inletID]
            /muFuel.boundaryField()[inletID]
        )
    );

    Info<< "ReFuel = "<<ReFuel<<endl;
}
