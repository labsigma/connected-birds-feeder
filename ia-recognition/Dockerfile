#https://hub.docker.com/_/python
FROM python:3.8-slim

WORKDIR /app

# Copy all the sources
COPY api.py /app
COPY requirements.txt /app
COPY resnet_model.py /app

COPY index_to_class_label/ /app/index_to_class_label
COPY models/ /app/models

# Install dependencies
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Command
CMD ["python", "./api.py"]