# Darknet Docker Wrapper

This project provides a Dockerized environment for the [Darknet](https://github.com/pjreddie/darknet) neural network framework, including support for OpenCV. It simplifies the build and execution process for both CPU and CUDA-enabled environments.

## Build Instructions

You can build the Docker image for either CPU or CUDA environments using the provided Dockerfiles.

### CPU Environment

To build the image for a CPU-only environment:

```bash
docker build -f Dockerfile.cpu -t darknet-cpu .
```

### CUDA Environment

To build the image for a CUDA-enabled environment (requires NVIDIA GPU and drivers):

```bash
docker build -f Dockerfile.cuda -t darknet-cuda .
```

## Usage

Once the image is built, you can run the Darknet container.

### Basic Run

To run the container interactively:

```bash
docker run --rm -it darknet-cpu
# OR for CUDA
docker run --gpus all --rm -it darknet-cuda
```

### Mounting Data

To use your own configuration files, weights, or data, mount your local directories to the container:

```bash
docker run --rm -it \
  -v $(pwd)/my_config:/opt/darknet/cfg \
  -v $(pwd)/my_data:/opt/darknet/data \
  darknet-cpu
```

## License

This project wraps Darknet, which is public domain. See the `LICENSE` file in the `darknet` subdirectory for more details on Darknet's licensing.
