function newMean = UpdateMean (OldMean, NewDataValue, n)
    newMean=OldMean + NewDataValue/n;
end

function newMedian = UpdateMedian (oldMedian, NewDataValue, A, n)
    if (mod(n,2)==0)
        if (NewDataValue<=A(n/2))
            newMedian = A(n/2);
        elseif (NewDataValue>=A(n/2+1))
            newMedian = A(n/2+1);
        else
            newMedian = NewDataValue;
        end
    else
        if (NewDataValue < A((n-1)/2))
            newMedian = (oldMedian + A((n-1)/2))/2;
        elseif (NewDataValue > A((n+3)/2))
            newMedian = (oldMedian + A((n+3)/2))/2;
        else
            newMedian = (NewDataValue+oldMedian)/2;
        end
    end
end


function newStd = UpdateStd (OldMean, OldStd, NewMean, NewDataValue, n)
    newStd = sqrt(OldStd^2+OldMean^2-NewMean^2+(NewDataValue^2)/n);
end