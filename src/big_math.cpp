/**
* TITLE: Big Math
* VERSION: 1.0
* AUTHOR: Jesper Glas
* DESC: A package to handle math to large for existing data types. It does this by converting string
*       representations to vectors containing int values.
*
*       Supports addition, multiplication, fac and sum with positive numbers.
*/

/* === PACKAGES === */
#include <iostream>
#include <vector>
#include <string>
#include <iterator> // For vec to string transform
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

/* === GLOBAL VARIABLES */
static const int BASE = 10;

/* === FUNCTIONS ===*/
void print_mat_2d(const vector<vector<int>> &vec_2d);
void print_vec(const vector<int> &vec, bool human=false);
vector<int> big_to_vec(const unsigned long long &value);
vector<int> big_to_vec(const string &value);
string vec_to_big(const vector<int> &vec);
bool valid_int_str(const string &int_str);

vector<int> big_add(const vector<int> &vec_a, const vector<int> &vec_b);
vector<int> big_add(const string &str_a, const string &str_b);

vector<int> big_mult(const vector<int> &vec_a, const vector<int> &vec_b);
vector<int> big_sum(const vector<vector<int>> &vec_2d);
vector<int> fac(int target);

void example();
void ui_add();

/* === CODE === */

/**
 * @brief Examples of the functions of the package.
 * 
 * @return int return status
 */
int main()
{
    bool done {false};
    char op {};

    while (!done) {
        cout << string(50, '=') << endl;
        cout << "Choose an operation:" << endl;
        cout << "[a, v, h, q]" << endl;
        cin >> op;

        if (op == 'q' || op == 'Q') {
            done = true;
        } else if (op == 'h' || op == 'H') {
            cout << "v - Test if a string is valid." << endl;
            cout << "h - For help." << endl;
            cout << "q - To quit the program." << endl;
        } else if (op == 'v' || op == 'V') {
            cout << "Enter a string representation of a number:" << endl;
            string temp {};
            cin >> temp;
            cout << temp << " is " << ((valid_int_str(temp)) ? "valid" : "not valid") << "." << endl;
        } else if(op == 'a' || op == 'A') {
            // TODO: Get this working.
            // ui_add();
        } else {
            cout << "Invalid operation, try again." << endl;
        }
    }

    return 0;
}

void ui_add()
{
    cout << "Enter two values to add (Separated by a whitespace):" << endl;
    string a {};
    string b {};

    bool valid {false};

    while (!valid) {
        cout << "Enter 2 values separated by a white space: " << endl;

        // TODO: Figure out how to extract two values. Stuck in loop with cin >> a and getline(cin, a).

        cout << valid_int_str(a) << endl;
        cout << valid_int_str(b) << endl;

        if (valid_int_str(a) && valid_int_str(b)) {
            valid = true;
        } else {
            cout << "One or both inputs is not valid, try again." << endl;
        }
    }

    cout << a << " + " << b << " = " << vec_to_big(big_add(a, b)) << endl;
}

/**
 * @brief Function displays examples using functions in the package.
 * 
 */
void example() 
{
    string separator (80, '=');
    unsigned long long a {892};
    string b {"5236"};

    cout << separator << endl << endl;
    cout << "Example 1: Addition" << endl;
    cout << endl;
    cout << a << "<unsigned long lon>" << endl;
    cout << b << "<string>" << endl;
    cout << a << " + " << b << " = ?" << endl;
    cout << endl;

    cout << "Convert values to vectors:" << endl;
    vector<int> vec_a = big_to_vec(a);
    vector<int> vec_b = big_to_vec(b);
    cout << "Vec a: ";
    print_vec(vec_a);
    cout << " =reversed=> ";
    print_vec(vec_a, true);
    cout << endl;
    cout << "Vec b: ";
    print_vec(vec_b);
    cout << " =reversed=> ";
    print_vec(vec_b, true);
    cout << endl << endl;
    
    cout << "Calculate addition:" << endl;
    vector<int> big_sum = big_add(vec_a, vec_b);
    cout << "Result is ";
    print_vec(big_sum);
    cout << " =reversed=> ";
    print_vec(big_sum, true);
    cout << endl << endl;

    cout << "Convert to readable form:" << endl;
    cout << a << " + " <<  b << " = ";
    cout << vec_to_big(big_sum);
    cout << endl << endl;

    cout << separator << endl << endl;

    cout << "Example 2: Multiplication" << endl;
    cout << endl;
    cout << a << "<unsigned long lon>" << endl;
    cout << b << "<string>" << endl;
    cout << a << " * " << b << " = ?" << endl;
    cout << endl;

    cout << "Convert values to vectors like in Example 1:" << endl;
    cout << endl;

    cout << "Calculate multiplication:" << endl;
    vector<int> vec_prod = big_mult(vec_a, vec_b);
    cout << "Result is ";
    print_vec(vec_prod);
    cout << " =reversed=> ";
    print_vec(vec_prod, true);
    cout << endl << endl;

    cout << "Convert to readable form:" << endl;
    cout << a << " * " << b << " = " << vec_to_big(vec_prod) << endl;
    cout << endl;

    cout << separator << endl << endl;

    cout << "Example 3: Factorial (Deck of cards):" << endl;
    cout << endl;

    string correct {"80658175170943878571660636856403766975289505440883277824000000000000"};
    cout << "Reference answer (Correct): " << endl << correct << endl;
    cout << endl;

    cout << "Calculate factorial of 52 (52!):" << endl;
    vector<int> fac52 = fac(52);
    cout << vec_to_big(fac52) << endl;
    cout << endl;

    cout << boolalpha;
    cout << "Do they match: " << (vec_to_big(fac52) == correct) << endl;
    cout << endl;

    cout << separator << endl;
}

/**
 * @brief Multiplies vectors representing large numbers.
 * 
 * @param vec_a Term a
 * @param vec_b Term b
 * @return vector<int> The resulting product in the form of a vector representing a large number.
 */
vector<int> big_mult(const vector<int> &vec_a, const vector<int> &vec_b)
{
    int size_a = static_cast<int>(vec_a.size());
    int size_b = static_cast<int>(vec_b.size());
    vector<vector<int>> prod_vec {};

    for (int i {0}; i < size_a; i++) {
        vector<int> row (i, 0);
        int carry {0};
        for (int j {0}; j < size_b; j++) {
            int prod {0};
            prod = vec_a.at(i) * vec_b.at(j) + carry;
            int value = prod % BASE;
            int overflow = prod / BASE;
            row.push_back(value);
            carry = (overflow > 0) ? overflow : 0;
        }
        if (carry > 0) {
            row.push_back(carry);
        }
        prod_vec.push_back(row);
    }

    vector<int> sum_vec = big_sum(prod_vec);

    return sum_vec;
}

/**
 * @brief Prints a vector.
 * 
 * @param vec Reference to the vector of ints to be printed.
 * @param human True to print vector in a human readable way
 */
void print_vec(const vector<int> &vec, bool human)
{
    cout << "[";
    if (human) {
        for (int i = static_cast<int>(vec.size())-1; i >= 0; i--) {
            cout << vec.at(i) << " ";
        }
    } else {
        for (int elem : vec) {
            cout << elem << " ";
        }
    }
    cout << "]";
}

/**
 * @brief Prints a 2d matrix (vector<vector<int>>).
 * 
 * @param vec_2d Reference to the matrix to print (vector<vector<int>>). 
 */
void print_mat_2d(const vector<vector<int>> &vec_2d)
{
    for (vector<int> row : vec_2d) {
        print_vec(row, true);
        cout << endl;
    }
}

/**
 * @brief Converts an unsigned long long value to a vector representation.
 * 
 * @param value Reference to the value to be converted.
 * @return vector<int> representation of value (LSD to MSD).
 */
vector<int> big_to_vec(const unsigned long long &value)
{
    vector<int> vec {};
    int rest = static_cast<int>(value);

    while (rest > BASE) {
        vec.push_back(rest % BASE);
        rest /= BASE;
    }
    vec.push_back(rest);

    return vec;
}

/**
 * @brief Converts a string representation value to a vector representation.
 * 
 * @param value Reference to the value to be converted.
 * @return vector<int> representation of value (LSD to MSD).
 */
vector<int> big_to_vec(const string &value)
{
    // TODO: Check valid string input
    vector<int> sum_vec {};

    for (int i = static_cast<int>(value.length())-1; i >= 0; i--) {
        sum_vec.push_back(value.at(i)-48);
    }
    return sum_vec;
}

/**
 * @brief Converts a vector representation of a large number to a string representation.
 * 
 * @param vec Reference to the vector representation.
 * @return string representation of the vector value.
 */
string vec_to_big(const vector<int> &vec)
{
    stringstream res_stream;
    copy(vec.begin(), vec.end(), ostream_iterator<int>(res_stream, ""));

    string res = res_stream.str();
    reverse(res.begin(), res.end());

    return res;
}

/**
 * @brief Adds two vectors representing large numbers (vec_a + vec_b).
 * 
 * @param a Reference to vector representation a.
 * @param b Reference to vector representation b.
 * @return vector<int> representation of vec_a + vec_b.
 */
vector<int> big_add(const vector<int> &vec_a, const vector<int> &vec_b)
{
    int vector_len = static_cast<int>(max(vec_a.size(), vec_b.size()));
    vector<int> sum_vec (vector_len, 0);

    int carry {0};
    for (int i {0}; i < vector_len; i++) {
        int sum {0};
        sum += (i < vec_a.size()) ? vec_a.at(i) : 0;
        sum += (i < vec_b.size()) ? vec_b.at(i) : 0;
        sum += carry;
        int value = sum % BASE;
        int overflow = sum / BASE;
        sum_vec.at(i) = value;
        carry = (overflow > 0) ? overflow : 0;
    }

    if (carry > 0) {
        sum_vec.push_back(carry);
    }

    return sum_vec;
}

/**
 * @brief Adds two strings representing large numbers (str_a + vec_b).
 * 
 * @param str_a Reference to string representation str_a.
 * @param str_b Reference to string representation str_b.
 * @return vector<int> representation of str_a + str_b.
 */
vector<int> big_add(const string &str_a, const string &str_b)
{
    vector<int> vec_a = big_to_vec(str_a);
    vector<int> vec_b = big_to_vec(str_b);

    return big_add(vec_a, vec_b);
}

 /**
  * @brief Sum of a vector containing vector representations of large numbers.
  * 
  * @param vec_2d 2d vector containing the vector representations of large numbers.
  * @return vector<int> representation of the sum as a vector representation.
  */
vector<int> big_sum(const vector<vector<int>> &vec_2d)
{
    vector<int> sum_vec (1, 0);

    for (vector<int> vec : vec_2d) {
        sum_vec = big_add(vec, sum_vec);
    }
    return sum_vec;
}

/**
 * @brief Factorial at target as a vector representation of a large number.
 * 
 * @param target Index of factorial.
 * @return vector<int> representation of factorial result as a vector representing a large number.
 */
vector<int> fac(int target)
{
    vector<int> result = big_to_vec(1);
    for (int n {1}; n < target+1; n++) {
        vector<int> vec_n = big_to_vec(n);
        result = big_mult(result, vec_n);
    }
    return result;
}

/**
 * @brief Checks if a string only contains ints.
 * 
 * @param int_str string of integer values.
 * @return true if string is valid.
 * @return false if string is unvalid.
 */
bool valid_int_str(const string &int_str)
{
    int low = static_cast<int>(int_str.length()) * 48;
    int high = static_cast<int>(int_str.length()) * 57;
    int str_value {0};

    if (int_str.size() > 1) {
        return false;
    }

    for (int c : int_str) {
        str_value += c;
    }

    return (low <= str_value) && (str_value <= high);
}