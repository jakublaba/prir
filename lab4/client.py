from multiprocessing import Queue

from util import load_matrix, load_vector, server_manager


class MultiplicationClient:
    def __init__(self, matrix_file: str, vector_file: str, ip: str = "127.0.0.1", port: int = 2137, ):
        self.__manager = server_manager(ip=ip, port=port)
        self.__manager.connect()
        self.__matrix: list[list[float]] = load_matrix(matrix_file)
        self.__vector: list[float] = load_vector(vector_file)
        self.__partition_data()

    def __partition_data(self):
        print("Preparing data for calculations...")
        tasks: Queue[tuple[int, list[float], list[float]]] = self.__manager.get_tasks()
        idx: int = 0
        for row in self.__matrix:
            print(f"\t- Preparing row {idx}...")
            tasks.put((idx, row, self.__vector))
            idx += 1

    def results(self) -> list[float]:
        print("Collecting results...")
        results: Queue[tuple[int, float]] = self.__manager.get_results()
        res: list[float] = [.0] * len(self.__vector)
        while not results.empty():
            idx, num = results.get()
            res[idx] = num
        return res
