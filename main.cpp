#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double Round(double x) {
    return floor(x * 10000 + 0.5) / 10000;
}

void FancyPrint(vector<vector<double>>& A, vector<double>& B, pair<int, int> selected = make_pair(-1, -1)) {
    for (int row = 0; row < B.size(); row++) {
        for (int col = 0; col < A[row].size(); col++) {
            if (selected.first == row && selected.second == col) {
                cout <<"\t\t" << Round(A[row][col]);
            } else {
                cout << "\t\t" << Round(A[row][col]);
            }
            cout << (col < A[row].size() - 1 ? " " : "");

        }
        cout << "\t\t=\t" << Round(B[row]) <<  endl;
    }
}

void SwapRows(vector<vector<double>>& A, vector<double>& B, int row1, int row2) {
    swap(A[row1], A[row2]);
    swap(B[row1], B[row2]);
}

void DivideRow(vector<vector<double>>& A, vector<double>& B, int row, double divider) {
    for (int i = 0; i < A[row].size(); i++) {
        A[row][i] /= divider;
    }
    B[row] /= divider;
}

void CombineRows(vector<vector<double>>& A, vector<double>& B, int row, int source_row, double weight) {
    for (int i = 0; i < A[row].size(); i++) {
        A[row][i] += Round(A[source_row][i] * weight);
    }
    B[row] += B[source_row] * weight;
}

vector<double> Gauss(vector<vector<double>> A, vector<double> B) {
    int column = 0;
    while (column < B.size()) {
        cout << "Look for the maximal modulo element in " << column + 1 << " column:" << endl;
        int current_row = -1;
        for (int r = column; r < A.size(); r++) {
            if (current_row == -1 || abs(A[r][column]) > abs(A[current_row][column])) {
                current_row = Round(r);
            }
        }
        if (current_row == -1) {
            cout << "no solutions" << endl;
            return vector<double>();
        }
        FancyPrint(A, B, make_pair(current_row, column));
        if (current_row != column) {
            cout << "Move the line with the found element higher:" << endl;
            SwapRows(A, B, current_row, column);
            FancyPrint(A, B, make_pair(column, column));
        }
        cout << "Normalize the string with the found element:" << endl;
        DivideRow(A, B, column,A[column][column]);
        FancyPrint(A, B, make_pair(column, column));
        for (int r = column + 1; r < B.size(); r++) {
            cout << "Process " << r + 1 << " row:" << endl;
            double weight = -A[r][column] / A[column][column];
            CombineRows(A, B, r, column, weight);
            FancyPrint(A, B, make_pair(r, column));
        }
        column++;
    }
    vector<double> X(B.size(), 0);
    for (int row = B.size() - 1; row >= 0; row--) {
        double sum = 0;
        for (int col = row + 1; col < A[row].size(); col++) {
            sum += Round(A[row][col] * X[col]);
        }
        X[row] = Round((B[row] - sum) / A[row][row]);
    }
    return X;
}

void check(vector<vector<double>> A, vector<double> B, vector<double> X){
    vector<vector<double>> A1 = A;
    vector<double> B1 = B;
    vector<double> X1 =X;
    double sum;
    cout <<"Source system:"<<endl;
    for (int cons = 0; cons < B.size(); cons++ ){
        for (int i = 0; i < B.size();i++){
            cout <<A1[cons][i] << " * (x"<< i+1<< ")" << Round(X1[i]) << "\t" ;
        }
        cout << "=\t" << B[cons];
        cout << endl;
    }
    cout << "Result of check:" << endl;
    for (int cons = 0; cons < B.size(); cons++ ){
        for (int i = 0; i < B.size();i++){
            sum += Round(A1[cons][i] * X1[i]);
            cout << "\t" <<Round(A1[cons][i] * X1[i]) << "\t";
        }
        cout << "\t=\t" << sum;
        cout << endl;
        sum =0;
    }
}


int main() {
    cout << "Enter the number of equations and variables: ";
    int n;
    cin >> n;
//    vector<vector<double>> A = {{0.3, 1.2, -0.2}, {-0.1, -0.2, 1.6}, {-1.5, -0.3, 0.1}};
//    vector<double> B = {-0.6, 0.3, 40};
    cout << "Enter the coefficients of the equations and constants: " << endl;
    vector<vector<double>> A(n, vector<double>(n, 0));
    vector<double> B(n, 0);
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            cout << "Coefficient x" << col+1 << ":" ;
            cin >> A[row][col];
        }
        cout << "Constant "<< row+1 << ":";
        cin >> B[row];
    }

    vector<double> X = Gauss(A, B);

    if (X.empty()) {
        cout << "No solutions" << endl;
        return 0;
    }
    cout << "Check:" << endl;
    check(A,B,X);
    cout << "Answer:";
    cout <<"(";
    for (int i = 0; i < X.size(); i++) {
        if (i< X.size()-1) {
            cout << X[i] << ";";
        }else{
            cout << X[i];
        }
    }
    cout << ")" << endl;
    system("pause");
    return 0;
}