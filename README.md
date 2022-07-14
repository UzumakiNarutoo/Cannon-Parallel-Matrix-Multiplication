Cannon's Algorithm For Parallel Matrix Multiplication
*The program starts asking to get the input ( the 2 matrices ).
*The algorithms works correctly if and only if :
(1) The number of rows in each matrix equals to the number of coulmns.
(2) The 2 matrices should have the same number rows and the same number of
coulmns.
(3) The number of processors equals to the number of cells in the matrix (which is equals
to the number of rows ^ 2).
* First the algorithms checks that everything will work correctly.
* After that the algorithms starts working in parallel.
* Each processor is responsable to get the answer of only 1 cell and we will get the result matrix
by merging all the cells together.
* Let resi at some processor i will store the answer of the cell that processor i is responsable to
solve.
*Let vala at some processor i equals to the current value at the cell that processor i is
responsable to solve at matrix a.
*Let valb at some processor i equals to the current value at the cell that processor i is
responsable to solve at matrix b.
* The algorithm do the folowing steps n-1 times(where n is the legnth of the matrix):
(1) For each processor i the processor will send vala to it's left processor and recieve
another vala from it's right processor.
(2) For each processor i the processor will send vala to it's up processor and recieve
another valb from it's down processor.
(3) resi += vala * valb
