***Algorithm Decrypt***

**Input**: string $A$ and its length integer $n$

**Output**: code of $A$ in the dictionary

**Strings whose codes need to be stored first**:
"$\alpha$ z", 
"$\alpha$ yz", 
"$\alpha$ xyz", 
"$\alpha$ wxyz", 
"$\alpha$ vwxyz",

where $\alpha$ represents the character that can form string that satisfies requirement.

	function Decrypt(A, n)
		for i from n to 1
			if A[i] = A[i-1]
				then A[i-1] <-- A[i-1] - 1
			endif
		endfor

		if n = 1
			then the ASCII code of A
		or if A has been stored
			return stored code of A
		else
			return Decrypt("z"*(n-1), n-1) + Decrypt((A[1]-1)+"z"*(n-1), n) 
				   + the ASCII code of A[n] - the ASCII code of A[n-1]
		endif
	endfunction