***Algorithm MatrixChainMultiplication***

**Input:** Prameters of n matrixes stored in matrix A whose column i stands for the number of rows of the ird matrix in the chain and matrix B whose column i stands for the number of columns of the ird matrix in the chain.

**Output:** The minimun of times of total basic caculations.

    function MCM(A,B,n)
        for i from 1 to n-1
            for j from i to n
                if j = i
                    then T[i,j] <- 0
                else if j = i + 1
                    then T[i,j] <- A[i] * B[j] * B[i] * (B[i] - 1)
                endif

                for s from i to j-1
                    T[i,j] <- min{ T[i,s] + T[s+1,j] + A[i]*B[j]*B[s]*(B[s]-1) }
                endfor
            endfor
        endfor
        return T[1,n]
    endfunction

