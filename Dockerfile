FROM ubuntu:latest

# Install dependencies
RUN apt-get update && \
    apt-get install -y g++ valgrind build-essential libncurses5-dev libncursesw5-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]

WORKDIR /app

# You can mount your source directory at runtime using Docker's -v option.
# No Dockerfile change is needed for live mounting.
# Example usage:
# docker run -it -v /Users/aaryabhatia/repos/ncurses-editor:/app ubuntu:latest /bin/bash