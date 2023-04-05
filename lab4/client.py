from multiprocessing import Queue

from util import load_matrix, load_vector, server_manager


class MultiplicationClient:
    def __init__(self, matrix_file: str, vector_file: str, ip: str = "127.0.0.1", port: int = 2137, ):
        self.__manager = server_manager(ip=ip, port=port)
        self.__manager.connect()
        self.__matrix: list[list[float]] = load_matrix(matrix_file)
        for row in self.__matrix:
            print(row)
        self.__vector: list[float] = load_vector(vector_file)
        print(self.__vector)
        self.__partition_data()

    def __partition_data(self):
        print("Preparing data for calculations...")
        tasks: Queue[tuple[list[float], list[float]]] = self.__manager.get_tasks()
        for row in self.__matrix:
            tasks.put((row, self.__vector))

    def results(self) -> list[float]:
        res: list[float] = []
        results = self.__manager.get_results()
        while not results.empty():
            res.append(results.get())
        return res
