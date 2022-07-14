#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<iostream>

using namespace std;

const int master = 0;

int n, *a, *b, *res;
int vala, valb, valres;

bool input(){
	int na, ma, nb, mb;

	cout << "\nThe first Matrix :\n";
	cout << "Please Enter the number of rows : "; cin >> na;
	cout << "Please Enter the number of coulmns : "; cin >> ma;
	a = (int*)calloc(sizeof(int), na*ma);
	cout << "Please Enter " << na*ma << " integers (the matrix) :\n";
	for (int i = 0; i < na; ++i) for (int j = 0; j < ma; ++j) cin >> a[i*ma + j];

	cout << "\nThe second Matrix :\n";
	cout << "Please Enter the number of rows : "; cin >> nb;
	cout << "Please Enter the number of coulmns : "; cin >> mb;
	b = (int*)calloc(sizeof(int), nb*mb);
	cout << "Please Enter " << nb*mb << " integers (the matrix) :\n";
	for (int i = 0; i < nb; ++i) for (int j = 0; j < mb; ++j) cin >> b[i*mb + j];

	if (na != nb || ma != mb){
		cout << "Wrong input : The 2 matrices should have the same sides (rows/coulmns).\n";
		return 0;
	}
	if (na != ma){
		cout << "Wrong input : The 2 matrices should be squares( The number of rows should equal to the number of culmns.\n";
		return 0;
	}
	n = na;
	return 1;
}

int main(int argc, char *argv[]){
	int size, rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == master){
		if (!input()){
			cout << "Terminating.....\n";
			MPI_Finalize();
			return 0;
		}
		if (size != n*n){
			cout << "The number of processors should be equal to the number of each matrix cells which equals to " << n*n << endl;
			cout << "Terminating.....\n";
			MPI_Finalize();
			return 0;
		}
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Comm cart_comm;
	int left = 0, right = 0, up = 0, down = 0;
	int mov[2], side[2], coordinates[2];
	mov[0] = mov[1] = 1, side[0] = side[1] = n;

	MPI_Cart_create(MPI_COMM_WORLD, 2, side, mov, 1, &cart_comm);
	MPI_Scatter(a, 1, MPI_INT, &vala, 1, MPI_INT, 0, cart_comm);
	MPI_Scatter(b, 1, MPI_INT, &valb, 1, MPI_INT, 0, cart_comm);
	MPI_Comm_rank(cart_comm, &rank);

	MPI_Cart_coords(cart_comm, rank, 2, coordinates);

	MPI_Cart_shift(cart_comm, 1, coordinates[0], &left, &right);
	MPI_Sendrecv_replace(&vala, 1, MPI_INT, left, 11, right, 11, cart_comm, MPI_STATUS_IGNORE);

	MPI_Cart_shift(cart_comm, 0, coordinates[1], &up, &down);
	MPI_Sendrecv_replace(&valb, 1, MPI_INT, up, 11, down, 11, cart_comm, MPI_STATUS_IGNORE);

	valres += vala * valb;
	for (int i = 1; i < n; ++i){
		MPI_Cart_shift(cart_comm, 1, 1, &left, &right);
		MPI_Sendrecv_replace(&vala, 1, MPI_INT, left, 11, right, 11, cart_comm, MPI_STATUS_IGNORE);
		MPI_Cart_shift(cart_comm, 0, 1, &up, &down);
		MPI_Sendrecv_replace(&valb, 1, MPI_INT, up, 11, down, 11, cart_comm, MPI_STATUS_IGNORE);
		valres += vala * valb;
	}

	res = (int*)calloc(sizeof(int), n*n);
	MPI_Gather(&valres, 1, MPI_INT, res, 1, MPI_INT, 0, cart_comm);

	if (rank == master){
		cout << "\nThe result matrix is :\n";
		for (int i = 0; i < n*n; ++i){
			cout << res[i];
			if ((i + 1) % n == 0) cout << "\n";
			else cout << " ";
		}
	}
	MPI_Finalize();
}