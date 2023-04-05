import sys
from multiprocessing import Queue
from multiprocessing.managers import BaseManager


class MultiplicationServer:
    def __init__(self, ip: str = "127.0.0.1", port: int = 2137, authkey: bytes = b"password"):
        print(f"Server running on {ip}:{port}")
        self.__manager = BaseManager(address=(ip, port), authkey=authkey)
        self.__register_queues()
        # Task is a pair of matrix row and vector to multiply
        self.__tasks: Queue[tuple[list[float], list[float]]] = Queue()
        self.__results: Queue[float] = Queue()
        self.__server = self.__manager.get_server()

    def start(self):
        self.__server.serve_forever()

    def __register_queues(self):
        BaseManager.register("get_tasks", callable=lambda: self.__tasks)
        BaseManager.register("get_results", callable=lambda: self.__results)


if __name__ == "__main__":
    ip = sys.argv[1]
    port = int(sys.argv[2])
    svr = MultiplicationServer(ip=ip, port=port)
    svr.start()
