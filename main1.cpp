#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

#define MAX_THREADS 7

using namespace std;

template<typename NUM>
vector<vector<int>> random_filling_square_vector(NUM type, int size)
{
    srand(time(0));
    vector<vector<NUM>> vector_;
    vector<NUM> row;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            row.push_back(rand());
        }
        vector_.push_back(row);
        row.clear();
    }

    return vector_;

}

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

    for (int k = 0; k < a_.size(); ++i)
    {
        result.push_back(Row_multiplication(a_[k], b_));
        vector<double> result2;
        for (int i = 0; i < b_.size(); ++i)
        {
            double cell = 0;

            for (int j = 0; j < a_[k].size(); ++j)
            {
                cell += a_[k][j] * b_[j][i];
            }

            result2.push_back(cell);
        }

        result.push_back(result2);
    }

    return result;

}

template<typename NUM>
vector<vector<double>> Threads_matrix_multiplication(const vector<vector<NUM>>& a_, const vector<vector<NUM>>& b_)
{
    if (a_.size() != b_.size())
        throw "Wrong sizes inputs";

    vector<thread> ths;
    vector<vector<double>> result;
    vector<vector<vector<double>>> temp_result(MAX_THREADS);

    ths.reserve(MAX_THREADS);

    int number_sep_process = a_.size() % MAX_THREADS;
    int number_common_process = a_.size() - number_sep_process;
    int number_processes_on_one_thread = number_common_process / MAX_THREADS;

    for (int j = 0; j < MAX_THREADS; ++j)
    {
        ths.emplace_back(thread([&number_processes_on_one_thread, &a_, &b_, &temp_result, j]()
            {
                for (int i = number_processes_on_one_thread * j; i < number_processes_on_one_thread * (j + 1); ++i)
                {
                    temp_result[j].emplace_back(Row_multiplication(a_[i], b_));
                };
            }));
    }

    for (int i = 0; i < MAX_THREADS; ++i)
    {
        ths[i].join();
        for (auto j : temp_result[i])
        {
            result.emplace_back(j);
        }
    }
    
    for (int i = number_processes_on_one_thread * MAX_THREADS; i < a_.size(); ++i)
    {
        result.emplace_back(Row_multiplication(a_[i], b_));
    }

    return result;
    
}

int main()

{
    vector<vector<int>> a;
    vector<vector<int>> b;

    int size = 1000;
    a = random_filling_square_vector((int)0, size);
    //output_square_vector(a);

    b = random_filling_square_vector((int)0, size);
    //output_square_vector(b);

    cout << "Max_hardware_threads: " << thread::hardware_concurrency() << endl;
    cout << "Allowed_threads: " << MAX_THREADS << endl;

    auto start = chrono::steady_clock::now();

    vector<vector<double>> c = Matrix_multiplication(a, b);

    auto finish = chrono::steady_clock::now();
    auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(finish - start);

    cout << "Complete for " << elapsed_ns << endl;

    //output_square_vector(c);

    auto start_th = chrono::steady_clock::now();

    vector<vector<double>> c_th = Threads_matrix_multiplication(a, b);

    auto finish_th = chrono::steady_clock::now();
    auto elapsed_ns_th = chrono::duration_cast<chrono::nanoseconds>(finish_th - start_th);

    cout << "Complete for " << elapsed_ns_th << endl;

    //output_square_vector(c_th);
    
}


