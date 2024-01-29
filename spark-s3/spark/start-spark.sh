#!/bin/bash
#start-spark.sh

# Copied from:
#   https://medium.com/@SaphE/testing-apache-spark-locally-docker-compose-and-kubernetes-deployment-94d35a54f222

. "/opt/spark/bin/load-spark-env.sh"
echo "$SPARK_WORKLOAD"
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
  ./run-tests.sh
  # When the spark work_load is worker run class org.apache.spark.deploy.master.Worker
  cd /opt/spark/bin \
    && ./spark-class org.apache.spark.deploy.worker.Worker \
      --webui-port $SPARK_WORKER_WEBUI_PORT \
      $SPARK_MASTER \
        >> $SPARK_WORKER_LOG

elif [ "$SPARK_WORKLOAD" == "thrift" ];
then
  # https://towardsdev.com/running-spark-thrift-service-on-kubernetes-a-comprehensive-guide-309d8f2ec91b
  # Ignoring some fields
  #       --conf spark.hadoop.hive.metastore.uris=thrift://<hive-metastore-host>:<hive-metastore-port>
  #       
  /opt/spark/sbin/start-thriftserver.sh \
    --master "${SPARK_MASTER}" \
    --packages com.typesafe:config:1.4.2,org.apache.hadoop:hadoop-aws:3.3.4,org.apache.spark:spark-hadoop-cloud_2.12:3.4.0,org.apache.spark:spark-sql-kafka-0-10_2.12:3.4.0 \
    && tail -f /dev/null
    # --conf spark.driver.extraJavaOptions="-Divy.cache.dir=/tmp -Divy.home=/tmp" \
    # --conf spark.dynamicAllocation.enabled=true \
    # --conf spark.hadoop.fs.s3a.access.key="${MINIO_ACCESS}" \
    # --conf spark.hadoop.fs.s3a.committer.name=directory \
    # --conf spark.hadoop.fs.s3a.endpoint="${MINIO_HOST}" \
    # --conf spark.hadoop.fs.s3a.fast.upload=true \
    # --conf spark.hadoop.fs.s3a.impl=org.apache.hadoop.fs.s3a.S3AFileSystem \
    # --conf spark.hadoop.fs.s3a.secret.key="${MINIO_SECRET}" \
    # --conf spark.hadoop.mapreduce.fileoutputcommitter.algorithm.version=2 \
    # --conf spark.hadoop.mapreduce.fileoutputcommitter.cleanup-failures.ignored=true \
    # --conf spark.hadoop.parquet.enable.summary-metadata=false \
    # --conf spark.kubernetes.container.image=apache/spark:v3.4.0 \
    # --conf spark.kubernetes.file.upload.path=s3a://some/tmp/folder/thriftserver \
    # --conf spark.sql.catalogImplementation=hive \
    # --conf spark.sql.hive.metastorePartitionPruning=true \
    # --conf spark.sql.parquet.filterPushdown=true \
    # --conf spark.sql.parquet.mergeSchema=false \
    # --conf spark.sql.parquet.output.committer.class=org.apache.spark.internal.io.cloud.BindingParquetOutputCommitter \
    # --conf spark.sql.sources.commitProtocolClass=org.apache.spark.internal.io.cloud.PathOutputCommitProtocol \
    # --conf spark.sql.sources.default=parquet \
    # --conf spark.sql.sources.partitionOverwriteMode=dynamic \
    # --conf spark.sql.warehouse.dir=s3a://warehouse \
    # --conf spark.kubernetes.driver.pod.name=spark-thrift-server-0 \
    # --conf spark.driver.host=spark-thrift-service \
    # --conf spark.driver.port=${SPARK_THRIFT_PORT} \


else
    echo "Undefined Workload Type $SPARK_WORKLOAD, must specify: master, worker, submit, thrift."
fi