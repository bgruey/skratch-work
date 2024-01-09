# Original code copied from:
#   https://medium.com/@SaphE/testing-apache-spark-locally-docker-compose-and-kubernetes-deployment-94d35a54f222

# Docker file for job-specific builds
FROM spark-image-base

COPY start-spark.sh /
RUN mkdir -p "${SPARK_HOME}/jobs"
COPY jobs/* "${SPARK_HOME}/jobs/"
COPY requirements.txt ./
RUN python -m pip install -r requirements.txt

CMD ["/bin/bash", "/start-spark.sh"]