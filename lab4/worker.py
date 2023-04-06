from multiprocessing import cpu_count, Process, Queue

from util import server_manager


def execute(tasks: Queue[tuple[int, list[float], list[float]]], results: Queue[tuple[int, float]]):
    while True:
        if tasks.empty():
            break
        task = tasks.get()
        idx, row, vec = task
        results.put((idx, sum([row[i] * vec[i] for i in range(len(vec))])))


class MultiplicationWorker:
    def __init__(self, ip: str = "127.0.0.1", port: int = 2137):
        self.__manager = server_manager(ip=ip, port=port)
        self.__manager.connect()
        self.__tasks: Queue[tuple[int, list[float], list[float]]] = self.__manager.get_tasks()
        self.__results: Queue[tuple[int, float]] = self.__manager.get_results()
        self.__processes: list[Process] = []
        self.__init_procs()

    def start(self):
        print("Starting worker processes...")
        for p in self.__processes:
            print(f"\t- {p}")
            p.start()

    def join(self):
        print("Waiting for workers to finish...")
        for p in self.__processes:
            print(f"\t- {p}")
            p.join()

    def __init_procs(self):
        print("Starting workers...")
        for i in range(cpu_count()):
            proc = Process(target=execute, args=[self.__tasks, self.__results])
            self.__processes.append(proc)
