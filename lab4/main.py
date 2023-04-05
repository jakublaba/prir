import sys

from client import MultiplicationClient
from worker import MultiplicationWorker

if __name__ == "__main__":
    mat_file = sys.argv[1]
    vec_file = sys.argv[2]
    ip = sys.argv[3]
    port = int(sys.argv[4])
    client = MultiplicationClient(mat_file, vec_file, ip=ip, port=port)
    worker = MultiplicationWorker(ip=ip, port=port)

    worker.start()
    worker.join()

    res = client.results()
    with open("res.dat", "w") as f:
        f.write(str(res))
