# Original code copied from:
#   https://medium.com/@SaphE/testing-apache-spark-locally-docker-compose-and-kubernetes-deployment-94d35a54f222

# Docker file for job-specific builds
FROM spark-image-base

WORKDIR ${SPARK_HOME}

COPY start-spark.sh /

RUN mkdir -p ./jobs
COPY jobs/ ./jobs/

COPY local/ ./local/

COPY run-tests.sh ./

COPY setup/ setup/

RUN mkdir -p /root/.aws
COPY aws_credentials /root/.aws/credentials
ENV AWS_PROFILE=local

COPY requirements.txt ./
RUN python -m pip install -r requirements.txt

CMD ["/bin/bash", "/start-spark.sh"]