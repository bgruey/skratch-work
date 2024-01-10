# Original code copied from:
#   https://medium.com/@SaphE/testing-apache-spark-locally-docker-compose-and-kubernetes-deployment-94d35a54f222

# Docker file for job-specific builds
FROM spark-image-base

WORKDIR ${SPARK_HOME}

COPY start-spark.sh /
#
RUN mkdir -p ./jobs
COPY jobs/ ./jobs/

#RUN mkdir -p ./local
COPY local/ ./local/

COPY run-tests.sh ./

#RUN mkdir -p setup
COPY setup/ setup/

COPY requirements.txt ./
RUN python -m pip install -r requirements.txt

CMD ["/bin/bash", "/start-spark.sh"]