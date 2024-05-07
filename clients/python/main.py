from rewind_viewer.client import RewindClient

if __name__ == "__main__":
    rewind_client = RewindClient("127.0.0.1", 9111)

    for i in range(10):
        rewind_client.end_frame()
