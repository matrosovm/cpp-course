#include <fstream>

#include "matrices.hpp"

Matrix::Matrix() : row_(0), column_(0), data_(nullptr) { } 

Matrix::Matrix(size_t row, size_t column) : row_(row), column_(column), data_(new double*[row]()) {
    for (size_t i = 0; i < row_; ++i) {
        data_[i] = new double[column_](); 
    }
}

void Matrix::print(std::ostream& os) const {
    os << row_ << " " << column_ << std::endl;
    
    for (size_t i = 0; i < row_; ++i) {
        for (size_t j = 0; j < column_; ++j) {
            os << data_[i][j] << " ";
        }
        os << std::endl;
    }
}

Matrix::~Matrix() {
    for (size_t i = 0; i < row_; ++i) {
        delete[] data_[i];
    }
    delete[] data_;   
}

void Matrix::add(const Matrix& mat) {
	for (size_t i = 0; i < row_; ++i) {
		for (size_t j = 0; j < column_; ++j) {
			data_[i][j] += mat.data_[i][j];
		}
	}
}

void swap(Matrix& left, Matrix& right) {
	std::swap(left.data_, right.data_);
	std::swap(left.row_, right.row_);
	std::swap(left.column_, right.column_);
}

void Matrix::mult(const Matrix& mat) {
	Matrix tmp(row_, mat.column_);
	for (size_t i = 0; i < tmp.row_; ++i) {
		for (size_t j = 0; j < tmp.column_; ++j) {
			for (size_t k = 0; k < column_; ++k)
				tmp.data_[i][j] += data_[i][k] * mat.data_[k][j];
		}
	} 
	swap(*this, tmp);
}

bool Matrix::read(const char* file_name) {
	std::ifstream in(file_name);
	if (!in.is_open())  
		return false;

	size_t row, column;
	in >> row >> column;   
	Matrix tmp(row, column);

	for (size_t i = 0; i < row; ++i) {
		for (size_t j = 0; j < column; ++j) {
	   		in >> tmp.data_[i][j];
	 	}
	}
	swap(*this, tmp);
	return true;
}

bool Matrix::isPossibleAdd(const Matrix& mat) const {
	return row_ == mat.row_ && column_ == mat.column_;
}

bool Matrix::isPossibleMult(const Matrix& mat) const {
	return column_ == mat.row_;	
}