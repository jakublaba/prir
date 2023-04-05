from multiprocessing import Process, Queue, Manager
import sys

class Serwer:
    def __init__(self) -> None:
        manager = Manager()
        self.data_to_process = manager.Queue()
        self.temp_makrs = manager.Queue()


class Klient:
    def __init__(self, file_matrix, file_vector) -> None:
        self.matrix = []
        self.vector = []
        self.read_matrix(file_matrix)
        self.read_vector(file_vector)
    
    def read_matrix(self, file) -> None:
        with open(file, 'r') as file:
            self.rows = int(file.readline())
            self.cols = int(file.readline())
            for i in range(self.rows):
                self.matrix.append(file.readline().strip().split(" "))
    
    def read_vector(self, file) -> None:
        with open(file, 'r') as file:
            self.vec_len = int(file.readline())
            for i in range(self.vec_len):
                self.vector.append(file.readline().strip())

    def prepare(self, data_to_process, size_of_workers) -> None:
        for i in range(self.rows):
            data_to_process.put((self.matrix[i], self.vector))
        for i in range(size_of_workers):
            data_to_process.put(None)

    def wait_for_workers(self, workers):
        for worker in workers:
            worker.join()

    def wait_for_summary(self, data):
        self.result = []
        while not data.empty():
            self.result.append(data.get())


class Worker:
    def execute(self, input, output):
        while True:
            task = input.get()
            if task == None:
                break
            submatrix, vector = task
            result = [int(submatrix[i]) * int(vector[i]) for i in range(len(submatrix))]
            output.put(sum(result))


if __name__ == "__main__":
    serwer = Serwer()
    workers = []
    for i in range(int(sys.argv[3])):
        worker = Process(target=Worker.execute, args=(serwer.data_to_process, serwer.temp_makrs))
        worker.start()
        workers.append(worker)

    klient = Klient(sys.argv[1], sys.argv[2])
    klient.prepare(serwer.data_to_process, len(workers))
    # klient.wait_for_workers(workers)
    klient.wait_for_summary(serwer.temp_makrs)
    print(klient.result)
