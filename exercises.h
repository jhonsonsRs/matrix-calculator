#pragma once
#include <QString>
#include <QList>
#include <QRandomGenerator>
#include "matrix.h"

enum class ExType { Sum, Subtract, Multiply, Scalar, Transpose, Determinant, Inverse };

struct Exercise {
    ExType type = ExType::Sum;
    QString statement;
    Matrix A, B;
    double k = 0;
    Matrix answer;
    double answerNumber = 0;
    bool isNumeric = false;
    QString solution;
};

inline int rnd(int lo, int hi) { return QRandomGenerator::global()->bounded(lo, hi + 1); }

inline Matrix randomMatrix(int r, int c, int lo = -5, int hi = 9) {
    Matrix m(r, std::vector<double>(c));
    for (auto& row : m) for (auto& x : row) x = rnd(lo, hi);
    return m;
}

// Matriz inteira com det = 1, então a inversa também tem números inteiros
inline Matrix randomUnimodular(int n) {
    Matrix L(n, std::vector<double>(n, 0)), U = L;
    for (int i = 0; i < n; i++) {
        L[i][i] = U[i][i] = 1;
        for (int j = 0; j < i; j++) L[i][j] = rnd(-2, 3);
        for (int j = i + 1; j < n; j++) U[i][j] = rnd(-2, 3);
    }
    return multiply(L, U);
}

// level: 1 = fácil (2x2), 2 = médio (até 3x3), 3 = difícil (até 3x3)
inline Exercise generateExercise(int level) {
    Exercise ex;
    int n = (level == 1) ? 2 : 3;

    QList<ExType> pool = { ExType::Sum, ExType::Subtract, ExType::Scalar,
                          ExType::Transpose, ExType::Multiply, ExType::Determinant };
    if (level >= 2) pool << ExType::Inverse;
    ex.type = pool[rnd(0, pool.size() - 1)];

    switch (ex.type) {
    case ExType::Sum:
    case ExType::Subtract: {
        int r = rnd(2, n), c = rnd(2, n);
        ex.A = randomMatrix(r, c); ex.B = randomMatrix(r, c);
        bool plus = ex.type == ExType::Sum;
        ex.answer = sum(ex.A, ex.B, plus ? 1 : -1);
        ex.statement = plus ? "Calcule A + B." : "Calcule A - B.";
        ex.solution = QString("Opere elemento a elemento: C[i][j] = A[i][j] %1 B[i][j].")
                          .arg(plus ? "+" : "-");
        break; }
    case ExType::Multiply: {
        int r = rnd(2, n), m = rnd(2, n), c = rnd(2, n);
        ex.A = randomMatrix(r, m, -3, 5); ex.B = randomMatrix(m, c, -3, 5);
        ex.answer = multiply(ex.A, ex.B);
        ex.statement = "Calcule A × B.";
        ex.solution = QString("C[i][j] = soma de A[i][k]·B[k][j]: multiplique a linha i de A pela "
                              "coluna j de B. A é %1×%2 e B é %2×%3, então o resultado é %1×%3.")
                          .arg(r).arg(m).arg(c);
        break; }
    case ExType::Scalar: {
        ex.A = randomMatrix(rnd(2, n), rnd(2, n));
        ex.k = rnd(-4, 5);
        if (ex.k == 0) ex.k = 2;
        ex.answer = scalar(ex.A, ex.k);
        ex.statement = QString("Calcule %1 · A.").arg(ex.k);
        ex.solution = QString("Multiplique cada elemento de A por %1.").arg(ex.k);
        break; }
    case ExType::Transpose: {
        ex.A = randomMatrix(rnd(2, n + 1), rnd(2, n + 1));
        ex.answer = transpose(ex.A);
        ex.statement = "Calcule a transposta de A.";
        ex.solution = "Troque linhas por colunas: At[j][i] = A[i][j].";
        break; }
    case ExType::Determinant: {
        ex.A = randomMatrix(n, n, -4, 6);
        determinant(ex.A, ex.answerNumber);
        ex.answerNumber = std::round(ex.answerNumber);
        ex.isNumeric = true;
        ex.statement = "Calcule o determinante de A.";
        ex.solution = (n == 2)
                          ? "Para 2×2: det = a·d - b·c."
                          : "Para 3×3 use a regra de Sarrus ou o teorema de Laplace (cofatores).";
        break; }
    case ExType::Inverse: {
        ex.A = randomUnimodular(n);
        inverse(ex.A, ex.answer);
        for (auto& row : ex.answer) for (auto& x : row) x = std::round(x);
        ex.statement = "Calcule a matriz inversa de A.";
        ex.solution = "A inversa existe pois det(A) = 1, que é diferente de 0. "
                      "Use Gauss-Jordan: escalone [A | I] até chegar em [I | A⁻¹]. "
                      "Confira: A × A⁻¹ deve dar a identidade.";
        break; }
    }
    return ex;
}

inline bool checkAnswer(const Exercise& ex, const Matrix& given, double givenNumber) {
    const double eps = 1e-4;
    if (ex.isNumeric) return std::fabs(ex.answerNumber - givenNumber) < eps;
    if (isEmpty(given) || given.size() != ex.answer.size() || given[0].size() != ex.answer[0].size())
        return false;
    for (size_t i = 0; i < given.size(); i++)
        for (size_t j = 0; j < given[0].size(); j++)
            if (std::fabs(given[i][j] - ex.answer[i][j]) > eps) return false;
    return true;
}
