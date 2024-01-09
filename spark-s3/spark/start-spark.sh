#!/bin/bash
#start-spark.sh

# Copied from:
#   https://medium.com/@SaphE/testing-apache-spark-locally-docker-compose-and-kubernetes-deployment-94d35a54f222

. "/opt/spark/bin/load-spark-env.sh"
# When the spark work_load is master run class org.apache.spark.deploy.master.Master
if [ "$SPARK_WORKLOAD" == "master" ];
then

  export SPARK_MASTER_HOST=`hostname`

  cd /opt/spark/bin \
    && ./spark-class org.apache.spark.deploy.master.Master \
      --ip $SPARK_MASTER_HOST \
      --port $SPARK_MASTER_PORT \
      --webui-port $SPARK_MASTER_WEBUI_PORT \
        >> $SPARK_MASTER_LOG

elif [ "$SPARK_WORKLOAD" == "worker" ];
then
  echo "-nfo Starting worker"
  # When the spark work_load is worker run class org.apache.spark.deploy.master.Worker
  cd /opt/spark/bin \
    && ./spark-class org.apache.spark.deploy.worker.Worker \
      --webui-port $SPARK_WORKER_WEBUI_PORT \
      $SPARK_MASTER \
        >> $SPARK_WORKER_LOG

#  echo "-nfo Submitting Clean Job"
#  /opt/spark/bin/spark-submit --master "${SPARK_MASTER}" --name clean-job ./jobs/clean.py
#  tail -f "${SPARK_WORKER_LOG}"

else
    echo "Undefined Workload Type $SPARK_WORKLOAD, must specify: master, worker, submit"
fi