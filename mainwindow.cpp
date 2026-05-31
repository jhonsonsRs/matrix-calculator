#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #1a1a2e;
            color: #e2e8f0;
            font-family: "Segoe UI";
        }
        QLabel {
            color: #e2e8f0;
            font-size: 13px;
        }
        QLabel#lbErrorText {
            color: #e86c6c;
            font-size: 12px;
            font-weight: bold;
        }
        QTableWidget#tableA, QTableWidget#tableB {
            background-color: #16213e;
            border: 1px solid #2d3561;
            border-radius: 8px;
            gridline-color: #2d3561;
            color: #e2e8f0;
            font-size: 13px;
        }
        QTableWidget#tableA::item, QTableWidget#tableB::item {
            color: #e2e8f0;
            padding: 4px;
        }
        QTableWidget#tableA::item:selected, QTableWidget#tableB::item:selected {
            background-color: #7c6af0;
            color: white;
        }
        QTableWidget#tableResult {
            background-color: #0d2137;
            border: 1px solid #4caf8740;
            border-radius: 8px;
            gridline-color: #1a3a52;
            color: #4caf87;
            font-family: "Courier New";
            font-size: 13px;
            font-weight: bold;
        }
        QTableWidget#tableResult::item {
            color: #4caf87;
            padding: 4px;
        }
        QHeaderView::section {
            background-color: #0f3460;
            color: #7c6af0;
            border: 1px solid #2d3561;
            font-size: 11px;
            font-weight: bold;
            padding: 4px;
        }
        QTableCornerButton::section {
            background-color: #0f3460;
            border: 1px solid #2d3561;
        }
        QSpinBox {
            background-color: #0f3460;
            border: 1px solid #7c6af0;
            border-radius: 6px;
            color: #7c6af0;
            font-size: 14px;
            font-weight: bold;
            padding: 4px 8px;
            min-width: 50px;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            background-color: #2d3561;
            border-radius: 3px;
            width: 16px;
        }
        QPushButton#btnSum {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #7c6af0, stop:1 #5a4fcf);
            border: none;
            border-radius: 8px;
            color: white;
            font-size: 13px;
            font-weight: bold;
            padding: 10px 20px;
        }
        QPushButton#btnSum:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #9a8af5, stop:1 #7c6af0);
        }
        QPushButton#btnSubtract {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #e86c6c, stop:1 #c94040);
            border: none;
            border-radius: 8px;
            color: white;
            font-size: 13px;
            font-weight: bold;
            padding: 10px 20px;
        }
        QPushButton#btnSubtract:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #f08080, stop:1 #e05050);
        }
        QPushButton#btnMultiply {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4caf87, stop:1 #2e8b6a);
            border: none;
            border-radius: 8px;
            color: white;
            font-size: 13px;
            font-weight: bold;
            padding: 10px 20px;
        }
        QPushButton#btnMultiply:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #5dc99e, stop:1 #3da07a);
        }
        QPushButton#btTransposeA, QPushButton#btTransposeB,
        QPushButton#btMultiplyA,  QPushButton#btMultiplyB {
            background-color: #16213e;
            border: 1px solid #2d3561;
            border-radius: 6px;
            color: #a8b4d8;
            font-size: 12px;
            padding: 6px 14px;
            min-width: 90px;
        }
        QPushButton#btTransposeA:hover, QPushButton#btTransposeB:hover,
        QPushButton#btMultiplyA:hover,  QPushButton#btMultiplyB:hover {
            background-color: #2d3561;
            color: #e2e8f0;
        }
        QPushButton#btTransposeA:pressed, QPushButton#btTransposeB:pressed,
        QPushButton#btMultiplyA:pressed,  QPushButton#btMultiplyB:pressed {
            background-color: #0f3460;
        }
        QPushButton#btnSum, QPushButton#btnSubtract, QPushButton#btnMultiply {
            min-width: 80px;
        }
    )");
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<std::vector<int>> sum_matrices_algorithm(
    const std::vector<std::vector<int>>& m1,
    const std::vector<std::vector<int>>& m2)
{
    if(m1.size() != m2.size() || m1[0].size() != m2[0].size())
        return {};
    int rows = m1.size(), cols = m1[0].size();
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[i][j] = m1[i][j] + m2[i][j];
    return result;
}

std::vector<std::vector<int>> subtract_matrices_algorithm(
    const std::vector<std::vector<int>>& m1,
    const std::vector<std::vector<int>>& m2)
{
    if(m1.size() != m2.size() || m1[0].size() != m2[0].size())
        return {};
    int rows = m1.size(), cols = m1[0].size();
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[i][j] = m1[i][j] - m2[i][j];
    return result;
}

std::vector<std::vector<int>> multiply_matrices_algorithm(
    const std::vector<std::vector<int>>& m1,
    const std::vector<std::vector<int>>& m2)
{
    int rA = m1.size(), cA = m1[0].size();
    int rB = m2.size(), cB = m2[0].size();
    if(cA != rB) return {};
    std::vector<std::vector<int>> result(rA, std::vector<int>(cB, 0));
    for(int i = 0; i < rA; i++)
        for(int j = 0; j < cB; j++)
            for(int k = 0; k < cA; k++)
                result[i][j] += m1[i][k] * m2[k][j];
    return result;
}

std::vector<std::vector<int>> scalar_matrices_algorithm(
    const std::vector<std::vector<int>>& m1, int value)
{
    int rows = m1.size(), cols = m1[0].size();
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[i][j] = m1[i][j] * value;
    return result;
}

std::vector<std::vector<int>> transpose_matrices_algorithm(
    const std::vector<std::vector<int>>& m1)
{
    int rows = m1.size(), cols = m1[0].size();
    std::vector<std::vector<int>> result(cols, std::vector<int>(rows));
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[j][i] = m1[i][j];
    return result;
}

double determinant_algorithm(std::vector<std::vector<double>> m)
{
    int n = m.size();
    if(n != (int)m[0].size()) return 0;
    double det = 1.0;
    for(int j = 0; j < n; j++){
        for(int i = j + 1; i < n; i++){
            double fator = m[i][j] / m[j][j];
            for(int k = j; k < n; k++)
                m[i][k] -= fator * m[j][k];
        }
        det *= m[j][j];
    }
    return det;
}

// ── Helpers UI ────────────────────────────────────────────────

std::vector<std::vector<int>> readTable(QTableWidget* table){
    int rows = table->rowCount(), cols = table->columnCount();
    std::vector<std::vector<int>> mat(rows, std::vector<int>(cols, 0));
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++){
            auto item = table->item(i, j);
            if(item) mat[i][j] = item->text().toInt();
        }
    return mat;
}

std::vector<std::vector<double>> readTableDouble(QTableWidget* table){
    int rows = table->rowCount(), cols = table->columnCount();
    std::vector<std::vector<double>> mat(rows, std::vector<double>(cols, 0));
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++){
            auto item = table->item(i, j);
            if(item) mat[i][j] = item->text().toDouble();
        }
    return mat;
}

void setTableResult(QTableWidget* table, const std::vector<std::vector<int>>& result){
    int rows = result.size(), cols = result[0].size();
    table->setRowCount(rows);
    table->setColumnCount(cols);
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            table->setItem(i, j, new QTableWidgetItem(QString::number(result[i][j])));
}

// ── Slots ─────────────────────────────────────────────────────

void MainWindow::on_btnSum_clicked(){
    auto matA = readTable(ui->tableA);
    auto matB = readTable(ui->tableB);
    auto result = sum_matrices_algorithm(matA, matB);
    if(result.empty()){ ui->lbErrorText->setText("Error: dimensions must be equal!"); return; }
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btnSubtract_clicked(){
    auto matA = readTable(ui->tableA);
    auto matB = readTable(ui->tableB);
    auto result = subtract_matrices_algorithm(matA, matB);
    if(result.empty()){ ui->lbErrorText->setText("Error: dimensions must be equal!"); return; }
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btnMultiply_clicked(){
    auto matA = readTable(ui->tableA);
    auto matB = readTable(ui->tableB);
    auto result = multiply_matrices_algorithm(matA, matB);
    if(result.empty()){ ui->lbErrorText->setText("Error: cols(A) must equal rows(B)!"); return; }
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btMultiplyA_clicked(){
    auto mat = readTable(ui->tableA);
    auto result = scalar_matrices_algorithm(mat, ui->spinValueA->value());
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btMultiplyB_clicked(){
    auto mat = readTable(ui->tableB);
    auto result = scalar_matrices_algorithm(mat, ui->spinValueB->value());
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btTransposeA_clicked(){
    auto result = transpose_matrices_algorithm(readTable(ui->tableA));
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btTransposeB_clicked(){
    auto result = transpose_matrices_algorithm(readTable(ui->tableB));
    ui->lbErrorText->setText("");
    setTableResult(ui->tableResult, result);
}

void MainWindow::on_btDeterminantA_clicked(){
    auto mat = readTableDouble(ui->tableA);
    if(mat.size() != mat[0].size()){ ui->lbErrorText->setText("Error: matrix must be square!"); return; }
    double det = determinant_algorithm(mat);
    ui->lbErrorText->setText("det(A) = " + QString::number(det));
}

void MainWindow::on_btDeterminantB_clicked(){
    auto mat = readTableDouble(ui->tableB);
    if(mat.size() != mat[0].size()){ ui->lbErrorText->setText("Error: matrix must be square!"); return; }
    double det = determinant_algorithm(mat);
    ui->lbErrorText->setText("det(B) = " + QString::number(det));
}

void MainWindow::on_spinRowsA_valueChanged(int arg1){ ui->tableA->setRowCount(arg1); }
void MainWindow::on_spinColsA_valueChanged(int arg1){ ui->tableA->setColumnCount(arg1); }
void MainWindow::on_spinRowsB_valueChanged(int arg1){ ui->tableB->setRowCount(arg1); }
void MainWindow::on_spinColsB_valueChanged(int arg1){ ui->tableB->setColumnCount(arg1); }
