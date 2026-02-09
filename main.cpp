#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

template<typename NUM>
vector<vector<int>> filling_square_vector(NUM type)
{
    vector<vector<NUM>> vector_;
    vector<NUM> row;
    cout << "Enter size of square matrix: ";
    int n;
    cin >> n;
    cout << endl;


    cout << "Enter numbers for matrix: " << endl;
    NUM temp;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> temp;
            row.push_back(temp);
        }
        vector_.push_back(row);
        row.clear();
    }
    cout << endl;
    
    return vector_;

}

template<typename NUM>
void output_square_vector(vector<vector<NUM>> &vector_)
{
    for (int i = 0; i < vector_.size(); ++i)
    {
        for (int j = 0; j < vector_.size(); ++j)
        {
            cout << vector_[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

template<typename NUM>
vector<double> Row_multiplication(const vector<NUM>& a_, const vector<vector<NUM>>& b_)
{
    vector<double> result;
    double cell;

    for (int i = 0; i < b_.size(); ++i)
    {
        cell = 0;

        for (int j = 0; j < a_.size(); ++j)
        {
            cell += a_[j] * b_[j][i];
        }

        result.push_back(cell);
    }

    return result;
}

template<typename NUM>
vector<vector<double>> Matrix_multiplication(const vector<vector<NUM>>& a_, const vector<vector<NUM>>& b_)
{
    vector<vector<double>> result;
    vector<double> row;

    for (int i = 0; i < a_.size(); ++i)
    {
        row = Row_multiplication(a_[i], b_);
        result.push_back(row);
        row.clear();

    }

    return result;

}

template<typename NUM>
vector<vector<double>> Threads_matrix_multiplication(const vector<vector<NUM>>& a_, const vector<vector<NUM>>& b_)
{

    vector<vector<double>> result;
    vector<double> row1;
    vector<double> row2;

    if (b_.size % 2 = 1)
    {
        for (int i = 0; i < a_.size()-1; ++i)
        {
            thread th1([&row1, a_, b_, i] {row1 = Row_multiplication(a_[i], b_); });
            thread th2([&row2, a_, b_, i] {row2 = Row_multiplication(a_[i], b_); });
            th1.join();
            th2.join();
            result.push_back(row1);
            result.push_back(row2);
            row1.clear();
            row2.clear();

        }
        row1 = Row_multiplication(a_[i], b_);
        result.push_back(row1);
    }
    else
    {
        for (int i = 0; i < a_.size(); ++i)
        {
            thread th1([&row1, a_, b_, i] {row1 = Row_multiplication(a_[i], b_); });
            thread th2([&row2, a_, b_, i] {row2 = Row_multiplication(a_[i], b_); });
            th1.join();
            th2.join();
            result.push_back(row1);
            result.push_back(row2);
            row1.clear();
            row2.clear();

        }
    }

    return result;

}

int main()
{
    vector<vector<int>> a /*= { {1, 2, 3},
        {4, 5, 6},
        {1, 2, 3} }*/;
    vector<vector<int>> b/* = { {4, 5, 6},
        {6, 5, 4},
        {4, 5, 6} }*/;

    a = filling_square_vector((int)0);
    output_square_vector(a);

    b = filling_square_vector((int)0);
    output_square_vector(b);

    auto start = chrono::steady_clock::now();

    vector<vector<double>> c = Matrix_multiplication(a, b);

    auto finish = chrono::steady_clock::now();
    auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(finish - start);

    cout << "Complete for " << elapsed_ns << endl;

    output_square_vector(c);

    auto start_th = chrono::steady_clock::now();

    vector<vector<double>> c_th = Threads_matrix_multiplication(a, b);

    auto finish_th = chrono::steady_clock::now();
    auto elapsed_ns_th = chrono::duration_cast<chrono::nanoseconds>(finish_th - start_th);

    cout << "Complete for " << elapsed_ns_th << endl;

    output_square_vector(c_th);
    
}


