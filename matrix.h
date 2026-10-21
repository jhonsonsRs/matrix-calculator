#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

using Matrix = std::vector<std::vector<double>>;

inline bool isEmpty(const Matrix& m) { return m.empty() || m[0].empty(); }

inline Matrix sum(const Matrix& a, const Matrix& b, int sign = 1) {
    if (isEmpty(a) || isEmpty(b) || a.size() != b.size() || a[0].size() != b[0].size())
        return {};
    Matrix r = a;
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < a[0].size(); j++)
            r[i][j] = a[i][j] + sign * b[i][j];
    return r;
}

inline Matrix multiply(const Matrix& a, const Matrix& b) {
    if (isEmpty(a) || isEmpty(b) || a[0].size() != b.size()) return {};
    Matrix r(a.size(), std::vector<double>(b[0].size(), 0));
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b[0].size(); j++)
            for (size_t k = 0; k < b.size(); k++)
                r[i][j] += a[i][k] * b[k][j];
    return r;
}

inline Matrix scalar(const Matrix& a, double v) {
    if (isEmpty(a)) return {};
    Matrix r = a;
    for (auto& row : r) for (auto& x : row) x *= v;
    return r;
}

inline Matrix transpose(const Matrix& a) {
    if (isEmpty(a)) return {};
    Matrix r(a[0].size(), std::vector<double>(a.size()));
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < a[0].size(); j++)
            r[j][i] = a[i][j];
    return r;
}

// Determinante por eliminação de Gauss com pivoteamento parcial
inline bool determinant(Matrix m, double& det) {
    size_t n = m.size();
    if (n == 0 || m[0].size() != n) return false;
    det = 1.0;
    for (size_t j = 0; j < n; j++) {
        size_t p = j;
        for (size_t i = j + 1; i < n; i++)
            if (std::fabs(m[i][j]) > std::fabs(m[p][j])) p = i;
        if (std::fabs(m[p][j]) < 1e-12) { det = 0; return true; }
        if (p != j) { std::swap(m[p], m[j]); det = -det; }
        det *= m[j][j];
        for (size_t i = j + 1; i < n; i++) {
            double f = m[i][j] / m[j][j];
            for (size_t k = j; k < n; k++) m[i][k] -= f * m[j][k];
        }
    }
    return true;
}

// Inversa por Gauss-Jordan. Retorna false se não for quadrada ou for singular.
inline bool inverse(const Matrix& a, Matrix& out) {
    size_t n = a.size();
    out.clear();
    if (n == 0 || a[0].size() != n) return false;
    Matrix m = a;
    out.assign(n, std::vector<double>(n, 0));
    for (size_t i = 0; i < n; i++) out[i][i] = 1;

    for (size_t j = 0; j < n; j++) {
        size_t p = j;
        for (size_t i = j + 1; i < n; i++)
            if (std::fabs(m[i][j]) > std::fabs(m[p][j])) p = i;
        if (std::fabs(m[p][j]) < 1e-12) { out.clear(); return false; }
        std::swap(m[p], m[j]); std::swap(out[p], out[j]);

        double piv = m[j][j];
        for (size_t k = 0; k < n; k++) { m[j][k] /= piv; out[j][k] /= piv; }
        for (size_t i = 0; i < n; i++) {
            if (i == j) continue;
            double f = m[i][j];
            for (size_t k = 0; k < n; k++) {
                m[i][k]   -= f * m[j][k];
                out[i][k] -= f * out[j][k];
            }
        }
    }
    return true;
}
