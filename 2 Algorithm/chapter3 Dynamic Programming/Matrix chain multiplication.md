***Algorithm MatrixChainMultiplication***

**Input:** Prameters of n matrixes stored in matrix A whose column i stands for the number of rows of the ird matrix in the chain and matrix B whose column i stands for the number of columns of the ird matrix in the chain.

**Output:** The minimun of times of total basic caculations.

    function MCM(A,B,n)
        for c from 0 to n-1
            for i from 1 to n
                j <- i + c

                if j > n
                    then break
                endif

                if j = i
                    then T[i,j] <- 0
                else if j = i + 1
                    then T[i,j] <- A[i] * B[j] * B[i] * (B[i] - 1)
                else
                    for s from i to j-1
                         T[i,j] <- min{ T[i,s] + T[s+1,j] + A[i]*B[j]*B[s]*(B[s]-1) }
                    endfor
               endif
               
            endfor
        endfor
        
        return T[1,n]
    endfunction

