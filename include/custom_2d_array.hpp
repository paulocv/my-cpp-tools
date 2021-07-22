/* 
Contiguous, row-major two-dimensional array with very simple interface.

Author: Paulo Cesar Ventura da Silva
https://github.com/paulocv
Adapted from: https://www.learncpp.com/cpp-tutorial/template-classes/ 
(Thanks a lot to the authors of this great tutorial website!)


*/

#ifndef CUSTOM_2D_ARRAY_HPP
#define CUSTOM_2D_ARRAY_HPP

#include <cassert>  // Allows assert() with template parameters


// DEV TODO: if I have guts, change the type of the size from int to another template parameter.

template <class T>
class Custom2DArray
{
private:
    int num_rows_{};
    int num_cols_{};
    int length_{};
    T *data_{};
 
public:
 
    // --- Main constructor
    Custom2DArray(int num_rows, int num_cols) 
    : num_rows_{num_rows}, num_cols_{num_cols}, length_{num_rows_ * num_cols_}
    {
        assert(num_rows > 0 && num_cols > 0);
        data_ = new T[num_rows * num_cols]{};
    }
 
    // We don't want to allow copies of IntArray to be created.
    Custom2DArray(const Custom2DArray&) = delete;
    Custom2DArray& operator=(const Custom2DArray&) = delete;
 
    // --- Main destructor
    ~Custom2DArray()
    {
        delete[] data_;
    }
 
    void erase()
    {
        delete[] data_;
        // We need to make sure we set data_ to 0 here, otherwise it will
        // be left pointing at deallocated memory!
        data_ = nullptr;
        num_cols_ = 0;
        num_rows_ = 0;
        length_ = 0;
    }
 
    // --- Indexing operations 

    // Indexing with (i, j): returns the (i, j) entry of the 2D array.
    T& operator()(int i, int j){
        assert(i >= 0 && i < num_rows_);
        assert(j >= 0 && j < num_cols_);
        return data_[num_cols_ * i + j];
    }

    // Indexing with [m]: returns the m-th entry of the flattened 1D array.
    T& operator[](int index)
    {
        assert(index >= 0 && index < length_);
        return data_[index];
    }
 
    // templated get_length() function defined below, just to examplify
    int get_length() const;
    int get_num_rows() const {return num_rows_;}
    int get_num_cols() const {return num_cols_;}
};
 
// member functions defined outside the class need their own template declaration
template <class T>
int Custom2DArray<T>::get_length() const // note class name is Array<T>, not Array
{
  return length_;
}


#endif
