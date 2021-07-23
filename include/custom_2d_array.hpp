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
namespace custom2darray{
    using size_t = long int;
}

/*
Homemade two-dimensional array, contiguous in row-major (column index varies first). Usies C++ 
dynamic allocation of a 1D array.

Provides basic functionality such as:
- Indexing with (i, j) to access elements by row and column
- Indexing with [m] to directly access the flat 1D array.
- Index assertion (Devnote: should remove for performance in core code?)
- A "null" state, in which size is zero and the data points to a nullptr.

Funcionalities not included
- Copying or deepcopying
*/
template <class T>
class Custom2DArray
{
private:

    custom2darray::size_t num_rows_{};
    custom2darray::size_t num_cols_{};
    custom2darray::size_t length_{};
    T *data_{};

    // Used both at the constructor and the rebuild() member function. 
    // If called out of context, could leave old data allocated withouht pointer (a memory leak).
    // That's why it's private.
    void build(custom2darray::size_t num_rows, custom2darray::size_t num_cols){
        num_rows_ = num_rows;
        num_cols_ = num_cols;
        length_ = num_rows * num_cols;
        data_ = new T[num_rows * num_cols]{};
    }
 
public:
 
    // --- Constructors

    // Construction of a 2D array with given dimensions (rows, cols).
    Custom2DArray(custom2darray::size_t num_rows, custom2darray::size_t num_cols){
        build(num_rows, num_cols);
    }

    // Empty (default) construction of a 2D array: leaves at null state.
    Custom2DArray() : num_cols_{0}, num_rows_{0}, length_{0}, data_{nullptr}  {};
 
    // We don't want to allow copies of IntArray to be created.
    Custom2DArray(const Custom2DArray&) = delete;
    Custom2DArray& operator=(const Custom2DArray&) = delete;
 
 
    // --- Destructor
    ~Custom2DArray()
    {
        delete[] data_;
    }
 

    // --- Refactoring functionality
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

    // Removes previous data and reallocates with new dimensions. 
    void rebuild(custom2darray::size_t num_rows, custom2darray::size_t num_cols){
        erase();
        build(num_rows, num_cols);
    }
 
    // --- Indexing operations 

    // Indexing with (i, j): returns the (i, j) entry of the 2D array.
    T& operator()(custom2darray::size_t i, custom2darray::size_t j){
        assert(i >= 0 && i < num_rows_);
        assert(j >= 0 && j < num_cols_);
        return data_[num_cols_ * i + j];
    }

    // Indexing with [m]: returns the m-th entry of the flattened 1D array.
    T& operator[](custom2darray::size_t index)
    {
        assert(index >= 0 && index < length_);
        return data_[index];
    }
 
    // templated get_length() function defined below, just to examplify
    custom2darray::size_t get_length() const;
    custom2darray::size_t get_num_rows() const {return num_rows_;}
    custom2darray::size_t get_num_cols() const {return num_cols_;}


    // --- Other functionalities
    void set_all_values_to(T val){
        for (custom2darray::size_t m{0}; m < length_; m++){
            data_[m] = val;
        }
    }
};
 
// member functions defined outside the class need their own template declaration
template <class T>
custom2darray::size_t Custom2DArray<T>::get_length() const // note class name is Array<T>, not Array
{
  return length_;
}


#endif
