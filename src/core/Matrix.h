#pragma once

// Matrix 4x4
class Matrix {
public:
  Matrix() {
    for (int i = 0; i < 16; i++)
      matrix[i] = 0.0f;
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
  }

  Matrix(float m0, float m1, float m2, float m3,
         float m4, float m5, float m6, float m7,
         float m8, float m9, float m10, float m11,
         float m12, float m13, float m14, float m15) {
    matrix[0] = m0;   matrix[1] = m1;   matrix[2] = m2;   matrix[3] = m3;
    matrix[4] = m4;   matrix[5] = m5;   matrix[6] = m6;   matrix[7] = m7;
    matrix[8] = m8;   matrix[9] = m9;   matrix[10] = m10; matrix[11] = m11;
    matrix[12] = m12; matrix[13] = m13; matrix[14] = m14; matrix[15] = m15;
  }
  
  void Set(int position, float value) {
    matrix[position] = value;
  }
  
  float Get(int position) const {
    return matrix[position];
  }
  
  const float *GetMatrix() const {
    return matrix;
  }
private:
  float matrix[16];
};