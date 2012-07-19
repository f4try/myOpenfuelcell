{
    // integrate heat flux over all patches
    
    scalar qVect = 0;      //patch integral of convective flux
    scalar qVectSum = 0;   //accumulator over all patches

    scalar qDuct = 0;      //patch integral of conductive flux
    scalar qDuctSum = 0;   //accumulator over all patches
    
    const polyPatchList& patches = mesh.boundaryMesh();

    forAll(patches, patchI)
    {
        // Int(rho Cp U & dA)
        qVect = gSum
	(
	    rhoCpPhiCell.boundaryField()[patchI]
	    *Tcell.boundaryField()[patchI]
	);
        qVectSum += qVect;

        // Int(k gradT $ dA)
        qDuct = gSum
        (
            kCell.boundaryField()[patchI]
            *Tcell.boundaryField()[patchI].snGrad()
            *mesh.magSf().boundaryField()[patchI]
        );
        qDuctSum += qDuct;
        
        if((qVect != 0) || (qDuct != 0))    //report non-zero values
        {
            Info<< patches[patchI].name()
                << " sum(rhoCpPhiT) = " << qVect
                << "    sum(kGradTdA) = " << qDuct
                << nl;
        }
    }
    Info<< "qVectSum = " << qVectSum 
        << "    qDuctSum = " << qDuctSum
        << nl << endl;


    // electrolyte heating
    scalar qSrcSum = gSum
    (
        TsourceCell.internalField()*mesh.V()
    );
    Info<< " sum(TsourceCell dV) = " << qSrcSum << nl << endl;
}

