#include <iostream>
using namespace std;

class CSR {
protected:
    int n; //The number rows of the original matrix
    int m; //The number of columns of the original matrix
    int nonZeros; //The number of non-zeros in the original matrix
    int* values; //value array assuming all are integers
    int* rowPtr; //Array that contains number of non-zero elements in each row of the original matrix
    int* colPos; //Array that contains the column number from the original matrix for the non-zero values.
    //There may be others you may need
public:
    CSR ( ); //default or empty constructor
    CSR (CSR& martrixB); //copy constructor
    CSR (int rows, int cols, int numNonZeros); // parametrized constructor
    int getNumRows ( ); //getter for numrows
    int getNumColumns(); //getter for numcolumns (m);
    void addValue (int value); //add a new value in the values array
    void addColumn (int col);//add a column in the colPos array
    void addRow (int row); //add a row in the rowPtr array
    void display (); //print the contents of the three arrays. Each array must be on a different line (separated by a
    //new line character) and you have exactly a single space between each value printed.
    int* matrixVectorMultiply (int* intputVector); //matrix-vector multiplication
    CSR* matrixMultiply (CSR& matrixB); //matrix-matrix multiplication
    int* getRowVec(int row);
    int * getColumnVector(int col);
    ~CSR(); //destructor
    //You may have other methods
};
//some of the methods – Sridhar Radhakrishnan
CSR::CSR () {
    n = 0;
    m = 0;
    values = NULL;
    rowPtr = NULL;
    colPos = NULL;
}
CSR:: CSR(CSR& matrixB) { //taking in the matrix using a pointer
    n = matrixB.n; //assigning the input "n" value to the current objects n value
    m = matrixB.m; // assigning the input "m" value to the current objects m value
    values = matrixB.values; // assigning the input "values" value to the objects values value
    rowPtr = matrixB.rowPtr; // assigning the input "rowPtr" value to the objects rowPtr value
    colPos = matrixB.colPos; // assigning the input "colPos" value to the objects colPos value
}
CSR::CSR (int rows, int cols, int numNonZeros) {
    n = rows;
    m = cols;
    nonZeros = numNonZeros;
    values = new int [nonZeros];
    colPos = new int [nonZeros];
    rowPtr = new int [n];
}
int CSR::getNumRows() {
    return this->n;
}
int CSR::getNumColumns() {
    return this->m;
}
void CSR::addValue(int value) {
    int* biggerVal = new int [nonZeros + 1]; //temporary array with the same size as rowPtr plus 1
    for (int i = 0; i < nonZeros; ++i) biggerVal[i] = values[i]; //adding each element of rowPtr to biggerRows
    biggerVal[nonZeros] = value; //adding the row value to the last element of bigger rows
    values = biggerVal; //setting rowPtr equal to biggerRows
    nonZeros++; //adding one to nonZeros to represent the added value
}
void CSR::addRow(int row) {
    int* biggerRows = new int [n + 1]; //a temporary array with the same size as rowPtr plus 1
    for (int i = 0; i < nonZeros; ++i) biggerRows[i] = rowPtr[i]; //adding each element of rowPtr to biggerRows
    biggerRows[nonZeros] = row; //adding the row value to the last element of bigger rows
    rowPtr = biggerRows; //setting rowPtr equal to biggerRows
    n++;//adding one to the variable that represents number of rows
}
void CSR::addColumn(int col) {
    m++;//adding one to the variable that represents number of columns
}
void CSR::display() {


    cout << "rowPtr: ";
    for (int i = 0; i < n; ++i) cout << rowPtr[n] << " ";
    cout << "\n";
    cout << "colPos: ";
    for (int i = 0; i < nonZeros; ++i) cout << colPos[nonZeros] << " ";
    cout << "\n";
    cout << "values: ";
    for (int i = 0; i < nonZeros; ++i) cout << values[nonZeros] << " ";
    cout << "\n";
}
int* CSR::matrixVectorMultiply (int* inputVector){
    int* outputVector = new int [n];

    for (int i=0; i < n; i++) outputVector[i] =0;

    int sum = 0;
    int start, end;
    for (int i=0; i < n; i++){
        sum = 0;
        start = rowPtr[i];
        if ((i + 1) == n) {
            end = nonZeros;
        }
        else {
            end = rowPtr[i+1];
        }
        for (int j = start; j < end; ++j) {
            sum = sum + (values[j] * inputVector[colPos[j]]);
        }
        outputVector[i] = sum;
    }

    return outputVector;
}
int * CSR::getColumnVector(int col) {
    int *colVector = new int[n];
    int r;
    for (int i = 0; i < n; ++i) {
        colVector[i] = 0;
    }
    bool found;
    int k, j;

    k = 0;
    for (int i = 0; i < n - 1; ++i) {
        r = rowPtr[i + 1] - rowPtr[i];
        k = rowPtr[i];
        found = false;
        j = 0;
        while ((j < r) && !found) {
            if (colPos[k] == col) {
                found = true;
                colVector[i] = values[k];
            }
            k++;
            j++;
        }
    }
    int p = rowPtr[n - 1];
    found = false;
    while ((p < (nonZeros)) && (!found)) {
        if (colPos[p] == col) {
            colVector[n-1] = values[p];
            found = true;
        }
        else {
            p++;
        }
    }
    return colVector;
}
int* CSR::getRowVec(int row) {

    int *vector = new int[n];
    for (int i = 0; i < n; ++i) {
        vector[i] = 0;
    }
    if (row < n-1) {
        for (int i = rowPtr[row]; i < rowPtr[row + 1]; i++) {
            for (int j = 0; j < m; j++) {
                if (colPos[i] == j) {
                    vector[j] = values[i];
                }
            }
        }
    }
        else {
            for (int i = rowPtr[row]; i < nonZeros; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (colPos[i] == j) {
                        vector[j] = values[i];
                    }
                }
            }
            return vector;
        }
    }
CSR::~CSR ( ) {
    if (values != NULL) delete [ ] values;
    if (rowPtr != NULL) delete [ ] rowPtr;
    if (colPos != NULL) delete [ ] colPos;
    cout << "CSR Object Destroyed!!" << endl;
    n = 0;
    m = 0;
    nonZeros = 0;
}

CSR *CSR::matrixMultiply(CSR &matrixB) {
    return nullptr;
}


//write the entire CSR class here with all the methods

int main ( ) {

    CSR* A;
    CSR* B;
    int* aVector;
    int numRows, numColumns, numNonZeros;
    int row, col, value;

    //read in the first matrix
    cin >> numRows >> numColumns;
    cin >> numNonZeros;

    A = new CSR (numRows, numColumns, numNonZeros);
    for (int i=0; i < numNonZeros; i++) {
        cin >> row >> col >> value;
        (*A).addValue(value);
        (*A).addRow(row);//needs to be done cleverly in the method
        (*A).addColumn (col);
    }
    cout << "Matrix A:" << endl; //labeling matrix A for display
    (*A).display ( );

    CSR* C = new CSR (*A); //calling the copy constructor
    cout << "Copied matrix C:" << endl; //labeling the copied matrix, C
    (*C).display ( );

//read in the second matrix which is similar to the first into the CSR pointer object B and display; Write code for this

    cin >> numRows >> numColumns;
    cin >> numNonZeros;

    B = new CSR(numRows, numColumns, numNonZeros);
    for (int i = 0; i < numNonZeros; i++) {
        cin >> row >> col >> value;
        (*B).addValue(value);
        (*B).addRow(row);
        (*B).addColumn(col);
    }

    cout << "Matrix B: " << endl;
    (*B).display ( );

//read in the vector
    cin >> numColumns;
    aVector = new int [numColumns];
    for (int i=0; i < numColumns; i++)
        cin >> aVector[i];

    cout << "Vector: " << endl;
    for (int i = 0; i < numColumns; ++i) {cout << aVector[i] << " ";}
    cout << endl;

//Matrix-Vector Multiplication
    cout << "A*Vector: " << endl;
    int* resultVector = (*A).matrixVectorMultiply (aVector);
    for (int i=0; i < (*A).getNumRows ( ); i++)
        cout << resultVector [i] << " ";
    cout << endl;

//Matrix-Matrix Multiplication
    cout << "A*B" << endl;
    CSR* resultMatrix = (*C).matrixMultiply (*B);
    (*resultMatrix).display ( );

// Call the destructors
    delete [ ] aVector;
    delete [ ] resultVector;
    delete A;
    delete B;
    delete C;
    delete resultMatrix;

    return 0;
}
