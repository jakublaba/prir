from multiprocessing.managers import BaseManager


def load_vector(file_name: str) -> list[float]:
    print(f"Loading vector from {file_name}...")
    with open(file_name, "r") as f:
        # Skip 1st two verses
        f.readline()
        f.readline()
        return [float(x) for x in f.readlines()]


def load_matrix(file_name: str) -> list[list[float]]:
    print(f"Loading matrix from {file_name}...")
    with open(file_name, "r") as f:
        rows = int(f.readline())
        cols = int(f.readline())
        matrix: list[list[float]] = []
        for j in range(cols):
            current_row: list[float] = []
            for i in range(rows):
                current_row.append(float(f.readline()))
            matrix.append(current_row)
        return matrix


def server_manager(ip: str = "127.0.0.1", port: int = 2137, authkey: bytes = b"password") -> BaseManager:
    BaseManager.register("get_tasks")
    BaseManager.register("get_results")
    return BaseManager(address=(ip, port), authkey=authkey)
