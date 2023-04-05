package com.krabelard.matrix;

import lombok.val;

import java.util.Arrays;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;

public class Matrix {

    private final double[][] data;

    public Matrix (double[][] data) {
        validateMatrixData(data);
        this.data = data;
    }

    public static Matrix of(double[][] data) {
        return new Matrix(data);
    }

    /**
     * Parallel matrix multiplication
     * @param other Matrix to multiply this one with
     * @param nThreads Number of threads to use
     * @param timeoutOpt Optional argument to specify desired timeout (seconds) before which calculations are aborted (5 by default)
     * @throws InterruptedException when calculations are aborted due to timeout
     */
    public Matrix multiplyParallel(Matrix other, int nThreads, int... timeoutOpt) throws InterruptedException {
        validateMultiplicationMatrix(other);

        val executor = Executors.newFixedThreadPool(nThreads);
        val result = new double[this.rows()][other.cols()];

        for (int i = 0; i < this.rows(); ++i) {
            val row = i;
            final Runnable job = () -> {
                for (int j = 0; j < other.cols(); ++j) {
                    for (int k = 0; k < other.rows(); ++k) {
                        result[row][j] += get(row, k) * other.get(k, j);
                    }
                }
            };

            executor.execute(job);
        }

        executor.shutdown();

        val timeout = timeoutOpt.length > 0 ? timeoutOpt[0] : 5;
        if (!executor.awaitTermination(timeout, TimeUnit.SECONDS)) {
            throw new InterruptedException(String.format("Multiplication terminated after %d seconds", timeout));
        }

        return Matrix.of(result);
    }

    /**
     * Parallel Frobenius norm
     * @param nThreads Number of threads to use
     * @param timeoutOpt Optional argument to specify desired timeout (seconds) before which calculations are aborted (5 by default)
     * @throws InterruptedException when calculations were aborted due to timeout
     */
    public double frobeniusParallel(int nThreads, int... timeoutOpt) throws InterruptedException {
        val executor = Executors.newFixedThreadPool(nThreads);
        val result = new AtomicReference<>(.0);

        for (int i = 0; i < this.rows(); ++i) {
            val row = i;
            final Runnable job = () -> {
                for (int j = 0; j < this.cols(); ++j) {
                    result.set(result.get() + this.get(row, j) * this.get(row, j));
                }
            };

            executor.execute(job);
        }

        executor.shutdown();

        val timeout = timeoutOpt.length > 0 ? timeoutOpt[0] : 5;
        if (!executor.awaitTermination(timeout, TimeUnit.SECONDS)) {
            throw new InterruptedException(String.format("Frobenius norm calculation terminated after %d seconds", timeout));
        }

        return Math.sqrt(result.get());
    }

    public double get(int i, int j) {
        return data[i][j];
    }

    public int rows() {
        return data.length;
    }

    public int cols() {
        return data.length > 0 ? data[0].length : 0;
    }

    @Override
    public String toString() {
        val rawStr = Arrays.deepToString(data);

        return rawStr
                .substring(1, rawStr.length() - 1)
                .replace("], ", "]\n");
    }

    private void validateMatrixData(double[][] data) {
        if (data == null) {
            throw new IllegalArgumentException("Data must not be null");
        }

        val flattenedData = Arrays.stream(data)
                .flatMapToDouble(Arrays::stream)
                .toArray();

        if (flattenedData.length == 0) {
            throw new IllegalArgumentException("Data must not be empty");
        }

        val rowLengths = Arrays.stream(data)
                .map(row -> row.length)
                .collect(Collectors.toSet());

        if (rowLengths.size() > 1) {
            throw new IllegalArgumentException("Rows must not differ in length");
        }
    }

    private void validateMultiplicationMatrix(Matrix other) {
        validateMatrixData(other.data);
        if (cols() != other.rows()) {
            throw new IllegalArgumentException("Other matrix must have number of rows equals to 1st matrix's number of columns");
        }
    }

}
