package com.krabelard;

import com.krabelard.matrix.Matrix;
import lombok.SneakyThrows;
import lombok.val;

import java.io.IOException;
import java.util.Objects;
import java.util.Scanner;

public class Main {

    @SneakyThrows
    public static void main(String... args) {
        val aData = load("A.txt");
        val bData = load("B.txt");
        val A = Matrix.of(aData);
        val B = Matrix.of(bData);

        System.out.printf("B\n%s\n\n", B);
        System.out.printf("A\n%s\n\n", A);
        val C = A.multiplyParallel(B, 5);
        System.out.printf("A x B\n%s\n", C);
        System.out.printf("Frobenius norm of A x B\n%s\n", C.frobeniusParallel(5));
    }

    /**
     * Loads matrix data from file in project's resources directory
     * @param fileName Name of the file
     * @return Matrix data represented as 2D array
     */
    private static double[][] load(String fileName) {
        try (
                val inputStream = Objects.requireNonNull(Main.class.getClassLoader().getResourceAsStream(fileName));
                val scanner = new Scanner(inputStream)
        ) {
            val rows = scanner.nextInt();
            val cols = scanner.nextInt();
            val arr = new double[rows][cols];

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    arr[i][j] = scanner.nextDouble();
                }
            }

            return arr;
        } catch (IOException e) {
            throw new RuntimeException("Failed to load " + fileName);
        }
    }

}