#include <iostream>
using namespace std;

class CSR {
protected:
    int n; //The number rows of the original matrix
    int m; //The number of columns of the original matrix
    int nonZeros; //The number of non-zeros in the original matrix
    int colAddCounter = 0;//counting which place to add the value into column array
    int valAddCounter = 0; //counting which place to add the value into value array
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
    int getNumNonZeros(); //getter for the nonzeros member
    int* getValues(); //getter for the values array
    int* getColPos(); //getter for the colPos array
    int* getRowPtr(); //getter for the rowPtr array
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
    n = matrixB.getNumRows(); //assigning the input "n" value to the current objects n value
    m = matrixB.getNumColumns(); // assigning the input "m" value to the current objects m value
    nonZeros = matrixB.getNumNonZeros();

    values = new int [matrixB.getNumNonZeros()];//initializing each of the arrays in the copy object
    colPos = new int [matrixB.getNumNonZeros()];
    rowPtr = new int [matrixB.getNumRows()];

    colPos = new int (*matrixB.colPos); //creating deep copies of each arrray so that when
    values = new int (*matrixB.values); //destructor is called on the original the memory of the copy isnt deleted too
    rowPtr = new int (*matrixB.rowPtr);

    for (int i = 0; i < matrixB.getNumNonZeros(); ++i) {values[i] = matrixB.getValues()[i];}
    for (int i = 0; i < matrixB.getNumNonZeros(); ++i) {colPos[i] = matrixB.getColPos()[i];}
    for (int i = 0; i < matrixB.getNumRows(); ++i) {rowPtr[i] = matrixB.getRowPtr()[i];}
}
CSR::CSR (int rows, int cols, int numNonZeros) { //assigning each of the inputs to their respective members
    n = rows;
    m = cols;
    nonZeros = numNonZeros;
    values = new int [numNonZeros]; //priming each of the positions in the arrays so it doesnt do the weird error
    colPos = new int [numNonZeros];
    rowPtr = new int [rows];

    for (int i = 0; i < n; ++i) this->rowPtr[i] = 0; //setting all elements to 0
    for (int j = 0; j < nonZeros; ++j) this->values[j] = 0;
    for (int k = 0; k < nonZeros; ++k) this->colPos[k] = 0;
}
int CSR::getNumRows() { //returns the numrows of this object
    return this->n;
}
int CSR::getNumColumns() { //returns the numcolumns of this object
    return this->m;
}
int CSR::getNumNonZeros() {
    return this->nonZeros;
}
int* CSR::getValues(){
    return this->values;
}
int* CSR::getColPos() {
    return this->colPos;
}
int* CSR::getRowPtr() {
    return this->rowPtr;
}
void CSR::addValue(int value) {
    values[valAddCounter] = value; //adding the value at the necessary position
    valAddCounter++; //incrementing counter so values arent overwritten
}
void CSR::addRow(int row) {
    for (int i = row; i < n - 1; ++i) { //whenever you add to a row this goes through and increments each element of rowPtr to represent the "shift"
        rowPtr[i + 1]++; //of elements from values
    }
}
void CSR::addColumn(int col) {
    colPos[colAddCounter] = col; //adding value to necessary position
    colAddCounter++; //incrementing counter so values arent overwritten
}
void CSR::display() {
    for(int i = 0; i < n; i++) { //going through each row to print the array
        for (int j = 0; j < m; j++){ cout << getRowVec(i)[j] << " "; } //printing each element of the array
        cout << endl; //adding a newline character at the end
    }

    cout << "rowPtr: ";
    for (int i = 0; i < this->getNumRows(); ++i) cout << rowPtr[i] << " "; //printing all values of rowptr
    cout << endl;
    cout << "colPos: ";
    for (int j = 0; j < this->getNumNonZeros(); ++j) cout << colPos[j] << " "; //printing all values of colPos
    cout << endl;
    cout << "values: ";
    for (int k = 0; k < this->getNumNonZeros(); ++k) cout << values[k] << " "; //printing all values of values
    cout << endl;
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
CSR *CSR::matrixMultiply(CSR &matrixB) {
    CSR *outputMatrix = new CSR(matrixB.getNumRows(), matrixB.getNumColumns(), this->getNumRows()*matrixB.getNumColumns());

    int product = 0;
    int sum = 0;

    for (int i = 0; i < n; ++i) { //incrementing row of A after done multiplying with each column of B
        for (int j = 0; j < matrixB.getNumColumns(); ++j) { //incrementing column after done multiplying with row of A
            for (int k = 0; k < sizeof matrixB.getColumnVector(j); ++k) { //going through array to multiply
                product = this->getRowVec(i)[k] * matrixB.getColumnVector(j)[k]; //multiplying each value of array
                sum += product; //adding each of the products together
            }
            if (sum != 0) {
                outputMatrix->addValue(sum);
                outputMatrix->addRow(i);
                outputMatrix->addColumn(j);
            } //adding the sum to the matrix
            sum = 0;
        }
    }

    return outputMatrix; //returning the output matrix
}
int* CSR::getColumnVector(int col) {		//all rows of column col
    int *colVector = new int[n];
    int r;
    for (int i = 0; i < n; i++)
        colVector[i] = 0;

    bool found;
    int k, j;

    k = 0;
    for (int i = 0; i < n - 1; i++) {
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
            colVector[n - 1] = values[p];
            found = true;
        } else
            p++;
    }
    return colVector;
}
int* CSR::getRowVec(int row) {

    int *vector = new int[n];
    for (int i = 0; i < n; i++)
        vector[i] = 0;

    if (row < n - 1) {

        for (int i = rowPtr[row]; i < rowPtr[row + 1]; i++) {
            for (int j = 0; j < m; j++) {
                if (colPos[i] == j)
                    vector[j] = values[i];
            }
        }
    } else {
        for (int i = rowPtr[row]; i < nonZeros; i++) {
            for (int j = 0; j < m; j++) {
                if (colPos[i] == j)
                    vector[j] = values[i];
            }
        }
    }
    return vector;
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

    cin >> numRows >> numColumns; //taking in numRows and numColumns from the file
    cin >> numNonZeros; //taking in numNonzeroes from file

    B = new CSR(numRows, numColumns, numNonZeros); //creating the object with the input data
    for (int i = 0; i < numNonZeros; i++) {
        cin >> row >> col >> value; //taking in each line of the matrix data from the file and adding to each respective array
        (*B).addValue(value);
        (*B).addRow(row);
        (*B).addColumn(col);
    }

    cout << "Matrix B: " << endl;
    (*B).display ( ); //displaying object

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
